#ifndef RIGHTBUTTONFILTER_H
#define RIGHTBUTTONFILTER_H

/* Eats right button clicks
 *
 * Initial author: Floris Bos
 * Maintained by Raspberry Pi
 *
 * See LICENSE.txt for license details
 *
 */

#include <QObject>

class RightButtonFilter : public QObject
{
    Q_OBJECT
public:
    explicit RightButtonFilter(QObject *parent = 0);
    virtual bool eventFilter(QObject *, QEvent *event);

signals:

public slots:

};

#endif // RIGHTBUTTONFILTER_H
