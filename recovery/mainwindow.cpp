#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "multiimagewritethread.h"
#include "initdrivethread.h"
#include "confeditdialog.h"
#include "progressslideshowdialog.h"
#include "config.h"
#include "languagedialog.h"
#include "json.h"
#include "util.h"
#include "twoiconsdelegate.h"
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
#include <QSettings>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkDiskCache>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <inttypes.h>

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

MainWindow::MainWindow(const QString &defaultDisplay, QSplashScreen *splash, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _qpd(NULL), _kcpos(0), _defaultDisplay(defaultDisplay), _splash(splash),
    _silent(false), _allowSilent(false), _settings(NULL),
    _activatedEth(false), _numInstalledOS(0), _netaccess(NULL), _displayModeBox(NULL)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    setContextMenuPolicy(Qt::NoContextMenu);
    update_window_title();
    _kc << 0x01000013 << 0x01000013 << 0x01000015 << 0x01000015 << 0x01000012
        << 0x01000014 << 0x01000012 << 0x01000014 << 0x42 << 0x41;
    ui->list->setItemDelegate(new TwoIconsDelegate(this));
    ui->list->installEventFilter(this);
    ui->advToolBar->setVisible(false);

    QRect s = QApplication::desktop()->screenGeometry();
    if (s.height() < 500)
        resize(s.width()-10, s.height()-100);

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
        connect(idt, SIGNAL(query(QString, QString, QMessageBox::StandardButton*)),
                this, SLOT(onQuery(QString, QString, QMessageBox::StandardButton*)),
                Qt::BlockingQueuedConnection);

        idt->start();
        _qpd->exec();
        setEnabled(true);
    }

    if (getFileContents("/proc/cmdline").contains("silentinstall"))
    {
        /* If silentinstall is specified, auto-install single image in /os */
        _allowSilent = true;
    }

    /* Disable online help buttons until network is functional */
    ui->actionBrowser->setEnabled(false);
    startNetworking();
    QTimer::singleShot(1, this, SLOT(populate()));
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

    /* Ask user to wait while list is populated */
    if (!_allowSilent)
    {
        _qpd = new QProgressDialog(tr("Please wait while NOOBS initialises"), QString(), 0, 0, this);
        _qpd->setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
        _qpd->show();
        QTimer::singleShot(2000, this, SLOT(hideDialogIfNoNetwork()));
    }

    if (QFile::exists(SETTINGS_PARTITION))
    {
        /* Try mounting read-only first, if fails try read-write as it may recover from journal */
        if (QProcess::execute("mount -o remount,ro /settings")!=0
            && QProcess::execute("mount -o remount,rw /settings")!=0)
        {
            if (QMessageBox::question(this,
                                      tr("Error mounting settings partition"),
                                      tr("Persistent settings partition seems corrupt. Reformat?"),
                                      QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
            {
                QProcess::execute("umount /settings");
                if (QProcess::execute("/usr/sbin/mkfs.ext4 " SETTINGS_PARTITION) != 0
                    || QProcess::execute("mount " SETTINGS_PARTITION " /settings") != 0)
                {
                    QMessageBox::critical(this, tr("Reformat failed"), tr("SD card might be damaged"), QMessageBox::Close);
                }

                rebuildInstalledList();
                QProcess::execute("mount -o remount,ro " SETTINGS_PARTITION);
            }
        }

        _settings = new QSettings("/settings/noobs.conf", QSettings::IniFormat, this);

        /* Restore saved display mode */
        qDebug() << "Default display mode is " << _defaultDisplay;
        int mode = _settings->value("display_mode", _defaultDisplay).toInt();
        if (mode)
        {
            displayMode(mode, true);
        }
        QProcess::execute("mount -o remount,rw /settings");
        _settings->setValue("display_mode", _defaultDisplay);
        _settings->sync();
        QProcess::execute("mount -o remount,ro /settings");

 }


    QProcess::execute("mount -o ro -t vfat /dev/mmcblk0p1 /mnt");

    // Fill in list of images
    repopulate();
    _availableMB = (getFileContents("/sys/class/block/mmcblk0p3/start").trimmed().toULongLong()-getFileContents("/sys/class/block/mmcblk0p2/start").trimmed().toULongLong())/2048;
    updateNeeded();

    if (ui->list->count() != 0)
    {
        QList<QListWidgetItem *> l = ui->list->findItems(RECOMMENDED_IMAGE, Qt::MatchExactly);

        if (!l.isEmpty())
        {
            ui->list->setCurrentItem(l.first());
        }
        else
        {
            ui->list->setCurrentRow(0);
        }

        if (_allowSilent && !QFile::exists(FAT_PARTITION_OF_IMAGE) && ui->list->count() == 1)
        {
            // No OS installed, perform silent installation
            qDebug() << "Performing silent installation";
            _silent = true;
            ui->list->item(0)->setCheckState(Qt::Checked);
            on_actionWrite_image_to_disk_triggered();
        }
    }

    bool osInstalled = QFile::exists(FAT_PARTITION_OF_IMAGE);
    ui->actionCancel->setEnabled(osInstalled);
}

void MainWindow::remountSettingsRW()
{
    QProcess::execute("mount -o remount,rw /settings");
}

void MainWindow::repopulate()
{
    QMap<QString,QVariantMap> images = listImages();
    ui->list->clear();
    bool haveicons = false;
    QSize currentsize = ui->list->iconSize();
    QIcon localIcon(":/icons/hdd.png");
    QIcon internetIcon(":/icons/download.png");
    _numInstalledOS = 0;

    foreach (QVariant v, images.values())
    {
        QVariantMap m = v.toMap();
        QString flavour = m.value("name").toString();
        QString description = m.value("description").toString();
        QString folder  = m.value("folder").toString();
        QString iconFilename = m.value("icon").toString();
        bool installed = m.value("installed").toBool();
        bool recommended = m.value("recommended").toBool();

        if (!iconFilename.isEmpty() && !iconFilename.contains('/'))
            iconFilename = folder+"/"+iconFilename;
        if (!QFile::exists(iconFilename))
        {
            iconFilename = folder+"/"+flavour+".png";
            iconFilename.replace(' ', '_');
        }

        QString friendlyname = flavour;
        if (recommended)
            friendlyname += " ["+tr("RECOMMENDED")+"]";
        if (installed)
        {
            friendlyname += " ["+tr("INSTALLED")+"]";
            _numInstalledOS++;
        }
        if (!description.isEmpty())
            friendlyname += "\n"+description;

        QIcon icon;
        if (QFile::exists(iconFilename))
        {
            icon = QIcon(iconFilename);
            QList<QSize> avs = icon.availableSizes();
            if (avs.isEmpty())
            {
                /* Icon file corrupt */
                icon = QIcon();
            }
            else
            {
                QSize iconsize = avs.first();
                haveicons = true;

                if (iconsize.width() > currentsize.width() || iconsize.height() > currentsize.height())
                {
                    /* Make all icons as large as the largest icon we have */
                    currentsize = QSize(qMax(iconsize.width(), currentsize.width()),qMax(iconsize.height(), currentsize.height()));
                    ui->list->setIconSize(currentsize);
                }
            }
        }
        QListWidgetItem *item = new QListWidgetItem(icon, friendlyname);
        item->setData(Qt::UserRole, m);
        if (installed)
        {
            item->setData(Qt::BackgroundColorRole, INSTALLED_OS_BACKGROUND_COLOR);
            item->setCheckState(Qt::Checked);
        }
        else
            item->setCheckState(Qt::Unchecked);

        if (folder.startsWith("/mnt"))
            item->setData(SecondIconRole, localIcon);
        else
            item->setData(SecondIconRole, internetIcon);

        if (recommended)
            ui->list->insertItem(0, item);
        else
            ui->list->addItem(item);
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

/* Whether this OS should be displayed in the list of installable OSes */
bool canInstallOs(const QString& name, const QVariantMap& values)
{
    /* Can't simply pull "name" from "values" because in some JSON files it's "os_name" and in others it's "name"

    /* RISC_OS needs a matching riscos_offset */
    if (nameMatchesRiscOS(name))
    {
        if (!values.contains(RISCOS_OFFSET_KEY) || (values.value(RISCOS_OFFSET_KEY).toInt() != RISCOS_OFFSET))
        {
            return false;
        }
    }

    return true;
}

QMap<QString, QVariantMap> MainWindow::listImages()
{
    QMap<QString,QVariantMap> images;

    /* Local image folders */
    QDir dir("/mnt/os", "", QDir::Name, QDir::Dirs | QDir::NoDotAndDotDot);
    QStringList list = dir.entryList();

    foreach (QString image,list)
    {
        QString imagefolder = "/mnt/os/"+image;
        if (!QFile::exists(imagefolder+"/os.json"))
            continue;
        QVariantMap osv = Json::loadFromFile(imagefolder+"/os.json").toMap();

        QString basename = osv.value("name").toString();
        if (canInstallOs(basename, osv))
        {
            if (QFile::exists(imagefolder+"/flavours.json"))
            {
                QVariantMap v = Json::loadFromFile(imagefolder+"/flavours.json").toMap();
                QVariantList fl = v.value("flavours").toList();

                foreach (QVariant f, fl)
                {
                    QVariantMap fm  = f.toMap();
                    if (fm.contains("name"))
                    {
                        QString name = fm.value("name").toString();
                        if (name == RECOMMENDED_IMAGE)
                            fm["recommended"] = true;
                        fm["folder"] = imagefolder;
                        fm["release_date"] = osv.value("release_date");
                        images[imagefolder+"#"+name] = fm;
                    }
                }
            }
            else
            {
                QString name = basename;
                if (name.contains(RECOMMENDED_IMAGE))
                    osv["recommended"] = true;
                osv["folder"] = imagefolder;
                images[imagefolder+"#"+name] = osv;
            }
        }
    }

    /* Also add information about files downloaded from Internet */
    if (_settings)
    {
        QVariantList i = Json::loadFromFile("/settings/installed_os.json").toList();
        foreach (QVariant v, i)
        {
            QVariantMap m = v.toMap();
            m["installed"] = true;
            QString flavour = m.value("name").toString();
            QString imagefolder = m.value("folder").toString();
            images[imagefolder+"#"+flavour] = m;
        }
    }

    for (QMap<QString,QVariantMap>::iterator i = images.begin(); i != images.end(); i++)
    {
        if (!i.value().contains("nominal_size"))
        {
            /* Calculate nominal_size based on information inside partitions.json */
            int nominal_size = 0;
            QVariantMap pv = Json::loadFromFile(i.value().value("folder").toString()+"/partitions.json").toMap();
            QVariantList pvl = pv.value("partitions").toList();

            foreach (QVariant v, pvl)
            {
                QVariantMap pv = v.toMap();
                nominal_size += pv.value("partition_size_nominal").toInt();
                nominal_size += 1; /* Overhead per partition for EBR */
            }

            i.value().insert("nominal_size", nominal_size);
        }
    }

    return images;
}

void MainWindow::on_actionWrite_image_to_disk_triggered()
{
    remountSettingsRW();

    if (_silent || QMessageBox::warning(this,
                                        tr("Confirm"),
                                        tr("Warning: this will install the selected Operating System(s). All existing data on the SD card will be overwritten, including any OSes that are already installed."),
                                        QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
    {
        setEnabled(false);
        _numMetaFilesToDownload = 0;

        QList<QListWidgetItem *> selected = selectedItems();
        foreach (QListWidgetItem *item, selected)
        {
            QVariantMap entry = item->data(Qt::UserRole).toMap();

            if (!entry.contains("folder"))
            {
                QDir d;
                QString folder = "/settings/os/"+entry.value("name").toString();
                folder.replace(' ', '_');
                if (!d.exists(folder))
                    d.mkpath(folder);

                downloadMetaFile(entry.value("os_info").toString(), folder+"/os.json");
                downloadMetaFile(entry.value("partitions_info").toString(), folder+"/partitions.json");

                if (entry.contains("marketing_info"))
                    downloadMetaFile(entry.value("marketing_info").toString(), folder+"/marketing.tar");

                if (entry.contains("partition_setup"))
                    downloadMetaFile(entry.value("partition_setup").toString(), folder+"/partition_setup.sh");

                if (entry.contains("icon"))
                    downloadMetaFile(entry.value("icon").toString(), folder+"/icon.png");
            }
        }

        if (_numMetaFilesToDownload == 0)
        {
            /* All OSes selected are local */
            startImageWrite();
        }
        else if (!_silent)
        {
            _qpd = new QProgressDialog(tr("The install process will begin shortly."), QString(), 0, 0, this);
            _qpd->setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
            _qpd->show();
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
    QProcess::execute("mount -o remount,rw /settings");
    QSettings settings("/settings/noobs.conf", QSettings::IniFormat, this);
    settings.setValue("default_partition_to_boot", "800");
    settings.sync();
    QProcess::execute("mount -o remount,ro /settings");

    if (!_silent)
        QMessageBox::information(this,
                                 tr("OS(es) installed"),
                                 tr("OS(es) Installed Successfully"), QMessageBox::Ok);
    _qpd->deleteLater();
    _qpd = NULL;
    close();
}

void MainWindow::onError(const QString &msg)
{
    _qpd->hide();
    QMessageBox::critical(this, tr("Error"), msg, QMessageBox::Close);
    setEnabled(true);
}

void MainWindow::onQuery(const QString &msg, const QString &title, QMessageBox::StandardButton* answer)
{
    _qpd->hide();
    *answer = QMessageBox::question(this, title, msg, QMessageBox::Yes|QMessageBox::No);
    setEnabled(true);
}

void MainWindow::on_list_currentRowChanged()
{
    QListWidgetItem *item = ui->list->currentItem();
    ui->actionEdit_config->setEnabled(item && item->data(Qt::UserRole).toMap().contains("partitions"));
}

void MainWindow::update_window_title()
{
    setWindowTitle(QString(tr("NOOBS v%1 - Built: %2")).arg(VERSION_NUMBER, QString::fromLocal8Bit(__DATE__)));
}

void MainWindow::changeEvent(QEvent* event)
{
    if (event && event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
        update_window_title();
        updateNeeded();
        //repopulate();
    }

    QMainWindow::changeEvent(event);
}

void MainWindow::displayMode(int modenr, bool silent)
{
#ifdef Q_WS_QWS
    QString cmd, mode;

    if (!silent && _displayModeBox)
    {
        /* User pressed another mode selection key while the confirmation box is being displayed */
        silent = true;
        _displayModeBox->close();
    }

    switch (modenr)
    {
    case 0:
        cmd  = "-p";
        mode = tr("HDMI preferred mode");
        break;
    case 1:
        cmd  = "-e \'DMT 4 DVI\'";
        mode = tr("HDMI safe mode");
        break;
    case 2:
        cmd  = "-c \'PAL 4:3\'";
        mode = tr("composite PAL mode");
        break;
    case 3:
        cmd  = "-c \'NTSC 4:3\'";
        mode = tr("composite NTSC mode");
        break;

    default:
        // unknown mode
        return;
    }
    _currentMode = modenr;

    // Trigger framebuffer resize
    QProcess *presize = new QProcess(this);
    presize->start(QString("sh -c \"tvservice -o; tvservice %1;\"").arg(cmd));
    presize->waitForFinished(4000);

    // Update screen resolution with current value (even if we didn't
    // get what we thought we'd get)
    QProcess *update = new QProcess(this);
    update->start(QString("sh -c \"tvservice -s | cut -d , -f 2 | cut -d \' \' -f 2 | cut -d x -f 1;tvservice -s | cut -d , -f 2 | cut -d \' \' -f 2 | cut -d x -f 2\""));
    update->waitForFinished(4000);
    update->setProcessChannelMode(QProcess::MergedChannels);

    QTextStream stream(update);
    int xres = stream.readLine().toInt();
    int yres = stream.readLine().toInt();
    int oTop = 0, oBottom = 0, oLeft = 0, oRight = 0;
    getOverscan(oTop, oBottom, oLeft, oRight);
    qDebug() << "Current overscan" << "top" << oTop << "bottom" << oBottom << "left" << oLeft << "right" << oRight;
    QScreen::instance()->setMode(xres-oLeft-oRight, yres-oTop-oBottom, 16);

    // Resize this window depending on screen resolution
    QRect s = QApplication::desktop()->screenGeometry();
    if (s.height() < 500)
        resize(s.width()-10, s.height()-100);
    else
        resize(575, 450);

    // Update UI item locations
    _splash->setPixmap(QPixmap(":/wallpaper.png"));
    LanguageDialog *ld = LanguageDialog::instance("en", "gb");
    ld->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignHCenter | Qt::AlignBottom, ld->size(), qApp->desktop()->availableGeometry()));
    this->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, this->size(), qApp->desktop()->availableGeometry()));

    // Refresh screen
    qApp->processEvents();
    QWSServer::instance()->refresh();

    // In case they can't see the message box, inform that mode change
    // is occuring by turning on the LED during the change
    QProcess *led_blink = new QProcess(this);
    connect(led_blink, SIGNAL(finished(int)), led_blink, SLOT(deleteLater()));
    led_blink->start("sh -c \"echo 1 > /sys/class/leds/led0/brightness; sleep 3; echo 0 > /sys/class/leds/led0/brightness\"");

    // Inform user of resolution change with message box.
    if (!silent && _settings)
    {
        _displayModeBox = new QMessageBox(QMessageBox::Question,
                      tr("Display Mode Changed"),
                      tr("Display mode changed to %1\nWould you like to make this setting permanent?").arg(mode),
                      QMessageBox::Yes | QMessageBox::No);
        _displayModeBox->installEventFilter(this);
        _displayModeBox->exec();

        if (_displayModeBox->standardButton(_displayModeBox->clickedButton()) == QMessageBox::Yes)
        {
            remountSettingsRW();
            _settings->setValue("display_mode", modenr);
            _settings->sync();
            ::sync();
        }
        _displayModeBox = NULL;
    }

    /*
    QMessageBox *mbox = new QMessageBox;
    mbox->setWindowTitle(tr("Display Mode Changed"));
    mbox->setText(QString(tr("Display mode changed to %1")).arg(mode));
    mbox->setStandardButtons(0);
    mbox->show();
    QTimer::singleShot(2000, mbox, SLOT(hide()));
    */

#else
    Q_UNUSED(modenr)
    Q_UNUSED(silent)
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
            displayMode(0);
        }
        // HDMI safe mode
        if (keyEvent->key() == Qt::Key_2 && _currentMode != 1)
        {
            displayMode(1);
        }
        // Composite PAL
        if (keyEvent->key() == Qt::Key_3 && _currentMode != 2)
        {
            displayMode(2);
        }
         // Composite NTSC
        if (keyEvent->key() == Qt::Key_4 && _currentMode != 3)
        {
            displayMode(3);
        }
        // Catch Return key to trigger OS boot
        if (keyEvent->key() == Qt::Key_Return)
        {
            on_list_doubleClicked(ui->list->currentIndex());
        }
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
    info->setGeometry(0,0,640,480);
    info->show();
}

void MainWindow::on_actionAdvanced_triggered(bool checked)
{
    ui->advToolBar->setVisible(checked);
}

void MainWindow::on_actionEdit_config_triggered()
{
    /* If no installed OS is selected, default to first extended partition */
    QString partition = FAT_PARTITION_OF_IMAGE;
    QListWidgetItem *item = ui->list->currentItem();

    if (item && item->data(Qt::UserRole).toMap().contains("partitions"))
    {
        QVariantList l = item->data(Qt::UserRole).toMap().value("partitions").toList();
        if (!l.isEmpty())
            partition = l.first().toString();
    }

    ConfEditDialog d(partition);
    d.exec();
}

void MainWindow::on_actionBrowser_triggered()
{
    startBrowser();
}

bool MainWindow::requireNetwork()
{
    if (!QFile::exists("/tmp/resolv.conf"))
    {
        QMessageBox::critical(this,
                              tr("No network access"),
                              tr("Wired network access is required for this feature. Please insert a network cable into the network port."),
                              QMessageBox::Close);
        return false;
    }

    return true;
}

void MainWindow::startBrowser()
{
    if (!requireNetwork())
        return;
    QProcess *proc = new QProcess(this);
    QString lang = LanguageDialog::instance("en", "gb")->currentLanguage();
    if (lang == "gb" || lang == "us" || lang == "")
        lang = "en";
    proc->start("arora -lang "+lang+" "+HOMEPAGE);
}

void MainWindow::on_list_doubleClicked(const QModelIndex &index)
{
    if (index.isValid())
    {
        QListWidgetItem *item = ui->list->currentItem();
        if (item->checkState() == Qt::Unchecked)
            item->setCheckState(Qt::Checked);
        else
            item->setCheckState(Qt::Unchecked);
    }
}

void MainWindow::startNetworking()
{
    if (!QFile::exists("/sys/class/net/eth0"))
    {
        /* eth0 not available yet, check back in a tenth of a second */
        QTimer::singleShot(100, this, SLOT(startNetworking()));
        return;
    }

    QByteArray carrier = getFileContents("/sys/class/net/eth0/carrier").trimmed();
    if (carrier.isEmpty() && !_activatedEth)
    {
        QProcess::execute("/sbin/ifconfig eth0 up");
        _activatedEth = true;
    }

    if (carrier != "1")
    {
        /* cable not detected yet, check back in a tenth of a second */
        QTimer::singleShot(100, this, SLOT(startNetworking()));
        return;
    }

    QProcess *proc = new QProcess(this);
    connect(proc, SIGNAL(finished(int)), this, SLOT(ifupFinished(int)));
    /* Try enabling interface twice as sometimes it times out before getting a DHCP lease */
    proc->start("sh -c \"ifup eth0 || ifup eth0\"");
}

void MainWindow::ifupFinished(int)
{
    QProcess *p = qobject_cast<QProcess*> (sender());

    if (QFile::exists("/etc/resolv.conf"))
    {
        qDebug() << "Network up";
        if (!_netaccess)
        {
            remountSettingsRW();
            QDir dir;
            dir.mkdir("/settings/cache");
            _netaccess = new QNetworkAccessManager(this);
            QNetworkDiskCache *_cache = new QNetworkDiskCache(this);
            _cache->setCacheDirectory("/settings/cache");
            _cache->setMaximumCacheSize(8 * 1024 * 1024);
            _netaccess->setCache(_cache);

            downloadList(DEFAULT_REPO_SERVER);
        }
        ui->actionBrowser->setEnabled(true);
        emit networkUp();
    }

    p->deleteLater();
}

void MainWindow::downloadList(const QString &urlstring)
{
    QNetworkReply *reply = _netaccess->get(QNetworkRequest(QUrl(urlstring)));
    connect(reply, SIGNAL(finished()), this, SLOT(downloadListRedirectCheck()));
}

void MainWindow::rebuildInstalledList()
{
    /* Recovery procedure for damaged settings partitions
     * Scan partitions for operating systems installed and regenerate a minimal
     * installed_os.json so that boot menu can function.
     */
    QDir dir;
    dir.mkdir("/mnt2");
    QVariantList installedlist;

    for (int i=5; i<=MAXIMUM_PARTITIONS; i++)
    {
        QString part = "/dev/mmcblk0p"+QString::number(i);

        if (QFile::exists(part) && QProcess::execute("mount -t vfat "+part+" /mnt2") == 0)
        {
            qDebug() << "Scanning" << part;
            if (QFile::exists("/mnt2/os_config.json"))
            {
                QVariantMap m = Json::loadFromFile("/mnt2/os_config.json").toMap();
                QString f = m.value("flavour").toString();
                if (!f.isEmpty())
                {
                    qDebug() << "OS found:" << f;
                    QVariantMap osinfo;
                    osinfo.insert("name", f);
                    osinfo.insert("release_date", m.value("release_date"));
                    osinfo.insert("partitions", m.value("partitions"));
                    osinfo.insert("folder", m.value("imagefolder"));
                    osinfo.insert("description", m.value("description"));
                    installedlist.append(osinfo);
                }
            }
            QProcess::execute("umount /mnt2");
        }
    }

    if (!installedlist.isEmpty())
    {
        Json::saveToFile("/settings/installed_os.json", installedlist);
    }
}

void MainWindow::downloadListComplete()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    int httpstatuscode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    if (reply->error() != reply->NoError || httpstatuscode < 200 || httpstatuscode > 399)
    {
        _qpd->hide();
        QMessageBox::critical(this, tr("Download error"), tr("Error downloading distribution list from Internet"), QMessageBox::Close);
    }
    else
    {
        processJson(Json::parse( reply->readAll() ));
    }

    reply->deleteLater();
}

void MainWindow::processJson(QVariant json)
{
    if (json.isNull())
    {
        QMessageBox::critical(this, tr("Error"), tr("Error parsing list.json downloaded from server"), QMessageBox::Close);
        return;
    }

    QSet<QString> iconurls;
    QIcon internetIcon(":/icons/download.png");
    QVariantList list = json.toMap().value("os_list").toList();

    foreach (QVariant osv, list)
    {
        QVariantMap  os = osv.toMap();

        QString basename = os.value("os_name").toString();
        if (canInstallOs(basename, os))
        {
            if (os.contains("flavours"))
            {
                QVariantList flavours = os.value("flavours").toList();

                foreach (QVariant flv, flavours)
                {
                    QVariantMap flavour = flv.toMap();
                    QVariantMap item = os;
                    QString name        = flavour.value("name").toString();
                    QString description = flavour.value("description").toString();
                    QString iconurl     = flavour.value("icon").toString();

                    item.insert("name", name);
                    item.insert("description", description);
                    item.insert("icon", iconurl);

                    if (!alreadyHasItem(name, item.value("release_date")))
                    {
                        if (!iconurl.isEmpty())
                            iconurls.insert(iconurl);

                        bool recommended = (name == RECOMMENDED_IMAGE);
                        if (recommended)
                            name += " ["+tr("RECOMMENDED")+"]";

                        QListWidgetItem *witem = new QListWidgetItem(name+"\n"+description);
                        witem->setCheckState(Qt::Unchecked);
                        witem->setData(Qt::UserRole, item);
                        witem->setData(SecondIconRole, internetIcon);

                        if (recommended)
                            ui->list->insertItem(0, witem);
                        else
                            ui->list->addItem(witem);
                    }
                }
            }
            if (os.contains("description"))
            {
                QString name = basename;
                QString description = os.value("description").toString();
                if (!alreadyHasItem(name, os.value("release_date")))
                {
                    os["name"] = name;
                    QString iconurl = os.value("icon").toString();
                    if (!iconurl.isEmpty())
                        iconurls.insert(iconurl);

                    QListWidgetItem *witem = new QListWidgetItem(name+"\n"+description, ui->list);
                    witem->setCheckState(Qt::Unchecked);
                    witem->setData(Qt::UserRole, os);
                    witem->setData(SecondIconRole, internetIcon);
                }
            }
        }
    }

    /* Download icons */
    _numIconsToDownload = iconurls.count();

    if (_numIconsToDownload)
    {
        foreach (QString iconurl, iconurls)
        {
            downloadIcon(iconurl, iconurl);
        }
    }
    else
    {
        if (_qpd)
        {
            _qpd->deleteLater();
            _qpd = NULL;
        }
    }
}

void MainWindow::downloadIcon(const QString &urlstring, const QString &originalurl)
{
    QUrl url(urlstring);
    QNetworkRequest request(url);
    request.setAttribute(QNetworkRequest::User, originalurl);
    QNetworkReply *reply = _netaccess->get(request);
    connect(reply, SIGNAL(finished()), this, SLOT(downloadIconRedirectCheck()));
}

bool MainWindow::alreadyHasItem(const QVariant &name, const QVariant &releasedate)
{
    for (int i=0; i<ui->list->count(); i++)
    {
        QVariantMap m = ui->list->item(i)->data(Qt::UserRole).toMap();
        if (m.value("name").toString() == name.toString() && m.value("release_date").toString() == releasedate.toString())
        {
            return true;
        }
    }

    return false;
}

void MainWindow::downloadIconComplete()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    QString url = reply->url().toString();
    QString originalurl = reply->request().attribute(QNetworkRequest::User).toString();
    int httpstatuscode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    if (reply->error() != reply->NoError || httpstatuscode < 200 || httpstatuscode > 399)
    {
        //QMessageBox::critical(this, tr("Download error"), tr("Error downloading icon '%1'").arg(reply->url().toString()), QMessageBox::Close);
        qDebug() << "Error downloading icon" << url;
    }
    else
    {
        QPixmap pix;
        pix.loadFromData(reply->readAll());
        QIcon icon(pix);

        for (int i=0; i<ui->list->count(); i++)
        {
            QVariantMap m = ui->list->item(i)->data(Qt::UserRole).toMap();
            ui->list->setIconSize(QSize(40,40));
            if (m.value("icon") == originalurl)
            {
                ui->list->item(i)->setIcon(icon);
            }
        }
    }
    if (--_numIconsToDownload == 0 && _qpd)
    {
        _qpd->hide();
        _qpd->deleteLater();
        _qpd = NULL;
    }

    reply->deleteLater();
}

