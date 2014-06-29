#include "multiimagewritethread.h"
#include "config.h"
#include "json.h"
#include "util.h"
#include "mbr.h"
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QProcess>
#include <QProcessEnvironment>
#include <QSettings>
#include <QTime>
#include <unistd.h>
#include <linux/fs.h>
#include <sys/ioctl.h>

MultiImageWriteThread::MultiImageWriteThread(QObject *parent) :
    QThread(parent), _extraSpacePerPartition(0), _part(5)
{
    QDir dir;

    if (!dir.exists("/mnt2"))
        dir.mkdir("/mnt2");
}

void MultiImageWriteThread::addImage(const QString &folder, const QString &flavour)
{
    _images.insert(folder, flavour);
}

void MultiImageWriteThread::run()
{
    /* Calculate space requirements */
    int totalnominalsize = 0, totaluncompressedsize = 0, numparts = 0, numexpandparts = 0, numext4expandparts = 0;
    bool RiscOSworkaround = false;
    int startSector = getFileContents("/sys/class/block/mmcblk0p2/start").trimmed().toULongLong();
    int availableMB = (getFileContents("/sys/class/block/mmcblk0p3/start").trimmed().toULongLong()-startSector)/2048;

    foreach (QString folder, _images.keys())
    {
        QVariantList partitions = Json::loadFromFile(folder+"/partitions.json").toMap().value("partitions").toList();
        if (partitions.isEmpty())
        {
            emit error(tr("partitions.json invalid"));
            return;
        }

        foreach (QVariant pv, partitions)
        {
            QVariantMap partition = pv.toMap();
            numparts++;
            if ( partition.value("want_maximised").toBool() )
                numexpandparts++;
            totalnominalsize += partition.value("partition_size_nominal").toInt();
            totaluncompressedsize += partition.value("uncompressed_tarball_size").toInt();
            if (partition.value("filesystem_type").toString() == "ext4")
            {
                totaluncompressedsize += /*0.035*/ 0.01 * totalnominalsize; /* overhead for file system meta data */
                if (partition.value("want_maximised").toBool())
                {
                    numext4expandparts++;
                }
            }
        }
        if (nameMatchesRiscOS(folder))
        {
            /* Check the riscos_offset in os.json matches what we're expecting.
               In theory we shouldn't hit either of these errors because the invalid RISC_OS
               should have been filtered out already (not added to OS-list) in mainwindow.cpp */
            QVariantMap vos = Json::loadFromFile(folder+"/os.json").toMap();
            if (vos.contains(RISCOS_OFFSET_KEY))
            {
                int riscos_offset = vos.value(RISCOS_OFFSET_KEY).toInt();
                if (riscos_offset != RISCOS_OFFSET)
                {
                    emit error(tr("RISCOS cannot be installed. RISCOS offset value mismatch."));
                    return;
                }
            }
            else
            {
                emit error(tr("RISCOS cannot be installed. RISCOS offset value missing."));
                return;
            }
            if (startSector > RISCOS_SECTOR_OFFSET-2048)
            {
                emit error(tr("RISCOS cannot be installed. Size of recovery partition too large."));
                return;
            }

            totalnominalsize += (RISCOS_SECTOR_OFFSET - startSector)/2048;
            RiscOSworkaround = true;
        }
    }

    /* 4 MB overhead per partition (logical partition table) */
    totalnominalsize += (numparts * 4);

    if (numexpandparts)
    {
        /* Extra spare space available for partitions that want to be expanded */
        _extraSpacePerPartition = (availableMB-totalnominalsize)/numexpandparts;
        /* Ext4 file system meta data overhead */
        //totaluncompressedsize += 0.035 * _extraSpacePerPartition * numext4expandparts;
    }

    emit parsedImagesize(qint64(totaluncompressedsize)*1024*1024);

    if (totalnominalsize > availableMB)
    {
        emit error(tr("Not enough disk space. Need %1 MB, got %2 MB").arg(QString::number(totalnominalsize), QString::number(availableMB)));
        return;
    }

    emit statusUpdate(tr("Clearing existing EBR"));
    clearEBR();

    /* Install RiscOS first */
    if (RiscOSworkaround)
    {
        for (QMultiMap<QString,QString>::const_iterator iter = _images.constBegin(); iter != _images.constEnd(); iter++)
        {
            if (nameMatchesRiscOS(iter.key()))
            {
                if (!processImage(iter.key(), iter.value()))
                    return;

                _images.remove(iter.key());
                break;
            }
        }
    }

    /* Process each image */
    for (QMultiMap<QString,QString>::const_iterator iter = _images.constBegin(); iter != _images.constEnd(); iter++)
    {
        if (!processImage(iter.key(), iter.value()))
            return;
    }

    emit statusUpdate(tr("Finish writing (sync)"));
    sync();
    emit completed();
}

