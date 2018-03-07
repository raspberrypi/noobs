#include "mainwindow.h"
#include "languagedialog.h"
#include "config.h"
#include "keydetection.h"
#include "gpioinput.h"
#include "rightbuttonfilter.h"
#include "longpresshandler.h"
#include "json.h"
#include "util.h"
#include "bootselectiondialog.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <linux/reboot.h>
#include <QApplication>
#include <QStyle>
#include <QDesktopWidget>
#include <QSplashScreen>
#include <QFile>
#include <QIcon>
#include <QProcess>
#include <QDir>
#include <QDebug>
#include <QTime>

#ifdef Q_WS_QWS
#include <QWSServer>
#endif

/*
 *
 * Initial author: Floris Bos
 * Maintained by Raspberry Pi
 *
 * See LICENSE.txt for license details
 *
 */

void showBootMenu(const QString &drive, const QString &defaultPartition, bool setDisplayMode)
{
    QByteArray reboot_part;
#ifdef Q_WS_QWS
    QWSServer::setBackground(BACKGROUND_COLOR);
    QWSServer::setCursorVisible(true);
#endif
    BootSelectionDialog bsd(drive, defaultPartition);
    if (setDisplayMode)
        bsd.setDisplayMode();
    bsd.exec();

    // Shut down networking
    QProcess::execute("ifdown -a");
    // Unmount file systems
    QProcess::execute("umount -ar");
    ::sync();
    // Reboot
    reboot_part = getFileContents("/run/reboot_part").trimmed();
    ::syscall(SYS_reboot, LINUX_REBOOT_MAGIC1, LINUX_REBOOT_MAGIC2, LINUX_REBOOT_CMD_RESTART2, reboot_part.constData());
}

bool hasInstalledOS(const QString &drive)
{
    bool installedOsFileExists = false;

    if (QProcess::execute("mount -o ro "+partdev(drive, SETTINGS_PARTNR)+" /settings") == 0)
    {
        installedOsFileExists = QFile::exists("/settings/installed_os.json");
        QProcess::execute("umount /settings");
    }

    return installedOsFileExists;
}

QString findRecoveryDrive()
{
    /* Search for drive with recovery.rfs */
    QString drive;
    QString dirname  = "/sys/class/block";
    QDir    dir(dirname);
    QStringList list = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    foreach (QString devname, list)
    {
        /* Only search first partition and partitionless devices. Skip virtual devices (such as ramdisk) */
        if ((devname.right(1).at(0).isDigit() && !devname.endsWith("1"))
                || QFile::symLinkTarget("/sys/class/block/"+devname).contains("/devices/virtual/"))
            continue;

        if (QProcess::execute("mount -t vfat -o ro /dev/"+devname+" /mnt") == 0)
        {

            if (QFile::exists("/mnt/recovery.rfs"))
            {
                qDebug() << "Found recovery.rfs at" << devname;

                // We are interested in the drive, not the exact partition
                drive = "/dev/"+devname;
                if (drive.endsWith("p1"))
                    drive.chop(2);
                else if (drive.endsWith("1"))
                    drive.chop(1);
            }

            QProcess::execute("umount /mnt");
        }

        if (!drive.isEmpty())
            break;
    }

    return drive;
}


