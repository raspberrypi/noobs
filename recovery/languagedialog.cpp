/* Language selection dialog
 *
 * Initial author: Floris Bos
 * Maintained by Raspberry Pi
 *
 * See LICENSE.txt for license details
 *
 */

#include "languagedialog.h"
#include "ui_languagedialog.h"
#include "config.h"
#include <QIcon>
#include <QDebug>
#include <QFile>
#include <QTranslator>
#include <QDir>
#include <QLocale>
#include <QKeyEvent>
#include <QWSServer>
#include <QKbdDriverFactory>
#include <QProcess>
#include <QSettings>

/* Extra strings for lupdate to detect and hand over to translator to translate */
#if 0
QT_TRANSLATE_NOOP("QDialogButtonBox","OK")
QT_TRANSLATE_NOOP("QDialogButtonBox","&OK")
QT_TRANSLATE_NOOP("QDialogButtonBox","Cancel")
QT_TRANSLATE_NOOP("QDialogButtonBox","&Cancel")
QT_TRANSLATE_NOOP("QDialogButtonBox","Close")
QT_TRANSLATE_NOOP("QDialogButtonBox","&Close")
QT_TRANSLATE_NOOP("QDialogButtonBox","&Yes")
QT_TRANSLATE_NOOP("QDialogButtonBox","&No")
#endif

LanguageDialog *LanguageDialog::_instance = NULL;

LanguageDialog::LanguageDialog(const QString &defaultLang, const QString &defaultKeyboard, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LanguageDialog),
    _trans(NULL), _qttrans(NULL)
{
    _instance = this;

    setAttribute(Qt::WA_ShowWithoutActivating);

    QDir settingsdir;
    settingsdir.mkdir("/settings");

    qDebug() << "Default language is " << defaultLang;
    qDebug() << "Default keyboard layout is " << defaultKeyboard;

    QProcess::execute("mount -o ro -t ext4 " SETTINGS_PARTITION " /settings");
    QSettings settings("/settings/noobs.conf", QSettings::IniFormat, this);
    QString savedLang = settings.value("language", defaultLang).toString();
    QString savedKeyLayout = settings.value("keyboard_layout", defaultKeyboard).toString();

    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_QuitOnClose, false);

    QDir kdir("/keymaps/", "*.qmap");
    QStringList keyboardlayouts = kdir.entryList();
    foreach (QString layoutfile, keyboardlayouts)
    {
        layoutfile.chop(5);
        ui->keyCombo->addItem(layoutfile, layoutfile);
    }

    ui->langCombo->addItem(QIcon(":/icons/gb.png"), "English (UK)", "gb");
    ui->langCombo->addItem(QIcon(":/icons/us.png"), "English (US)", "us");

    /* Search for translation resource files */
    QDir dir(":/", "translation_*.qm");
    QStringList translations = dir.entryList();

    foreach (QString langfile, translations)
    {
        QString langcode = langfile.mid(12);
        langcode.chop(3);
        QLocale loc(langcode);
        /* Display languagename in English, e.g. German, French */
        /* QString languagename = QLocale::languageToString(loc.language()); */
        /* should Display languagename in native language, e.g. Deutsch, Français  */
        QString languagename = loc.nativeLanguageName();
        QString iconfilename = ":/icons/"+langcode+".png";

        if (QFile::exists(iconfilename))
            ui->langCombo->addItem(QIcon(iconfilename), languagename, langcode);
        else
            ui->langCombo->addItem(languagename, langcode);

        if (langcode.compare(savedLang, Qt::CaseInsensitive) == 0)
        {
            _currentLang = langcode;
            ui->langCombo->setCurrentIndex(ui->langCombo->count() - 1);
        }
    }

    changeLanguage(savedLang);
    changeKeyboardLayout(savedKeyLayout);
    ui->keyCombo->setCurrentIndex(ui->keyCombo->findData(savedKeyLayout));
}

LanguageDialog::~LanguageDialog()
{
    delete ui;
}

