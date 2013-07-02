#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "imagewritethread.h"
#include "initdrivethread.h"
#include "confeditdialog.h"
#include "progressslideshowdialog.h"
#include "config.h"
#include "languagedialog.h"
#include <QMessageBox>
#include <QProgressDialog>
#include <QMap>
#include <QProcess>
#include <QDir>
#include <QDebug>
#include <QTimer>
#include <QLabel>
#include <QPixmap>
#include <QPainter>
#include <QKeyEvent>
#include <QApplication>
#include <QScreen>
#include <QSplashScreen>
#include <QDesktopWidget>

#ifdef Q_WS_QWS
#include <QWSServer>
#endif

/* Main window
 *
 * Initial author: Floris Bos
 * Maintained by Raspberry Pi
 *
 * See LICENSE.txt for license details
 *
 */

/* Flag to keep track wheter or not we already repartitioned. */
bool MainWindow::_partInited = false;

/* Flag to keep track of current display mode. */
int MainWindow::_currentMode = 0;

/* Which ListItem (if any) points to the recommended image. */
QListWidgetItem *recommendedItem = NULL;

MainWindow::MainWindow(QString *currentLangCode, QSplashScreen *splash, LanguageDialog *ld, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _qpd(NULL), _kcpos(0), _silent(false), _allowSilent(true), _currentLang(currentLangCode), _splash(splash), _ld(ld)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    setContextMenuPolicy(Qt::NoContextMenu);
    update_window_title();
    _kc << 0x01000013 << 0x01000013 << 0x01000015 << 0x01000015 << 0x01000012
        << 0x01000014 << 0x01000012 << 0x01000014 << 0x42 << 0x41;
    ui->list->installEventFilter(this);
    ui->advToolBar->setVisible(false);

    if (qApp->arguments().contains("-runinstaller") && !_partInited)
    {
        /* Repartition SD card first */
        _partInited = true;
        setEnabled(false);
        _qpd = new QProgressDialog( tr("Setting up SD card"), QString(), 0, 0, this);
        _qpd->setWindowModality(Qt::WindowModal);
        _qpd->setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowTitleHint);

        InitDriveThread *idt = new InitDriveThread(this);
        connect(idt, SIGNAL(statusUpdate(QString)), _qpd, SLOT(setLabelText(QString)));
        connect(idt, SIGNAL(completed()), _qpd, SLOT(deleteLater()));
        connect(idt, SIGNAL(error(QString)), this, SLOT(onError(QString)));

        idt->start();
        _qpd->exec();
        setEnabled(true);
    }

    populate();
}


MainWindow::~MainWindow()
{
    QProcess::execute("umount /mnt");
    delete ui;
}

/* Mount FAT partition, discover which images we have, and fill in the list */
void MainWindow::populate()
{
    if (!QFile::exists("/dev/mmcblk0p1"))
    {
        // mmcblk0p1 not ready yet, check back in a tenth of a second
        QTimer::singleShot(100, this, SLOT(populate()));
        return;
    }

    QProcess::execute("mount /dev/mmcblk0p1 /mnt");
    QMap<QString,QString> images = listInstalledImages();

    if (images.isEmpty())
    {
      QMessageBox::critical(this, tr("No OS Images Found on SD Card"), tr("Please add at least one OS image to the /images directory in order to proceed"));
      _allowSilent = false;
    }

    // Fill in list of images
    repopulate();

    if (!images.isEmpty())
    {
        QList<QListWidgetItem *> l = ui->list->findItems(RECOMMENDED_IMAGE, Qt::MatchContains);

        if (!l.isEmpty())
        {
            recommendedItem = l.first();
            update_recommended_label();
            ui->list->setCurrentItem(recommendedItem);
        }
        else
        {
            ui->list->setCurrentRow(0);
        }

        if (_allowSilent && !QFile::exists(FAT_PARTITION_OF_IMAGE) && images.count() == 1)
        {
            // No OS installed, perform silent installation
            qDebug() << "Performing silent installation";
            _silent = true;
            on_actionWrite_image_to_disk_triggered();
        }
    }

    bool osInstalled = QFile::exists(FAT_PARTITION_OF_IMAGE);
    ui->actionEdit_config->setEnabled(osInstalled);
    ui->actionCancel->setEnabled(osInstalled);
}

