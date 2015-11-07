/*
 * Wifi network selection dialog
 *
 * Initial author: Floris Bos
 * Maintained by Raspberry Pi
 *
 * See LICENSE.txt for license details
 */

#include "wifisettingsdialog.h"
#include "ui_wifisettingsdialog.h"
#include "wpa_supplicant/wpafactory.h"
#include "twoiconsdelegate.h"
#include <QMessageBox>
#include <QPushButton>
#include <QProgressDialog>
#include <QNetworkInterface>
#include <QProcess>

WifiSettingsDialog::WifiSettingsDialog(const QString &preferredInterface, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WifiSettingsDialog), _qpd(NULL), _connecting(false)
{
    setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    ui->setupUi(this);
    ui->list->setItemDelegate(new TwoIconsDelegate(this));
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    connect(ui->passwordEdit, SIGNAL(textChanged(QString)), this, SLOT(checkSettings()));

    FiW1Wpa_supplicant1Interface *wpa = WpaFactory::createWpaSupplicantProxy(this);
    _ifpath = wpa->GetInterface(preferredInterface).value();

    if (_ifpath.path().isEmpty())
    {
        /* Preferred wlan interface not available, take first one out of list instead */
        QList<QDBusObjectPath> ifaces = wpa->interfaces();

        if (ifaces.count())
        {
            _ifpath = ifaces.first();
        }
        else
        {
            QMessageBox::critical(parent, tr("No wifi interfaces"), tr("No wifi interfaces available"), QMessageBox::Close);
            close();
        }
    }

    _interface = WpaFactory::createInterfaceProxy(_ifpath, this);
    _ifname = _interface->ifname();
    qDebug() << "Using wifi interface" << _ifname;

    _currentBSS = _interface->currentBSS();
    connect(_interface, SIGNAL(BSSAdded(QDBusObjectPath,QVariantMap)), this, SLOT(onBSSAdded(QDBusObjectPath)));
    connect(_interface, SIGNAL(PropertiesChanged(QVariantMap)), this, SLOT(onPropertiesChanged(QVariantMap)));

    /* Call onBSSAdded() for BSSes already scanned prior to opening the dialog */
    QList<QDBusObjectPath> bSSs = _interface->bSSs();
    foreach (QDBusObjectPath bsspath, bSSs)
    {
        onBSSAdded(bsspath);
    }
    if (ui->list->count() && ui->list->currentRow() == -1)
        ui->list->setCurrentRow(0);

    /* Copy existing login information if possible
       Password is only retrievable if network has just been added */
    QDBusObjectPath cnp = _interface->currentNetwork();
    if (!cnp.path().isEmpty())
    {
        FiW1Wpa_supplicant1NetworkInterface *cni = WpaFactory::createNetworkProxy(cnp);
        QVariantMap prop = cni->properties();
        if (prop.contains("identity") && prop.contains("password"))
        {
            ui->userEdit->setText(removeQuotes( prop.value("identity").toString() ));
            ui->passwordEdit->setText(removeQuotes( prop.value("password").toString() ));
        }
        else if (prop.contains("psk"))
        {
            ui->passwordEdit->setText(removeQuotes( prop.value("psk").toString() ));
        }
        cni->deleteLater();
    }

    QVariantMap args;
    args.insert("Type", "passive");
    _interface->Scan(args);
}

WifiSettingsDialog::~WifiSettingsDialog()
{
    delete ui;
}

