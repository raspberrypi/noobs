/* Prepare SD card for first use thread
 *
 * Initial author: Floris Bos
 * Maintained by Raspberry Pi
 *
 * See LICENSE.txt for license details
 *
 */

#include "initdrivethread.h"
#include "mbr.h"
#include "util.h"
#include <QProcess>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QMessageBox>
#include <unistd.h>
#include <linux/fs.h>
#include <sys/ioctl.h>

InitDriveThread::InitDriveThread(QObject *parent) :
    QThread(parent)
{
}

void InitDriveThread::run()
{
    QDir dir;

    emit statusUpdate("Waiting for SD card to be ready");
    while (!QFile::exists("/dev/mmcblk0"))
    {
        QThread::usleep(100);
    }

    emit statusUpdate(tr("Mounting FAT partition"));
    mountSystemPartition();

    //if (sizeofBootFilesInKB() > (MAXIMUM_BOOTFILES_SIZE*1024))
    //{
        //emit error(tr("SD card contains extra files that do not belong to this distribution. Please copy them to another disk and delete them from card."));
        //return;

        // Try to resize existing partitions
        if (!method_resizePartitions())
        {
            return;
        }
//}
//  else
//  {
        //      // Reformat the drive
        //if (!method_reformatDrive())
            //{
            //return;
            //      }
// }

    emit statusUpdate(tr("Formatting settings partition"));
    if (!formatSettingsPartition())
    {
        emit error(tr("Error formatting settings partition"));
    }

    dir.mkdir("/mnt/os");
    emit statusUpdate(tr("Editing cmdline.txt"));

    QString cmdlinefilename = "/mnt/recovery.cmdline";
    if (!QFile::exists(cmdlinefilename))
        cmdlinefilename = "/mnt/cmdline.txt";

    /* Remove "runinstaller" from cmdline.txt */
    QFile f(cmdlinefilename);
    if (!f.open(f.ReadOnly))
    {
        emit error(tr("Error opening %1").arg(cmdlinefilename));
        return;
    }
    QByteArray line = f.readAll().trimmed();
    line = line.replace("runinstaller", "").trimmed();
    f.close();
    f.open(f.WriteOnly);
    f.write(line);
    f.close();

#ifdef RISCOS_BLOB_FILENAME
    if (QFile::exists(RISCOS_BLOB_FILENAME))
    {
        emit statusUpdate(tr("Writing RiscOS blob"));
        if (!writeRiscOSblob())
        {
            emit error(tr("Error writing RiscOS blob"));
            return;
        }
    }
#endif

    /* Finish writing */
    emit statusUpdate(tr("Unmounting boot partition"));
    umountSystemPartition();

    emit statusUpdate(tr("Finish writing to disk (sync)"));
    sync();

    /* Perform a quick test to verify our changes were written
     * Drop page cache to make sure we are reading from card, and not from cache */
    QFile dc("/proc/sys/vm/drop_caches");
    dc.open(f.WriteOnly);
    dc.write("3\n");
    dc.close();

    emit statusUpdate(tr("Mounting boot partition again"));
    mountSystemPartition();

    /* Verify that cmdline.txt was written correctly */
    f.open(f.ReadOnly);
    QByteArray cmdlineread = f.readAll();
    f.close();
    umountSystemPartition();

    if (cmdlineread != line)
    {
        emit error(tr("SD card broken (writes do not persist)"));
        return;
    }

    emit completed();
}

bool InitDriveThread::method_reformatDrive()
{
    emit statusUpdate(tr("Saving boot files to memory"));
    if (!saveBootFiles() )
    {
        emit error(tr("Error saving boot files to memory. SD card may be damaged."));
        return false;
    }
    if (!umountSystemPartition())
    {
        emit error(tr("Error unmounting system partition."));
        return false;
    }

    emit statusUpdate(tr("Zeroing partition table"));
    if (!zeroMbr())
    {
        emit error(tr("Error zero'ing MBR/GPT. SD card may be broken or advertising wrong capacity."));
        return false;
    }

    emit statusUpdate(tr("Creating partitions"));

    if (!partitionDrive())
    {
        emit error(tr("Error partitioning"));
        return false;
    }

    emit statusUpdate(tr("Formatting boot partition (fat)"));
    if (!formatBootPartition())
    {
       emit error(tr("Error formatting boot partition (fat)"));
       return false;
    }

    emit statusUpdate(tr("Copying boot files to storage"));
    mountSystemPartition();
    restoreBootFiles();

    return true;
}

