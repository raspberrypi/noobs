#ifndef LANGUAGEDIALOG_H
#define LANGUAGEDIALOG_H

/* Language selection dialog
 *
 * Initial author: Floris Bos
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
    explicit LanguageDialog(QWidget *parent = 0);
    ~LanguageDialog();
    void changeLanguage(const QString &langcode);

protected:
    Ui::LanguageDialog *ui;
    QTranslator *_trans, *_qttrans;
    QString _currentLang;
    virtual void changeEvent(QEvent *event);

private slots:
    void on_langCombo_currentIndexChanged(int index);
    void on_actionOpenComboBox_triggered();

};

#endif // LANGUAGEDIALOG_H
