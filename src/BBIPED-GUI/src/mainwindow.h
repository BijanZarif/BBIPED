#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QHash>
#include <QString>
#include <QMenu>
#include <QSignalMapper>

#include "Data/ProjectData/VO/projectdata.h"
#include "Data/Solver/VO/solvervo.h"
#include "Solver/solverconfigmanager.h"
#include "Forms/SolverMenuDialog/solverbasicdialog.h"
#include "Forms/summarydialog.h"
#include "Parser/Solver/outputsolverparser.h"
#include "Data/Solver/VO/solverresidualvo.h"
#include "Plotter/plotterhelper.h"

#include "WindowHelper/MainWindow/inputdatatabhelper.h"
#include "WindowHelper/MainWindow/residualtabhelper.h"

#include "Plotter/solverplotter.h"

#include "Forms/SolverMenuDialog/meshdialog.h"

#include "Data/ProjectData/VO/multizoneprojectdata.h"
#include "Data/PreferencesData/userpreferencesdata.h"
#include "Multizone/multizonemanager.h"
#include "Data/ProjectData/VO/projectdetails.h"
#include "WindowHelper/projectmenutreehelper.h"
#include "Data/Solver/VirtualMRF/virtualmrfvo.h"
#include "Data/Solver/MeshInfo/meshinfovo.h"


namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:

    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();
    //  void mouseReleaseEvent ( QMouseEvent * event );
    void saveMultizoneData(QString _configFile, QString _meshFile, QHash<QString,QString> * _configVal,
                           QList<VirtualMrfVO> * _virtualMrfList);


protected:
    void closeEvent(QCloseEvent *event);

signals:

    void outlogtext(QString ver);

private slots:

    /************************************ MENU ACTIONS ************************************/
    void on_actionNew_Project_triggered();
    void on_actionOpen_Project_triggered();
    void on_actionSave_triggered();
    void on_actionSave_As_triggered();
    void on_actionSave_Temporal_triggered();
    void on_actionClose_Project_triggered();
    void on_actionHelp_Contents_triggered();
    void on_actionExit_triggered();

    /**************** Help Actions */
    void on_actionAbout_triggered();
    void on_actionDeveloper_Guide_2_triggered();
    void on_actionUser_GuideLine_2_triggered();
    void on_actionOnline_Help_triggered();

    // Solver
    void on_actionRun_Project_triggered();
    void on_actionStop_Execution_triggered();
    void on_actionImport_Mesh_File_triggered();

    // Other tools
    void on_actionPreferences_triggered();
    void on_actionSalome_Mesh_triggered();
    void on_actionParaview_File_triggered();

    /************************************ BUTTON ACTIONS ************************************/
    // TOOL BUTTONS
    void on_solverButton_clicked();
    void on_paraviewButton_clicked();
    void on_salomeButton_clicked();

    // Tab functions
    void on_solverTabwidget_tabCloseRequested(int index);

    // Configuration File Tab Buttons
    //void on_newActionButton_clicked();
    //- Configure physics and boundaries
    void on_basivViewEditButton_clicked();
    void on_importMeshFileButton_clicked();
    void on_setMeshPropertiesButton_clicked();
    void on_savePhysicsButton_clicked();
    void on_seeSummaryChangesButton_clicked();
    //- Save
    void on_actionSave_Configuration_File_triggered();
    void on_actionSave_Configuration_File_As_triggered();
    //- Run/Stop/Restart
    void on_restartButton_clicked();
    void on_runSimulationButton_clicked();
    void on_stopButton_clicked();

    /************************************ TREE VIEW ************************************/
    void on_projectTreeView_doubleClicked(const QModelIndex &index);

    /************************************ SLOTS ************************************/
    void outlog();
    void slotProcessError(QProcess::ProcessError error);
    void slotStarted();
    void slotFinishResult(int _val, QProcess::ExitStatus status);

    /************************************ Multizone ************************************/
    void on_actionNew_Multizone_Project_triggered();
    void on_actionOpen_Multizone_Project_triggered();

    /************************************ Project Settings *****************************/
    void on_actionProject_Settings_triggered();

    /************************************ Template ************************************/
    void on_actionCreate_Configuration_Template_triggered();
    void on_actionEdit_Configuration_Template_triggered();
    void on_actionLoad_Configuration_Template_triggered();

    /************************************ OThers ************************************/
    void on_actionCompare_Configuration_Files_triggered();
    void on_actionSynchronise_all_files_triggered();
    void openRecentFile();
    void on_actionSave_Residual_Image_triggered();
    void getSalomePathSlot();


    /************************************** MULTIZONE TAB **********************************/
    void on_actionLoad_Templates_triggered();

    void on_actionAutomatic_Optimization_triggered();

    void on_multizoneNewButton_clicked();

    void on_multizoneOpenEditButton_clicked();

    void on_multizoneDeleteButton_clicked();

    void on_multizoneSummaryAllConfigButton_clicked();

    void on_multizoneSaveAllButton_clicked();

    void on_multizoneRunSimulationButton_clicked();

    void on_multizoneStopSimulationButton_clicked();

    void on_multizoneRestartSimulation_clicked();


    void on_actionCreate_New_Configuration_File_triggered();

    void on_actionLoad_Configuration_File_triggered();

