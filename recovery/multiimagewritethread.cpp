#include "multiimagewritethread.h"
#include "config.h"
#include "json.h"
#include "util.h"
#include "mbr.h"
#include "osinfo.h"
#include "partitioninfo.h"
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
#include <QtEndian>

MultiImageWriteThread::MultiImageWriteThread(QObject *parent) :
    QThread(parent), _extraSpacePerPartition(0), _part(5)
{
    QDir dir;

    if (!dir.exists("/mnt2"))
        dir.mkdir("/mnt2");
}

void MultiImageWriteThread::addImage(const QString &folder, const QString &flavour)
{
    _images.append(new OsInfo(folder, flavour, this));
    //_images.insert(folder, flavour);
}

void MultiImageWriteThread::run()
{
    /* Calculate space requirements, and check special requirements */
    int totalnominalsize = 0, totaluncompressedsize = 0, numparts = 0, numexpandparts = 0;
    int startSector = getFileContents("/sys/class/block/mmcblk0p5/start").trimmed().toULongLong()
                    + getFileContents("/sys/class/block/mmcblk0p5/size").trimmed().toULongLong();
    int totalSectors = getFileContents("/sys/class/block/mmcblk0/size").trimmed().toULongLong();
    int availableMB = (totalSectors-startSector)/2048;

    /* key: partition number, value: partition information */
    QMap<int, PartitionInfo *> partitionMap;

    foreach (OsInfo *image, _images)
    {
        QList<PartitionInfo *> *partitions = image->partitions();
        if (partitions->isEmpty())
        {
            emit error(tr("partitions.json invalid"));
            return;
        }

        if (nameMatchesRiscOS( image->folder() ))
        {
            /* Check the riscos_offset in os.json matches what we're expecting.
               In theory we shouldn't hit either of these errors because the invalid RISC_OS
               should have been filtered out already (not added to OS-list) in mainwindow.cpp */
            if (image->riscosOffset())
            {
                if (image->riscosOffset() != RISCOS_OFFSET)
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

            partitions->first()->setRequiresPartitionNumber(6);
            partitions->first()->setOffset(RISCOS_SECTOR_OFFSET);
            partitions->last()->setRequiresPartitionNumber(7);
        }

        foreach (PartitionInfo *partition, *partitions)
        {
            numparts++;
            if ( partition->wantMaximised() )
                numexpandparts++;
            totalnominalsize += partition->partitionSizeNominal();
            totaluncompressedsize += partition->uncompressedTarballSize();
            if (partition->fsType() == "ext4")
            {
                totaluncompressedsize += /*0.035*/ 0.01 * totalnominalsize; /* overhead for file system meta data */
            }
            int reqPart = partition->requiresPartitionNumber();
            if (reqPart)
            {
                if (partitionMap.contains(reqPart))
                {
                    emit error(tr("More than one operating system requires partition number %1").arg(reqPart));
                    return;
                }
                if (reqPart == 1 || reqPart == 2 || reqPart == 5)
                {
                    emit error(tr("Operating system cannot require a system partition (1,2,5)"));
                    return;
                }
                partition->setPartitionDevice("/dev/mmcblk0p"+QByteArray::number(reqPart));
                partitionMap.insert(reqPart, partition);
            }

            /* Maximum overhead per partition for alignment */
#ifdef SHRINK_PARTITIONS_TO_MINIMIZE_GAPS
            if (partition->wantMaximised() || (partition->partitionSizeNominal()*2048) % PARTITION_ALIGNMENT != 0)
                totalnominalsize += PARTITION_ALIGNMENT/2048;
#else
            totalnominalsize += PARTITION_ALIGNMENT/2048;
#endif
        }
    }

    if (numexpandparts)
    {
        /* Extra spare space available for partitions that want to be expanded */
        _extraSpacePerPartition = (availableMB-totalnominalsize)/numexpandparts;
    }

    emit parsedImagesize(qint64(totaluncompressedsize)*1024*1024);

    if (totalnominalsize > availableMB)
    {
        emit error(tr("Not enough disk space. Need %1 MB, got %2 MB").arg(QString::number(totalnominalsize), QString::number(availableMB)));
        return;
    }

    /* Assign logical partition numbers to partitions that did not reserve a special number */
    int pnr;
    if (partitionMap.isEmpty())
        pnr = 6;
    else
        pnr = qMax(partitionMap.keys().last(), 5)+1;

    foreach (OsInfo *image, _images)
    {
        foreach (PartitionInfo *partition, *(image->partitions()))
        {
            if (!partition->requiresPartitionNumber())
            {
                partitionMap.insert(pnr, partition);
                partition->setPartitionDevice("/dev/mmcblk0p"+QByteArray::number(pnr));
                pnr++;
            }
        }
    }

    /* Set partition starting sectors and sizes.
     * First allocate space to all logical partitions, then to primary partition 3 and 4 */
    QList<PartitionInfo *> log_before_prim = partitionMap.values();
    if (!log_before_prim.isEmpty() && log_before_prim.first()->requiresPartitionNumber() == 3)
        log_before_prim.push_back(log_before_prim.takeFirst());
    if (!log_before_prim.isEmpty() && log_before_prim.first()->requiresPartitionNumber() == 4)
        log_before_prim.push_back(log_before_prim.takeFirst());

    int offset = startSector;

    foreach (PartitionInfo *p, log_before_prim)
    {
        if (p->offset()) /* OS wants its partition at a fixed offset */
        {
            if (p->offset() <= offset)
            {
                emit error(tr("Fixed partition offset too low"));
                return;
            }

            offset = p->offset();
        }
        else
        {
            offset += PARTITION_GAP;
            /* Align at 4 MiB offset */
            if (offset % PARTITION_ALIGNMENT != 0)
            {
                    offset += PARTITION_ALIGNMENT-(offset % PARTITION_ALIGNMENT);
            }

            p->setOffset(offset);
        }

        int partsizeMB = p->partitionSizeNominal();
        if ( p->wantMaximised() )
            partsizeMB += _extraSpacePerPartition;
        int partsizeSectors = partsizeMB * 2048;

        if (p == log_before_prim.last())
        {
            /* Let last partition have any remaining space that we couldn't divide evenly */
            int spaceleft = totalSectors - offset - partsizeSectors;

            if (spaceleft > 0 && p->wantMaximised())
            {
                partsizeSectors += spaceleft;
            }
        }
        else
        {
#ifdef SHRINK_PARTITIONS_TO_MINIMIZE_GAPS
            if (partsizeSectors % PARTITION_ALIGNMENT == 0 && p->fsType() != "raw")
            {
                /* Partition size is dividable by 4 MiB
                   Take off a couple sectors of the end of our partition to make room
                   for the EBR of the next partition, so the next partition can
                   align nicely without having a 4 MiB gap */
                partsizeSectors -= PARTITION_GAP;
            }
#endif
            if (p->wantMaximised() && (partsizeSectors+PARTITION_GAP) % PARTITION_ALIGNMENT != 0)
            {
                /* Enlarge partition to close gap to next partition */
                partsizeSectors += PARTITION_ALIGNMENT-((partsizeSectors+PARTITION_GAP) % PARTITION_ALIGNMENT);
            }
        }

        p->setPartitionSizeSectors(partsizeSectors);
        offset += partsizeSectors;
    }

    /* Delete information about previously installed operating systems */
    QFile f("/settings/installed_os.json");
    if (f.exists())
        f.remove();

    emit statusUpdate(tr("Writing partition table"));
    if (!writePartitionTable(partitionMap))
        return;

    /* Zero out first sector of partitions, to make sure to get rid of previous file system (label) */
    emit statusUpdate(tr("Zero'ing start of each partition"));
    foreach (PartitionInfo *p, partitionMap.values())
    {
        if (p->partitionSizeSectors())
            QProcess::execute("/bin/dd count=1 bs=512 if=/dev/zero of="+p->partitionDevice());
    }

    /* Install each operating system */
    foreach (OsInfo *image, _images)
    {
        if (!processImage(image))
            return;
    }

    emit statusUpdate(tr("Finish writing (sync)"));
    sync();
    emit completed();
}

bool MultiImageWriteThread::writePartitionTable(const QMap<int, PartitionInfo *> &partitionMap)
{
    /* Write partition table using sfdisk */

    /* Fixed NOOBS partition */
    int startP1 = getFileContents("/sys/class/block/mmcblk0p1/start").trimmed().toInt();
    int sizeP1  = getFileContents("/sys/class/block/mmcblk0p1/size").trimmed().toInt();
    /* Fixed start of extended partition. End is not fixed, as it depends on primary partition 3 & 4 */
    int startP2 = getFileContents("/sys/class/block/mmcblk0p2/start").trimmed().toInt();
    /* Fixed settings partition */
    int startP5 = getFileContents("/sys/class/block/mmcblk0p5/start").trimmed().toInt();
    int sizeP5  = getFileContents("/sys/class/block/mmcblk0p5/size").trimmed().toInt();

    if (!startP1 || !sizeP1 || !startP2 || !startP5 || !sizeP5)
    {
        emit error(tr("Error reading existing partition table"));
        return false;
    }

    QByteArray partitionTable;
    partitionTable  = QByteArray::number(startP1)+","+QByteArray::number(sizeP1)+",0E\n"; /* FAT partition */

    /* Extended partition may not overlap P3 or P4 */
    int startP3P4 = 0;
    if (partitionMap.contains(3))
        startP3P4 = partitionMap.value(3)->offset();
    else if (partitionMap.contains(4))
        startP3P4 = partitionMap.value(4)->offset();

    if (startP3P4)
    {
        int sizeP2 = startP3P4 - startP2;
        partitionTable += QByteArray::number(startP2)+","+QByteArray::number(sizeP2)+",X\n";
    }
    else
    {
        /* No P3 nor P4. Extended can have all remaining space */
        partitionTable += QByteArray::number(startP2)+",,X\n";
    }

    /* Add partitions */
    qDebug() << "partition map:" << partitionMap;
    for (int i=3; i <= qMax(5, partitionMap.keys().last()); i++)
    {
        if (i == 5)
        {
            /* Settings partition */
            partitionTable += QByteArray::number(startP5)+","+QByteArray::number(sizeP5)+",L\n";
        }
        else if (partitionMap.contains(i))
        {
            PartitionInfo *p = partitionMap.value(i);

            partitionTable += QByteArray::number(p->offset())+","+QByteArray::number(p->partitionSizeSectors())+","+p->partitionType();
            if (p->active())
                partitionTable += " *";
            partitionTable += "\n";
        }
        else
        {
            partitionTable += "0,0\n";
        }
    }

    qDebug() << "New partition table:";
    qDebug() << partitionTable;

    /* Unmount everything before modifying partition table */
    QProcess::execute("umount /mnt");
    QProcess::execute("umount /settings");

    /* Let sfdisk write a proper partition table */
    QProcess proc;
    proc.setProcessChannelMode(proc.MergedChannels);
    proc.start("/sbin/sfdisk -uS /dev/mmcblk0");
    proc.write(partitionTable);
    proc.closeWriteChannel();
    proc.waitForFinished(-1);

    qDebug() << "sfdisk done, output:" << proc.readAll();
    ::sync();
    QThread::msleep(500);

    QProcess::execute("/usr/sbin/partprobe");
    QThread::msleep(500);

    /* Remount */
    QProcess::execute("mount -o ro -t vfat /dev/mmcblk0p1 /mnt");
    QProcess::execute("mount -t ext4 /dev/mmcblk0p5 /settings");

    if (proc.exitCode() != 0)
    {
        emit error(tr("Error creating partition table")+"\n"+proc.readAll());
        return false;
    }

    return true;
}

bool MultiImageWriteThread::processImage(OsInfo *image)
{
    QString os_name = image->name();
    qDebug() << "Processing OS:" << os_name;

    QList<PartitionInfo *> *partitions = image->partitions();
    if (partitions->isEmpty())
    {
        emit error(tr("No partitions defined in partitions.json"));
        return false;
    }

    foreach (PartitionInfo *p, *partitions)
    {
        QByteArray fstype   = p->fsType();
        QByteArray mkfsopt  = p->mkfsOptions();
        QByteArray label = p->label();
        QString tarball  = p->tarball();
        bool emptyfs     = p->emptyFS();

        if (!emptyfs && tarball.isEmpty())
        {
            /* If no tarball URL is specified, we expect the tarball to reside in the folder and be named <label.tar.xz> */
            if (fstype == "raw" || fstype.startsWith("partclone"))
                tarball = image->folder()+"/"+label+".xz";
            else
                tarball = image->folder()+"/"+label+".tar.xz";

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
        QByteArray partdevice = p->partitionDevice();

        if (fstype == "raw")
        {
            emit statusUpdate(tr("%1: Writing OS image").arg(os_name));
            if (!dd(tarball, partdevice))
                return false;
        }
        else if (fstype.startsWith("partclone"))
        {
            emit statusUpdate(tr("%1: Writing OS image").arg(os_name));
            if (!partclone_restore(tarball, partdevice))
                return false;
        }
        else if (fstype != "unformatted")
        {
            emit runningMKFS();
            emit statusUpdate(tr("%1: Creating filesystem (%2)").arg(os_name, QString(fstype)));
            if (!mkfs(partdevice, fstype, label, mkfsopt))
                return false;
            emit finishedMKFS();

            if (!emptyfs)
            {
                emit statusUpdate(tr("%1: Mounting file system").arg(os_name));
                QString mountcmd;
                if (fstype == "ntfs")
                    mountcmd = "/sbin/mount.ntfs-3g ";
                else
                    mountcmd = "mount ";
                if (QProcess::execute(mountcmd+partdevice+" /mnt2") != 0)
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

    QString description = getDescription(image->folder(), image->flavour());
    QVariantList vpartitions;
    foreach (PartitionInfo *p, *partitions)
    {
        vpartitions.append(p->partitionDevice());
    }
    QSettings settings("/settings/noobs.conf", QSettings::IniFormat);
    int videomode = settings.value("display_mode", 0).toInt();
    QString language = settings.value("language", "en").toString();
    QString keyboard = settings.value("keyboard_layout", "gb").toString();

    QByteArray fstypeFirstPartition = partitions->first()->fsType();
    bool firstPartitionIsFat = (fstypeFirstPartition == "FAT" || fstypeFirstPartition == "fat");
    bool firstPartitionIsBootable = image->bootable() && firstPartitionIsFat;

    if (firstPartitionIsBootable)
    {
        emit statusUpdate(tr("%1: Mounting FAT partition").arg(os_name));
        if (QProcess::execute("mount "+partitions->first()->partitionDevice()+" /mnt2") != 0)
        {
            emit error(tr("%1: Error mounting file system").arg(os_name));
            return false;
        }

        emit statusUpdate(tr("%1: Creating os_config.json").arg(os_name));

        QVariantMap qm;
        qm.insert("flavour", image->flavour());
        qm.insert("release_date", image->releaseDate());
        qm.insert("imagefolder", image->folder());
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
        QString postInstallScript = image->folder()+"/partition_setup.sh";
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
            int pnr = 1;
            foreach (PartitionInfo *p, *partitions)
            {
                QString part  = p->partitionDevice();
                QString nr    = QString::number(pnr);
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
                pnr++;
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

        PartitionInfo *p = partitions->first();
        if (!p->bcdFile().isEmpty())
        {
            emit statusUpdate(tr("%1: Patching BCD file").arg(os_name));
            patchBcdFile("/mnt2/"+p->bcdFile(), p->bcdDiskId(), p->bcdEfiSector(), p->bcdMainSector() );
        }

        emit statusUpdate(tr("%1: Unmounting FAT partition").arg(os_name));
        if (QProcess::execute("umount /mnt2") != 0)
        {
            emit error(tr("%1: Error unmounting").arg(os_name));
        }
    }

    /* Save information about installed operating systems in installed_os.json */
    QVariantMap ventry;
    ventry["name"]        = image->flavour();
    ventry["description"] = description;
    ventry["folder"]      = image->folder();
    ventry["release_date"]= image->releaseDate();
    ventry["partitions"]  = vpartitions;
    ventry["bootable"]    = firstPartitionIsBootable;
    QString iconfilename  = image->folder()+"/"+image->flavour()+".png";
    iconfilename.replace(" ", "_");
    if (QFile::exists(iconfilename))
        ventry["icon"] = iconfilename;
    else if (QFile::exists(image->folder()+"/icon.png"))
        ventry["icon"] = image->folder()+"/icon.png";
    installed_os.append(ventry);

    Json::saveToFile("/settings/installed_os.json", installed_os);

    return true;
}

bool MultiImageWriteThread::mkfs(const QByteArray &device, const QByteArray &fstype, const QByteArray &label, const QByteArray &mkfsopt)
{
    QString cmd;

    if (fstype == "fat" || fstype == "FAT")
    {
        cmd = "/sbin/mkfs.fat ";
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
    else if (fstype == "ntfs")
    {
        cmd = "/sbin/mkfs.ntfs --fast ";
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

bool MultiImageWriteThread::partclone_restore(const QString &imagePath, const QString &device)
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

    cmd += " | partclone.restore -q -s - -o "+device+" \"";

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

quint32 MultiImageWriteThread::getDiskId(const QString &device)
{
    mbr_table mbr;

    QFile f(device);
    f.open(f.ReadOnly);
    f.read((char *) &mbr, sizeof(mbr));
    f.close();

    return qFromLittleEndian<quint32>(mbr.diskid);
}

bool MultiImageWriteThread::patchBcdFile(const QByteArray bcdfile, unsigned int origDiskId, unsigned int origEfiSector, unsigned int origMainSector)
{
    QByteArray data;
    QFile f(bcdfile);

    quint64 origOffsetEfi  = origEfiSector * 512;
    quint64 origOffsetMain = origMainSector * 512;
    quint32 newDiskId = getDiskId();
    quint64 newOffsetEfi  = getFileContents("/sys/class/block/mmcblk0p3/start").trimmed().toULongLong() * 512;
    quint64 newOffsetMain = getFileContents("/sys/class/block/mmcblk0p4/start").trimmed().toULongLong() * 512;
    QByteArray search32(4, '\0'), replace32(4, '\0'), search64(8, '\0'), replace64(8, '\0');

    f.open(f.ReadWrite);
    data = f.readAll();

    /* Search and replace partition offsets and disk IDs in binary BCD file */
    qToLittleEndian<quint32>(origDiskId, (uchar *) search32.data());
    qToLittleEndian<quint32>(newDiskId, (uchar *) replace32.data());
    data.replace(search32, replace32);

    qToLittleEndian<quint64>(origOffsetEfi, (uchar *) search64.data());
    qToLittleEndian<quint64>(newOffsetEfi, (uchar *) replace64.data());
    data.replace(search64, replace64);

    qToLittleEndian<quint64>(origOffsetMain, (uchar *) search64.data());
    qToLittleEndian<quint64>(newOffsetMain, (uchar *) replace64.data());
    data.replace(search64, replace64);

    f.seek(0);
    f.write(data);
    f.close();

    return true;
}
