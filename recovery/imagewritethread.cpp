#include "imagewritethread.h"
#include "mbr.h"
#include "config.h"
#include <QProcess>
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QTime>
#include <unistd.h>
#include <linux/fs.h>
#include <sys/ioctl.h>

/* "Apply image" thread
 *
 * - Extracts image file to extended partition
 * - enlarges the ext4 partition to span the disk
 * - patches /etc/fstab and cmdline.txt
 *
 * Initial author: Floris Bos
 * Maintained by Raspberry Pi
 *
 * See LICENSE.txt for license details
 *
 */

ImageWriteThread::ImageWriteThread(const QString &image, QObject *parent) :
    QThread(parent),
    _image(image),
    _parseArchiveHeader(true)
{
    QDir dir;

    if (!dir.exists("/mnt2"))
        dir.mkdir("/mnt2");

#ifdef USE_EXTENDED_PARTITIONS
    _fatPartition = "/dev/mmcblk0p5";
    _extPartition = "/dev/mmcblk0p6";
#else
    _fatPartition = "/dev/mmcblk0p2";
    _extPartition = "/dev/mmcblk0p3";
#endif
}

void ImageWriteThread::run()
{
    if (_image.contains("risc", Qt::CaseInsensitive))
    {
        run_riscos();
        return;
    }

    /* Find out the end of our FAT partition, we dd after that */
    _offsetInSectors = startOfSecondPartition();
    _offsetInBytes   = qint64(_offsetInSectors) * 512;

    emit statusUpdate(tr("Writing image to SD card"));
    if (!dd())
        return;

    emit statusUpdate(tr("Resizing file system"));
    if (!resizePartition())
        return;

    emit statusUpdate(tr("Patching /boot/cmdline.txt"));
    if (!patchCmdlineTXT())
        return;

    emit statusUpdate(tr("Patching /etc/fstab"));
    if (!patchFstab())
        return;

    emit statusUpdate(tr("Finish writing (sync)"));
    sync();
    emit completed();
}

void ImageWriteThread::run_riscos()
{
    /* RiscOS likes to be at a fixed location */
    mbr_table extended_mbr;
    _offsetInSectors    = RISCOS_SECTOR_OFFSET;
    _offsetInBytes      = qint64(_offsetInSectors) * 512;
    int startOfExtended = startOfSecondPartition();

    if (startOfExtended > RISCOS_SECTOR_OFFSET)
    {
        emit error(tr("RISCOS cannot be installed. Size of rescue partition too large."));
        return;
    }

    emit statusUpdate(tr("Writing image to SD card"));
    if (!dd())
        return;

    emit statusUpdate(tr("Creating FAT partition for RiscOS"));
    memset(&extended_mbr, 0, sizeof extended_mbr);
    extended_mbr.signature[0] = 0x55;
    extended_mbr.signature[1] = 0xAA;
    /* Starting sector in extended MBR is relative to start of extended partition */
    extended_mbr.part[0].starting_sector = _offsetInSectors - startOfExtended;
    extended_mbr.part[0].nr_of_sectors   = RISCOS_FAT_SIZE;
    extended_mbr.part[0].id = 0x0E; /* FAT (LBA) */

    QFile f("/dev/mmcblk0");
    f.open(f.ReadWrite);
    f.seek(startOfExtended*512);
    f.write((char *) &extended_mbr, sizeof(extended_mbr));
    f.close();

    emit statusUpdate(tr("Finish writing (sync)"));
    sync();

    emit completed();
}

