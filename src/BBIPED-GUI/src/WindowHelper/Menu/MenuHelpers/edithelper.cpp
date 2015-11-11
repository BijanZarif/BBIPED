/***
 *    This file is part of BBIPED GUI.
 *
 *   BBIPED GUI is free software: you can redistribute it and/or modify
 *   it under the terms of the Lesser GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   BBIPED GUI is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   Lesser GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with Foobar. If not, see <http://opensource.org/licenses/LGPL-3.0>.
***/
#include "edithelper.h"

#include "Forms/EditMenuDialog/preferencedialog.h"
#include "Forms/EditMenuDialog/projectsettingsdialog.h"
/**
 * @brief
 *
 */
EditHelper::EditHelper()
{
}



/**
 * @brief
 *
 * @param _userPreferencesData
 * @param _solverManager
 */
void EditHelper::launchPreferencesDialog(UserPreferencesData * _userPreferencesData, SolverConfigManager * _solverManager)
{
    PreferenceDialog * _prefDialog =  new PreferenceDialog();
    _prefDialog->initialize(* _userPreferencesData);
    // Executing the Dialog
    _prefDialog->exec();

    // Updating the user values from the Dialog
    if  (_prefDialog->isPreferencesAccepted() )
    {
        UserPreferencesData _newData = _prefDialog->getUserPreferencesData();
        _newData.setRecentProjectsList(_userPreferencesData->getRecentProjectsList());
        _userPreferencesData->updateData(_newData);
        if (_userPreferencesData->getTemplateConfigPath().isEmpty())
            _solverManager->resetPersonalizedMenu();
    }
}
/**
 * @brief
 *
 * @param _isMultizoneProject
 * @param _multizoneProjectData
 * @param _projectData
 * @param _projectDetailsData
 * @return bool
 */
bool EditHelper::launchSettingsDialog(bool _isMultizoneProject,MultizoneProjectData * _multizoneProjectData, ProjectData * _projectData,
                          ProjectDetails * _projectDetailsData)
{
    ProjectSettingsDialog _settingsDialog;
    // Loading the data
    if (!_isMultizoneProject)
        _settingsDialog.setProjectData(*_projectData);
    else
        _settingsDialog.setMultizoneProjectData(*_multizoneProjectData);
    _settingsDialog.setProjectDetailsData(*_projectDetailsData);
    // Executing
    _settingsDialog.exec();
    bool _changesDone = _settingsDialog.userAcceptChanges();;
    if (_changesDone)
    {
        if (_isMultizoneProject)
            _multizoneProjectData->updateMultizoneProjectData(_settingsDialog.getMultizoneProjectData());
        else
            _projectData->updateProjectData(_settingsDialog.getProjectData());
        _projectDetailsData->updateProjectDetails(_settingsDialog.getProjectDetailsData());
    }

    return _changesDone;
}
