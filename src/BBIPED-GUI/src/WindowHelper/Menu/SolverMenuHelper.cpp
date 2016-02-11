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

#include "Data/Solver/VO/solvervo.h"
#include "WindowHelper/Menu/MenuHelpers/solverhelper.h"
#include "Defines/General/GeneralDefines.h"
/**
 * @brief Import Mesh file
 *
 */
void MainWindow::on_actionImport_Mesh_File_triggered()
{
    importMeshFile();
}
/**
 * @brief Run Solver Project
 *
 */
void MainWindow::on_actionRun_Project_triggered()
{
    if (!_isMultizoneProject)
    {
        if (_userConfigValuesSolver.isEmpty())
            return;
        // Checking if RESTART_SOL is enabled
       if ( (_userConfigValuesSolver.contains("RESTART_SOL")) &&
            (QString::compare(_userConfigValuesSolver.value("RESTART_SOL"),"NO") != StrEqual) )
        {
                _userConfigValuesSolver.insert("RESTART_SOL","NO");
                _status_NeedToBeSaved = true;
        }
    }
    runSimulation();
}


/**
 * @brief Stop current simulation
 *
 */
void MainWindow::on_actionStop_Execution_triggered()
{
    stopSolverProcess();
}

/**
 * @brief SOLVER: Main functionality to save the changes of the configuration file
 *
 */
void MainWindow::on_actionSave_Configuration_File_triggered()
{
    bool _savedFile = saveConfigFile();
    if (_savedFile)
    {
        enableSolverRunAction();
        setEnableSaveProjectIcons(_status_NeedToBeSaved);
        setEnabledSaveConfigIcons(_status_NeedToBeSaved);
        ui->savePhysicsButton->setEnabled(_status_NeedToBeSaved);
    }

}

/**
 * @brief SOLVER: Main functionality to save the changes of the configuration file with a different name
 *
 */
void MainWindow::on_actionSave_Configuration_File_As_triggered()
{
    QString filename = QFileDialog::getSaveFileName(
                this,
                tr("Save Solver Configuration File As"),
                     QDir(_projectData.getProjectPath()).filePath(TOOL4),
                tr("Configuration File name (*.cfg)") );

    QString _fileName = QFileInfo(filename).fileName();
    QString _fileExt = QFileInfo(filename).suffix();
    if (_fileExt.isEmpty())
        _fileName = _fileName +".cfg";
    QString _newFileConfigPath = QDir (QDir(_projectData.getProjectPath()).filePath(TOOL4)).filePath(_fileName);

    SolverVO _solver = _projectData.getSolverDataInformation();
    _solver.setSolverConfigFilePath(_newFileConfigPath);
    _projectData.setSolverDataInformation(_solver);

    if (_isMultizoneProject)
        _mManager.updateOpenEditConfigFile(_newFileConfigPath, &_multizoneProjectData,&_projectData, _isNewConfigFile);

   // Saving the Configuration File
   bool _savedFile = saveConfigFile();

   if (_savedFile)
   {       
        removeDefaultValuesReadFromConfigFile();
        setEnableSaveProjectIcons(_status_NeedToBeSaved);
        setEnabledSaveConfigIcons(_status_NeedToBeSaved);
        ui->savePhysicsButton->setEnabled(_status_NeedToBeSaved);
        enableSolverRunAction();
        if (!_isMultizoneProject)
            _inputDataTabHelper.fillFileNamesInTab(_projectData, &_userConfigValuesSolver);
   }
}

/**
 * @brief this funciton controls the compare Configuration file action
 *
 */
void MainWindow::on_actionCompare_Configuration_Files_triggered()
{
    SolverHelper _solverHelper;
    _solverHelper.launchFileComparisonDialog(_isMultizoneProject,_multizoneProjectData, _projectData,
                                                       _solverManager, _projectDetailsData,
                                                      _userVirtualMRFDataList);
}
/**
 * @brief This function controls the synchronise file action
 *
 */
void MainWindow::on_actionSynchronise_all_files_triggered()
{
    SolverHelper _solverHelper;
    _solverHelper.launchFileComparisonDialog(_isMultizoneProject,_multizoneProjectData, _projectData,
                                                       _solverManager, _projectDetailsData,
                                                      _userVirtualMRFDataList);
}
/*********************************************************************************/
/**
 * @brief This function allows to save the residual image generated in a simulation
 *
 */
void MainWindow::on_actionSave_Residual_Image_triggered()
{
    plotResiduals->exportPlot();
}
/**************************************** TEMPLATE *****************************************************/
/**
 * @brief This function is for creating customized templates for the configuration menus
 *
 */
void MainWindow::on_actionCreate_Configuration_Template_triggered()
{
    SolverHelper _solverHelper;
    _solverHelper.createConfigurationTemplateByUser(&_solverManager);
}


/**
 * @brief This function allows to edit existing configuration templates
 *
 */
void MainWindow::on_actionEdit_Configuration_Template_triggered()
{

    SolverHelper _solverHelper;
    _solverHelper.editConfigurationTemplateByUser(this,&_solverManager);
}

/**
 * @brief This function allows to load specific configuration template
 *
 */
void MainWindow::on_actionLoad_Configuration_Template_triggered()
{

    SolverHelper _solverHelper;
    _solverHelper.loadConfigurationTemplateByUser(this,&_solverManager,&_userPreferencesData);

}