int main(int argc, char *argv[])
{
    bool hasTouchScreen = QFile::exists("/sys/devices/platform/rpi_ft5406");

    // Unless we have a touch screen, wait for keyboard to appear before displaying anything
    if (!hasTouchScreen)
        KeyDetection::waitForKeyboard();

    int rev = readBoardRevision();

    qDebug() << "Board revision is " << rev;

    int gpioChannel;

    if (rev == 2 || rev == 3)
        gpioChannel = 0;
    else
        gpioChannel = 2;

    QApplication a(argc, argv);
    RightButtonFilter rbf;
    LongPressHandler lph;
    GpioInput gpio(gpioChannel);

    bool runinstaller = false;
    bool gpio_trigger = false;
    bool keyboard_trigger = true;
    bool force_trigger = false;

    QString defaultLang = "en";
    QString defaultKeyboard = "gb";
    QString defaultDisplay = "0";
    QString defaultPartition = "800";

    // Process command-line arguments
    for (int i=1; i<argc; i++)
    {
        // Flag to indicate first boot
        if (strcmp(argv[i], "-runinstaller") == 0)
            runinstaller = true;
        // Enables use of GPIO 3 to force NOOBS to launch by pulling low
        else if (strcmp(argv[i], "-gpiotriggerenable") == 0)
            gpio_trigger = true;
        // Disables use of keyboard to trigger recovery GUI
        else if (strcmp(argv[i], "-keyboardtriggerdisable") == 0)
            keyboard_trigger = false;
        // Forces display of recovery GUI every time
        else if (strcmp(argv[i], "-forcetrigger") == 0)
            force_trigger = true;
        // Allow default language to be specified in commandline
        else if (strcmp(argv[i], "-lang") == 0)
        {
            if (argc > i+1)
                defaultLang = argv[i+1];
        }
        // Allow default keyboard layout to be specified in commandline
        else if (strcmp(argv[i], "-kbdlayout") == 0)
        {
            if (argc > i+1)
                defaultKeyboard = argv[i+1];
        }
        // Allow default display mode to be specified in commandline
        else if (strcmp(argv[i], "-dispmode") == 0)
        {
            if (argc > i+1)
                defaultDisplay = --argv[i+1];
        }
        // Allow default boot partition to be specified in commandline
        else if (strcmp(argv[i], "-partition") == 0)
        {
            if (argc > i+1)
                defaultPartition = argv[i+1];
        }
    }

    // Intercept right mouse clicks sent to the title bar
    a.installEventFilter(&rbf);

    // Treat long holds as double-clicks
    if (hasTouchScreen)
        a.installEventFilter(&lph);

#ifdef Q_WS_QWS
    QWSServer::setCursorVisible(false);
#endif

    QDir settingsdir;
    settingsdir.mkdir("/settings");

    // Set wallpaper and icon, if we have resource files for that
    if (QFile::exists(":/icons/raspberry_icon.png"))
        a.setWindowIcon(QIcon(":/icons/raspberry_icon.png"));

#ifdef Q_WS_QWS
    QWSServer::setBackground(BACKGROUND_COLOR);
#endif
    QSplashScreen *splash = new QSplashScreen(QPixmap(":/wallpaper.png"));
    splash->show();
    QApplication::processEvents();

    // Wait for drive device to show up
    QString drive;
    bool driveReady = false;
    QTime t;
    t.start();

    while (t.elapsed() < 10000)
    {
        if (drive.isEmpty())
        {
            /* We do not know the exact drive name to wait for */
            drive = findRecoveryDrive();
            if (!drive.isEmpty())
            {
                driveReady = true;
                break;
            }
        }
        else if (drive.startsWith("/dev"))
        {
            if (QFile::exists(drive))
            {
                driveReady = true;
                break;
            }
        }

        QApplication::processEvents(QEventLoop::WaitForMoreEvents, 100);
    }
    if (!driveReady)
    {
        QMessageBox::critical(NULL, "Files not found", QString("Cannot find the drive with NOOBS files %1").arg(drive), QMessageBox::Close);
        return 1;
    }
    qDebug() << "NOOBS drive:" << drive;

    // If -runinstaller is not specified, only continue if SHIFT is pressed, GPIO is triggered,
    // or no OS is installed (/settings/installed_os.json does not exist)
    bool bailout = !runinstaller
        && !force_trigger
        && !(gpio_trigger && (gpio.value() == 0 ))
        && hasInstalledOS(drive);

    if (bailout && keyboard_trigger)
    {
        t.start();

        while (t.elapsed() < 2000)
        {
            QApplication::processEvents(QEventLoop::WaitForMoreEvents, 10);
            if (QApplication::queryKeyboardModifiers().testFlag(Qt::ShiftModifier))
            {
                bailout = false;
                qDebug() << "Shift detected";
                break;
            }
            if (hasTouchScreen && QApplication::mouseButtons().testFlag(Qt::LeftButton))
            {
                bailout = false;
                qDebug() << "Tap detected";
                break;
            }
        }
    }

    if (bailout)
    {
        splash->hide();
        showBootMenu(drive, defaultPartition, true);
    }

#ifdef Q_WS_QWS
    QWSServer::setCursorVisible(true);
#endif

    // Main window in the middle of screen
    MainWindow mw(drive, defaultDisplay, splash);
    mw.setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, mw.size(), a.desktop()->availableGeometry()));
    mw.show();

#ifdef ENABLE_LANGUAGE_CHOOSER
     // Language chooser at the bottom center
    LanguageDialog* ld = new LanguageDialog(defaultLang, defaultKeyboard);
    ld->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignHCenter | Qt::AlignBottom, ld->size(), a.desktop()->availableGeometry()));
    ld->show();
#endif

    a.exec();
    showBootMenu(drive, defaultPartition, false);

    return 0;
}