bool MultiImageWriteThread::processImage(const QString &folder, const QString &flavour)
{

    int firstPartition = _part;

    QString os_name = (folder.split("/")).at(3);

    qDebug() << "Processing OS:" << os_name;

    QVariantList partitions = Json::loadFromFile(folder+"/partitions.json").toMap().value("partitions").toList();
    foreach (QVariant pv, partitions)
    {
        QVariantMap partition = pv.toMap();

        QByteArray fstype   = partition.value("filesystem_type").toByteArray();
        QByteArray mkfsopt  = partition.value("mkfs_options").toByteArray();
        QByteArray label = partition.value("label").toByteArray();
        QString tarball  = partition.value("tarball").toString();
        bool emptyfs     = partition.value("empty_fs", false).toBool();

        if (!emptyfs && tarball.isEmpty())
        {
            /* If no tarball URL is specified, we expect the tarball to reside in the folder and be named <label.tar.xz> */
            if (fstype == "raw")
                tarball = folder+"/"+label+".xz";
            else
                tarball = folder+"/"+label+".tar.xz";

            if (!QFile::exists(tarball))
            {
                emit error(tr("File '%1' does not exist").arg(tarball));
                return false;
            }
        }
        if (label.size() > 15)
        {
            label.clear();
        }
        else if (!isLabelAvailable(label))
        {
            for (int i=0; i<10; i++)
            {
                if (isLabelAvailable(label+QByteArray::number(i)))
                {
                    label = label+QByteArray::number(i);
                    break;
                }
            }
        }

        int partsizeMB = partition.value("partition_size_nominal").toInt();
        if (!partsizeMB)
        {
            emit error(tr("Nominal partition size not specified or zero"));
            return false;
        }

        if ( partition.value("want_maximised").toBool() )
            partsizeMB += _extraSpacePerPartition;

        QByteArray partdevice = "/dev/mmcblk0p"+QByteArray::number(_part);
        int partsizeSectors = partsizeMB * 2048;
        int parttype;
        int specialOffset = 0;

        if (fstype == "FAT" || fstype == "fat")
            parttype = 0x0c; /* FAT32 LBA */
        else if (fstype == "swap")
            parttype = 0x82;
        else
            parttype = 0x83; /* Linux native */

        if (nameMatchesRiscOS(folder) && (fstype == "FAT" || fstype == "fat"))
        {
            /* Let Risc OS start at known offset */
            int startSector   = getFileContents("/sys/class/block/mmcblk0p2/start").trimmed().toULongLong();
            specialOffset = RISCOS_SECTOR_OFFSET - startSector - 2048;
        }

        emit statusUpdate(tr("%1: Creating partition entry").arg(os_name));
        if (!addPartitionEntry(partsizeSectors, parttype, specialOffset))
            return false;

        if (fstype == "raw")
        {
            emit statusUpdate(tr("%1: Writing OS image").arg(os_name));
            if (!dd(tarball, partdevice))
                return false;
        }
        else
        {
            emit runningMKFS();
            emit statusUpdate(tr("%1: Creating filesystem (%2)").arg(os_name, QString(fstype)));
            if (!mkfs(partdevice, fstype, label, mkfsopt))
                return false;
            emit finishedMKFS();

            if (!emptyfs)
            {
                emit statusUpdate(tr("%1: Mounting file system").arg(os_name));
                if (QProcess::execute("mount "+partdevice+" /mnt2") != 0)
                {
                    emit error(tr("%1: Error mounting file system").arg(os_name));
                    return false;
                }

                if (tarball.startsWith("http"))
                    emit statusUpdate(tr("%1: Downloading and extracting filesystem").arg(os_name));
                else
                    emit statusUpdate(tr("%1: Extracting filesystem").arg(os_name));

                bool result = untar(tarball);

                QProcess::execute("umount /mnt2");

                if (!result)
                    return false;
            }
        }

        _part++;
    }

    emit statusUpdate(tr("%1: Mounting FAT partition").arg(os_name));
    if (QProcess::execute("mount /dev/mmcblk0p"+QString::number(firstPartition)+" /mnt2") != 0)
    {
        emit error(tr("%1: Error mounting file system").arg(os_name));
        return false;
    }

    emit statusUpdate(tr("%1: Creating os_config.json").arg(os_name));

    QString description = getDescription(folder, flavour);
    QVariantList vpartitions;
    for (int i=firstPartition; i<_part; i++)
    {
        vpartitions.append("/dev/mmcblk0p"+QString::number(i));
    }
    QSettings settings("/settings/noobs.conf", QSettings::IniFormat);
    int videomode = settings.value("display_mode", 0).toInt();
    QString language = settings.value("language", "en").toString();
    QString keyboard = settings.value("keyboard_layout", "gb").toString();

    QVariantMap vos = Json::loadFromFile(folder+"/os.json").toMap();
    QVariant releasedate = vos.value("release_date");

    QVariantMap qm;
    qm.insert("flavour", flavour);
    qm.insert("release_date", releasedate);
    qm.insert("imagefolder", folder);
    qm.insert("description", description);
    qm.insert("videomode", videomode);
    qm.insert("partitions", vpartitions);
    qm.insert("language", language);
    qm.insert("keyboard", keyboard);

    Json::saveToFile("/mnt2/os_config.json", qm);

    emit statusUpdate(tr("%1: Saving display mode to config.txt").arg(os_name));
    patchConfigTxt();

    /* Partition setup script can either reside in the image folder
     * or inside the boot partition tarball */
    QString postInstallScript = folder+"/partition_setup.sh";
    if (!QFile::exists(postInstallScript))
        postInstallScript = "/mnt2/partition_setup.sh";

    if (QFile::exists(postInstallScript))
    {
        emit statusUpdate(tr("%1: Running partition setup script").arg(os_name));
        QProcess proc;
        QProcessEnvironment env;
        QStringList args(postInstallScript);
        env.insert("PATH", "/bin:/usr/bin:/sbin:/usr/sbin");

        /* - Parameters to the partition-setup script are supplied both as
         *   command line parameters and set as environement variables
         * - Boot partition is mounted, working directory is set to its mnt folder
         *
         *  partition_setup.sh part1=/dev/mmcblk0p3 id1=LABEL=BOOT part2=/dev/mmcblk0p4
         *  id2=UUID=550e8400-e29b-41d4-a716-446655440000
         */
        for (int i=firstPartition, pcount = 1; i<_part; i++, pcount++)
        {
            QString part  = "/dev/mmcblk0p"+QString::number(i);
            QString nr    = QString::number(pcount);
            QString uuid  = getUUID(part);
            QString label = getLabel(part);
            QString id;
            if (!label.isEmpty())
                id = "LABEL="+label;
            else
                id = "UUID="+uuid;

            qDebug() << "part" << part << uuid << label;

            args << "part"+nr+"="+part << "id"+nr+"="+id;
            env.insert("part"+nr, part);
            env.insert("id"+nr, id);
        }

        qDebug() << "Executing: sh" << args;
        qDebug() << "Env:" << env.toStringList();
        proc.setProcessChannelMode(proc.MergedChannels);
        proc.setProcessEnvironment(env);
        proc.setWorkingDirectory("/mnt2");
        proc.start("/bin/sh", args);
        proc.waitForFinished(-1);
        qDebug() << proc.exitStatus();

        if (proc.exitCode() != 0)
        {
            emit error(tr("%1: Error executing partition setup script").arg(os_name)+"\n"+proc.readAll());
            return false;
        }
    }

emit statusUpdate(tr("%1: Unmounting FAT partition").arg(os_name));
    if (QProcess::execute("umount /mnt2") != 0)
    {
        emit error(tr("%1: Error unmounting").arg(os_name));
    }

    /* Save information about installed operating systems in installed_os.json */
    QVariantMap ventry;
    ventry["name"]        = flavour;
    ventry["description"] = description;
    ventry["folder"]      = folder;
    ventry["release_date"]= releasedate;
    ventry["partitions"]  = vpartitions;
    if (vos.contains("bootable"))
        ventry["bootable"] = vos.value("bootable").toBool();
    QString iconfilename = folder+"/"+flavour+".png";
    iconfilename.replace(" ", "_");
    if (QFile::exists(iconfilename))
        ventry["icon"] = iconfilename;
    else if (QFile::exists(folder+"/icon.png"))
        ventry["icon"] = folder+"/icon.png";
    installed_os.append(ventry);

    QProcess::execute("mount -o remount,rw /settings");
    Json::saveToFile("/settings/installed_os.json", installed_os);
    QProcess::execute("mount -o remount,ro /settings");

    return true;
}

