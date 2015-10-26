#ifndef WIFISETTINGSDIALOG_H
#define WIFISETTINGSDIALOG_H

/*
 * Wifi network selection dialog
 *
 * Initial author: Floris Bos
 * Maintained by Raspberry Pi
 *
 * See LICENSE.txt for license details
 */

#include <QDialog>
#include <QDBusObjectPath>
#include <QVariantMap>

namespace Ui {
class WifiSettingsDialog;
}

class FiW1Wpa_supplicant1InterfaceInterface;
class AccessPoint;
class QListWidgetItem;
class QProgressDialog;

class WifiSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WifiSettingsDialog(const QString &preferredInterface = "wlan0", QWidget *parent = 0);
    ~WifiSettingsDialog();
    bool connectToWifi(const QString &ssid, const QString &username, const QString &password, bool useWPS = false);

protected:
    Ui::WifiSettingsDialog *ui;
    QString _ifname, _laststate;
    QDBusObjectPath _ifpath;
    FiW1Wpa_supplicant1InterfaceInterface *_interface;
    QDBusObjectPath _currentBSS;
    QProgressDialog *_qpd;
    bool _connecting;
    void msleep(int msec);
    QString removeQuotes(QString str);

protected slots:
    virtual void accept();
    void onBSSAdded(const QDBusObjectPath &path);
    void onPropertiesChanged(const QVariantMap &properties);
    void checkSettings();

private slots:
    void on_passwordRadio_toggled(bool checked);
    void on_list_currentItemChanged(QListWidgetItem *current);
};

#endif // WIFISETTINGSDIALOG_H
