#include "json.h"
#include <qjson/parser.h>
#include <qjson/serializer.h>
#include <QDebug>
#include <QFile>

/* Json helper class
 *
 * Outsources the hard work to QJson
 *
 * Initial author: Floris Bos
 * Maintained by Raspberry Pi
 *
 * See LICENSE.txt for license details
 *
 */

QVariant Json::parse(const QByteArray &json)
{
    QJson::Parser parser;
    bool ok;
    QVariant result = parser.parse (json, &ok);

    if (!ok)
    {
        qDebug() << "Error parsing json";
        qDebug() << "Json input:" << json;
    }

    return result;
}

QVariant Json::loadFromFile(const QString &filename)
{
    QFile f(filename);
    QJson::Parser parser;
    bool ok;

    if (!f.open(f.ReadOnly))
    {
        qDebug() << "Error opening file:" << filename;
        return QVariant();
    }


    QVariant result = parser.parse (&f, &ok);
    f.close();

    if (!ok)
    {
        qDebug() << "Error parsing json file:" << filename;
    }

    return result;
}

QByteArray Json::serialize(const QVariant &json)
{
    QJson::Serializer serializer;
    bool ok;

    serializer.setIndentMode(QJson::IndentFull);
    QByteArray result = serializer.serialize(json, &ok);

    if (!ok)
    {
        qDebug() << "Error serializing json";
    }

    return result;
}

void Json::saveToFile(const QString &filename, const QVariant &json)
{
    QFile f(filename);
    QJson::Serializer serializer;
    bool ok;

    if (!f.open(f.WriteOnly))
    {
        qDebug() << "Error opening file for writing: " << filename;
        return;
    }
    serializer.setIndentMode(QJson::IndentFull);
    serializer.serialize(json, &f, &ok);
    f.close();

    if (!ok)
    {
        qDebug() << "Error serializing json to file:" << filename;
    }
}