void MultiImageWriteThread::clearEBR()
{
    mbr_table ebr;
    int startOfExtended = getFileContents("/sys/class/block/mmcblk0p2/start").trimmed().toULongLong();

    /* Write out empty extended partition table with signature */
    memset(&ebr, 0, sizeof ebr);
    ebr.signature[0] = 0x55;
    ebr.signature[1] = 0xAA;
    QFile f("/dev/mmcblk0");
    f.open(f.ReadWrite);
    f.seek(qint64(startOfExtended)*512);
    f.write((char *) &ebr, sizeof(ebr));
    f.close();
}

bool MultiImageWriteThread::addPartitionEntry(int sizeInSectors, int type, int specialOffset)
{
    /* Unmount everything before modifying partition table */
    //QProcess::execute("umount -r /mnt");
    //QProcess::execute("umount -r /settings");

    unsigned int startOfExtended = getFileContents("/sys/class/block/mmcblk0p2/start").trimmed().toULongLong();
    unsigned int offsetInSectors = 0;
    mbr_table ebr;
    QFile f("/dev/mmcblk0");
    if (!f.open(f.ReadWrite))
    {
        emit error(tr("Error opening /dev/mmcblk0 for writing"));
        return false;
    }

    /* Find last EBR */
    do
    {
        f.seek(qint64(startOfExtended+offsetInSectors)*512);
        f.read((char *) &ebr, sizeof(ebr));

        if (ebr.part[1].starting_sector)
        {
            if (ebr.part[1].starting_sector > offsetInSectors)
            {
                offsetInSectors = ebr.part[1].starting_sector;
            }
            else
            {
                emit error(tr("Internal error in partitioning"));
                return false;
            }
        }
    } while (ebr.part[1].starting_sector != 0);

    if (ebr.part[0].starting_sector)
    {
        /* Add reference to new EBR to old last EBR */
        ebr.part[1].starting_sector = offsetInSectors+ebr.part[0].starting_sector+ebr.part[0].nr_of_sectors;
        ebr.part[1].nr_of_sectors = sizeInSectors + specialOffset + 2048;
        ebr.part[1].id = 0x0F;

        f.seek(qint64(startOfExtended+offsetInSectors)*512);
        f.write((char *) &ebr, sizeof(ebr));
        offsetInSectors = ebr.part[1].starting_sector;
        qDebug() << "add tail";
    }

    memset(&ebr, 0, sizeof ebr);
    ebr.signature[0] = 0x55;
    ebr.signature[1] = 0xAA;

    if (specialOffset)
        ebr.part[0].starting_sector = 2048 + specialOffset;
    else
        ebr.part[0].starting_sector = ((((startOfExtended+offsetInSectors+specialOffset+2048)+6144)/8192)*8192) - (startOfExtended+offsetInSectors);

    ebr.part[0].nr_of_sectors = sizeInSectors;
    ebr.part[0].id = type;
    f.seek(qint64(startOfExtended+offsetInSectors)*512);
    f.write((char *) &ebr, sizeof(ebr));
    f.flush();
    /* Tell Linux to re-read the partition table */
    ioctl(f.handle(), BLKRRPART);
    f.close();

    /* Call partprobe just in case. BLKRRPART should be enough though */
    QProcess::execute("/usr/sbin/partprobe");
    QThread::msleep(500);

    /* Remount */
    //QProcess::execute("mount -o ro -t vfat /dev/mmcblk0p1 /mnt");
    //QProcess::execute("mount -t ext4 /dev/mmcblk0p3 /settings");

    return true;
}