bool ImageWriteThread::dd()
{
    /* We just call busybox's unzip/gzip/xz/bzip2/lzop and dd,
     * instead of doing the image writing in C++ code.
     * Advantage is that it is easy to process compressed content, without creating
     * dependencies on several external libraries, and having to comply with their licenses.
     * Disadvantage is the lack of status callbacks.
     * Can simply get the number of bytes written from the kernel block device stats though
     */
    QString imagePath = "/mnt/images/"+_image;
    QString cmd = "sh -o pipefail -c \"";
    qint64 unCompSize = 0;

    if (_image.endsWith(".gz"))
    {
        cmd += "gzip -dc";
    }
    else if (_image.endsWith(".xz"))
    {
        cmd += "xz -dc";

        if (_parseArchiveHeader)
        {
            unCompSize = getUncompressedSizeXZ(imagePath);
        }
    }
    else if (_image.endsWith(".bz2"))
    {
        cmd += "bzip2 -dc";
    }
    else if (_image.endsWith(".lzo"))
    {
        cmd += "lzop -dc";

        if (_parseArchiveHeader)
        {
            unCompSize = getUncompressedSizeLZO(imagePath);
        }
    }
    else if (_image.endsWith(".zip"))
    {
        /* Note: the image must be the only file inside the .zip */
        cmd += "unzip -p";

        if (_parseArchiveHeader)
        {
            unCompSize = getUncompressedSizeZIP(imagePath);
        }
    }
    else
    {
        emit error(tr("Unknown image format file extension. Expecting .lzo, .gz, .xz, .bz2 or .zip"));
        return false;
    }
    cmd += " "+imagePath;

    if (unCompSize == -1)
    {
        emit error(tr("Image file corrupt"));
        return false;
    }
    else if (unCompSize > 0)
    {
        emit parsedImagesize(unCompSize);
    }

    if (_offsetInSectors % 2048 == 0)
    {
        /* Use 1 MB block size if our partition is nicely aligned on a MB boundary */
        cmd += " | dd of=/dev/mmcblk0 conv=fsync obs=1M seek="+QString::number(_offsetInSectors/2048)+"\"";
    }
    else
    {
        cmd += " | dd of=/dev/mmcblk0 conv=fsync obs=512 seek="+QString::number(_offsetInSectors)+"\"";
    }


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
        emit error(tr("Error writing image to SD card")+"\n"+p.readAll());
        return false;
    }
    qDebug() << "finished writing image in" << (t1.elapsed()/1000.0) << "seconds";

    QProcess::execute("umount /mnt");
    return true;
}

/* Code for doing it programmatically instead */
/*
bool ImageWriteThread::dd()
{
    QFile fin(_image);
    if (!fin.open(fin.ReadOnly))
    {
        emit error(tr("Error opening image file '%1'").arg(_image));
        return false;
    }


    QFile fout("/dev/mmcblk0");
    if (!fout.open(fout.WriteOnly))
    {
        emit error(tr("Error opening SD card device"));
        return false;
    }
    fout.seek(_offsetInBytes);

    while (!fin.atEnd())
    {
        char buf[4096];
        qint64 bytesRead = fin.read(buf, sizeof(buf));
        if (bytesRead == -1)
        {
            emit error(tr("Error reading image"));
            return false;
        }

        // <- add code to handle decompression

        if (fin.write(buf, bytesRead) != bytesRead)
        {
            emit error(tr("Error writing to SD card"));
            return false;
        }
    }

    if (fsync(fout.handle()) != 0)
    {
        emit error(tr("Error writing to SD card"));
        return false;
    }

    fout.close();
    fin.close();

    QProcess::execute("umount /mnt");
    return true;
}
*/

bool ImageWriteThread::patchFstab()
{
    if (QProcess::execute("mount -t ext4 "+_extPartition+" /mnt2") != 0)
    {
        emit error(tr("Error mounting ext4 partition"));
        return false;
    }

    replaceMMCBLKreferences("/mnt2/etc/fstab");
    QProcess::execute("umount /mnt2");

    return true;
}

bool ImageWriteThread::patchCmdlineTXT()
{
    if (QProcess::execute("mount -t vfat "+_fatPartition+" /mnt2") != 0)
    {
        emit error(tr("Error mounting FAT partition of image"));
        return false;
    }

    replaceMMCBLKreferences("/mnt2/cmdline.txt");

    QProcess::execute("mount /dev/mmcblk0p1 /mnt");

    if (QFile::exists("/mnt/fat2/start.elf"))
    {
        QDir dir("/mnt/fat2");
        QStringList files = dir.entryList(QDir::Files);

        foreach (QString file, files)
        {
            qDebug() << "Copying file" << file;
            QFile::remove("/mnt2/"+file);
            QFile::copy("/mnt/fat2/"+file, "/mnt2/"+file);
        }
    }

    QProcess::execute("umount /mnt2");

    return true;
}

