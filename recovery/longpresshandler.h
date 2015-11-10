#ifndef LONGPRESSHANDLER_H
#define LONGPRESSHANDLER_H

/* Convert long presses to double click events
 *
 * Double clicks are hard to do on touch devices as the system does
 * not considers a double tap a double click if the position is not the same.
 *
 * Initial author: Floris Bos
 * Maintained by Raspberry Pi
 *
 * See LICENSE.txt for license details
 *
 */

#include <QObject>
#include <QTime>
#include <QPoint>

class LongPressHandler : public QObject
{
    Q_OBJECT
public:
    explicit LongPressHandler(QObject *parent = 0);
    virtual bool eventFilter(QObject *obj, QEvent *event);

protected:
    QTime _holdTime;
    QPoint _holdPoint;
    int _holdInterval;
    int _maxPosDifference;
};

#endif // LONGPRESSHANDLER_H