QList<QListWidgetItem *> MainWindow::selectedItems()
{
    QList<QListWidgetItem *> selected;

    for (int i=0; i < ui->list->count(); i++)
    {
        QListWidgetItem *item = ui->list->item(i);
        if (item->checkState())
        {
            selected.append(item);
        }
    }

    return selected;
}

void MainWindow::updateNeeded()
{
    bool enableOk = false;
    QColor colorNeededLabel = Qt::black;
    bool bold = false;

    _neededMB = 0;
    QList<QListWidgetItem *> selected = selectedItems();

    foreach (QListWidgetItem *item, selected)
    {
        QVariantMap entry = item->data(Qt::UserRole).toMap();
        _neededMB += entry.value("nominal_size").toInt();

        if (nameMatchesRiscOS(entry.value("name").toString()))
        {
            /* RiscOS needs to start at a predetermined sector, calculate the extra space needed for that */
            int startSector = getFileContents("/sys/class/block/mmcblk0p2/start").trimmed().toULongLong();
            _neededMB += (RISCOS_SECTOR_OFFSET - startSector)/2048;
        }
    }

    ui->neededLabel->setText(QString("%1: %2 MB").arg(tr("Needed"), QString::number(_neededMB)));
    ui->availableLabel->setText(QString("%1: %2 MB").arg(tr("Available"), QString::number(_availableMB)));

    if (_neededMB > _availableMB)
    {
        /* Selection exceeds available space, make label red to alert user */
        colorNeededLabel = Qt::red;
        bold = true;
    }
    else
    {
        if (_neededMB)
        {
            /* Enable OK button if a selection has been made that fits on the card */
            enableOk = true;
        }
    }

    ui->actionWrite_image_to_disk->setEnabled(enableOk);
    QPalette p = ui->neededLabel->palette();
    if (p.color(QPalette::WindowText) != colorNeededLabel)
    {
        p.setColor(QPalette::WindowText, colorNeededLabel);
        ui->neededLabel->setPalette(p);
    }
    QFont font = ui->neededLabel->font();
    font.setBold(bold);
    ui->neededLabel->setFont(font);
}

