#include "wifiselection.h"
#include "ui_wifiselection.h"
#include <QDebug>

WifiSelection::WifiSelection(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WifiSelection)
{
    ui->setupUi(this);
    ui->passPhraseLabel->hide();
    ui->passPhraseLineEdit->hide();
    connect(ui->wifiListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(itemClicked(QListWidgetItem *)));
}

void WifiSelection::setAccPts(QStringList &ac)
{
    ui->wifiListWidget->addItems(ac);
}

void WifiSelection::itemClicked(QListWidgetItem *itemClicked)
{
    qDebug() << "Clicked: an item has been clicked.";
    ui->passPhraseLabel->show();
    ui->passPhraseLineEdit->show();
}

WifiSelection::~WifiSelection()
{
    delete ui;
}
