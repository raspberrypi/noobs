#ifndef WPAFACTORY_H
#define WPAFACTORY_H

/**
 * Utility functions to create wpa_supplicant DBus proxy objects
 */

#include "wpa_supplicant/wpasupplicant.h"
#include "wpa_supplicant/interface.h"
#include "wpa_supplicant/wpsinterface.h"
#include "wpa_supplicant/bss.h"
#include "wpa_supplicant/network.h"
#include <QDBusObjectPath>

class WpaFactory
{
public:
    static FiW1Wpa_supplicant1Interface *createWpaSupplicantProxy(QObject *parent = 0);
    static FiW1Wpa_supplicant1InterfaceInterface *createInterfaceProxy(const QDBusObjectPath &path, QObject *parent = 0);
    static FiW1Wpa_supplicant1InterfaceWPSInterface *createWPSInterfaceProxy(const QDBusObjectPath &path, QObject *parent = 0);
    static FiW1Wpa_supplicant1BSSInterface *createBSSProxy(const QDBusObjectPath &path, QObject *parent = 0);
    static FiW1Wpa_supplicant1NetworkInterface *createNetworkProxy(const QDBusObjectPath &path, QObject *parent = 0);

    static inline const char *serviceName()
    {
        return "fi.w1.wpa_supplicant1";
    }

    static inline const char *rootPath()
    {
        return "/fi/w1/wpa_supplicant1";
    }

};

#endif // WPAFACTORY_H
