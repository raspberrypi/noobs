#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "imagewritethread.h"
#include "initdrivethread.h"
#include "confeditdialog.h"
#include "progressslideshowdialog.h"
#include "config.h"
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

/* Which ListItem (if any) points to the recommended image. */
QListWidgetItem *recommendedItem = NULL;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _qpd(NULL), _kcpos(0), _silent(false), _allowSilent(true)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    setContextMenuPolicy(Qt::NoContextMenu);
    setWindowTitle(QString(tr("Pi Recovery - Built:%1")).arg(QString::fromLocal8Bit(__DATE__)));
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
            recommendedItem->setText(recommendedItem->text()+" "+tr("[RECOMMENDED]"));
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
        if (basefile(imagefile).contains("risc", Qt::CaseInsensitive))
            slidesDirectory = "/mnt/slides/RISCOS";
        else if (basefile(imagefile).contains("arch", Qt::CaseInsensitive))
            slidesDirectory = "/mnt/slides/Archlinux";
        else if (basefile(imagefile).contains("pidora", Qt::CaseInsensitive))
            slidesDirectory = "/mnt/slides/Pidora";
        else if ((basefile(imagefile).contains("wheezy", Qt::CaseInsensitive)) || (basefile(imagefile).contains("raspbian", Qt::CaseInsensitive))) 
            slidesDirectory = "/mnt/slides/Raspbian";
        else if (basefile(imagefile).contains("OpenELEC", Qt::CaseInsensitive))
            slidesDirectory = "/mnt/slides/OpenELEC";
        else if (basefile(imagefile).contains("raspbmc", Qt::CaseInsensitive))
            slidesDirectory = "/mnt/slides/RaspBMC";
        else
            slidesDirectory = "/mnt/slides/default";

        ImageWriteThread *t = new ImageWriteThread(imagefile, this);

        ProgressSlideshowDialog *p = new ProgressSlideshowDialog(slidesDirectory, "", 20, this);
        connect(t, SIGNAL(parsedImagesize(uint)), p, SLOT(setMaximum(uint)));
        
        if (QFile::exists(sizefile))
            {
                QFile f(sizefile);
                f.open(f.ReadOnly);
                p->setMaximum(f.readAll().trimmed().toUInt());
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

void MainWindow::changeEvent(QEvent* event)
{
    if (event && event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
        if (recommendedItem)
        {
            recommendedItem->setText(imageFilenameToFriendlyName(recommendedItem->data(Qt::UserRole).toString())+" "+tr("[RECOMMENDED]"));
        }
    }

    QMainWindow::changeEvent(event);
}

bool MainWindow::eventFilter(QObject *, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);

        if (_kc.at(_kcpos) == keyEvent->key())
        {
            _kcpos++;
            if (_kcpos == _kc.size())
            {
                inputSequence();
                _kcpos = 0;
            }
        }
        else
        {
            _kcpos = 0;
        }
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