void ImageWriteThread::replaceMMCBLKreferences(const QString &filename)
{
    QByteArray data = getFileContents(filename);
    if (data.isEmpty())
        return;

#ifdef USE_EXTENDED_PARTITIONS
    data.replace("mmcblk0p5", "mmcblk0p6");
    data.replace("mmcblk0p1", "mmcblk0p5");
#else
    data.replace("mmcblk0p3", "mmcblk0p4");
    data.replace("mmcblk0p2", "mmcblk0p3");
    data.replace("mmcblk0p1", "mmcblk0p2");
#endif

    putFileContents(filename, data);
}


int ImageWriteThread::startOfSecondPartition()
{
#ifdef USE_EXTENDED_PARTITIONS
    return getFileContents("/sys/class/block/mmcblk0p2/start").trimmed().toInt();
#else
    int startsector = getFileContents("/sys/class/block/mmcblk0p1/start").trimmed().toInt();
    int size = getFileContents("/sys/class/block/mmcblk0p1/size").trimmed().toInt();

    return startsector+size;
#endif
}

bool ImageWriteThread::resizePartition()
{
    mbr_table ebr, lbr;

    QFile f("/dev/mmcblk0");
    f.open(f.ReadWrite);

    f.seek(_offsetInBytes);
    f.read((char *) &ebr, sizeof(ebr));

#ifdef USE_EXTENDED_PARTITIONS

    int sizeOfDisk = getFileContents("/sys/class/block/mmcblk0/size").trimmed().toULongLong();
    int sizeOfExtended = sizeOfDisk-startOfSecondPartition();
    int sizeOfLogical = sizeOfExtended - ebr.part[1].starting_sector;

    if (!(ebr.signature[0] == 0x55 && ebr.signature[1] == 0xAA)){
        emit error(tr("Extended boot record (EBR) not found"));
        return false;
    }

    if (ebr.part[1].starting_sector)
    {
        // Enlarge partition
        ebr.part[1].nr_of_sectors = sizeOfExtended - ebr.part[1].starting_sector;
        // Linux only cares about LBA, zeroing out
        // obsolete head/sector/cylinder fields
        ebr.part[1].begin_hsc[0] = ebr.part[1].begin_hsc[1] = ebr.part[1].begin_hsc[2] = 0;
        ebr.part[1].end_hsc[0] = ebr.part[1].end_hsc[1] = ebr.part[1].end_hsc[2] = 0;
    }
    else
    {
        emit error(tr("No partitions found inside image's extended boot record (EBR)"));
        return false;
    }

    // Write our modified extended MBR to disk
    f.seek(_offsetInBytes);
    f.write((char *) &ebr, sizeof(ebr));

    qint64 logicalOffsetinBytes = _offsetInBytes + qint64(ebr.part[1].starting_sector)*512;

    // Handle logical partition
    f.seek(logicalOffsetinBytes);
    f.read((char *) &lbr, sizeof(lbr));

    if (!(lbr.signature[0] == 0x55 && lbr.signature[1] == 0xAA)){
        emit error(tr("Logical boot record (LBR) not found"));
        return false;
    }

    if (lbr.part[0].starting_sector)
    {
        // Enlarge partition
        lbr.part[0].nr_of_sectors = sizeOfLogical - lbr.part[0].starting_sector;
        // Linux only cares about LBA, zeroing out
        // obsolete head/sector/cylinder fields
        lbr.part[0].begin_hsc[0] = lbr.part[0].begin_hsc[1] = lbr.part[0].begin_hsc[2] = 0;
        lbr.part[0].end_hsc[0] = lbr.part[0].end_hsc[1] = lbr.part[0].end_hsc[2] = 0;
    }
    else
    {
        emit error(tr("No partitions found inside image's logical boot record (LBR)"));
        return false;
    }

    // Write our modified extended MBR to disk
    f.seek(logicalOffsetinBytes);
    f.write((char *) &lbr, sizeof(lbr));
    f.flush();

    // Tell Linux to re-read the partition table
    ioctl(f.handle(), BLKRRPART);

    f.close();

    QString lastPartStr = "/dev/mmcblk0p6";
#else
    // Rewriting primary MBR

    QByteArray partitionTable;
    int offset = startOfSecondPartition();
    f.close();

    // Populating partition table in reverse order
    for (int i=3; i>=0; i--)
    {
        if (mbr.part[i].starting_sector)
        {
            QByteArray partitionEntry = QByteArray::number(offset+mbr.part[i].starting_sector)+",";
            if (lastPartition == -1)
            {
                // Give partition all remaining space
                partitionEntry += ",";
                lastPartition = i;
            }
            else
            {
                partitionEntry += QByteArray::number(mbr.part[i].nr_of_sectors)+",";
            }

            partitionEntry += QByteArray::number(mbr.part[i].id, 16)+"\n"; /* Partition type in hexadecimal */
            partitionTable = partitionEntry + partitionTable;
        }
        else
        {
            partitionTable = "0,0\n"+partitionTable;
        }
    }

    QByteArray startOfOurPartition = getFileContents("/sys/class/block/mmcblk0p1/start").trimmed();
    QByteArray sizeOfOurPartition  = getFileContents("/sys/class/block/mmcblk0p1/size").trimmed();
    partitionTable = startOfOurPartition+","+sizeOfOurPartition+",0E\n"+partitionTable;
    qDebug() << "New parition table:\n" << partitionTable;

    // Calling sfdisk to rewrite the main MBR as it also takes care of clean CHS calculations and such
    QString sfcmd = "/sbin/sfdisk -uS /dev/mmcblk0";
    QProcess proc;
    proc.setProcessChannelMode(proc.MergedChannels);
    proc.start(sfcmd);
    proc.write(partitionTable);
    proc.closeWriteChannel();
    proc.waitForFinished(-1);

    QString lastPartStr = "/dev/mmcblk0p"+QString::number(lastPartition+2);
#endif

    QThread::msleep(500);

    if (!QFile::exists(lastPartStr))
    {
        qDebug() << "BLKRRPART did not work?";
        /* Probe again */
        sync();
        QProcess::execute("/usr/sbin/partprobe");
        QThread::msleep(1500);
    }

    // Mount filesystem briefly to make sure journal is ok or resize2fs may complain
    if (QProcess::execute("mount -t ext4 "+lastPartStr+" /mnt2") != 0)
    {
        emit error(tr("Error mounting ext4 partition"));
        return false;
    }
    QProcess::execute("umount /mnt2");

    // Call resize2fs to actually resize the file system structures
    QString cmd = "/usr/sbin/resize2fs "+lastPartStr;
    QTime t1;
    t1.start();
    qDebug() << "Executing:" << cmd;
    if (QProcess::execute(cmd) != 0)
    {
        emit error(tr("Error resizing ext4 partition"));
        return false;
    }
    qDebug() << "Finished resizing image in" << (t1.elapsed()/1000.0) << "seconds";

    return true;
}