void WifiSettingsDialog::onBSSAdded(const QDBusObjectPath &path)
{
    FiW1Wpa_supplicant1BSSInterface *bss = WpaFactory::createBSSProxy(path);

    QString ssid = bss->sSID();
    int   signal = bss->signal();
    QVariantMap properties;
    properties.insert("SSID", bss->sSID());
    properties.insert("WPA", bss->wPA());
    properties.insert("WPS", bss->wPS());
    properties.insert("RSN", bss->rSN());
    properties.insert("Frequency", bss->frequency());
    qDebug() << "New BSS detected:" << path.path() << ssid << "freq" << bss->frequency() << "WPS support:" << bss->wPS();

    if (ui->list->findItems(ssid, Qt::MatchExactly).count() )
    {
        bss->deleteLater();
        return;
    }

    QString icon;
    if (signal > -50)
        icon = ":/icons/network-wireless-connected-100.png";
    else if (signal > -60)
        icon = ":/icons/network-wireless-connected-75.png";
    else if (signal > -70)
        icon = ":/icons/network-wireless-connected-50.png";
    else if (signal > -80)
        icon = ":/icons/network-wireless-connected-25.png";
    else
        icon = ":/icons/network-wireless-connected-00.png";

    QListWidgetItem *item = new QListWidgetItem(QIcon(icon), ssid);
    item->setData(Qt::UserRole, properties);
    item->setData(Qt::UserRole+1, signal);
    if (path == _currentBSS)
        item->setData(SecondIconRole, QIcon(":/icons/tick.png"));

    /* Insert the discovered BSS before entries with lower strength */
    for (int i=0; i<ui->list->count(); i++)
    {
        if (ui->list->item(i)->data(Qt::UserRole+1).toInt() < signal)
        {
            ui->list->insertItem(i, item);
            if (path == _currentBSS)
                ui->list->setCurrentItem(item);
            return;
        }
    }

    /* No existing entries with lower signal, append to end of list. */
    ui->list->addItem(item);

    if (path == _currentBSS)
        ui->list->setCurrentItem(item);

    bss->deleteLater();
}

void WifiSettingsDialog::on_passwordRadio_toggled(bool checked)
{
    ui->passwordGroupBox->setEnabled(checked);
    checkSettings();
}

void WifiSettingsDialog::on_list_currentItemChanged(QListWidgetItem *current)
{
    /* Enable authentication options appropiate for the SSID selected */
    QVariantMap properties = current->data(Qt::UserRole).toMap();
    QStringList keymgmt = properties.value("WPA").toMap().value("KeyMgmt").toStringList();
    QStringList keymgmt_wpa2 = properties.value("RSN").toMap().value("KeyMgmt").toStringList();
    QVariantMap wps = properties.value("WPS").toMap();
    bool supportsEAP = keymgmt.contains("wpa-eap") || keymgmt_wpa2.contains("wpa-eap");
    bool supportsPSK = keymgmt.contains("wpa-psk") || keymgmt_wpa2.contains("wpa-psk");
    bool supportsWPS = !wps.empty();

    if (supportsPSK || supportsEAP)
    {
        ui->authGroupbox->setEnabled(true);
        ui->WpsRadio->setEnabled(supportsWPS);
        if (!supportsWPS)
            ui->passwordRadio->setChecked(true);
        else if (ui->passwordEdit->text().isEmpty()
                 && wps.value("Type").toString() == "pbc")
        {
            /* user pressed WPS button on router recently,
               assume he wants to authenticate through that */
            ui->WpsRadio->setChecked(true);
        }

        /* Entering an username is only needed for wpa-eap */
        ui->userLabel->setEnabled(supportsEAP);
        ui->userEdit->setEnabled(supportsEAP);

        if (ui->passwordRadio->isChecked())
        {
            if (supportsEAP)
                ui->userEdit->setFocus();
            else
                ui->passwordEdit->setFocus();
        }
    }
    else
    {
        /* Open wifi (or WEP?) */
        ui->authGroupbox->setEnabled(false);
    }

    checkSettings();
}

/* Enable OK button if settings look ok. E.g. password is long enough */
void WifiSettingsDialog::checkSettings()
{
    bool looksok = !ui->authGroupbox->isEnabled()
            || ui->WpsRadio->isChecked()
            || ui->passwordEdit->text().count() > 7;

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(looksok);
}

void WifiSettingsDialog::onPropertiesChanged(const QVariantMap &properties)
{
    if (properties.contains("State"))
    {
        QString state = properties.value("State").toString();
        qDebug() << "WPA state changed to:" << state;
        if (_connecting && _qpd)
        {
            if (state == "completed")
            {
                /* wpa completed, now waiting for the dhcp client to do its job */
                _qpd->setLabelText(tr("Authenticated but still waiting for DHCP lease"));
            }
            if (state == "disconnected" && _laststate == "4way_handshake")
            {
                /* if we get a "disconnected" state change signal during connecting, authentication has failed */
                _connecting = false;
                _qpd->cancel();
                QMessageBox::critical(this, tr("Connecting to wifi failed"), tr("Connecting to the wifi access point failed. Check your password"), QMessageBox::Close);
            }
        }
        _laststate = state;
    }
}