void MainWindow::repopulate()
{
    QMap<QString,QString> images = listInstalledImages();
    ui->list->clear();
    bool haveicons = false;
    QSize currentsize = ui->list->iconSize();
    QString iconFilename;

    for (QMap<QString,QString>::const_iterator iter = images.constBegin(); iter != images.constEnd(); iter++)
    {
        if (iter.key().contains("risc", Qt::CaseInsensitive) && QFile::exists("/mnt/images/RiscOS.png"))
            iconFilename = "/mnt/images/RiscOS.png";
        else if (iter.key().contains("arch", Qt::CaseInsensitive) && QFile::exists("/mnt/images/Archlinux.png"))
            iconFilename = "/mnt/images/Archlinux.png";
        else if (iter.key().contains("pidora", Qt::CaseInsensitive) && QFile::exists("/mnt/images/Pidora.png"))
            iconFilename = "/mnt/images/Pidora.png";
        else if ((iter.key().contains("wheezy", Qt::CaseInsensitive) || iter.key().contains("raspbian", Qt::CaseInsensitive)) && QFile::exists("/mnt/images/Raspbian.png"))
            iconFilename = "/mnt/images/Raspbian.png";
        else if (iter.key().contains("OpenELEC", Qt::CaseInsensitive) && QFile::exists("/mnt/images/OpenELEC.png"))
            iconFilename = "/mnt/images/OpenELEC.png";
        else if (iter.key().contains("raspbmc", Qt::CaseInsensitive) && QFile::exists("/mnt/images/RaspBMC.png"))
            iconFilename = "/mnt/images/RaspBMC.png";
        else
            iconFilename = "/mnt/images/default.png";

        QIcon icon;
        if (QFile::exists(iconFilename))
        {
            haveicons = true;
            icon = QIcon(iconFilename);
            QSize iconsize = icon.availableSizes().first();

            if (iconsize.width() > currentsize.width() || iconsize.height() > currentsize.height())
            {
                /* Make all icons as large as the largest icon we have */
                currentsize = QSize(qMax(iconsize.width(), currentsize.width()),qMax(iconsize.height(), currentsize.width()));
                ui->list->setIconSize(currentsize);
            }
        }
        QListWidgetItem *item = new QListWidgetItem(icon, iter.value(), ui->list);
        item->setData(Qt::UserRole, iter.key());
    }

    if (haveicons)
    {
        /* Giving items without icon a dummy icon to make them have equal height and text alignment */
        QPixmap dummyicon = QPixmap(currentsize.width(), currentsize.height());
        dummyicon.fill();

        for (int i=0; i< ui->list->count(); i++)
        {
            if (ui->list->item(i)->icon().isNull())
            {
                ui->list->item(i)->setIcon(dummyicon);
            }
        }
    }
}

QMap<QString,QString> MainWindow::listInstalledImages()
{
    QMap<QString,QString> images;
    QDir dir("/mnt/images","*.zip *.lzo *.gz *.bz2 *.xz");
    QStringList list = dir.entryList();

    foreach (QString image,list)
    {
        images[image] = imageFilenameToFriendlyName(image);
    }

    return images;
}

QString MainWindow::imageFilenameToFriendlyName(const QString &name)
{
    QString n = basefile(name);

    /* Replace underscores with spaces */
    n.replace('_', ' ');

    return n;
}

QString MainWindow::basefile(const QString &filename)
{
    /* Chop filename extension off */
    int imgpos = filename.lastIndexOf(".");

    if (imgpos != -1)
        return filename.mid(0, imgpos);
    else
        return filename;
}