bool MultiImageWriteThread::mkfs(const QByteArray &device, const QByteArray &fstype, const QByteArray &label, const QByteArray &mkfsopt)
{
    QString cmd;

    if (fstype == "fat" || fstype == "FAT")
    {
        cmd = "/sbin/mkdosfs ";
        if (!label.isEmpty())
        {
            cmd += "-n "+label+" ";
        }
    }
    else if (fstype == "ext4")
    {
        cmd = "/usr/sbin/mkfs.ext4 ";
        if (!label.isEmpty())
        {
            cmd += "-L "+label+" ";
        }
    }

    if (!mkfsopt.isEmpty())
        cmd += mkfsopt+" ";

    cmd += device;

    qDebug() << "Executing:" << cmd;
    QProcess p;
    p.setProcessChannelMode(p.MergedChannels);
    p.start(cmd);
    p.closeWriteChannel();
    p.waitForFinished(-1);

    if (p.exitCode() != 0)
    {
        emit error(tr("Error creating file system")+"\n"+p.readAll());
        return false;
    }

    return true;
}

bool MultiImageWriteThread::isLabelAvailable(const QByteArray &label)
{
    return (QProcess::execute("/sbin/findfs LABEL="+label) != 0);
}

bool MultiImageWriteThread::untar(const QString &tarball)
{
    QString cmd = "sh -o pipefail -c \"";

    if (tarball.startsWith("http:"))
        cmd += "wget --no-verbose --tries=inf -O- "+tarball+" | ";

    if (tarball.endsWith(".gz"))
    {
        cmd += "gzip -dc";
    }
    else if (tarball.endsWith(".xz"))
    {
        cmd += "xz -dc";
    }
    else if (tarball.endsWith(".bz2"))
    {
        cmd += "bzip2 -dc";
    }
    else if (tarball.endsWith(".lzo"))
    {
        cmd += "lzop -dc";
    }
    else if (tarball.endsWith(".zip"))
    {
        /* Note: the image must be the only file inside the .zip */
        cmd += "unzip -p";
    }
    else
    {
        emit error(tr("Unknown compression format file extension. Expecting .lzo, .gz, .xz, .bz2 or .zip"));
        return false;
    }
    if (!tarball.startsWith("http:"))
    {
        cmd += " "+tarball;
    }
    cmd += " | tar x -C /mnt2 ";
    cmd += "\"";

    QTime t1;
    t1.start();
    qDebug() << "Executing:" << cmd;

    QProcess p;
    p.setProcessChannelMode(p.MergedChannels);
    p.start(cmd);
    p.closeWriteChannel();
    p.waitForFinished(-1);

    if (p.exitCode() != 0)
    {
        QByteArray msg = p.readAll();
        qDebug() << msg;
        emit error(tr("Error downloading or extracting tarball")+"\n"+msg);
        return false;
    }
    qDebug() << "finished writing filesystem in" << (t1.elapsed()/1000.0) << "seconds";

    return true;
}

