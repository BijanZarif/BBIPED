#ifndef ASKFORTEMPLATENAMEDIALOG_H
#define ASKFORTEMPLATENAMEDIALOG_H

#include <QDialog>
#include <QString>


namespace Ui {
class AskForTemplateNameDialog;
}

class AskForTemplateNameDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit AskForTemplateNameDialog(QWidget *parent = 0);
    ~AskForTemplateNameDialog();
    QString getTemplateName();
    //QString getTemplateFileName();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_toolButton_clicked();

private:
    Ui::AskForTemplateNameDialog *ui;
    QString _templateName;
   // QString _templateFileName;

};

#endif // ASKFORTEMPLATENAMEDIALOG_H
