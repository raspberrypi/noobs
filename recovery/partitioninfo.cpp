#include "partitioninfo.h"

PartitionInfo::PartitionInfo(const QVariantMap &m, QObject *parent) :
    QObject(parent)
{
    _fstype        = m.value("filesystem_type").toByteArray().toLower();
    _mkfsOptions   = m.value("mkfs_options").toByteArray();
    _label         = m.value("label").toByteArray();
    _tarball       = m.value("tarball").toString();
    _wantMaximised = m.value("want_maximised", false).toBool();
    _emptyFS       = m.value("empty_fs", false).toBool();
    _offset        = m.value("offset_in_sectors").toInt();
    _partitionSizeNominal = m.value("partition_size_nominal").toInt();
    _requiresPartitionNumber = m.value("requires_partition_number").toInt();
    _uncompressedTarballSize = m.value("uncompressed_tarball_size").toInt();
    _active        = m.value("active", false).toBool();

    QByteArray defaultPartType;
    if (_fstype.contains("fat"))
        defaultPartType = "0c"; /* FAT32 LBA */
    else if (_fstype == "swap")
        defaultPartType = "82";
    else if (_fstype.contains("ntfs"))
        defaultPartType = "07";
    else
        defaultPartType = "83"; /* Linux native */

    _partitionType = m.value("partition_type", defaultPartType).toByteArray();
}

PartitionInfo::PartitionInfo(int partitionNr, int offset, int sectors, const QByteArray &partType, QObject *parent) :
    QObject(parent), _partitionType(partType), _requiresPartitionNumber(partitionNr), _offset(offset), _partitionSizeSectors(sectors), _active(false)
{
}