bool MultiImageWriteThread::dd(const QString &imagePath, const QString &device)
{
    QString cmd = "sh -o pipefail -c \"";

    if (imagePath.startsWith("http:"))
        cmd += "wget --no-verbose --tries=inf -O- "+imagePath+" | ";

    if (imagePath.endsWith(".gz"))
    {
        cmd += "gzip -dc";
    }
    else if (imagePath.endsWith(".xz"))
    {
        cmd += "xz -dc";
    }
    else if (imagePath.endsWith(".bz2"))
    {
        cmd += "bzip2 -dc";
    }
    else if (imagePath.endsWith(".lzo"))
    {
        cmd += "lzop -dc";
    }
    else if (imagePath.endsWith(".zip"))
    {
        /* Note: the image must be the only file inside the .zip */
        cmd += "unzip -p";
    }
    else
    {
        emit error(tr("Unknown compression format file extension. Expecting .lzo, .gz, .xz, .bz2 or .zip"));
        return false;
    }

    if (!imagePath.startsWith("http:"))
        cmd += " "+imagePath;

    cmd += " | dd of="+device+" conv=fsync obs=4M\"";

    QTime t1;
    t1.start();
    qDebug() << "Executing:" << cmd;

    QProcess p;
    p.setProcessChannelMode(p.MergedChannels);
    p.start(cmd);
    p.closeWriteChannel();
    p.waitForFinished(-1);

    if (p.exitCode() != 0)
    {
        emit error(tr("Error downloading or writing OS to SD card")+"\n"+p.readAll());
        return false;
    }
    qDebug() << "finished writing filesystem in" << (t1.elapsed()/1000.0) << "seconds";

    return true;
}

