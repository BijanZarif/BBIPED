#ifndef MESHINGTOOLCONFIGURATIONWARNINGDIALOG_H
#define MESHINGTOOLCONFIGURATIONWARNINGDIALOG_H

#include <QDialog>
#include "Data/PreferencesData/userpreferencesdata.h"
#include "Solver/solverconfigmanager.h"

namespace Ui {
class MeshingToolConfigurationWarningDialog;
}

class MeshingToolConfigurationWarningDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MeshingToolConfigurationWarningDialog(QWidget *parent = 0);
    ~MeshingToolConfigurationWarningDialog();
    void initializeStructures(UserPreferencesData * _userData,SolverConfigManager * _manager );

private slots:
    void on_warningMeshingButtonBox_accepted();

    void on_warningMeshingButtonBox_rejected();

private:
    Ui::MeshingToolConfigurationWarningDialog *ui;
    UserPreferencesData * _userPreferencesData;
    SolverConfigManager * _solverManager;
};

#endif // MESHINGTOOLCONFIGURATIONWARNINGDIALOG_H