void MainWindow::on_list_itemChanged(QListWidgetItem *)
{
    updateNeeded();
}

void MainWindow::downloadMetaFile(const QString &urlstring, const QString &saveAs)
{
    qDebug() << "Downloading" << urlstring << "to" << saveAs;
    _numMetaFilesToDownload++;
    QUrl url(urlstring);
    QNetworkRequest request(url);
    request.setAttribute(QNetworkRequest::User, saveAs);
    QNetworkReply *reply = _netaccess->get(request);
    connect(reply, SIGNAL(finished()), this, SLOT(downloadMetaRedirectCheck()));
}

void MainWindow::downloadListRedirectCheck()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    int httpstatuscode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QString redirectionurl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toString();

    if (httpstatuscode > 300 && httpstatuscode < 400)
    {
        qDebug() << "Redirection - Re-trying download from" << redirectionurl;
        _numMetaFilesToDownload--;
        downloadList(redirectionurl);
    }
    else
        downloadListComplete();
}

void MainWindow::downloadIconRedirectCheck()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    int httpstatuscode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QString redirectionurl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toString();
    QString originalurl = reply->request().attribute(QNetworkRequest::User).toString();;

    if (httpstatuscode > 300 && httpstatuscode < 400)
    {
        qDebug() << "Redirection - Re-trying download from" << redirectionurl;
        _numMetaFilesToDownload--;
        downloadIcon(redirectionurl, originalurl);
    }
    else
        downloadIconComplete();
}

