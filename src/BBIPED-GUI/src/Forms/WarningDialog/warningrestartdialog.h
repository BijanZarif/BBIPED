#ifndef WARNINGRESTARTDIALOG_H
#define WARNINGRESTARTDIALOG_H

#include <QDialog>

namespace Ui {
class WarningRestartDialog;
}

class WarningRestartDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WarningRestartDialog(QWidget *parent = 0);
    ~WarningRestartDialog();
    bool userAcceptToRestart();


private slots:
    void on_restart_buttonBox_accepted();

    void on_restart_buttonBox_rejected();

private:
    Ui::WarningRestartDialog *ui;
    bool _acceptToRestart;
};

#endif // WARNINGRESTARTDIALOG_H