bool InitDriveThread::method_resizePartitions()
{
    int newStartOfRescuePartition = getFileContents("/sys/class/block/mmcblk0p1/start").trimmed().toInt();
    int newSizeOfRescuePartition  = sizeofBootFilesInKB()/1000 + 100;

    if (!umountSystemPartition())
    {
        emit error(tr("Error unmounting system partition."));
        return false;
    }

    if (!QFile::exists("/dev/mmcblk0p1"))
    {
        // SD card does not have a MBR.

        // Warn user that their SD card does not have an MBR and ask
        // if they would like us to create one for them
        QMessageBox::StandardButton answer;
        emit query(tr("Would you like NOOBS to create one for you?\nWARNING: This will erase all data on your SD card"),
                   tr("Error: No MBR present on SD Card"),
                   &answer);

        if(answer == QMessageBox::Yes)
        {
            emit statusUpdate(tr("Zeroing partition table"));
            if (!zeroMbr())
            {
                emit error(tr("Error zero'ing MBR/GPT. SD card may be broken or advertising wrong capacity."));
                return false;
            }

            // Create MBR containing single FAT partition
            emit statusUpdate(tr("Writing new MBR"));
            QProcess proc;
            proc.setProcessChannelMode(proc.MergedChannels);
            proc.start("/usr/sbin/parted /dev/mmcblk0 --script -- mktable msdos mkpartfs primary fat32 8192s -1");
            proc.waitForFinished(-1);
            if (proc.exitCode() != 0)
            {
                // Warn user if we failed to create an MBR on their card
                emit error(tr("Error creating MBR")+"\n"+proc.readAll());
                return false;
            }
            qDebug() << "Created missing MBR on SD card. parted output:" << proc.readAll();

            // Advise user that their SD card has now been formatted
            // suitably for installing NOOBS and that they will have to
            // re-copy the files before rebooting
            emit error(tr("SD card has now been formatted ready for NOOBS installation. Please re-copy the NOOBS files onto the card and reboot"));
            return false;
        }
        else
        {
            emit error(tr("SD card has not been formatted correctly. Please reformat using the SD Association Formatting Tool and try again."));
            return false;
        }

    }

    emit statusUpdate(tr("Removing partitions 2,3,4"));

    QFile f("/dev/mmcblk0");
    f.open(f.ReadWrite);
    // Seek to partition entry 2
    f.seek(462);
    // Zero out partition 2,3,4 to prevent parted complaining about invalid constraints
    f.write(QByteArray(16*3, '\0'));
    f.flush();
    // Tell Linux to re-read the partition table
    ioctl(f.handle(), BLKRRPART);
    f.close();
    QThread::msleep(500);

    emit statusUpdate(tr("Resizing FAT partition"));

    /* Relocating the start of the FAT partition is a write intensive operation
     * only move it when it is not aligned on a MiB boundary already */
    if (newStartOfRescuePartition < 2048 || newStartOfRescuePartition % 2048 != 0)
    {
        newStartOfRescuePartition = 8192; /* 4 MiB */
    }

    QString cmd = "/usr/sbin/parted --script /dev/mmcblk0 resize 1 "+QString::number(newStartOfRescuePartition)+"s "+QString::number(newSizeOfRescuePartition)+"M";
    qDebug() << "Executing" << cmd;
    QProcess p;
    p.setProcessChannelMode(p.MergedChannels);
    p.start(cmd);
    p.closeWriteChannel();
    p.waitForFinished(-1);

    if (p.exitCode() != 0)
    {
        emit error(tr("Error resizing existing FAT partition")+"\n"+p.readAll());
        return false;
    }
    qDebug() << "parted done, output:" << p.readAll();
    QThread::msleep(500);

    emit statusUpdate(tr("Creating extended partition"));

    mbr_table extended_mbr;
    QByteArray partitionTable;
    int startOfOurPartition = getFileContents("/sys/class/block/mmcblk0p1/start").trimmed().toInt();
    int sizeOfOurPartition  = getFileContents("/sys/class/block/mmcblk0p1/size").trimmed().toInt();
    int startOfExtended = startOfOurPartition+sizeOfOurPartition;
    // Align on 4 MiB boundary
    startOfExtended += 8192-(startOfExtended % 8192);

    int sizeOfDisk = getFileContents("/sys/class/block/mmcblk0/size").trimmed().toULongLong();
    int sizeOfExtended = sizeOfDisk - startOfExtended - SETTINGS_PARTITION_SIZE ;
    int startOfSettings = startOfExtended+sizeOfExtended;

    partitionTable  = QByteArray::number(startOfOurPartition)+","+QByteArray::number(sizeOfOurPartition)+",0E\n"; /* FAT partition */
    partitionTable += QByteArray::number(startOfExtended)+","+QByteArray::number(sizeOfExtended)+",X\n"; /* Extended partition with all remaining space */
    partitionTable += QByteArray::number(startOfSettings)+",,L\n"; /* Settings partition */
    partitionTable += "0,0\n";
    qDebug() << "Writing partition table" << partitionTable;

    /* Write out empty extended partition table with signature */
    memset(&extended_mbr, 0, sizeof extended_mbr);
    extended_mbr.signature[0] = 0x55;
    extended_mbr.signature[1] = 0xAA;
    f.open(f.ReadWrite);
    f.seek(startOfExtended*512);
    f.write((char *) &extended_mbr, sizeof(extended_mbr));
    f.close();

    /* Let sfdisk write a proper partition table */
    cmd = QString("/sbin/sfdisk -uS /dev/mmcblk0");
    QProcess proc;
    proc.setProcessChannelMode(proc.MergedChannels);
    proc.start(cmd);
    proc.write(partitionTable);
    proc.closeWriteChannel();
    proc.waitForFinished(-1);
    if (proc.exitCode() != 0)
    {
        emit error(tr("Error creating extended partition")+"\n"+proc.readAll());
        return false;
    }
    qDebug() << "sfdisk done, output:" << proc.readAll();
    QThread::msleep(500);

    /* For reasons unknown Linux sometimes
     * only finds /dev/mmcblk0p2 and /dev/mmcblk0p1 goes missing */
    if (!QFile::exists("/dev/mmcblk0p1"))
    {
        /* Probe again */
        QProcess::execute("/usr/sbin/partprobe");
        QThread::msleep(1500);
    }

    QProcess::execute("/sbin/mlabel p:RECOVERY");

    emit statusUpdate(tr("Mounting FAT partition"));
    if (!mountSystemPartition())
    {
        emit error(tr("Error mounting system partition."));
        return false;
    }

    return true;
}

