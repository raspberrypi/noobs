#ifndef LANGUAGEDIALOG_H
#define LANGUAGEDIALOG_H

/* Language selection dialog
 *
 * Initial author: Floris Bos
 * Maintained by Raspberry Pi
 *
 * See LICENSE.txt for license details
 *
 */

#include <QDialog>

namespace Ui {
class LanguageDialog;
}
class QTranslator;

class LanguageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LanguageDialog(const QString &defaultLang, const QString &defaultKeyboard, QWidget *parent = 0);
    ~LanguageDialog();
    void changeLanguage(const QString &langcode);
    void changeKeyboardLayout(const QString &langcode);
    QString currentLanguage();
    static LanguageDialog *instance(const QString &defaultLang, const QString &defaultKeyboard);

protected:
    Ui::LanguageDialog *ui;
    QTranslator *_trans, *_qttrans;
    QString _currentLang;
    static LanguageDialog *_instance;
    virtual void changeEvent(QEvent *event);

private slots:
    void on_langCombo_currentIndexChanged(int index);
    void on_actionOpenComboBox_triggered();

    void on_actionOpenKeyCombo_triggered();
    void on_keyCombo_currentIndexChanged(int index);
};

#endif // LANGUAGEDIALOG_H
