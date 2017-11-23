#include "progressslideshowdialog.h"
#include "ui_progressslideshowdialog.h"
#include "util.h"
#include <QDir>
#include <QFile>
#include <QPixmap>
#include <QDesktopWidget>
#include <QDebug>

/* Progress dialog with slideshow
 *
 * Initial author: Floris Bos
 * Maintained by Raspberry Pi
 *
 * See LICENSE.txt for license details
 *
 */

ProgressSlideshowDialog::ProgressSlideshowDialog(const QStringList &slidesDirectories, const QString &statusMsg, int changeInterval, const QString &drive, QWidget *parent) :
    QDialog(parent),
    _drive(drive),
    _pos(0),
    _changeInterval(changeInterval),
    _maxSectors(0),
    _pausedAt(0),
    ui(new Ui::ProgressSlideshowDialog)
{
    ui->setupUi(this);
    setLabelText(statusMsg);

    QRect s = QApplication::desktop()->screenGeometry();
    if (s.height() < 400)
        setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    else
        setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowTitleHint);

    foreach (QString slidesDirectory, slidesDirectories)
    {
        QDir dir(slidesDirectory, "*.jpg *.jpeg *.png");
        if (dir.exists())
        {
            QStringList s = dir.entryList();
            s.sort();

            foreach (QString slide, s)
            {
                _slides.append(slidesDirectory+"/"+slide);
            }
        }
    }
    qDebug() << "Available slides" << _slides;

    if (_slides.isEmpty())
    {
        /* Resize window to just show progress bar */
        ui->imagespace->setMinimumSize(0, 0);
        resize(this->width(), 50);

    }
    else
    {
        /* Resize window to size of first image in slide directory */
        QPixmap pixmap(_slides.first());
        ui->imagespace->setMinimumSize(pixmap.width(), pixmap.height());
        resize(pixmap.width(), pixmap.height()+50);

        ui->imagespace->setPixmap(pixmap);

        connect(&_timer, SIGNAL(timeout()), this, SLOT(nextSlide()));
        _timer.start(changeInterval * 1000);
    }
    connect(&_iotimer, SIGNAL(timeout()), this, SLOT(updateIOstats()));
    enableIOaccounting();
}

ProgressSlideshowDialog::~ProgressSlideshowDialog()
{
    delete ui;
}

void ProgressSlideshowDialog::setLabelText(const QString &text)
{
    QString txt = text;
    txt.replace('\n',' ');
    ui->statusLabel->setText(txt);
    qDebug() << text;
}

void ProgressSlideshowDialog::nextSlide()
{
    if (++_pos >= _slides.size())
        _pos = 0;

    QString newSlide = _slides.at(_pos);
    if (QFile::exists(newSlide))
        ui->imagespace->setPixmap(QPixmap(newSlide));
}

/* IO accounting functionality for analyzing SD card write speed / showing progress */

void ProgressSlideshowDialog::enableIOaccounting()
{
    _sectorsStart = sectorsWritten();
    _t1.start();
    _iotimer.start(1000);
}

void ProgressSlideshowDialog::disableIOaccounting()
{
    _iotimer.stop();
    ui->mbwrittenLabel->setText("");
}

void ProgressSlideshowDialog::pauseIOaccounting()
{
    _iotimer.stop();
    _pausedAt = sectorsWritten();
}

void ProgressSlideshowDialog::resumeIOaccounting()
{
    _sectorsStart += sectorsWritten()-_pausedAt;
    _iotimer.start(1000);
}

void ProgressSlideshowDialog::changeDrive(const QString &drive)
{
    pauseIOaccounting();
    _drive = drive;
    resumeIOaccounting();
}

void ProgressSlideshowDialog::setMaximum(qint64 bytes)
{
    _maxSectors = bytes/512;
    ui->progressBar->setMaximum(_maxSectors);
}

void ProgressSlideshowDialog::updateIOstats()
{
    int sectors = sectorsWritten()-_sectorsStart;
    double sectorsPerSec = sectors * 1000.0 / _t1.elapsed();
    if (_maxSectors)
    {
        sectors = qMin(_maxSectors, sectors);
        ui->progressBar->setValue(sectors);
        ui->mbwrittenLabel->setText(tr("%1 MB of %2 MB written (%3 MB/sec)")
                                    .arg(QString::number(sectors/2048), QString::number(_maxSectors/2048), QString::number(sectorsPerSec/2048.0, 'f', 1)));
    }
    else
    {
        ui->mbwrittenLabel->setText(tr("%1 MB written (%2 MB/sec)")
                                    .arg(QString::number(sectors/2048), QString::number(sectorsPerSec/2048.0, 'f', 1)));
    }
}

int ProgressSlideshowDialog::sectorsWritten()
{
    /* Poll kernel counters to get number of bytes written
     *
     * Fields available in /sys/block/<DEVICE>/stat
     * (taken from https://www.kernel.org/doc/Documentation/block/stat.txt )
     *
     * Name            units         description
     * ----            -----         -----------
     * read I/Os       requests      number of read I/Os processed
     * read merges     requests      number of read I/Os merged with in-queue I/O
     * read sectors    sectors       number of sectors read
     * read ticks      milliseconds  total wait time for read requests
     * write I/Os      requests      number of write I/Os processed
     * write merges    requests      number of write I/Os merged with in-queue I/O
     * write sectors   sectors       number of sectors written
     * write ticks     milliseconds  total wait time for write requests
     * in_flight       requests      number of I/Os currently in flight
     * io_ticks        milliseconds  total time this block device has been active
     * time_in_queue   milliseconds  total wait time for all requests
     */

    QFile f(sysclassblock(_drive)+"/stat");
    f.open(f.ReadOnly);
    QByteArray ioline = f.readAll().simplified();
    f.close();

    QList<QByteArray> stats = ioline.split(' ');

    if (stats.count() >= 6)
        return stats.at(6).toInt(); /* write sectors */
    else
        return 0;
}