bool InitDriveThread::saveBootFiles()
{
    return QProcess::execute("cp -a /mnt /tmp") == 0;
}

bool InitDriveThread::restoreBootFiles()
{
    bool status = QProcess::execute("cp -a /tmp/mnt /") == 0;
    QProcess::execute("rm -rf /tmp/mnt");
    return status;
}

int InitDriveThread::sizeofBootFilesInKB()
{
    QProcess proc;
    proc.start("du -s /mnt");
    proc.waitForFinished();
    return proc.readAll().split('\t').first().toInt();
}

int InitDriveThread::sizeofSDCardInBlocks()
{
    QFile f("/sys/class/block/mmcblk0/size");
    f.open(f.ReadOnly);
    int blocks = f.readAll().trimmed().toULongLong();
    f.close();

    return blocks;
}

bool InitDriveThread::mountSystemPartition()
{
    return QProcess::execute("mount /dev/mmcblk0p1 /mnt") == 0 || QProcess::execute("mount /dev/mmcblk0 /mnt") == 0;
}

bool InitDriveThread::umountSystemPartition()
{
    return QProcess::execute("umount /mnt") == 0;
}

bool InitDriveThread::formatBootPartition()
{
    return QProcess::execute("/sbin/mkdosfs -n RECOVERY /dev/mmcblk0p1") == 0;
}

