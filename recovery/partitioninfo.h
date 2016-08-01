#ifndef PARTITIONINFO_H
#define PARTITIONINFO_H

/*
 * Partition information model
 * Contains information about a single partition from partitions.json
 * and runtime information like the partition device (/dev/mmcblk0pX) it was assigned
 */

#include <QObject>
#include <QVariantMap>

class PartitionInfo : public QObject
{
    Q_OBJECT
public:
    /* Constructor. Gets called from OsInfo with info from json file */
    explicit PartitionInfo(const QVariantMap &m, QObject *parent = 0);

    explicit PartitionInfo(int partitionNr, int offset, int sectors, const QByteArray &partType, QObject *parent = 0);

    inline void setPartitionDevice(const QByteArray &partdevice)
    {
        _partitionDevice = partdevice;
    }

    inline QByteArray partitionDevice()
    {
        return _partitionDevice;
    }

    inline void setRequiresPartitionNumber(int nr)
    {
        _requiresPartitionNumber = nr;
    }

    inline int requiresPartitionNumber()
    {
        return _requiresPartitionNumber;
    }

    inline QByteArray fsType()
    {
        return _fstype;
    }

    inline QByteArray mkfsOptions()
    {
        return _mkfsOptions;
    }

    inline QByteArray label()
    {
        return _label;
    }

    inline QString tarball()
    {
        return _tarball;
    }

    inline int partitionSizeNominal()
    {
        return _partitionSizeNominal;
    }

    inline bool emptyFS()
    {
        return _emptyFS;
    }

    inline bool wantMaximised()
    {
        return _wantMaximised;
    }

    inline int uncompressedTarballSize()
    {
        return _uncompressedTarballSize;
    }

    inline void setOffset(int offset)
    {
        _offset = offset;
    }

    inline int offset()
    {
        return _offset;
    }

    inline void setPartitionSizeSectors(int size)
    {
        _partitionSizeSectors = size;
    }

    inline int partitionSizeSectors()
    {
        return _partitionSizeSectors;
    }

    inline int endSector()
    {
        return _offset + _partitionSizeSectors;
    }

    inline bool active()
    {
        return _active;
    }

    inline QByteArray partitionType()
    {
        return _partitionType;
    }

    inline bool bootable()
    {
        return (_fstype == "fat" || _fstype == "FAT");
    }

protected:
    QByteArray _fstype, _mkfsOptions, _label, _partitionDevice, _partitionType;
    QString _tarball;
    int _partitionSizeNominal, _requiresPartitionNumber, _offset, _uncompressedTarballSize, _partitionSizeSectors;
    bool _emptyFS, _wantMaximised, _active;
};

#endif // PARTITIONINFO_H
