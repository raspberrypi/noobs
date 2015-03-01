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
    void setAccPts(QStringList &ac, QStringList &enc, QStringList &inter);
    QString getAP();
    QString getEnc();
    QString getPass();
    QString getInt();
    ~WifiSelection();

protected:
    QStringList _apSSID;
    QStringList _apEnc;
    QStringList _apInt;
    int _apInd;
    void accept();


public slots:
    void listSelectionChanged();

private:
    Ui::WifiSelection *ui;
};

#endif // WIFISELECTION_H
