#ifndef CONFEDITDIALOG_H
#define CONFEDITDIALOG_H

/* Configuration edit dialog
 *
 * Initial author: Floris Bos
 * Maintained by Raspberry Pi
 *
 * See LICENSE.txt for license details
 *
 */

#include <QDialog>
#include <QList>

namespace Ui {
class ConfEditDialog;
}

class ConfEditDialogTab;

class ConfEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfEditDialog(const QString &partition, QWidget *parent = 0);
    ~ConfEditDialog();

public slots:
    virtual void accept();

private:
    Ui::ConfEditDialog *ui;
    QList<ConfEditDialogTab *> _tabs;
};

#endif // CONFEDITDIALOG_H
