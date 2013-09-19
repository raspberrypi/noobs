/* Delegate for a QListWidget that can draw a second icon
 * at the right side of the item
 *
 * Initial author: Floris Bos
 * Maintained by Raspberry Pi
 *
 * See LICENSE.txt for license details
 *
 */

#include "twoiconsdelegate.h"
#include <QPainter>
#include <QStyleOptionViewItem>
#include <QModelIndex>

TwoIconsDelegate::TwoIconsDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

void TwoIconsDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyledItemDelegate::paint(painter, option, index);

    QVariant v = index.data(SecondIconRole);
    if (!v.isNull() && v.canConvert<QIcon>())
    {
        QIcon icon = v.value<QIcon>();
        QSize size = icon.availableSizes().first();

        painter->drawPixmap(option.rect.right()-size.width(), option.rect.top(), icon.pixmap(size));
    }
}
