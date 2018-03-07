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
#include <linux/magic.h>
#include <sys/statfs.h>
#include <sys/ioctl.h>
#include <QtEndian>

MultiImageWriteThread::MultiImageWriteThread(const QString &bootdrive, const QString &rootdrive, QObject *parent) :
    QThread(parent), _drive(rootdrive), _bootdrive(bootdrive), _extraSpacePerPartition(0), _part(5)
{
    QDir dir;
    _multiDrives = (bootdrive != rootdrive);

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
    int startSector = getFileContents(sysclassblock(_drive, 5)+"/start").trimmed().toULongLong()
                    + getFileContents(sysclassblock(_drive, 5)+"/size").trimmed().toULongLong();
    int totalSectors = getFileContents(sysclassblock(_drive)+"/size").trimmed().toULongLong();
    int availableMB = (totalSectors-startSector)/2048;

    /* key: partition number, value: partition information */
    QMap<int, PartitionInfo *> partitionMap, bootPartitionMap;

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
                if (reqPart == 1 || reqPart == 5)
                {
                    emit error(tr("Operating system cannot require a system partition (1,5)"));
                    return;
                }
                if ((reqPart == 2 && partitionMap.contains(4)) || (reqPart == 4 && partitionMap.contains(2)))
                {
                    emit error(tr("Operating system cannot claim both primary partitions 2 and 4"));
                    return;
                }

                partition->setPartitionDevice(partdev(_drive, reqPart));
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
    int pnr, bootpnr, offset = 0;
    if (partitionMap.isEmpty())
        pnr = 6;
    else
        pnr = qMax(partitionMap.keys().last(), 5)+1;

    if (_multiDrives)
    {
        bootpnr = 6;
        offset = getFileContents(sysclassblock(_bootdrive, 5)+"/start").trimmed().toULongLong()
               + getFileContents(sysclassblock(_bootdrive, 5)+"/size").trimmed().toULongLong();
    }

    foreach (OsInfo *image, _images)
    {
        foreach (PartitionInfo *partition, *(image->partitions()))
        {
            if (!partition->requiresPartitionNumber())
            {
                if (_multiDrives && partition->bootable() && !partition->wantMaximised() )
                {
                    bootPartitionMap.insert(bootpnr, partition);
                    partition->setPartitionDevice(partdev(_bootdrive, bootpnr));
                    bootpnr++;
                    offset += PARTITION_GAP;

                    /* Align at 4 MiB offset */
                    if (offset % PARTITION_ALIGNMENT != 0)
                    {
                            offset += PARTITION_ALIGNMENT-(offset % PARTITION_ALIGNMENT);
                    }
                    partition->setOffset(offset);
                    int partsizeSectors = partition->partitionSizeNominal() * 2048;
                    partition->setPartitionSizeSectors(partsizeSectors);
                    offset += partsizeSectors;
                }
                else
                {
                    partitionMap.insert(pnr, partition);
                    partition->setPartitionDevice(partdev(_drive, pnr));
                    pnr++;
                }
            }
        }
    }

    /* Set partition starting sectors and sizes.
     * First allocate space to all logical partitions, then to primary partitions */
    QList<PartitionInfo *> log_before_prim = partitionMap.values();
    if (!log_before_prim.isEmpty() && log_before_prim.first()->requiresPartitionNumber() == 2)
        log_before_prim.push_back(log_before_prim.takeFirst());
    if (!log_before_prim.isEmpty() && log_before_prim.first()->requiresPartitionNumber() == 3)
        log_before_prim.push_back(log_before_prim.takeFirst());
    if (!log_before_prim.isEmpty() && log_before_prim.first()->requiresPartitionNumber() == 4)
        log_before_prim.push_back(log_before_prim.takeFirst());

    offset = startSector;

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
    if (!writePartitionTable(_drive, partitionMap))
        return;

    /* Zero out first sector of partitions, to make sure to get rid of previous file system (label) */
    emit statusUpdate(tr("Zero'ing start of each partition"));
    foreach (PartitionInfo *p, partitionMap.values())
    {
        if (p->partitionSizeSectors())
            QProcess::execute("/bin/dd count=3 bs=512 if=/dev/zero of="+p->partitionDevice());
    }

    /* Write partition table to boot drive (if using multiple drives) */
    if (_multiDrives)
    {
        emit statusUpdate(tr("Writing boot partition table"));

        if (!writePartitionTable(_bootdrive, bootPartitionMap))
            return;

        /* Zero out first sector of partitions, to make sure to get rid of previous file system (label) */
        emit statusUpdate(tr("Zero'ing start of each partition"));
        foreach (PartitionInfo *p, bootPartitionMap.values())
        {
            if (p->partitionSizeSectors())
                QProcess::execute("/bin/dd count=3 bs=512 if=/dev/zero of="+p->partitionDevice());
        }

        if (QProcess::execute("mount -t ext4 "+partdev(_bootdrive, SETTINGS_PARTNR)+" /mnt2") == 0)
        {
            QFile f("/mnt2/installed_os.json");
            if (f.exists())
                f.remove();

            QProcess::execute("umount /mnt2");
        }
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

bool MultiImageWriteThread::writePartitionTable(const QString &drive, const QMap<int, PartitionInfo *> &pmap)
{
    /* Write partition table using sfdisk */

    /* Fixed NOOBS partition */
    int startP1 = getFileContents(sysclassblock(drive, 1)+"/start").trimmed().toInt();
    int sizeP1  = getFileContents(sysclassblock(drive, 1)+"/size").trimmed().toInt();
    /* Fixed start of extended partition. End is not fixed, as it depends on primary partition 3 & 4 */
    int startExtended = startP1+sizeP1;
    /* Fixed settings partition */
    int startP5 = getFileContents(sysclassblock(drive, SETTINGS_PARTNR)+"/start").trimmed().toInt();
    int sizeP5  = getFileContents(sysclassblock(drive, SETTINGS_PARTNR)+"/size").trimmed().toInt();

    if (!startP1 || !sizeP1 || !startP5 || !sizeP5)
    {
        emit error(tr("Error reading existing partition table"));
        return false;
    }

    /* Clone partition map, and add our system partitions to it */
    QMap<int, PartitionInfo *> partitionMap(pmap);

    partitionMap.insert(1, new PartitionInfo(1, startP1, sizeP1, "0E", this)); /* FAT boot partition */
    partitionMap.insert(5, new PartitionInfo(5, startP5, sizeP5, "L", this)); /* Ext4 settings partition */

    int sizeExtended = partitionMap.values().last()->endSector() - startExtended;
    if (!partitionMap.contains(2))
    {
        partitionMap.insert(2, new PartitionInfo(2, startExtended, sizeExtended, "E", this));
    }
    else
    {
        /* If an OS already claimed primary partition 2, use out-of-order partitions, and store extended at partition 4 */
        partitionMap.insert(4, new PartitionInfo(4, startExtended, sizeExtended, "E", this));
    }

    /* Add partitions */
    qDebug() << "partition map:" << partitionMap;

    QByteArray partitionTable;
    for (int i=1; i <= partitionMap.keys().last(); i++)
    {
        if (partitionMap.contains(i))
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
    QString driveP1 = partdev(drive, 1).replace("/dev/", "");
    if (drive == _bootdrive)
    {
        QProcess::execute("umount /mnt");
        QProcess::execute("umount /settings");
    }
    if (QFile::exists("/tmp/media/"+driveP1))
    {
        QProcess::execute("umount /tmp/media/"+driveP1);
    }

    /* Let sfdisk write a proper partition table */
    QProcess proc;
    proc.setProcessChannelMode(proc.MergedChannels);
    proc.start("/sbin/sfdisk -uS --force "+drive);
    proc.write(partitionTable);
    proc.closeWriteChannel();
    proc.waitForFinished(-1);

    qDebug() << "sfdisk done, output:" << proc.readAll();
    ::sync();
    QThread::msleep(500);

    QProcess::execute("/usr/sbin/partprobe");
    QThread::msleep(500);

    /* Remount */
    if (drive == _bootdrive)
    {
        QProcess::execute("mount -o ro -t vfat "+partdev(drive, 1)+" /mnt");
        QProcess::execute("mount -t ext4 "+partdev(drive, SETTINGS_PARTNR)+" /settings");
    }
    if (QFile::exists("/tmp/media/"+driveP1))
    {
        QProcess::execute("mount -o ro -t vfat /dev/"+driveP1+" /tmp/media/"+driveP1);
    }

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

        QByteArray partdevice = p->partitionDevice();
        emit newDrive(partdevice);

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

    emit statusUpdate(tr("%1: Mounting FAT partition").arg(os_name));
    if (QProcess::execute("mount "+partitions->first()->partitionDevice()+" /mnt2") != 0)
    {
        emit error(tr("%1: Error mounting file system").arg(os_name));
        return false;
    }

    if (QFile::exists("/mnt/firmware.override"))
    {
        if (::system("cp /mnt/firmware.override/* /mnt2") != 0) { }
    }

    emit statusUpdate(tr("%1: Creating os_config.json").arg(os_name));

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
            QString partuuid = getPartUUID(part);
            QString id;
            if (!label.isEmpty())
                id = "LABEL="+label;
            else
                id = "UUID="+uuid;
            if (_drive != "/dev/mmcblk0")
                part = partuuid;

            qDebug() << "part" << part << uuid << label;

            args << "part"+nr+"="+part << "id"+nr+"="+id;
            env.insert("part"+nr, part);
            env.insert("id"+nr, id);
            env.insert("partuuid"+nr, partuuid);
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

    emit statusUpdate(tr("%1: Unmounting FAT partition").arg(os_name));
    if (QProcess::execute("umount /mnt2") != 0)
    {
        emit error(tr("%1: Error unmounting").arg(os_name));
    }

    /* Save information about installed operating systems in installed_os.json */
    QVariantMap ventry;
    ventry["name"]        = image->flavour();
    ventry["description"] = description;
    ventry["folder"]      = image->folder();
    ventry["release_date"]= image->releaseDate();
    ventry["partitions"]  = vpartitions;
    ventry["bootable"]    = image->bootable();
    if (!image->supportedModels().isEmpty())
        ventry["supported_models"] = image->supportedModels();
    QString iconfilename  = image->folder()+"/"+image->flavour()+".png";
    iconfilename.replace(" ", "_");
    if (QFile::exists(iconfilename))
    {
        if (iconfilename.startsWith("/tmp/media/"))
        {
            /* Copy icon to settings folder, as USB storage may take longer to get ready on boot */
            QDir dir;
            QString dirname = "/settings/os/"+image->flavour().replace(" ", "_");
            dir.mkpath(dirname);
            QFile::copy(iconfilename, dirname+"/icon.png");
            iconfilename = dirname+"/icon.png";
        }

        ventry["icon"] = iconfilename;
    }
    else if (QFile::exists(image->folder()+"/icon.png"))
        ventry["icon"] = image->folder()+"/icon.png";
    installed_os.append(ventry);

    Json::saveToFile("/settings/installed_os.json", installed_os);

    return true;
}

QString MultiImageWriteThread::shorten(QString example, int maxLabelLen)
{
    QString test;
    if (example.size()<=maxLabelLen)
    {
        return(example);
    }
    example.replace("_","#");
    example.replace("-","#");
    example.replace(" ","#");

    QStringList parts = example.split("#", QString::SkipEmptyParts);
    int numParts = qMin(3, parts.count());
    int r;
    int len;
    int l1,l2;
    int rem;
    switch (numParts)
    {
        case 3:
            len=parts.last().size();
            r=qMin((maxLabelLen-4),len);
            rem = maxLabelLen -r-2;
            l2 = rem/2;
            l1 = rem-l2;
            test= parts.first().left(l1)+"_"+parts.at(1).right(l2)+"_"+parts.last().left(r);
            break;

        case 2:
            len=parts.last().size();
            r=qMin(maxLabelLen-2, len);
            test = parts.first().left(maxLabelLen-r-1) + "_" + parts.last().left(r);
            break;

        default:
            test = parts.first();
            test=test.left(maxLabelLen);
            break;
    }
    return(test);
}

QByteArray MultiImageWriteThread::makeLabelUnique(QByteArray label, int maxLabelLen)
{
    if (label.size() > maxLabelLen)
    {   //restrict to maximum size
        label = shorten(label, maxLabelLen).toAscii();
    }

    if (!isLabelAvailable(label))
    {
        if (label.size() == maxLabelLen)
        {   //Make room for extra digit
            label = label.left(maxLabelLen-1);
        }
        for (int i=0; i<10; i++)
        {
            if (isLabelAvailable(label+QByteArray::number(i)))
            {
                label = label+QByteArray::number(i);
                return(label);
            }
        }
        //Let's add some more now that we can have 56 OSes on a USB installed!
        for (char c='A'; c<='Z'; c++)
        {
            if (isLabelAvailable(label+c))
            {
                label = label+c;
                return(label);
            }
        }
        //No hope if we get to here
        label="";
    }
    return (label);
}

bool MultiImageWriteThread::mkfs(const QByteArray &device, const QByteArray &fstype, const QByteArray &label, const QByteArray &mkfsopt)
{
    QString cmd;

    if (fstype == "fat" || fstype == "FAT")
    {
        cmd = "/sbin/mkfs.fat ";
        if (!label.isEmpty())
        {
            cmd += "-n "+makeLabelUnique(label, 11)+" ";
        }
    }
    else if (fstype == "ext4")
    {
        cmd = "/usr/sbin/mkfs.ext4 ";
        if (!label.isEmpty())
        {
            cmd += "-L "+makeLabelUnique(label, 16)+" ";
        }
    }
    else if (fstype == "ntfs")
    {
        cmd = "/sbin/mkfs.ntfs --fast ";
        if (!label.isEmpty())
        {
            cmd += "-L "+makeLabelUnique(label, 32)+" ";
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

    if (isURL(tarball))
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
    if (!isURL(tarball))
    {
        cmd += " "+tarball;
    }

    cmd += " | bsdtar -xf - -C /mnt2 ";

    struct statfs st;
    if (statfs("/mnt2", &st) == 0 && st.f_type == MSDOS_SUPER_MAGIC)
    {
        /* File system does not support uid/gid, tell bsdtar not to set those or it will error out */
        cmd += " --no-same-owner ";
    }

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

    if (isURL(imagePath))
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

    if (!isURL(imagePath))
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

    if (isURL(imagePath))
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

    if (!isURL(imagePath))
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
            dispOptions = "hdmi_force_hotplug=1\r\n";
            break;
        case 1: /* HDMI VGA */
            dispOptions = "hdmi_ignore_edid=0xa5000080\r\nhdmi_force_hotplug=1\r\nhdmi_group=2\r\nhdmi_mode=4\r\n";
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

bool MultiImageWriteThread::isURL(const QString &s)
{
    return s.startsWith("http:") || s.startsWith("https:");
}

QByteArray MultiImageWriteThread::getDiskId(const QString &device)
{
    mbr_table mbr;

    QFile f(device);
    f.open(f.ReadOnly);
    f.read((char *) &mbr, sizeof(mbr));
    f.close();

    quint32 diskid = qFromLittleEndian<quint32>(mbr.diskid);
    return QByteArray::number(diskid, 16).rightJustified(8, '0');;
}

QByteArray MultiImageWriteThread::getPartUUID(const QString &devpart)
{
    QByteArray r;

    QRegExp partnrRx("([0-9]+)$");
    if (partnrRx.indexIn(devpart) != -1)
    {
        QString drive = devpart.left(partnrRx.pos());
        if (drive.endsWith("p"))
            drive.chop(1);

        r = "PARTUUID="+getDiskId(drive);
        int partnr = partnrRx.cap(1).toInt();
        QByteArray partnrstr = QByteArray::number(partnr, 16).rightJustified(2, '0');
        r += '-'+partnrstr;
    }

    return r;
}