void MainWindow::on_actionWrite_image_to_disk_triggered()
{
    if (_silent || QMessageBox::warning(this,
                                        tr("Confirm"),
                                        tr("Warning: this will install the selected Operating System. All existing data on the SD card will be overwritten."),
                                        QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
    {
        setEnabled(false);

        QString imagefile = ui->list->currentItem()->data(Qt::UserRole).toString();

        /* Optional file containing size of uncompressed image */
        QString sizefile = "/mnt/images/"+basefile(imagefile)+".size";
        QString slidesDirectory;

        /* Image specific slide directory */
        if (basefile(imagefile).contains("risc", Qt::CaseInsensitive) && QDir("/mnt/slides/RISCOS").exists())
            slidesDirectory = "/mnt/slides/RISCOS";
        else if (basefile(imagefile).contains("arch", Qt::CaseInsensitive) && QDir("/mnt/slides/Archlinux").exists())
            slidesDirectory = "/mnt/slides/Archlinux";
        else if (basefile(imagefile).contains("pidora", Qt::CaseInsensitive) && QDir("/mnt/slides/Pidora").exists())
            slidesDirectory = "/mnt/slides/Pidora";
        else if (((basefile(imagefile).contains("wheezy", Qt::CaseInsensitive)) || (basefile(imagefile).contains("raspbian", Qt::CaseInsensitive)))  && QDir("/mnt/slides/Raspbian").exists())
            slidesDirectory = "/mnt/slides/Raspbian";
        else if (basefile(imagefile).contains("OpenELEC", Qt::CaseInsensitive) && QDir("/mnt/slides/OpenELEC").exists())
            slidesDirectory = "/mnt/slides/OpenELEC";
        else if (basefile(imagefile).contains("raspbmc", Qt::CaseInsensitive) && QDir("/mnt/slides/RaspBMC").exists())
            slidesDirectory = "/mnt/slides/RaspBMC";
        else
            slidesDirectory = "/mnt/slides/default";

        /* Check if a subdirectory of localised slides exists */
        QString localisedSlidesDirectory = slidesDirectory + "/" + *_currentLang;
        if (QDir(localisedSlidesDirectory).exists())
        {
            slidesDirectory = localisedSlidesDirectory;
        }

        ImageWriteThread *t = new ImageWriteThread(imagefile, this);

        ProgressSlideshowDialog *p = new ProgressSlideshowDialog(slidesDirectory, "", 20, this);
        connect(t, SIGNAL(parsedImagesize(qint64)), p, SLOT(setMaximum(qint64)));

        if (QFile::exists(sizefile))
            {
                QFile f(sizefile);
                f.open(f.ReadOnly);
                p->setMaximum(f.readAll().trimmed().toLongLong());
                f.close();
                t->setParseArchiveHeader(false);
            }
        _qpd = p;
        _qpd->show();

        connect(t, SIGNAL(completed()), this, SLOT(onCompleted()));
        connect(t, SIGNAL(error(QString)), this, SLOT(onError(QString)));
        connect(t, SIGNAL(statusUpdate(QString)), _qpd, SLOT(setLabelText(QString)));
        t->start();

        if (_silent)
        {
            QTimer::singleShot(100, _qpd, SLOT(raise()));
        }
    }
}

void MainWindow::on_actionCancel_triggered()
{
    close();
}

void MainWindow::onCompleted()
{
    _qpd->hide();
    if (!_silent)
        QMessageBox::information(this,
                                 tr("Restore complete"),
                                 tr("Image applied successfully"), QMessageBox::Ok);
    _qpd->deleteLater();
    close();
}

void MainWindow::onError(const QString &msg)
{
    _qpd->hide();
    QMessageBox::critical(this, tr("Error"), msg, QMessageBox::Close);
    setEnabled(true);
}

void MainWindow::on_list_currentRowChanged()
{
    ui->actionWrite_image_to_disk->setEnabled(true);
    ui->actionRemove_image->setEnabled(true);
}

void MainWindow::update_window_title()
{
    setWindowTitle(QString(tr("NOOBS v%1 - Built: %2")).arg(VERSION_NUMBER, QString::fromLocal8Bit(__DATE__)));
}

void MainWindow::update_recommended_label()
{
    if (recommendedItem)
    {
        recommendedItem->setText(imageFilenameToFriendlyName(recommendedItem->data(Qt::UserRole).toString())+QString(" [%1]").arg(tr("RECOMMENDED")));
    }
}

void MainWindow::changeEvent(QEvent* event)
{
    if (event && event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
        update_window_title();
        update_recommended_label();
    }

    QMainWindow::changeEvent(event);
}

void MainWindow::displayMode(QString cmd, QString mode)
{
#ifdef Q_WS_QWS
    // Trigger framebuffer resize
    QProcess *resize = new QProcess(this);
    resize->start(QString("sh -c \"tvservice -o; tvservice %1;\"").arg(cmd));
    resize->waitForFinished(4000);

    // Update screen resolution with current value (even if we didn't
    // get what we thought we'd get)
    QProcess *update = new QProcess(this);
    update->start(QString("sh -c \"tvservice -s | cut -d , -f 2 | cut -d \' \' -f 2 | cut -d x -f 1;tvservice -s | cut -d , -f 2 | cut -d \' \' -f 2 | cut -d x -f 2\""));
    update->waitForFinished(4000);
    update->setProcessChannelMode(QProcess::MergedChannels);

    QTextStream stream(update);
    QString xres = stream.readLine();
    QString yres = stream.readLine();

    QScreen::instance()->setMode(xres.toInt(), yres.toInt(), 16);

    // Update UI item locations
    _splash->setPixmap(QPixmap(":/wallpaper.png"));
    _ld->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignHCenter | Qt::AlignBottom, _ld->size(), qApp->desktop()->availableGeometry()));
    this->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, this->size(), qApp->desktop()->availableGeometry()));

    // Refresh screen
    qApp->processEvents();
    QWSServer::instance()->refresh();

    // TODO: Write choice to config.txt of installed OS
    // during OS install and use this choice if present by default

    // Inform user of resolution change with message box.
    QMessageBox *mbox = new QMessageBox;
    mbox->setWindowTitle(tr("Display Mode Changed"));
    mbox->setText(QString(tr("Display mode changed to %1")).arg(mode));
    mbox->setStandardButtons(0);
    mbox->show();
    QTimer::singleShot(2000, mbox, SLOT(hide()));

    // In case they can't see the message box, inform that mode change
    // is occuring by turning on the LED during the change
    QProcess *led_blink = new QProcess(this);
    led_blink->start("sh -c \"echo 1 > /sys/class/leds/led0/brightness; sleep 3; echo 0 > /sys/class/leds/led0/brightness\"");
