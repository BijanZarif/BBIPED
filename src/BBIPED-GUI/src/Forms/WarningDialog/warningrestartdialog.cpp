#include "warningrestartdialog.h"
#include "ui_warningrestartdialog.h"

WarningRestartDialog::WarningRestartDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WarningRestartDialog)
{
    ui->setupUi(this);
    _acceptToRestart = false;
}

WarningRestartDialog::~WarningRestartDialog()
{
    delete ui;
}

void WarningRestartDialog::on_restart_buttonBox_accepted()
{
    _acceptToRestart = true;
    close();
}

void WarningRestartDialog::on_restart_buttonBox_rejected()
{
    close();

}

bool WarningRestartDialog::userAcceptToRestart()
{
    return _acceptToRestart;
}
