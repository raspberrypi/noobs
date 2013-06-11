/* Language selection dialog
 *
 *
 * Initial author: Floris Bos
 * Maintained by Raspberry Pi
 *
 * See LICENSE.txt for license details
 *
 */

#include "languagedialog.h"
#include "ui_languagedialog.h"
#include <QIcon>
#include <QDebug>
#include <QFile>
#include <QTranslator>
#include <QDir>
#include <QLocale>
#include <QKeyEvent>

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

LanguageDialog::LanguageDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LanguageDialog),
    _trans(NULL), _qttrans(NULL)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_QuitOnClose, false);

    ui->langCombo->addItem(QIcon(":/icons/gb.png"), "English", "");

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
    }
}

LanguageDialog::~LanguageDialog()
{
    delete ui;
}

void LanguageDialog::changeLanguage(const QString &langcode)
{
    if (langcode == _currentLang)
        return;

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

    if (!langcode.isEmpty() )
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

    _currentLang = langcode;
}

void LanguageDialog::on_langCombo_currentIndexChanged(int index)
{
    changeLanguage(ui->langCombo->itemData(index).toString());
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