#else
    Q_UNUSED(cmd)
    Q_UNUSED(mode)
#endif
}

bool MainWindow::eventFilter(QObject *, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);

        // Let user find the best display mode for their display
        // experimentally by using keys 1-4. NOOBS will default to using HDMI preferred mode.

        // HDMI preferred mode
        if (keyEvent->key() == Qt::Key_1 && _currentMode != 0)
        {
            displayMode("-p", tr("HDMI preferred mode"));
            _currentMode = 0;
        }
        // HDMI safe mode
        if (keyEvent->key() == Qt::Key_2 && _currentMode != 1)
        {
            displayMode("-e \'DMT 4 DVI\'", tr("HDMI safe mode"));
            _currentMode = 1;
        }
        // Composite PAL
        if (keyEvent->key() == Qt::Key_3 && _currentMode != 2)
        {
            displayMode("-c \'NTSC 4:3\'", tr("composite PAL mode"));
            _currentMode = 2;
        }
         // Composite NTSC
        if (keyEvent->key() == Qt::Key_4 && _currentMode != 3)
        {
            displayMode("-c \'PAL 4:3\'", tr("composite NTSC mode"));
            _currentMode = 3;
        }
        // Catch Return key to trigger OS install
        if (keyEvent->key() == Qt::Key_Return)
            on_actionWrite_image_to_disk_triggered();
        else if (_kc.at(_kcpos) == keyEvent->key())
        {
            _kcpos++;
            if (_kcpos == _kc.size())
            {
                inputSequence();
                _kcpos = 0;
            }
        }
        else
            _kcpos=0;
    }

    return false;
}

void MainWindow::inputSequence()
{
    QLabel* info = new QLabel(this);
    info->setPixmap(QPixmap("/usr/data"));
    info->setGeometry(50,10,389,260);
    info->show();
}

void MainWindow::on_actionAdvanced_triggered(bool checked)
{
    ui->advToolBar->setVisible(checked);
}

void MainWindow::on_actionEdit_config_triggered()
{
    ConfEditDialog d;
    d.exec();
}

void MainWindow::on_actionBrowser_triggered()
{
    if (!QFile::exists("/tmp/resolv.conf"))
    {
        _qpd = new QProgressDialog(tr("Enabling network interface"), QString(), 0,0, this);
        _qpd->setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
        _qpd->show();
        QProcess *proc = new QProcess(this);
        connect(proc, SIGNAL(finished(int)), _qpd, SLOT(hide()));
        connect(proc, SIGNAL(finished(int)), this, SLOT(startBrowser()));
        connect(proc, SIGNAL(finished(int)), _qpd, SLOT(deleteLater()));
        proc->start("/sbin/ifup eth0");
    }
    else
    {
        startBrowser();
    }
}

void MainWindow::startBrowser()
{
    if (!QFile::exists("/tmp/resolv.conf"))
    {
        QMessageBox::critical(this,
                              tr("No network access"),
                              tr("Wired network access is required for this feature. Please insert a network cable into the network port."),
                              QMessageBox::Close);
    }
    else
    {
        QProcess *proc = new QProcess(this);
        proc->start("arora " HOMEPAGE);
    }
}

void MainWindow::on_list_doubleClicked(const QModelIndex &index)
{
    if (index.isValid())
    {
        on_actionWrite_image_to_disk_triggered();
    }
}
