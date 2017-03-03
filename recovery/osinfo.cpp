#include "osinfo.h"
#include "partitioninfo.h"
#include "json.h"

OsInfo::OsInfo(const QString &folder, const QString &flavour, QObject *parent) :
    QObject(parent), _folder(folder), _flavour(flavour)
{
    QVariantMap m = Json::loadFromFile(folder+"/os.json").toMap();
    _name = m.value("name").toString();
    _version = m.value("version").toString();
    _description = m.value("description").toString();
    _releaseDate = m.value("release_date").toString();
    _bootable = m.value("bootable", true).toBool();
    _riscosOffset = m.value("riscos_offset").toInt();
    _managevideo = m.value("managevideo", false).toBool();

    QVariantList parts = Json::loadFromFile(folder+"/partitions.json").toMap().value("partitions").toList();
    foreach (QVariant pv, parts)
    {
        _partitions.append(new PartitionInfo(pv.toMap(), this));
    }
}
