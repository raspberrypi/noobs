#include "keydetection.h"
#include "config.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/input.h>
#include <fcntl.h>
#include <QDebug>
#include <QDir>

/* Key detection class
 *
 * We are using evdev's EVIOCGKEY ioctl to get a list of all keys currently held down
 * Advantage over reading the key code from the tty is that it also works if the keyboard
 * is ready before the application starts and the key was send to another application.
 *
 * In practice we are usually started earlier than USB is up & running though,
 * so we have to wait for a keyboard device to appear.
 *
 * Initial author: Floris Bos
 * Maintained by Raspberry Pi
 *
 * See LICENSE.txt for license details
 *
 */

#define test_bit(bit, array)  (array[bit / 8] & (1 << (bit % 8)))

bool KeyDetection::waitForKeyboard()
{
    int fd = -1;

    // Wait up to 2.1 seconds for a keyboard to appear.
    for (int i=0; i<21; i++)
    {
        usleep(100000);
        fd = openKeyboard();
        if (fd != -1)
        {
            close(fd);
            return true;
        }
    }

    return false;
}

bool KeyDetection::isF10pressed()
{
    int fd = openKeyboard();

    if (fd == -1)
    {
        qDebug() << "No keyboard found...";
        return false;
    }

    // Wait 2 seconds for key press
    for (int i=0; i<200; i++)
    {
        if (_isF10pressed(fd))
            return true;
        usleep(10000);
    }

    // Final check
    bool pressed = _isF10pressed(fd);

    if (!pressed)
        qDebug() << "No key press detected...";


    close(fd);

    return pressed;
}

bool KeyDetection::_isF10pressed(int fd)
{
    u_int8_t keymap[KEY_MAX/8+1] = {0};

    if (ioctl(fd, EVIOCGKEY(sizeof(keymap)), &keymap) == -1)
    {
        qDebug() << "Error getting global key state";
    }

    bool pressed = test_bit(KEY_F10, keymap) || test_bit(KEY_LEFTSHIFT, keymap) || test_bit(KEY_RIGHTSHIFT, keymap);
    //qDebug() << "Key press detection:" << pressed;

    return pressed;
}

int KeyDetection::openKeyboard()
{
    int fd;
    u_int8_t evtype_bitmask[EV_MAX/8+1] = {0};
    QDir dir("/sys/class/input", "event*");
    QStringList inputDevices = dir.entryList();

    foreach (QString inputDevice, inputDevices)
    {
        QByteArray inputDeviceFile = "/dev/input/"+inputDevice.toLatin1();
        qDebug() << "Testing if input device is a keyboard" << inputDeviceFile;
        fd = open(inputDeviceFile.constData(), O_RDONLY);

        if (ioctl(fd, EVIOCGBIT(0, EV_MAX), evtype_bitmask) != -1
           && test_bit(EV_KEY, evtype_bitmask)
           && !test_bit(EV_REL, evtype_bitmask)
           && !test_bit(EV_ABS, evtype_bitmask))
        {
            /* If the input device has keys and not relative or absolute data we assume it is a keyboard */
            qDebug() << "Keyboard found:" << inputDeviceFile;
            return fd;
        }

        close(fd);
    }

    return -1;
}
