/* Configuration edit dialog
 *
 * Initial author: Floris Bos
 * Maintained by Raspberry Pi
 *
 * See LICENSE.txt for license details
 *
 */

#include "confeditdialog.h"
#include "ui_confeditdialog.h"
#include "config.h"
#include <QDir>
#include <QFile>
#include <QtGui/QPlainTextEdit>
#include <QProcess>
#include <QMessageBox>
#include <unistd.h>

/* Private class - For each configuration file there is a tab class */
class ConfEditDialogTab : public QWidget
{
public:
    ConfEditDialogTab(const QString &title, const QString &filename, bool ro, QTabWidget *parent)
        : QWidget(parent), _file(filename), _ro(ro)
    {
        if (_file.open(_file.ReadOnly))
        {
            _origData = _file.readAll();
            _file.close();
        }

        _textEdit = new QPlainTextEdit();
        _textEdit->setPlainText(_origData);
        if (ro)
            _textEdit->setReadOnly(true);

        QVBoxLayout *layout = new QVBoxLayout();
        layout->addWidget(_textEdit);
        setLayout(layout);
        parent->addTab(this, title);
    }

    void save()
    {
        QByteArray txt = _textEdit->toPlainText().toAscii();

        if (!_ro && txt != _origData)
        {
            if (_file.open(_file.WriteOnly))
            {
                _file.write(txt);
                _file.close();
            }
        }
    }

protected:
    QFile _file;
    QByteArray _origData;
    bool _ro;
    QPlainTextEdit *_textEdit;
};
/* --- */


ConfEditDialog::ConfEditDialog(const QString &partition, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfEditDialog)
{
    QDir dir;
    if (!dir.exists("/boot"))
        dir.mkdir("/boot");

    setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    ui->setupUi(this);
    ui->tabWidget->clear();

    if (QProcess::execute("mount -t vfat "+partition+" /boot") != 0)
    {
        QMessageBox::critical(this,
                              tr("Error"),
                              tr("Error mounting boot partition"),
                              QMessageBox::Close);
        return;
    }

    _tabs.append(new ConfEditDialogTab("config.txt", "/boot/config.txt", false, ui->tabWidget));
    _tabs.append(new ConfEditDialogTab("cmdline.txt", "/boot/cmdline.txt", false, ui->tabWidget));
}

ConfEditDialog::~ConfEditDialog()
{
    delete ui;
    QProcess::execute("umount /boot");
}

void ConfEditDialog::accept()
{
    foreach (ConfEditDialogTab *tab, _tabs)
    {
        tab->save();
    }
    sync();
    QDialog::accept();
}
