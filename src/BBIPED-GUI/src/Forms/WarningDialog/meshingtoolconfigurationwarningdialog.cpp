#include "meshingtoolconfigurationwarningdialog.h"
#include "ui_meshingtoolconfigurationwarningdialog.h"

#include "WindowHelper/Menu/MenuHelpers/edithelper.h"

MeshingToolConfigurationWarningDialog::MeshingToolConfigurationWarningDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MeshingToolConfigurationWarningDialog)
{
    ui->setupUi(this);
}

MeshingToolConfigurationWarningDialog::~MeshingToolConfigurationWarningDialog()
{
    delete ui;
}

void MeshingToolConfigurationWarningDialog::on_warningMeshingButtonBox_accepted()
{
    EditHelper _editHelper;
    _editHelper.launchPreferencesDialog(_userPreferencesData,_solverManager);
    close();
}

void MeshingToolConfigurationWarningDialog::on_warningMeshingButtonBox_rejected()
{
    close();
}
void MeshingToolConfigurationWarningDialog::initializeStructures(UserPreferencesData * _userData,SolverConfigManager * _manager )
{
    _userPreferencesData = _userData;
    _solverManager = _manager;
}
