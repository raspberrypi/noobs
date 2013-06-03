#include "mainwindow.h"
#include "languagedialog.h"
#include "config.h"
#include "keydetection.h"
#include "gpioinput.h"
#include "rightbuttonfilter.h"
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

void reboot_to_extended()
{
    // Unmount any open file systems
    QProcess::execute("umount -ar");
    // Shut down networking
    QProcess::execute("ifdown -a");
    // Reboot
    ::reboot(RB_AUTOBOOT);
}

int main(int argc, char *argv[])
{
    GpioInput gpio(3);
    QApplication a(argc, argv);
    RightButtonFilter rbf;

    // Intercept right mouse clicks sent to the title bar
    a.installEventFilter(&rbf);

#ifdef Q_WS_QWS
    QWSServer::setCursorVisible(false);

    // Set wallpaper and icon, if we have resource files for that
    if (QFile::exists(":/icons/raspberry_icon.png"))
        a.setWindowIcon(QIcon(":/icons/raspberry_icon.png"));

    if (QFile::exists(":/wallpaper.png"))
    {
#ifdef CENTER_BACKGROUND_IMAGE
        // Using QSplashScreen to get a centered background image
        QWSServer::setBackground(BACKGROUND_COLOR);
        QSplashScreen *splash = new QSplashScreen(QPixmap(":/wallpaper.png"));
        splash->show();
        QApplication::processEvents();
#else
        // Scale background image to fit screen
        QRect dim = a.desktop()->availableGeometry();
        QWSServer::setBackground(QImage(":/wallpaper.png").scaled(dim.width(), dim.height()));
#endif
    }
    else
    {
        QWSServer::setBackground(BACKGROUND_COLOR);
    }
#endif

    // If -runinstaller is not specified, only continue if SHIFT is pressed, GPIO is triggered or no OS is installed (/dev/mmcblk0p6 does not exist)
    bool bailout = (argc < 2 || strcmp(argv[1], "-runinstaller") != 0)
         && gpio.value() != 0
         && !KeyDetection::isF10pressed()
         && QFile::exists(FAT_PARTITION_OF_IMAGE);

    if (bailout)
    {
        reboot_to_extended();
        //return 1;
    }

    // Keyboard detection done, load normal hid driver
    QProcess::execute("/sbin/modprobe usbhid");

#ifdef Q_WS_QWS
    QWSServer::setCursorVisible(true);
#endif

#ifdef ENABLE_LANGUAGE_CHOOSER
    // Language chooser at the bottom center
    LanguageDialog ld;
    ld.setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignHCenter | Qt::AlignBottom, ld.size(), a.desktop()->availableGeometry()));
    ld.show();
#endif

    // Main window in the middle of screen
    MainWindow mw;
    mw.setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, mw.size(), a.desktop()->availableGeometry()));
    mw.show();

    a.exec();
    reboot_to_extended();

    return 0;
}