bool InitDriveThread::formatSettingsPartition()
{
    return QProcess::execute("/usr/sbin/mkfs.ext4 -L SETTINGS " SETTINGS_PARTITION) == 0;
}

bool InitDriveThread::zeroMbr()
{
    /* First 512 bytes should be enough to zero out the MBR, but we zero out 8 kb to make sure we also erase any
     * GPT primary header and get rid of any partitionless FAT headers.
     * also zero out the last 4 kb of the card to get rid of any secondary GPT header
     *
     * Using conv=fsync to make sure we get notified of write errors
     */
    return QProcess::execute("/bin/dd conv=fsync count=1 bs=8192 if=/dev/zero of=/dev/mmcblk0") == 0
        && QProcess::execute("/bin/dd conv=fsync count=8 bs=512 if=/dev/zero seek="+QString::number(sizeofSDCardInBlocks()-8)+" of=/dev/mmcblk0") == 0;
}

bool InitDriveThread::partitionDrive()
{
    /* Partition layout:
     *
     * First 1MB (2048 blocks) kept empty for alignment
     * Followed by FAT partition of RESCUE_PARTITION_SIZE (default 1 GB)
     * Followed by extended partition spanning remainder of space
     * Followed by NOOBS persistent settings partition
     */
    QByteArray partitionTable;
    int rescueBlocks = RESCUE_PARTITION_SIZE*1024*2;

    mbr_table extended_mbr;
    int startOfExtended = 2048+rescueBlocks;
    int sizeOfDisk = getFileContents("/sys/class/block/mmcblk0/size").trimmed().toULongLong();
    int sizeOfExtended = sizeOfDisk - startOfExtended - SETTINGS_PARTITION_SIZE;
    int startOfSettings = startOfExtended + sizeOfExtended;

    partitionTable = "2048,"+QByteArray::number(rescueBlocks)+",0E\n"; /* FAT partition */
    partitionTable += QByteArray::number(startOfExtended)+","+QByteArray::number(sizeOfExtended)+",X\n"; /* Extended partition with all remaining space */
    partitionTable += QByteArray::number(startOfSettings)+",,L\n"; /* Settings partition */
    partitionTable += "0,0\n";

    /* Write out empty extended partition table with signature */
    memset(&extended_mbr, 0, sizeof extended_mbr);
    extended_mbr.signature[0] = 0x55;
    extended_mbr.signature[1] = 0xAA;
    QFile f("/dev/mmcblk0");
    f.open(f.ReadWrite);
    f.seek(startOfExtended*512);
    f.write((char *) &extended_mbr, sizeof(extended_mbr));
    f.close();

    /* Write main partition table */
    QString cmd = QString("/sbin/sfdisk -uS /dev/mmcblk0");
    QProcess proc;
    proc.setProcessChannelMode(proc.MergedChannels);
    proc.start(cmd);
    proc.write(partitionTable);
    proc.closeWriteChannel();
    proc.waitForFinished(-1);
    QThread::msleep(500);

    return proc.exitCode() == 0;
}

#ifdef RISCOS_BLOB_FILENAME
bool InitDriveThread::writeRiscOSblob()
{
    qDebug() << "writing RiscOS blob";
    return QProcess::execute("/bin/dd conv=fsync bs=512 if=" RISCOS_BLOB_FILENAME " of=/dev/mmcblk0 seek="+QString::number(RISCOS_BLOB_SECTOR_OFFSET)) == 0;
}
#endif