QByteArray ImageWriteThread::getFileContents(const QString &filename)
{
    QByteArray r;
    QFile f(filename);
    f.open(f.ReadOnly);
    r = f.readAll();
    f.close();

    return r;
}

void ImageWriteThread::putFileContents(const QString &filename, const QByteArray &data)
{
    QFile f(filename);
    f.open(f.WriteOnly);
    f.write(data);
    f.close();
}

void ImageWriteThread::setParseArchiveHeader(bool b)
{
    _parseArchiveHeader = b;
}

qint64 ImageWriteThread::getUncompressedSizeZIP(const QString &filename)
{
    /* Obtain uncompressed file size from local file header in .zip file */
    quint32 signature, crc, compSize, unCompSize;
    quint16 extractVersion, bitFlag, compMethod, modTime, modDate;

    QFile f(filename);
    f.open(f.ReadOnly);
    QDataStream s(&f);
    s.setByteOrder(QDataStream::LittleEndian);
    s >> signature;
    if (signature != 0x04034b50)
        return -1;

    s >> extractVersion >> bitFlag >> compMethod;
    s >> modTime >> modDate >> crc >> compSize >> unCompSize;
    f.close();

    return unCompSize;
}

qint64 ImageWriteThread::getUncompressedSizeLZO(const QString &filename)
{
    /* LZO does not provide the file's size in the main header.
     * But we do can get that information in a reasonable time by iterating
     * over the individual block headers */
    enum lzo_flags {F_ADLER32_D = 1, F_ADLER32_C = 2, F_CRC32_D = 0x100, F_CRC32_C = 0x200};
    qint64 size = 0;

    /* LZO header */
    char magic[9] = {0};
    quint16 version, lib_version, version_to_extract;
    quint8  method, level;
    quint32 flags, mode, mtime, gmtdiff;
    quint8  namelen;
    quint32 chksum;

    QFile f(filename);
    f.open(f.ReadOnly);
    QDataStream s(&f);
    s.setByteOrder(QDataStream::BigEndian);

    s.readRawData(magic, sizeof(magic));
    if (memcmp(magic, "\x89LZO\0\r\n\x1a\n", 9) != 0)
    {
        qDebug() << "lzo magic mismatch";
        return -1;
    }

    s >> version >> lib_version >> version_to_extract;
    s >> method;
    if (version >= 0x0940)
        s >> level;
    s >> flags >> mode >> mtime;
    if (version >= 0x0940)
        s >> gmtdiff;
    s >> namelen;
    if (namelen)
        s.skipRawData(namelen);
    s >> chksum;

    /* LZO data blocks headers */
    quint32 uncompBlockSize, compBlockSize, uncompAdler, uncompCRC, compAdler, compCRC;

    while (true)
    {
        s >> uncompBlockSize;
        if (uncompBlockSize == 0) /* trailer */
            break;

        size += uncompBlockSize;
        s >> compBlockSize;
        if (flags & F_ADLER32_D)
            s >> uncompAdler;
        if (flags & F_CRC32_D)
            s >> uncompCRC;
        if (flags & F_ADLER32_C)
            s >> compAdler;
        if (flags & F_CRC32_C)
            s >> compCRC;

        if (!compBlockSize || s.skipRawData(compBlockSize) == -1)
        {
            qDebug() << "lzo file ended prematurely, expecting more data";
            return -1;
        }
    }
    f.close();

    return size;
}