void MultiImageWriteThread::patchConfigTxt()
{

        QSettings settings("/settings/noobs.conf", QSettings::IniFormat);
        int videomode = settings.value("display_mode", 0).toInt();

        QByteArray dispOptions;

        switch (videomode)
        {
        case 0: /* HDMI PREFERRED */
            dispOptions = "hdmi_force_hotplug=1\r\nconfig_hdmi_boost=4\r\noverscan_left=24\r\noverscan_right=24\r\noverscan_top=16\r\noverscan_bottom=16\r\ndisable_overscan=0\r\n";
            break;
        case 1: /* HDMI VGA */
            dispOptions = "hdmi_ignore_edid=0xa5000080\r\nhdmi_force_hotplug=1\r\nconfig_hdmi_boost=4\r\nhdmi_group=2\r\nhdmi_mode=4\r\n";
            break;
        case 2: /* PAL */
            dispOptions = "hdmi_ignore_hotplug=1\r\nsdtv_mode=2\r\n";
            break;
        case 3: /* NTSC */
            dispOptions = "hdmi_ignore_hotplug=1\r\nsdtv_mode=0\r\n";
            break;
        }


        QFile f("/mnt2/config.txt");
        f.open(f.Append);
        f.write("\r\n# NOOBS Auto-generated Settings:\r\n"+dispOptions);
        f.close();

}

QByteArray MultiImageWriteThread::getLabel(const QString part)
{
    QByteArray result;
    QProcess p;
    p.start("/sbin/blkid -s LABEL -o value "+part);
    p.waitForFinished();

    if (p.exitCode() == 0)
        result = p.readAll().trimmed();

    return result;
}

QByteArray MultiImageWriteThread::getUUID(const QString part)
{
    QByteArray result;
    QProcess p;
    p.start("/sbin/blkid -s UUID -o value "+part);
    p.waitForFinished();

    if (p.exitCode() == 0)
        result = p.readAll().trimmed();

    return result;
}

QString MultiImageWriteThread::getDescription(const QString &folder, const QString &flavour)
{
    if (QFile::exists(folder+"/flavours.json"))
    {
        QVariantMap v = Json::loadFromFile(folder+"/flavours.json").toMap();
        QVariantList fl = v.value("flavours").toList();

        foreach (QVariant f, fl)
        {
            QVariantMap fm  = f.toMap();
            if (fm.value("name").toString() == flavour)
            {
                return fm.value("description").toString();
            }
        }
    }
    else if (QFile::exists(folder+"/os.json"))
    {
        QVariantMap v = Json::loadFromFile(folder+"/os.json").toMap();
        return v.value("description").toString();
    }

    return "";
}
