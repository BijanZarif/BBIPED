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

#include "WindowHelper/Menu/MenuHelpers/toolshelper.h"
/******************************************** SPECIAL TOOLS *************************************************************/

/**
 * @brief This function is to create automatic mesh using Salome
 *
 */
void MainWindow::on_actionLoad_Templates_triggered()
{

    ToolsHelper _meshGenerationHelper;
    _meshGenerationHelper.createAutomaticMesh(ui->textEdit,_projectData, _userPreferencesData);

}

/**
 * @brief This function is to create automatic mesh using Salome but with optimization parameters
 *
 */
void MainWindow::on_actionAutomatic_Optimization_triggered()
{
    ToolsHelper _meshGenerationHelper;
    _meshGenerationHelper.createAutomaticMeshWithOptimization( _projectData, _userPreferencesData);

}
