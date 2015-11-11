#ifndef INPUTDATATABHELPER_H
#define INPUTDATATABHELPER_H
#include <QWidget>
#include <QProcess>
#include <QString>
#include <QTextEdit>
#include <QLineEdit>
#include <QRadioButton>
#include <QPushButton>
#include <QLabel>
//#include <QCommandLinkButton>
#include <QToolButton>
#include <QTabWidget>
#include <QHash>

#include "Data/ProjectData/VO/projectdata.h"
#include "Forms/SolverMenuDialog/solverbasicdialog.h"
#include "Solver/solverconfigmanager.h"
#include "Data/ProjectData/VO/projectdetails.h"

#include "Data/Solver/VirtualMRF/virtualmrfvo.h"

class InputDataTabHelper
{
public:
    InputDataTabHelper();

    /************************** Register Actions ********************/
    void registerButtons(QPushButton * _mybasicV,// QPushButton * _myAdvV,
                         QPushButton * _mySummaryB, QPushButton * _myRunSimB,
                         QPushButton * _myRestartB);
   // void registerLineEdits(QLineEdit * _myConfigLine, QLineEdit * _myMeshLine);
    void registerLineEdits(QLabel * _myConfigLine, QLabel * _myMeshLine);
    void registerTab(QTabWidget * _myTab);

    /************************************************ Show/Hide ***********************/
    void showAllSolverMenus( ProjectData _projectData);
    void hideAllSolverMenus();


    /*****************************Update/Save *************************************/
    void updateConfigurationFilePathInActionGroup(QString _newPath);
    void updateMeshFileNameInActionGroup(QString _newMeshPath,QHash<QString, QString> * _userMap);

    bool updateSolverUserValues(ProjectData _projectData,
                                QHash<QString,QString> * _userConfigVal,
                                QHash<QString,QString> * _boundaryMeshVal,
                               QHash<QString,QString> * _boundaryConfigVal,
                                 QList<VirtualMrfVO> * _virtualVOList);

    void saveAsConfigFile(QString _fileName,ProjectData * _projectData, SolverConfigManager * _manager,
                         QHash<QString,QString> _userVal, ProjectDetails _projectDetails,QList<VirtualMrfVO> _virtualVOList);

    /*****************************Launch/Run *************************************/
    bool launchSolverConfigDialog(QWidget * _parent,bool _isBasicView,QHash<QString,QString> * _configValToSave,
                                  SolverConfigManager * _manager,QList<VirtualMrfVO> * _virtualVOList,
                                  bool _is3DMesh);


    /*****************************Fill/Build *************************************/
    void fillFileNamesInTab(ProjectData _projectData,
                                                QHash<QString,QString> * _userMap);

    void parseFile(int _option,QString _pathFile, QHash<QString,QString> * _userConfigVal,
                                       QHash<QString,QString> * _boundaryMeshVal, QList<VirtualMrfVO> * _virtualVOList);

    /************************************************ ENABLE/DISABLE ***********************/

    void enableConfigurationViewMenu(ProjectData _projectData);
    void enableChangeConfigActionMenu();
    void enableRestartSimButtons();

    void disableChangeConfigActionMenu();
    void disableRestartSimButtons();
    void disableAllInputDataTab( );

private:
    QPushButton * _basicV;
    //QPushButton * _advV;
    QPushButton * _summaryB;
    QPushButton * _runSimB;

  //  QLineEdit * _configLine;
  //  QLineEdit * _meshLine;
    QLabel * _configLineLabel;
    QLabel * _meshLineLabel;

    QPushButton * _saveB;
    QPushButton * _saveAsB;
    QTabWidget * _tab;
    QPushButton * _restartB;

    /*****************************Initialize *************************************/
    void initializeSolverBasicDialog(SolverBasicDialog * _dialog, bool _basicView,
                                     QHash<QString,QString> _userConfigVal,
                                     SolverConfigManager * _manager,QList<VirtualMrfVO>  _virtualVOList,
                                     bool _is3DMesh);

    /***************************** Update *************************************/

    void updateUserConfigValues(QHash<QString,QString> _newMap,
                                QHash<QString,QString> * _configValToSave,
                                bool _reseted);

    /*****************************Enable/Disable *************************************/
    void enableCurrentSimActionMenu();
    void disableCurrentSimActionMenu();

};

#endif // INPUTDATATABHELPER_H
