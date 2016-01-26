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
    explicit MultiImageWriteThread(QObject *parent = 0);
    void addImage(const QString &folder, const QString &flavour);

protected:
    virtual void run();
    bool processImage(OsInfo *image);
    bool mkfs(const QByteArray &device, const QByteArray &fstype = "ext4", const QByteArray &label = "", const QByteArray &mkfsopt = "");
    bool dd(const QString &imagePath, const QString &device);
    bool partclone_restore(const QString &imagePath, const QString &device);
    bool untar(const QString &tarball);
    bool isLabelAvailable(const QByteArray &label);
    QByteArray getLabel(const QString part);
    QByteArray getUUID(const QString part);
    void patchConfigTxt();
    QString getDescription(const QString &folder, const QString &flavour);
    bool writePartitionTable(const QMap<int, PartitionInfo *> &partitionMap);
    bool isURL(const QString &s);

    /* key: folder, value: flavour */
    QList<OsInfo *> _images;

    int _extraSpacePerPartition, _sectorOffset, _part;
    QVariantList installed_os;
    
signals:
    void error(const QString &msg);
    void statusUpdate(const QString &msg);
    void parsedImagesize(qint64 size);
    void completed();
    void runningMKFS();
    void finishedMKFS();
    
public slots:
    
};

#endif // MULTIIMAGEWRITETHREAD_H
