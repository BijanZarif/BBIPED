#ifndef HELPDIALOG_H
#define HELPDIALOG_H

#include <QDialog>

namespace Ui {
class HelpDialog;
}



class HelpDialog : public QDialog
{
    Q_OBJECT
    
public:

    explicit HelpDialog(QWidget *parent = 0);

    ~HelpDialog();
    void setHelpText(QString _text);
    void loadHelpText();
    
private slots:
    void on_okHelpDialog_clicked();

private:
    Ui::HelpDialog *ui;
    QString _helpText;

};

#endif // HELPDIALOG_H
