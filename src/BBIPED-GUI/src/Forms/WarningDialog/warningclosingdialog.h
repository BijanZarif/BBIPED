#ifndef WARNINGCLOSINGDIALOG_H
#define WARNINGCLOSINGDIALOG_H

#include <QDialog>

namespace Ui {
class WarningClosingDialog;
}

class WarningClosingDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit WarningClosingDialog(QWidget *parent = 0);
    ~WarningClosingDialog();
    bool needsToCloseApplication();
    void setTitleMessage(QString _newTitle);
    bool getShowAgainValue();
    
private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_donotShowMessageCheckBox_clicked(bool checked);

private:
    Ui::WarningClosingDialog *ui;
    bool _needToBeClosed;
    bool _showWindow;
};

#endif // WARNINGCLOSINGDIALOG_H