void MainWindow::downloadMetaRedirectCheck()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    int httpstatuscode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QString redirectionurl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toString();
    QString saveAs = reply->request().attribute(QNetworkRequest::User).toString();

    if (httpstatuscode > 300 && httpstatuscode < 400)
    {
        qDebug() << "Redirection - Re-trying download from" << redirectionurl;
        _numMetaFilesToDownload--;
        downloadMetaFile(redirectionurl, saveAs);
    }
    else
        downloadMetaComplete();
}

void MainWindow::downloadMetaComplete()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    int httpstatuscode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    if (reply->error() != reply->NoError || httpstatuscode < 200 || httpstatuscode > 399)
    {
        if (_qpd)
        {
            _qpd->hide();
            _qpd->deleteLater();
            _qpd = NULL;
        }
        QMessageBox::critical(this, tr("Download error"), tr("Error downloading meta file")+"\n"+reply->url().toString(), QMessageBox::Close);
        setEnabled(true);
    }
    else
    {
        QString saveAs = reply->request().attribute(QNetworkRequest::User).toString();
        QFile f(saveAs);
        f.open(f.WriteOnly);
        if (f.write(reply->readAll()) == -1)
        {
            QMessageBox::critical(this, tr("Download error"), tr("Error writing downloaded file to SD card. SD card or file system may be damaged."), QMessageBox::Close);
            setEnabled(true);
        }
        else
        {
            _numMetaFilesToDownload--;
        }
        f.close();
    }

    if (_numMetaFilesToDownload == 0)
    {
        if (_qpd)
        {
            _qpd->hide();
            _qpd->deleteLater();
            _qpd = NULL;
        }
        startImageWrite();
    }
}

