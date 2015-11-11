#ifndef MULTIZONEPROJECTDIALOG_H
#define MULTIZONEPROJECTDIALOG_H

#include <QDialog>
#include <QString>
#include <QStringList>
#include "Solver/solverconfigmanager.h"
#include "Data/ProjectData/VO/multizoneprojectdata.h"
#include "Data/ProjectData/VO/projectdetails.h"
#include "Data/Solver/VirtualMRF/virtualmrfvo.h"

namespace Ui {
class MultizoneProjectDialog;
}

class MultizoneProjectDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit MultizoneProjectDialog(QWidget *parent = 0);
    ~MultizoneProjectDialog();
    bool isMultizoneProjectCreated();
    MultizoneProjectData getMultizoneProjectData();
    void setProjectDetails(ProjectDetails _projectDetails);
    
private slots:
    void on_multizoneNoSpinBox_valueChanged(const QString &arg1);
    void on_projectSelectionToolButton_clicked();
    void on_configTable_cellClicked(int row, int column);
    void on_buttonBox_rejected();


private:
    Ui::MultizoneProjectDialog *ui;
    QString _projectPath;
    int _noZones;
    QStringList _newFilesList;
    QStringList _importedFilesList;

    bool _isMultizoneProjectCreated;
    MultizoneProjectData  _multizoneProjectData;
    ProjectDetails _projectDetailsData;
    QList<VirtualMrfVO> _virtualMRFData;


    void createRowInConfigFileTable(int _noFiles);
    void createConfigurationRow(QString _content, int _idRow, int _idZone);
    void createMeshFileRow(QString _content, int _idRow, int _idZone);
    void enableMultizoneParameters();

    void launchCreateNewFile(int _row);
    void launchImportNewFile(int _row);
    void deleteFile(int _row);
    void decreaseZoneNumber();

    bool buildFilesInProject();

    bool checkConsistency(QString _configFilePath, QString _meshFilePath,SolverConfigManager _solverManager);
    bool isMeshFileNameValid(QString _file);
    void accept();
    void updateZoneNumberInCell();


};

#endif // MULTIZONEPROJECTDIALOG_H
