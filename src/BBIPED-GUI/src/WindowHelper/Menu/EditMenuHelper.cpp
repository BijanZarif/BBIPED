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
 *
 *  *  Author: Dec 2016: Carmen Alonso Montes calonso@bcamath.org

***/
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "WindowHelper/Menu/MenuHelpers/edithelper.h"
#include "Defines/General/GeneralDefines.h"
#include "Preferences/preferencesmanager.h"
/**
 * @brief This funciton controls the project settings action
 *
 */
void MainWindow::on_actionProject_Settings_triggered()
{
    launchActionProjectSettingsDialog();
}

/**
 * @brief This function controls the launching of the Project Settings Dialog
 *
 */
void MainWindow::launchActionProjectSettingsDialog()
{
    EditHelper _editHelper;
    bool _userMadeChanges = _editHelper.launchSettingsDialog(_isMultizoneProject,&_multizoneProjectData,&_projectData,&_projectDetailsData);
    if (_userMadeChanges)
    {
        bool _needToRemoveMaps = false;
        if (!_isMultizoneProject)
        {
           // _projectData =  _settingsDialog.getProjectData();
            if (_projectData.getSolverDataInformation().isConfigFileEmpty())
            {
               _inputDataTabHelper.updateConfigurationFilePathInActionGroup("");
               ui->meshFileLineLabel->setText("");
               _userConfigValuesSolver.remove("MESH_FILENAME");

             // If we remove the configuration file we MUST remove the associated mesh file and their values
               if (!_projectData.getSolverDataInformation().isMeshFileEmpty())
                   _projectData.getSolverDataInformation().clear();
              _needToRemoveMaps = true;
            }
        }
        else
        {
           // Refresh the Multizone Project Data
           _mManager.refreshSimulationFilesView(&_multizoneProjectData);
           QString _openProject;
           if (!_projectData.isEmpty())
               _openProject = _projectData.getSolverDataInformation().getSolverConfigFilePath();
            QStringList _listConfigFiles = _multizoneProjectData.getListConfigFiles();
            // We must remove all the data only in the case that the opened project has been removed by the user
            if (!_listConfigFiles.contains(_openProject))
            {
                _needToRemoveMaps = true;
                // Restore the view names
                _mManager.updateConfigFileNameinTabView("");
                _mManager.updateMeshFileNameinTabView("");

            }

        }
        if (_needToRemoveMaps)
        {
            _userConfigValuesSolver.clear();
            resetBoundaryValues();
            /*_userBoundaryValuesSolver.clear();
            _boundaryMeshValues.clear();*/
            // Disable the elements of the menu
            ui->basivViewEditButton->setEnabled(false);
            //ui->meshRefreshButton->setEnabled(false);
            ui->setMeshPropertiesButton->setEnabled(false);
            ui->savePhysicsButton->setEnabled(false);
            ui->seeSummaryChangesButton->setEnabled(false);
        }

        // Enabling the values
        setEnableSaveProjectIcons(true);
        _status_NeedToBeSaved = true;
    }
}

/**
 * @brief This method controls the Preference dialog launch
 *
 */
void MainWindow::on_actionPreferences_triggered()
{
    EditHelper _editHelper;
    _editHelper.launchPreferencesDialog(&_userPreferencesData, &_solverManager);

}

/**
 * @brief This function returns which type of Solver Engine is defined
 *
 * @return int
 */
int MainWindow::getCurrentUserPreferences()
{
    if (_userPreferencesData.isDefaultSolverEngineSelected())
        return DefaultSolverOption;
    if (_userPreferencesData.isVirtualSolverEngineSelected())
        return VirtualSolverOption;
    if (_userPreferencesData.isMultizoneSolverEngineSelected())
        return MultizoneSolverOption;

    return -1;
}
/**
 * @brief This function initialize the variables associated with user preferences
 *
 */
void MainWindow::initializeUserPreferences()
{
    PreferencesManager _prefManager;
    _userPreferencesData = _prefManager.getUserPreferences();
    // Updating the Recent Files Area
    updateRecentProjectsSection();

}


