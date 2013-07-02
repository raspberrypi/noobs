#ifndef IMAGEWRITETHREAD_H
#define IMAGEWRITETHREAD_H

#include <QThread>

/* "Apply image" thread
 *
 * - Extracts image file to extended partition
 * - enlarges the ext4 partition to span the disk
 * - patches /etc/fstab and cmdline.txt
 *                                                                                                                                                                             
 * Initial author: Floris Bos                                                                                                                                                  
 * Maintained by Raspberry Pi                                                                                                                                                    
 *                                                                                                                                                                             
 * See LICENSE.txt for license details                                                                                                                                         
 *                                                                                                                                                                             
 */

class ImageWriteThread : public QThread
{
    Q_OBJECT
public:
    explicit ImageWriteThread(const QString &image, QObject *parent = 0);

    /* Wheter or not to parse the header of .zip and .lzo files to look for
     * the size of the uncompressed image. Useful for progress indication. Defaults to true */
    void setParseArchiveHeader(bool b);

protected:
    QString _image, _fatPartition, _extPartition;
    int _offsetInSectors, _lastPartition;
    quint64 _offsetInBytes;
    bool _parseArchiveHeader;

    virtual void run();
    void run_riscos();
    bool dd();
    bool resizePartition();
    bool patchFstab();
    bool patchCmdlineTXT();
    int startOfSecondPartition();
    void replaceMMCBLKreferences(const QString &filename);
    QByteArray getFileContents(const QString &filename);
    void putFileContents(const QString &filename, const QByteArray &data);
    qint64 getUncompressedSizeZIP(const QString &filename);
    qint64 getUncompressedSizeLZO(const QString &filename);
    qint64 getUncompressedSizeXZ(const QString &filename);
    
signals:
    void error(const QString &msg);
    void statusUpdate(const QString &msg);
    void parsedImagesize(qint64 size);
    void completed();

public slots:
    
};

#endif // IMAGEWRITETHREAD_H
