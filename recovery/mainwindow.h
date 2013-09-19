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
    explicit MainWindow(const QString &defaultDisplay, QSplashScreen *splash, QWidget *parent = 0);
    ~MainWindow();

protected:
    Ui::MainWindow *ui;
    QDialog *_qpd;
    QList <int> _kc;
    int _kcpos;
    const QString _defaultDisplay;
    bool _silent, _allowSilent;
    static bool _partInited;
    static int _currentMode;
    QSplashScreen *_splash;
    QSettings *_settings;
    bool _activatedEth;
    int _numInstalledOS;
    QNetworkAccessManager *_netaccess;
    int _neededMB, _availableMB, _numMetaFilesToDownload, _numIconsToDownload;
    QMessageBox *_displayModeBox;

    QMap<QString,QVariantMap> listImages();
    virtual void changeEvent(QEvent * event);
    virtual bool eventFilter(QObject *obj, QEvent *event);
    void inputSequence();
    void repopulate();
    void displayMode(int modenr, bool silent = false);
    void update_window_title();
    bool requireNetwork();
    QStringList getFlavours(const QString &folder);
    void rebuildInstalledList();
    void remountSettingsRW();
    bool alreadyHasItem(const QVariant &name, const QVariant &releasedate);
    QList<QListWidgetItem *> selectedItems();
    void updateNeeded();
    void downloadMetaFile(const QString &url, const QString &saveAs);
    void downloadIcon(const QString &urlstring, const QString &originalurl);
    void downloadList(const QString &urlstring);
    void startImageWrite();

protected slots:
    void populate();
    void startBrowser();
    void startNetworking();
    void ifupFinished(int exitCode);
    void downloadListComplete();
    void processJson(QVariant json);
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

signals:
    void networkUp();
};

#endif // MAINWINDOW_H
