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

namespace Ui {
class MainWindow;
}
class QProgressDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QString *currentLangCode, QSplashScreen *splash, LanguageDialog *ld, QWidget *parent = 0);
    ~MainWindow();

protected:
    Ui::MainWindow *ui;
    QDialog *_qpd;
    QList <int> _kc;
    int _kcpos;
    bool _silent, _allowSilent;
    static bool _partInited;
    static int _currentMode;
    QString *_currentLang;
    QSplashScreen *_splash;
    LanguageDialog *_ld;


    QMap<QString,QString> listInstalledImages();
    QString basefile(const QString &filename);
    QString imageFilenameToFriendlyName(const QString &name);
    virtual void changeEvent(QEvent * event);
    virtual bool eventFilter(QObject *obj, QEvent *event);
    void inputSequence();
    void repopulate();
    void displayMode(QString cmd, QString mode);
    void update_window_title();
    void update_recommended_label();

protected slots:
    void populate();
    void startBrowser();
    /* Events from ImageWriterThread */
    void onError(const QString &msg);
    void onCompleted();

private slots:
    /* UI events */
    void on_actionWrite_image_to_disk_triggered();
    void on_actionCancel_triggered();
    void on_list_currentRowChanged();
    void on_actionAdvanced_triggered(bool checked);
    void on_actionEdit_config_triggered();
    void on_actionBrowser_triggered();
    void on_list_doubleClicked(const QModelIndex &index);
};

#endif // MAINWINDOW_H
