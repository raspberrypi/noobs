#ifndef TWOICONSDELEGATE_H
#define TWOICONSDELEGATE_H

/* Delegate for a QListWidget that can draw a second icon
 * at the right side of the item
 *
 * Initial author: Floris Bos
 * Maintained by Raspberry Pi
 *
 * See LICENSE.txt for license details
 *
 */

#include <QStyledItemDelegate>

#define SecondIconRole  (Qt::UserRole+10)

class TwoIconsDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit TwoIconsDelegate(QObject *parent = 0);
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
signals:

public slots:

};

#endif // TWOICONSDELEGATE_H
