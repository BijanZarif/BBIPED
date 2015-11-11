#ifndef PROJECTSETTINGSDIALOG_H
#define PROJECTSETTINGSDIALOG_H

#include <QDialog>
#include "Data/ProjectData/VO/multizoneprojectdata.h"
#include "Data/ProjectData/VO/projectdata.h"
#include "Data/ProjectData/VO/projectdetails.h"


namespace Ui {
class ProjectSettingsDialog;
}

class ProjectSettingsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ProjectSettingsDialog(QWidget *parent = 0);
    ~ProjectSettingsDialog();

    void setProjectData(ProjectData _data);
    ProjectData getProjectData();

    void setMultizoneProjectData(MultizoneProjectData _data);
    MultizoneProjectData getMultizoneProjectData();

    void setProjectDetailsData(ProjectDetails _prjDetails);
    ProjectDetails getProjectDetailsData();

    bool userAcceptChanges();
    
private slots:
    void on_buttonBox_accepted();

    void on_associatedFileTable_cellClicked(int row, int column);

    void on_buttonBox_rejected();

private:
    enum SettingsColumnsTypes{
        SettingsColZoneNo = 0,
        SettingsColFileType = 1,
        SettingsColDelFile =2

    };


    Ui::ProjectSettingsDialog *ui;
    ProjectData _projectData;
    MultizoneProjectData _multizoneData;
    ProjectDetails _projectDetailsData;

    QStringList _removedConfigFiles;
    QStringList _removedMeshFiles;
    bool _userAcceptChanges;

    void updateProjectInformationInSettingsView();
    void updateFileTableInSettingsView();
    void addRowToTable(int _idRow, int _idZone, QString _fileName);
    void addValuesToTable(QStringList _list1, QStringList _list2);
    void updateProjectData();
    void updateProjectDetailsInProjectDetailsTabView();


};

#endif // PROJECTSETTINGSDIALOG_H