void WifiSettingsDialog::accept()
{
    QString username, password;
    QString ssid = ui->list->currentItem()->text();
    bool useWPS = ui->WpsRadio->isEnabled() && ui->WpsRadio->isChecked();

    if (ui->userEdit->isEnabled())
        username = ui->userEdit->text();
    if (ui->passwordEdit->isEnabled())
        password = ui->passwordEdit->text();

    if (connectToWifi(ssid, username, password, useWPS))
        QDialog::accept();
}

bool WifiSettingsDialog::connectToWifi(const QString &ssid, const QString &username, const QString &password, bool useWPS)
{
    /* Clear the icon showing the current connection (if any) */
    for (int i=0; i< ui->list->count(); i++)
    {
        QListWidgetItem *wi = ui->list->item(i);
        if ( wi->data(SecondIconRole).isValid() )
        {
            wi->setData(SecondIconRole, QVariant() );
        }
    }
    _currentBSS = QDBusObjectPath();

    _qpd = new QProgressDialog("", tr("Cancel"), 0, 0, this, Qt::Window | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    _qpd->show();
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    if (!QNetworkInterface::interfaceFromName(_ifname).addressEntries().isEmpty())
    {
        _qpd->setLabelText(tr("Releasing old DHCP lease"));
        QApplication::processEvents();

        QStringList args;
        args << "--release" << _ifname;
        QProcess::execute("/sbin/dhcpcd", args);
        /* Give the dhcp client some time to say goodbye to the DHCP server */
        msleep(100);

        /* Make sure IP is unset before proceeding */
        args.clear();
        args << _ifname << "0.0.0.0";
        QProcess::execute("/sbin/ifconfig", args);
    }

    _qpd->setLabelText(tr("Disconnecting"));
    QApplication::processEvents();
    _interface->Disconnect().waitForFinished();
    _interface->RemoveAllNetworks();

    _qpd->setLabelText(tr("Connecting"));
    QApplication::processEvents();

    if (useWPS)
    {
        /* WPS authentication */
        FiW1Wpa_supplicant1InterfaceWPSInterface *wps = WpaFactory::createWPSInterfaceProxy(_ifpath);
        QVariantMap config;
        config.insert("Role", "enrollee");
        config.insert("Type", "pbc");
        /* TODO: should we restrict WPS to the BSSID of the access point selected? */
        wps->Start(config).waitForFinished();
        wps->deleteLater();
    }
    else
    {
        QVariantMap config;
        config.insert("ssid", ssid);
        if (!username.isEmpty())
        {
            config.insert("key_mgmt", "WPA-EAP");
            config.insert("identity", username);
            config.insert("password", password);
        }
        else if (!password.isEmpty())
        {
            config.insert("psk", password);
        }
        else
        {
            config.insert("key_mgmt", "NONE");
            config.insert("auth_alg", "OPEN");
        }

        qDebug() << "Connecting to new wifi network, connection parameters:" << config;
        /* Set _connectiong to true. If we receive a "state => disconnected"
           property change signal from this point, assume authentication failed */
        _connecting = true;
        /* Add network to configuration, and select the network */
        _interface->SelectNetwork( _interface->AddNetwork(config).value() );
    }

    /* Check if we have a DHCP lease every 100 msec, while displaying the progress dialog */
    while (QNetworkInterface::interfaceFromName(_ifname).addressEntries().isEmpty() && !_qpd->wasCanceled())
    {
        msleep(100);
    }

    _connecting = false;
    _qpd->hide();
    _qpd->deleteLater();
    _qpd = NULL;

    if (QNetworkInterface::interfaceFromName(_ifname).addressEntries().isEmpty())
    {
        _interface->Disconnect();
        /* Re-enable OK button */
        checkSettings();
        return false;
    }
    else
    {
        /* There does not seem to be a DBus command to save the configuration.
           So ask wpa_cli to do it for us. */
        QStringList args;
        args << "-i"+_ifname << "save_config";
        qDebug() << "Saving wifi configuration";
        QProcess::execute("/usr/sbin/wpa_cli", args);

        return true;
    }
}

/* Sleep for a number of miliseconds while processing events */
void WifiSettingsDialog::msleep(int msec)
{
    QTime t1;
    t1.start();
    int timeleft = msec;

    do
    {
        QApplication::processEvents(QEventLoop::WaitForMoreEvents, timeleft);
        timeleft = msec - t1.elapsed();
    }
    while (timeleft > 0);
}

QString WifiSettingsDialog::removeQuotes(QString str)
{
    if (str.startsWith('"') && str.endsWith('"'))
        str = str.mid(1, str.length()-2);

    return str;
}
