#include "wifiselection.h"
#include "ui_wifiselection.h"
#include <QDebug>
#include <QPushButton>

/* Dialog to allow the selection of a wifi network
 *
 * Takes a number of stringlists that contain the interface
 * SSID list and encryption type
 *
 * Provides a number of helper methods to get the selected
 * information back along with the pass code if required
 */

WifiSelection::WifiSelection(QWidget *parent) :
    QDialog(parent),
    _apSSID(NULL),
    _apEnc(NULL),
    _apInt(NULL),
    _apInd(-1),
    ui(new Ui::WifiSelection)
{
    ui->setupUi(this);
    ui->passPhraseLabel->hide();
    ui->passPhraseLineEdit->hide();
    connect(ui->wifiListWidget, SIGNAL(itemSelectionChanged()), this, SLOT(listSelectionChanged()));
}

void WifiSelection::setAccPts(QStringList &ac, QStringList &enc, QStringList &inter)
{
    _apSSID = ac;
    _apEnc = enc;
    _apInt = inter;
    ui->wifiListWidget->addItems(ac);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}

void WifiSelection::listSelectionChanged()
{
    QList<QListWidgetItem *> s;
    QListWidgetItem *it;
    int i;

    s = ui->wifiListWidget->selectedItems();

    if (s.size() > 0)
    {
        it = s.at(0);
        i = _apSSID.indexOf(it->text());

        /* prompt for a passcode if required */

        if ((i != -1) && (_apEnc.at(i) != "NONE")) {
            ui->passPhraseLabel->show();
            ui->passPhraseLineEdit->show();
            ui->passPhraseLineEdit->setFocus();
        }
        else
        {
            ui->passPhraseLabel->hide();
            ui->passPhraseLineEdit->hide();
        }

        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    }
}

void WifiSelection::accept()
{
    QList<QListWidgetItem *> s;
    QListWidgetItem *it;
    int i;
    s = ui->wifiListWidget->selectedItems();
    if (s.size() > 0)
    {
        it = s.at(0);
        i = _apSSID.indexOf(it->text());
        if (i != -1)
        {
            qDebug() << "OK pressed .. AP:" << _apSSID.at(i);
            _apInd = i;
        }
    }
    QDialog::accept();
}

/* Get selected access point */

QString WifiSelection::getAP()
{
    QString ret = "";
    if ((_apInd >= 0) && (_apInd < _apSSID.size()))
    {
        ret = _apSSID.at(_apInd);
    }
    return ret;
}

/* Get AP encryption */

QString WifiSelection::getEnc()
{
    QString ret = "";
    if ((_apInd >= 0) && (_apInd < _apEnc.size()))
    {
        ret = _apEnc.at(_apInd);
    }
    return ret;
}

/* Get AP pass phrase */

QString WifiSelection::getPass()
{
    QString ret;
    ret = ui->passPhraseLineEdit->text();
    return ret;
}

/* Get the interface associated with the wifi access point */

QString WifiSelection::getInt()
{
    QString ret = "";
    if ((_apInd >= 0) && (_apInd < _apInt.size()))
    {
        ret = _apInt.at(_apInd);
    }
    return ret;
}


WifiSelection::~WifiSelection()
{
    delete ui;
}