static quint64 _parseVariableSizedInt(QDataStream &s)
{
    quint64 result;
    quint8 c;

    s >> c;
    result = c & 0x7F;

    for (int i=1; i<9 && (c & 0x80); i++)
    {
        s >> c;
        if (c == 0)
            return 0;

        result |= (quint64)(c & 0x7F) << (i * 7);
    }

    return result;
}

qint64 ImageWriteThread::getUncompressedSizeXZ(const QString &filename)
{
    qint64 size = 0;

    /* XZ stream header */
    QFile f(filename);
    f.open(f.ReadOnly);

    QByteArray magic = f.read(6);
    if (magic != QByteArray("\xFD" "7zXZ" "\0", 6))
    {
        qDebug() << "xz: header magic mismatch";
        return -1;
    }

    /* XZ stream footer */
    quint32 crc_footer, stored_backward_size;
    quint16 flags;
    char footer_magic[2] = {0};

    f.seek(f.size()-12);
    QDataStream s(&f);
    s.setByteOrder(QDataStream::LittleEndian);
    s >> crc_footer >> stored_backward_size >> flags;
    s.readRawData(footer_magic, 2);

    if (footer_magic[0] != 'Y' || footer_magic[1] != 'Z')
    {
        qDebug() << "xz: footer magic mismatch";
        return -1;
    }
    quint32 real_backward_size = (stored_backward_size + 1) * 4;
    qDebug() << real_backward_size;

    /* Index */
    quint8 index_indicator;
    quint64 nr_records;
    /* Index record */
    quint64 unpadded_size;
    quint64 uncompr_size;

    f.seek(f.size()-12-real_backward_size);
    s >> index_indicator;
    if (index_indicator != 0)
    {
        qDebug() << "xz: index indicator mismatch";
        return -1;
    }
    nr_records = _parseVariableSizedInt(s);

    if (!nr_records)
    {
        qDebug() << "xz: nr of index records invalid";
        return -1;
    }

    for (unsigned int i=0; i < nr_records; i++)
    {
        if (s.atEnd())
        {
            qDebug() << "xz: reached eof while expecting more";
            return -1;
        }

        unpadded_size = _parseVariableSizedInt(s);
        uncompr_size  = _parseVariableSizedInt(s);

        if (unpadded_size < 5)
        {
            qDebug() << "xz: unpadded size should not be smaller than 5";
            return -1;
        }

        size += uncompr_size;
    }

    f.close();
    return size;
}
