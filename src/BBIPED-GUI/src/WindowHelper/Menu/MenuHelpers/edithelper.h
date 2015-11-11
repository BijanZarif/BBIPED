#ifndef EDITHELPER_H
#define EDITHELPER_H

#include "Data/PreferencesData/userpreferencesdata.h"
#include "Solver/solverconfigmanager.h"
#include "Data/ProjectData/VO/multizoneprojectdata.h"
#include "Data/ProjectData/VO/projectdata.h"
#include "Data/ProjectData/VO/projectdetails.h"
class EditHelper
{
public:
    EditHelper();

    void launchPreferencesDialog(UserPreferencesData * _userPreferencesData, SolverConfigManager * _solverManager);
    bool launchSettingsDialog(bool _isMultizoneProject,MultizoneProjectData * _multizoneProjectData, ProjectData * _projectData,
                              ProjectDetails * _projectDetailsData);
};

#endif // EDITHELPER_H
