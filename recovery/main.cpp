#include "mainwindow.h"
#include "languagedialog.h"
#include "config.h"
#include "keydetection.h"
#include "gpioinput.h"
#include "rightbuttonfilter.h"
#include "json.h"
#include "util.h"
#include "bootselectiondialog.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/reboot.h>
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

void reboot_to_extended(const QString &defaultPartition, bool setDisplayMode)
{
    // Unmount any open file systems
    QProcess::execute("umount -r /mnt");
    QProcess::execute("umount -r /settings");

    if (QFile::exists("/dev/mmcblk0p7"))
    {
#ifdef Q_WS_QWS
        QWSServer::setBackground(Qt::white);
        QWSServer::setCursorVisible(true);
#endif
        BootSelectionDialog bsd(defaultPartition);
        if (setDisplayMode)
            bsd.setDisplayMode();
        bsd.exec();
    }

    // Shut down networking
    QProcess::execute("ifdown -a");
    // Reboot
    ::reboot(RB_AUTOBOOT);
}

int main(int argc, char *argv[])
{
    // Wait for keyboard to appear before displaying anything
    KeyDetection::waitForKeyboard();

    QFile f("/sys/module/bcm2708/parameters/boardrev");
    f.open(f.ReadOnly);
    int rev = f.readAll().trimmed().toInt();
    f.close();

    qDebug() << "Board revision is " << rev;

    int gpioChannel;

    if (rev == 2 || rev == 3)
        gpioChannel = 0;
    else
        gpioChannel = 2;

    QApplication a(argc, argv);
    RightButtonFilter rbf;
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

#ifdef Q_WS_QWS
    QWSServer::setCursorVisible(false);
#endif

    // Set wallpaper and icon, if we have resource files for that
    if (QFile::exists(":/icons/raspberry_icon.png"))
        a.setWindowIcon(QIcon(":/icons/raspberry_icon.png"));

#ifdef Q_WS_QWS
        QWSServer::setBackground(BACKGROUND_COLOR);
#endif
        QSplashScreen *splash = new QSplashScreen(QPixmap(":/wallpaper.png"));
        splash->show();
        QApplication::processEvents();

    // If -runinstaller is not specified, only continue if SHIFT is pressed, GPIO is triggered,
    // or no OS is installed (/dev/mmcblk0p5 does not exist)
    bool bailout = !runinstaller
        && !force_trigger
        && !(gpio_trigger && (gpio.value() == 0 ))
        && !(keyboard_trigger && KeyDetection::isF10pressed())
        && QFile::exists(FAT_PARTITION_OF_IMAGE);

    // Default to booting first extended partition
    putFileContents("/sys/module/bcm2708/parameters/reboot_part", "5\n");

    if (bailout)
    {
        splash->hide();
        reboot_to_extended(defaultPartition, true);
    }

#ifdef Q_WS_QWS
    QWSServer::setCursorVisible(true);
#endif

    // Main window in the middle of screen
    MainWindow mw(defaultDisplay, splash);
    mw.setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, mw.size(), a.desktop()->availableGeometry()));
    mw.show();

#ifdef ENABLE_LANGUAGE_CHOOSER
     // Language chooser at the bottom center
    LanguageDialog* ld = new LanguageDialog(defaultLang, defaultKeyboard);
    ld->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignHCenter | Qt::AlignBottom, ld->size(), a.desktop()->availableGeometry()));
    ld->show();
#endif

    a.exec();
    reboot_to_extended(defaultPartition, false);

    return 0;
}
