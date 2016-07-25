#ifndef UTIL_H
#define UTIL_H

#include <QString>
#include <QByteArray>
#include <QVariant>

/*
 * Convenience functions
 *
 * Initial author: Floris Bos
 * Maintained by Raspberry Pi
 *
 * See LICENSE.txt for license details
 */

QByteArray getFileContents(const QString &filename);
void putFileContents(const QString &filename, const QByteArray &data);
void getOverscan(int &top, int &bottom, int &left, int &right);
bool nameMatchesRiscOS(const QString &name);
uint readBoardRevision();
bool canBootOs(const QString& name, const QVariantMap& values);
bool setRebootPartition(QByteArray partition);
QByteArray partdev(const QString &drivedev, int nr);
QByteArray sysclassblock(const QString &drivedev, int partnr = -1);
#endif // UTIL_H
