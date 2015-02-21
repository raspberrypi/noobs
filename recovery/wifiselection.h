#ifndef WIFISELECTION_H
#define WIFISELECTION_H

#include <QDialog>
#include <QStringList>
#include <QListWidgetItem>

namespace Ui {
class WifiSelection;
}

class WifiSelection : public QDialog
{
    Q_OBJECT

public:
    explicit WifiSelection(QWidget *parent = 0);
    void setAccPts(QStringList &ac);
    ~WifiSelection();

public slots:
    void itemClicked(QListWidgetItem *itemClicked);

private:
    Ui::WifiSelection *ui;
};

#endif // WIFISELECTION_H