void LanguageDialog::changeKeyboardLayout(const QString &langcode)
{
#ifdef Q_WS_QWS
    QString keymapfile = QString("/keymaps/%1.qmap").arg(langcode);

    if (QFile::exists(keymapfile))
    {
        QWSServer *q = QWSServer::instance();
        q->closeKeyboard();
        q->setKeyboardHandler(QKbdDriverFactory::create("TTY", "keymap="+keymapfile));
    }
#else
    Q_UNUSED(langcode)
#endif

        // Save new language choice to INI files
        QProcess::execute("mount -o remount, rw /settings");
        QSettings settings("/settings/noobs.conf", QSettings::IniFormat, this);
        settings.setValue("keyboard_layout", langcode);
        settings.sync();
        QProcess::execute("mount -o remount, ro /settings");
}

void LanguageDialog::changeLanguage(const QString &langcode)
{
    //if (langcode == _currentLang)
    //    return;

    if (_trans)
    {
        QApplication::removeTranslator(_trans);
        delete _trans;
        _trans = NULL;
    }
    if (_qttrans)
    {
        QApplication::removeTranslator(_qttrans);

        delete _qttrans;
        _qttrans = NULL;
    }

    if (!(langcode == "us" || langcode == "gb"))
    {
        /* qt_<languagecode>.qm are generic language translation files provided by the Qt team
         * this can translate common things like the "OK" and "Cancel" button of dialog boxes
         * Unfortuneately, they are not available for all languages, but use one if we have one. */
        if ( QFile::exists(":/qt_"+langcode+".qm" ))
        {
            _qttrans = new QTranslator();
            _qttrans->load(":/qt_" + langcode+".qm");
            QApplication::installTranslator(_qttrans);
        }

        /* the translation_<languagecode>.qm file is specific to our application */
        if ( QFile::exists(":/translation_"+langcode+".qm"))
        {
            _trans = new QTranslator();
            _trans->load(":/translation_"+langcode+".qm");
            QApplication::installTranslator(_trans);
        }
    }

    /* Update keyboard layout */
    QString defaultKeyboardLayout;
    if (langcode == "nl")
    {
        /* In some countries US keyboard layout is more predominant, although they
         * also do have there own keyboard layout for historic reasons */
        defaultKeyboardLayout = "us";
    }
    else if (langcode == "ja")
    {
        defaultKeyboardLayout = "jp";
    }
    else
    {
        defaultKeyboardLayout = langcode;
    }
    int idx = ui->keyCombo->findData(defaultKeyboardLayout);

    if (idx == -1)
    {
        /* Default to US keyboard layout if there is no keyboard layout for the language */
        idx = ui->keyCombo->findData("us");
    }
    if (idx != -1)
    {
        ui->keyCombo->setCurrentIndex(idx);
    }

    _currentLang = langcode;

    // Save new language choice to INI file
    QProcess::execute("mount -o remount,rw /settings");
    QSettings settings("/settings/noobs.conf", QSettings::IniFormat, this);
    settings.setValue("language", langcode);
    settings.sync();
    QProcess::execute("mount -o remount,ro /settings");

}

void LanguageDialog::on_langCombo_currentIndexChanged(int index)
{
    QString langcode = ui->langCombo->itemData(index).toString();

    changeLanguage(langcode);

}

void LanguageDialog::changeEvent(QEvent* event)
{
    if (event && event->type() == QEvent::LanguageChange)
        ui->retranslateUi(this);

    QDialog::changeEvent(event);
}

void LanguageDialog::on_actionOpenComboBox_triggered()
{
    ui->langCombo->showPopup();
}

void LanguageDialog::on_actionOpenKeyCombo_triggered()
{
    ui->keyCombo->showPopup();
}

void LanguageDialog::on_keyCombo_currentIndexChanged(int index)
{
    QString keycode = ui->keyCombo->itemData(index).toString();

    changeKeyboardLayout(keycode);

}

LanguageDialog *LanguageDialog::instance(const QString &defaultLang, const QString &defaultKeyboard)
{
    /* Singleton */
    if (!_instance)
        new LanguageDialog(defaultLang, defaultKeyboard);

    return _instance;
}

QString LanguageDialog::currentLanguage()
{
    return _currentLang;
}
