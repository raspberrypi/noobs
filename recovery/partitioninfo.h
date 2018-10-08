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

    explicit PartitionInfo(int partitionNr, uint offset, uint sectors, const QByteArray &partType, QObject *parent = 0);

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

    inline uint partitionSizeNominal()
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

    inline uint uncompressedTarballSize()
    {
        return _uncompressedTarballSize;
    }

    inline void setOffset(uint offset)
    {
        _offset = offset;
    }

    inline uint offset()
    {
        return _offset;
    }

    inline void setPartitionSizeSectors(uint size)
    {
        _partitionSizeSectors = size;
    }

    inline uint partitionSizeSectors()
    {
        return _partitionSizeSectors;
    }

    inline uint endSector()
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
    int _requiresPartitionNumber;
    uint _partitionSizeNominal, _offset, _uncompressedTarballSize, _partitionSizeSectors;
    bool _emptyFS, _wantMaximised, _active;
};

#endif // PARTITIONINFO_H
