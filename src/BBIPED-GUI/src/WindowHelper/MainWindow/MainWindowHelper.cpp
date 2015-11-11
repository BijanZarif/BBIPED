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
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "Defines/General/GeneralDefines.h"
//#include <QSyntaxHighlighter>

/**
 * @brief Enable correspondent Buttons and disable the one that is clicked
 *
 * @param _buttonName
 */

void MainWindow::enableToolButtons(int _buttonName)
{
    enableAllMainButtons();
    switch(_buttonName)
    {
        case (Tool1Id):
            ui->salomeButton->setEnabled(false);
            _inputDataTabHelper.hideAllSolverMenus();
            break;
        case(Tool3Id):
            ui->paraviewButton->setEnabled(false);
            _inputDataTabHelper.hideAllSolverMenus();
            break;
        case (Tool4Id):
            ui->solverButton->setEnabled(false);
            break;
    }


}

/************************************ TREE VIEW ************************************/

/**
 * @brief This function allow to open in a tab a configuration file or any text file with supported
 *          extensions
 *
 * @param _index
 */
void MainWindow::on_projectTreeView_doubleClicked(const QModelIndex &_index)
{

  QFileSystemModel * _model = (QFileSystemModel *) ui->projectTreeView->model();
  QFileInfo _file = _model->fileInfo(_index);

  if( (_file.exists()) && (!_file.isDir()) &&  (_file.isFile()) )
          if ( ((QString::compare(_file.suffix(),SOLVER_PROJECT_FILE_EXT) == StrEqual))
               ||((QString::compare(_file.suffix(),MULTIZONE_PROJECT_FILE_EXT) == StrEqual)) )
      launchActionProjectSettingsDialog();
   else
   {
          _projectTextEditor = new QTextEdit(this);
         // ProjectMenuTreeHelper _treeHelper;
          _treeHelper.openFileInTabFromClickedElement(_file,_projectTextEditor,ui->solverTabwidget);
          //QSyntaxHighlighter * _highlighter = new QSyntaxHighlighter(_projectTextEditor->document());
    }


}

/************* TOOL BUTTONS ******************************/
/************************************ TOOLS BUTTON ACTIONS */
/**
 * @brief Action when the Solver Button is clicked
 *
 */
void MainWindow::on_solverButton_clicked()
{
    enableToolButtons(Tool4Id);
    setEnableMenuSolver(true);

    _inputDataTabHelper.showAllSolverMenus(_projectData);
    if (!_solverManager.isInitialized())
        _solverManager.initializeManager();
    if (!_isMultizoneProject)
        _inputDataTabHelper.fillFileNamesInTab(_projectData, &_userConfigValuesSolver);

    enableMeshOptions();
    //setEnabledRunIcon(true);
    ui->actionSave_Residual_Image->setEnabled(true);
    if (!_isMultizoneProject)
    {
        ui->solverTabwidget->setTabEnabled(0,true);
        ui->solverTabwidget->setTabEnabled(1,false);
    }
}

/**
 * @brief This function launchs the Paraview tool from the button click
 *
 */
void MainWindow::on_paraviewButton_clicked()
{
    seeParaviewResults();
}



/**
 * @brief This function launchs the Salome tool
 *
 */
void MainWindow::on_salomeButton_clicked()
{
    // Check if Salome is set up


    launchSalomeMeshTool();
}
