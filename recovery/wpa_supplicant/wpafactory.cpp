#include "wpafactory.h"
#include <QDBusConnection>

FiW1Wpa_supplicant1Interface *WpaFactory::createWpaSupplicantProxy(QObject *parent)
{
    return new FiW1Wpa_supplicant1Interface(serviceName(), rootPath(), QDBusConnection::systemBus(), parent);
}

FiW1Wpa_supplicant1InterfaceInterface *WpaFactory::createInterfaceProxy(const QDBusObjectPath &path, QObject *parent)
{
    return new FiW1Wpa_supplicant1InterfaceInterface(serviceName(), path.path(), QDBusConnection::systemBus(), parent);
}

FiW1Wpa_supplicant1InterfaceWPSInterface *WpaFactory::createWPSInterfaceProxy(const QDBusObjectPath &path, QObject *parent)
{
    return new FiW1Wpa_supplicant1InterfaceWPSInterface(serviceName(), path.path(), QDBusConnection::systemBus(), parent);
}

FiW1Wpa_supplicant1BSSInterface *WpaFactory::createBSSProxy(const QDBusObjectPath &path, QObject *parent)
{
    return new FiW1Wpa_supplicant1BSSInterface(serviceName(), path.path(), QDBusConnection::systemBus(), parent);
}

FiW1Wpa_supplicant1NetworkInterface *WpaFactory::createNetworkProxy(const QDBusObjectPath &path, QObject *parent)
{
    return new FiW1Wpa_supplicant1NetworkInterface(serviceName(), path.path(), QDBusConnection::systemBus(), parent);
}