private:
    // MainWindow Object
    Ui::MainWindow *ui;

    // Dialog Variables
    SummaryDialog * _summaryDialog;
    MeshDialog * _meshWindow;
    QTextEdit * _projectTextEditor;

    // Recent Projects Variables
    QSignalMapper * _recentProjectsMapper;
    QList<QAction *> _recentProjectsList;


    // Helper Variables
    ResidualTabHelper * _residualTabHelper;
    InputDataTabHelper _inputDataTabHelper;
    ProjectMenuTreeHelper _treeHelper;


    // Project Data
    ProjectData  _projectData;
    MultizoneProjectData _multizoneProjectData;
    ProjectDetails _projectDetailsData;
    MeshInfoVO _meshData;

    // Preferences
    UserPreferencesData _userPreferencesData;


    // Project Variables
    bool _status_NeedToBeSaved;
    bool _projectExists;

    // Solver
    SolverConfigManager _solverManager;
    OutputSolverParser * _outputSolverParser;
    SolverResidualVO * _residualVO;

    // Boundary
    QHash<QString, QString> _userConfigValuesSolver;
    QHash<QString, QString> _userBoundaryValuesSolver; // Boundary Values loaded or made by the user

    QHash<QString,QString> _boundaryMeshValues;
    QHash<QString,QString> _boundaryConfigValues;


    // Process and Other tools
    QProcess * _myprocess;
    QProcess * _salomeLocationProccess;

    // Plotting
    QPolygonF points;
    QwtPlotCurve *curve;
    SolverPlotter * plotResiduals;
    bool _stopped;
    bool _updatedLegend;

    // Multizone Variables
    bool _isMultizoneProject;
    bool _isNewConfigFile;

    MultizoneManager _mManager;

    // Virtual MRF Variables
    QList<VirtualMrfVO> _userVirtualMRFDataList;

    // Mesh variables
    bool _is3DMesh;

    // TAB ENUM
    enum tabVariables{
        tabInputData = 0,
        tabMultizoneInputData =1,
        tabResidual = 2
    };

    /************************************ Initialize Functions ************************************/
    void initializeStructures();
    void initializeHelper();
    void initializeManagers();

    void loadMeshDialog();

    /********** Functios for Dialogs */
    bool openSolverConfigurationFileDialog();
    void importMeshFile();
    void importMeshFile(QString _meshConfigFile);
    void importMeshInfoFile(QString _file);

    void seeParaviewResults();
    void launchSalomeMeshTool();

    /************************************ Save/Update Functions ************************************/
    bool saveConfigFile();
    void saveMeshVariables();

    void updateProjectDataToFile();
    void updateBoundaryValues();

    void getLoadedBoundaryValues();

    /************************************ disable/enable Functions ************************************/
    void enableAllMainButtons();
    void disableAllMainButtons();

    void enableToolButtons(int _buttonName);
    void enableSolverRunAction();

    void setEnableCloseProjectIcons(bool _val);
    void setEnableSaveProjectIcons(bool _val);
    void setEnabledRunIcon(bool _val);
    void setEnabledSaveConfigIcons(bool _val);
    void setEnabledInputTabSaveButtons(bool _val);

    /************************************ clear Functions ************************************/
    void clearProjectTreeView();
    void clearConsoleText();    
    void clearAllData();

    void removeMarkerElementsFromConfigFile();

    /************************************ close/stop/reset Functions */
    void closeProject();
    void stopSolverProcess();
    void resetSolverTabEnabledElements();
    void resetOldMarkerValues();
    void resetSolverMenu();
    void resetUserConfigurationValues();
    void resetBooleanValues();


    /************************ Configuration File Tab */
    void runSolverProject();
    void runSimulation();
    //void setPlotEnvironment(QwtPlot * _plot);
    void setResidualPlot();
    void setUpStatusBar();

    /************************ Getter/Setter ***************************************/
    QString getSolverCommand();

    /********************** MULTIZONE FUNCTIONS ***********************************/
    void registerUIMultizoneElements();

    void connectProcessSignalSlots();
    QString buildSolverCommand();
    //void launchFileComparisonDialog();
    void enableMeshOptions();
    int getCurrentUserPreferences();
    void removeDefaultValuesReadFromConfigFile();
    void checkMeshNames();
    bool exitApplication();

    /*********************************** USER PREFERENCES *******************************/
    void initializeUserPreferences();
    void updateRecentProjectsSection();
    void createProjectActions(QStringList _list);
    void openProject(QString _projectFilePath);
    void addProjectToRecentProjectsSection(QString _projectPathName);

    QAction *  createRecentProjectAction(QString _name);
    void enableMultizoneElements();
    void closeExistentProject();
    void launchActionProjectSettingsDialog();
    void checkProjectBasePathConsistency(ProjectData * _openedProject ,QString _currentFilePath);

    void updateMainWindowElements(QString _projectPath, bool _isNewProject, QString _finalPath);
    void setEnabledProjectIconButtons(bool _val);
    void resetBoundaryValues();


    /**************** MULTIZONE NEW TAB ***************************/
    void fillMultizoneTable();
    void resetMultizoneTable();
    QList<int> getSelectedFiles();
    void updateMultizoneUserData(QString _configFile,QString _meshFile);
    void insertMultizoneRow(int _pos, QString _configName, QString _meshName);
    void multizoneSaveAll();
    void runSolverProject(QString _projectPath);
    bool notSavedMultizoneData();


    void updateMeshView(QString _solverFilePath);

    void setEnableMenuSolver(bool _val);

    void createNewConfigurationFile();

    void removeOldMeshFiles();


};

#endif // MAINWINDOW_H
