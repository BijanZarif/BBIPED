#ifndef SOLVERHELPER_H
#define SOLVERHELPER_H

#include <QString>
#include <QMainWindow>
#include "Solver/solverconfigmanager.h"
#include "Data/PreferencesData/userpreferencesdata.h"
#include "Data/ProjectData/VO/multizoneprojectdata.h"
#include "Data/ProjectData/VO/projectdata.h"
#include "Data/ProjectData/VO/projectdetails.h"
#include "Data/Solver/VirtualMRF/virtualmrfvo.h"

class SolverHelper
{
public:
    SolverHelper();

    void createConfigurationTemplateByUser(SolverConfigManager * _solverConfigManager);
    void editConfigurationTemplateByUser(QMainWindow * _mainWindow,SolverConfigManager * _solverConfigManager);
    void loadConfigurationTemplateByUser(QMainWindow * _mainWindow,SolverConfigManager * _solverConfigManager,
                                         UserPreferencesData * _userPreferencesData);


    void launchFileComparisonDialog(bool _isMultizoneProject,MultizoneProjectData _multizoneProjectData, ProjectData _projectData,
                                                  SolverConfigManager _solverManager, ProjectDetails _projectDetailsData,
                                                  QList<VirtualMrfVO> _userVirtualMRFDataList);

};

#endif // SOLVERHELPER_H
