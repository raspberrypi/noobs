#include "util.h"
#include <sys/ioctl.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <QFile>
#include <QFileInfo>
#include <QProcess>
#include <QDebug>
#include <QList>

/*
 * Convenience functions
 *
 * Initial author: Floris Bos
 * Maintained by Raspberry Pi
 *
 * See LICENSE.txt for license details
 */

QByteArray getFileContents(const QString &filename)
{
    QByteArray r;
    QFile f(filename);
    f.open(f.ReadOnly);
    r = f.readAll();
    f.close();

    return r;
}

void putFileContents(const QString &filename, const QByteArray &data)
{
    QFile f(filename);
    f.open(f.WriteOnly);
    f.write(data);
    f.close();
}

/* Utility function to query current overscan setting */
#define VCMSG_GET_OVERSCAN 0x0004000a
#define VCMSG_SET_OVERSCAN 0x0004800a
#define IOCTL_MBOX_PROPERTY _IOWR(100, 0, char *)
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"

struct vc_msg_overscan {
    uint32_t msg_size; /* sizeof(struct vc_msg_overscan) */
    uint32_t request_code;
    uint32_t tag_id;
    uint32_t buffer_size; /* 16 bytes */
    uint32_t request_size;
    uint32_t top;
    uint32_t bottom;
    uint32_t left;
    uint32_t right;
    uint32_t end_tag; /* an end identifier (NULL) */
} __attribute__ ((packed));

void getOverscan(int &top, int &bottom, int &left, int &right)
{
    int fd;
    vc_msg_overscan msg = {0};

    if (!QFile::exists("/dev/mailbox"))
        QProcess::execute("mknod /dev/mailbox c 100 0");

    fd = ::open("/dev/mailbox", 0);
    if (!fd)
    {
        qDebug() << "Error opening mailbox";
    }
    else
    {
        msg.msg_size = sizeof(msg);
        msg.tag_id = VCMSG_GET_OVERSCAN;
        msg.buffer_size = 16;
        if (ioctl(fd, IOCTL_MBOX_PROPERTY, &msg) != 0)
        {
            qDebug() << "Error getting mailbox property";
        }
        else
        {
            top = msg.top;
            left = msg.left;
            right = msg.right;
            bottom = msg.bottom;
        }

        ::close(fd);
    }
}

bool nameMatchesRiscOS(const QString &name)
{
    return name.contains("risc", Qt::CaseInsensitive);
}

static uint revision = 0;
uint readBoardRevision()
{
    if (revision == 0)
    {
        QProcess proc;
        proc.start("vcgencmd otp_dump");
        proc.waitForFinished();
        QList<QByteArray> lines = proc.readAll().split('\n');
        for (int i=0; i < lines.size(); i++)
        {
            if (lines.at(i).startsWith("30:"))
            {
                bool ok;
                revision = lines.at(i).right(8).toUInt(&ok, 16) & 0xFFFFFF;
                break;
            }
        }
    }
    return revision;
}

/* Whether this OS should be displayed in the list of bootable OSes */
bool canBootOs(const QString& name, const QVariantMap& values)
{
    /* Can't simply pull "name" from "values" because in some JSON files it's "os_name" and in others it's "name" */

    /* Check if it's explicitly not bootable */
    bool bootable = values.value("bootable", true).toBool();
    if (!bootable)
    {
        return false;
    }

    /* Data Partition isn't bootable */
    if (name == "Data Partition")
    {
        return false;
    }

    return true;
}

bool setRebootPartition(QByteArray partition)
{
    if (QFileInfo("/sys/module/bcm2708/parameters/reboot_part").exists())
    {
        putFileContents("/sys/module/bcm2708/parameters/reboot_part", partition+"\n");
        return true;
    }
    else if (QFileInfo("/sys/module/bcm2709/parameters/reboot_part").exists())
    {
        putFileContents("/sys/module/bcm2709/parameters/reboot_part", partition+"\n");
        return true;
    }
    return false;
}

/* Returns device name for drive and partition number
 *
 * partdev("mmcblk0",1) -> mmcblk0p1
 * partdev("sda",2) -> sda2
 */
QByteArray partdev(const QString &drivedev, int nr)
{
    if (drivedev.right(1).at(0).isDigit())
        return drivedev.toAscii()+"p"+QByteArray::number(nr);
    else
        return drivedev.toAscii()+QByteArray::number(nr);
}

/* Returns /sys/class/block path for given drive and optional partition number */
QByteArray sysclassblock(const QString &drivedev, int partnr)
{
    QByteArray b;

    if (partnr == -1)
        b = drivedev.toAscii();
    else
        b = partdev(drivedev, partnr);

    if (b.startsWith("/dev/"))
        b = b.mid(5);

    return "/sys/class/block/"+ b;
}