void MainWindow::startImageWrite()
{
    /* All meta files downloaded, extract slides tarball, and launch image writer thread */
    MultiImageWriteThread *imageWriteThread = new MultiImageWriteThread();
    QString folder, slidesFolder;
    QStringList slidesFolders;

    QList<QListWidgetItem *> selected = selectedItems();
    foreach (QListWidgetItem *item, selected)
    {
        QVariantMap entry = item->data(Qt::UserRole).toMap();

        if (entry.contains("folder"))
        {
            /* Local image */
            folder = entry.value("folder").toString();
        }
        else
        {
            folder = "/settings/os/"+entry.value("name").toString();
            folder.replace(' ', '_');

            QString marketingTar = folder+"/marketing.tar";
            if (QFile::exists(marketingTar))
            {
                /* Extract tarball with slides */
                QProcess::execute("tar xf "+marketingTar+" -C "+folder);
                QFile::remove(marketingTar);
            }

            /* Insert tarball download URL information into partition_info.json */
            QVariantMap json = Json::loadFromFile(folder+"/partitions.json").toMap();
            QVariantList partitions = json["partitions"].toList();
            int i=0;
            QStringList tarballs = entry.value("tarballs").toStringList();
            foreach (QString tarball, tarballs)
            {
                QVariantMap partition = partitions[i].toMap();
                partition.insert("tarball", tarball);
                partitions[i] = partition;
                i++;
            }
            json["partitions"] = partitions;
            Json::saveToFile(folder+"/partitions.json", json);
        }

        slidesFolder.clear();
        //QRect s = QApplication::desktop()->screenGeometry();
        //if (s.width() > 640 && QFile::exists(folder+"/slides"))
        //{
        //    slidesFolder = folder+"/slides";
        //}
        if (QFile::exists(folder+"/slides_vga"))
        {
            slidesFolder = folder+"/slides_vga";
        }
        imageWriteThread->addImage(folder, entry.value("name").toString());
        if (!slidesFolder.isEmpty())
            slidesFolders.append(slidesFolder);
    }

    if (slidesFolders.isEmpty())
        slidesFolder.append("/mnt/defaults/slides");

    _qpd = new ProgressSlideshowDialog(slidesFolders, "", 20, this);
    connect(imageWriteThread, SIGNAL(parsedImagesize(qint64)), _qpd, SLOT(setMaximum(qint64)));
    connect(imageWriteThread, SIGNAL(completed()), this, SLOT(onCompleted()));
    connect(imageWriteThread, SIGNAL(error(QString)), this, SLOT(onError(QString)));
    connect(imageWriteThread, SIGNAL(statusUpdate(QString)), _qpd, SLOT(setLabelText(QString)));
    connect(imageWriteThread, SIGNAL(runningMKFS()), _qpd, SLOT(pauseIOaccounting()), Qt::BlockingQueuedConnection);
    connect(imageWriteThread, SIGNAL(finishedMKFS()), _qpd , SLOT(resumeIOaccounting()), Qt::BlockingQueuedConnection);
    imageWriteThread->start();
    hide();
    _qpd->exec();
}

void MainWindow::hideDialogIfNoNetwork()
{
    if (_qpd)
    {
        QByteArray carrier = getFileContents("/sys/class/net/eth0/carrier").trimmed();
        if (carrier != "1")
        {
            /* No network cable inserted */
            _qpd->hide();
            _qpd->deleteLater();
            _qpd = NULL;

            if (ui->list->count() == 0)
            {
                /* No local images either */
                QMessageBox::critical(this,
                                      tr("No network access"),
                                      tr("Wired network access is required to use NOOBS without local images. Please insert a network cable into the network port."),
                                      QMessageBox::Close);
            }
        }
    }
}
