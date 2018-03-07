#ifndef MULTIIMAGEWRITETHREAD_H
#define MULTIIMAGEWRITETHREAD_H

#include <QThread>
#include <QStringList>
#include <QMultiMap>
#include <QVariantList>
#include <QList>

class OsInfo;
class PartitionInfo;

class MultiImageWriteThread : public QThread
{
    Q_OBJECT
public:
    explicit MultiImageWriteThread(const QString &bootdrive, const QString &rootdrive, QObject *parent = 0);
    void addImage(const QString &folder, const QString &flavour);

protected:
    virtual void run();
    bool processImage(OsInfo *image);
    QString shorten(QString example, int maxLabelLen);
    QByteArray makeLabelUnique(QByteArray label, int maxLabelLen);
    bool mkfs(const QByteArray &device, const QByteArray &fstype = "ext4", const QByteArray &label = "", const QByteArray &mkfsopt = "");
    bool dd(const QString &imagePath, const QString &device);
    bool partclone_restore(const QString &imagePath, const QString &device);
    bool untar(const QString &tarball);
    bool isLabelAvailable(const QByteArray &label);
    QByteArray getLabel(const QString part);
    QByteArray getUUID(const QString part);
    void patchConfigTxt();
    QString getDescription(const QString &folder, const QString &flavour);
    bool writePartitionTable(const QString &drive, const QMap<int, PartitionInfo *> &partitionMap);
    bool isURL(const QString &s);
    QByteArray getDiskId(const QString &device);
    QByteArray getPartUUID(const QString &devpart);


    /* key: folder, value: flavour */
    QList<OsInfo *> _images;

    QString _drive, _bootdrive;
    int _extraSpacePerPartition, _sectorOffset, _part;
    QVariantList installed_os;
    bool _multiDrives;
    
signals:
    void error(const QString &msg);
    void statusUpdate(const QString &msg);
    void parsedImagesize(qint64 size);
    void completed();
    void runningMKFS();
    void finishedMKFS();
    void newDrive(const QString&);

public slots:
    
};

#endif // MULTIIMAGEWRITETHREAD_H
