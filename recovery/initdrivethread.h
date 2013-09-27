#ifndef INITDRIVETHREAD_H
#define INITDRIVETHREAD_H

/* Prepare SD card for first use thread
 *
 * Initial author: Floris Bos
 * Maintained by Raspberry Pi
 *
 * See LICENSE.txt for license details
 *
 */

#include <QThread>
#include <QMessageBox>
#include "config.h"

class InitDriveThread : public QThread
{
    Q_OBJECT
public:
    explicit InitDriveThread(QObject *parent = 0);

protected:
    virtual void run();

    bool method_reformatDrive();
    bool method_resizePartitions();
    bool saveBootFiles();
    bool restoreBootFiles();
    int sizeofBootFilesInKB();
    int sizeofSDCardInBlocks();
    bool mountSystemPartition();
    bool umountSystemPartition();
    bool zeroMbr();
    bool formatBootPartition();
    bool partitionDrive();
    bool formatSettingsPartition();
#ifdef RISCOS_BLOB_FILENAME
    bool writeRiscOSblob();
#endif

signals:
    void error(const QString &msg);
    void statusUpdate(const QString &msg);
    void completed();
    void query(const QString &msg, const QString &title, QMessageBox::StandardButton* answer);

public slots:

};

#endif // INITDRIVETHREAD_H
