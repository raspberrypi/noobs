#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/* Main window
 *
 * Initial author: Floris Bos
 * Maintained by Raspberry Pi
 *
 * See LICENSE.txt for license details
 *
 */

#include "languagedialog.h"
#include <QMainWindow>
#include <QModelIndex>
#include <QSplashScreen>
#include <QMessageBox>
#include <QTimer>
#include <QTime>

namespace Ui {
class MainWindow;
}
class QProgressDialog;
class QSettings;
class QListWidgetItem;
class QNetworkAccessManager;
class QMessageBox;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(const QString &drive, const QString &defaultDisplay, QSplashScreen *splash, QWidget *parent = 0);
    ~MainWindow();

protected:
    Ui::MainWindow *ui;
    QDialog *_qpd;
    QList <int> _kc;
    int _kcpos;
    const QString _defaultDisplay;
    bool _silent, _allowSilent, _showAll, _fixate, _usbimages;
    static bool _partInited;
    static int _currentMode;
    QSplashScreen *_splash;
    QSettings *_settings;
    bool _hasWifi;
    int _numInstalledOS, _devlistcount;
    QNetworkAccessManager *_netaccess;
    int _neededMB, _availableMB, _numMetaFilesToDownload, _numIconsToDownload;
    QMessageBox *_displayModeBox;
    QTimer _networkStatusPollTimer, _piDrivePollTimer;
    QTime _time;
    QString _model, _repo, _drive, _bootdrive;

    QMap<QString,QVariantMap> listImages(const QString &folder = "/mnt/os", bool includeInstalled = true);
    virtual void changeEvent(QEvent * event);
    virtual bool eventFilter(QObject *obj, QEvent *event);
    void inputSequence();
    void repopulate();
    void displayMode(int modenr, bool silent = false);
    void update_window_title();
    bool requireNetwork();
    bool isOnline();
    QStringList getFlavours(const QString &folder);
    void rebuildInstalledList();
    QListWidgetItem *findItem(const QVariant &name);
    QList<QListWidgetItem *> selectedItems();
    void updateNeeded();
    void downloadMetaFile(const QString &url, const QString &saveAs);
    void downloadIcon(const QString &urlstring, const QString &originalurl);
    void downloadList(const QString &urlstring);
    void downloadLists();
    void startImageWrite();
    bool canInstallOs(const QString &name, const QVariantMap &values);
    bool isSupportedOs(const QString &name, const QVariantMap &values);
    void addImagesFromUSB(const QString &device);
    void filterList();

protected slots:
    void populate();
    void startBrowser();
    void startNetworking();
    void pollNetworkStatus();
    void onOnlineStateChanged(bool online);
    void downloadListComplete();
    void processJson(QVariant json);
    void processJsonOs(const QString &name, QVariantMap &details, QSet<QString> &iconurls);
    /* Events from ImageWriterThread */
    void onError(const QString &msg);
    void onCompleted();
    void downloadIconComplete();
    void downloadMetaRedirectCheck();
    void downloadIconRedirectCheck();
    void downloadListRedirectCheck();
    void downloadMetaComplete();
    void onQuery(const QString &msg, const QString &title, QMessageBox::StandardButton* answer);
    void hideDialogIfNoNetwork();
    void pollForNewDisks();

private slots:
    /* UI events */
    void on_actionWrite_image_to_disk_triggered();
    void on_actionCancel_triggered();
    void on_list_currentRowChanged();
    void on_actionAdvanced_triggered(bool checked);
    void on_actionEdit_config_triggered();
    void on_actionBrowser_triggered();
    void on_list_doubleClicked(const QModelIndex &index);
    void on_list_itemChanged(QListWidgetItem *item);
    void on_actionWifi_triggered();

    void on_targetCombo_currentIndexChanged(int index);

signals:
    void networkUp();
};

#endif // MAINWINDOW_H
