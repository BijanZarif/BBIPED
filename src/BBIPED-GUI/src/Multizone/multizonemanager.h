#ifndef MULTIZONEMANAGER_H
#define MULTIZONEMANAGER_H
#include <QString>
#include <QList>
#include "../Data/ProjectData/VO/projectdata.h"
#include "../Data/ProjectData/VO/multizoneprojectdata.h"
#include "../Data/PreferencesData/userpreferencesdata.h"
#include "../Data/ProjectData/VO/projectdetails.h"
#include "../WindowHelper/projectmenutreehelper.h"


#include <QPushButton>
#include <QLabel>
#include <QAction>
#include <QGroupBox>
#include <QLineEdit>
#include <QTreeView>

class MultizoneManager
{
public:
    MultizoneManager();
    /************************************* REGISTER **************************************************************/
    void registerUIElements(QPushButton * _button, QPushButton * _meshButton, //QPushButton * _meshRefresh,
                                              QLabel * _confLabel, QLabel * _meshLabel,
                                              QAction * _runAction,
                                              QAction * _compareConfigFilesAction,
                            //QGroupBox * _filesGroupBox,
                            QGroupBox * _viewGB,
                                              QGroupBox * _meshProperties,
                                              QLabel * _confLineLabel, QLabel * _meshLineLabel,
                                              QTreeView * _tree);
    bool isUIRegistered();

    /************************************* Create/Build **************************************************************/

    void createInitialProjectDataForMultizoneProject(ProjectData *_projectData,MultizoneProjectData *_multizoneProjectData);
    void buildProjectDataForMultizoneProject(QString _newConfigFile, MultizoneProjectData *_multizoneProjectData,
                                                                ProjectData * _projectData);
    /************************************* Launch/Open **************************************************************/

    bool launchMultizoneDialog(MultizoneProjectData *_multizoneProjectData, UserPreferencesData * _userPreferencesData,
                                                 ProjectData * _projectData);
    bool openMultizoneProject(QString _projectFilePath, MultizoneProjectData *_multizoneProjectData, UserPreferencesData * _userPreferencesData,
                             ProjectData * _projectData, ProjectDetails * _projectDetails);


    /****************************************** Update ***************************************************************/
    void updateMultiZoneValues(QString _filePath, int _option, MultizoneProjectData *_multizoneProjectData,
                                                 ProjectData * _projectData, bool _isNewConfigFile);
    void updateConfigFileNameinTabView(QString _file);
    void updateMeshFileNameinTabView(QString _file);
    void changeTabViewToMultizoneProject(MultizoneProjectData *_multizoneProjectData);

    void updateViewForNewFile(QString _newFile, int _optionMultizone, MultizoneProjectData *_multizoneProjectData,
                                    ProjectData * _projectData, bool _isNewConfigFile);

    void updateOpenEditConfigFile(QString _newConfigFile, MultizoneProjectData *_multizoneProjectData, ProjectData * _projectData,
                                  bool _isNewConfigFile);
    /****************************************** Restore/refresh ***************************************************************/
    void refreshSimulationFilesView(MultizoneProjectData *_multizoneProjectData);

    void restoreTabViewToGeneralProject();

private:
    enum LabelLineNames{
        confPos = 2, //0
        meshPos =3 //1
    };
    enum ActionNames{
        runAction = 0,
        compareConfigFileAction = 1
    };

    enum GroupBoxNames{
        filesGroupBox = 0,
        viewGroupBox = 1,
        meshProperties = 2
    };

    enum ButtonNames{
        runSimulation = 0,
        meshProp = 1,
        meshRefresh = 2
    };
    QList<QPushButton *> _registeredButton;
    QList<QLabel *> _registeredLabels;
    QList<QAction *> _registeredActions;
    QList<QGroupBox *> _registeredGroupBox;
    QList<QLineEdit *> _registeredLineEdit;
   // QTreeView * _registeredTreeView;
    bool _uiRegistered;
    ProjectMenuTreeHelper _treeHelper;


    int getPositionToAddFileMultizone( MultizoneProjectData *_multizoneProjectData);
    int getConfigurationFilePosition(MultizoneProjectData *_multizoneProjectData,ProjectData * _projectData);
    bool checkProjectBasePathConsistency(MultizoneProjectData * _multizoneProjectData,QString _currentFilePath);
    QStringList updateFilePaths(QStringList _listNames, QString _cPathRoot);


};

#endif // MULTIZONEMANAGER_H
