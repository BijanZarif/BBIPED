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

#include "Help/helpmanager.h"
#include "Forms/aboutusdialog.h"

/**
 * @brief This fucntion initalize all the data of the Tab helper, like button registration and so on
 *
 */
void MainWindow::initializeHelper()
{
    _inputDataTabHelper.registerButtons(ui->basivViewEditButton,
                         ui->seeSummaryChangesButton,  ui->runSimulationButton,
                         ui->restartButton);
    _inputDataTabHelper.registerLineEdits(ui->configFileLineLabel,ui->meshFileLineLabel);
    _inputDataTabHelper.registerTab(ui->solverTabwidget);

    // Initialize TreeViewHelper
    _treeHelper.registerMenuTree(ui->projectTreeView);
}
/*************************** HELP SECTION *********************************/
/**
 * @brief This function launchs the Qt Assistant for showing the help of BBIPED
 *
 */
void MainWindow::on_actionHelp_Contents_triggered()
{
    HelpManager _helpManager;
    _helpManager.openQtAssistant();
}

/**
 * @brief This action shows the html documentation of the tool
 *
 */
/*void MainWindow::on_actionDeveloper_Guide_triggered()
{
    HelpManager _helpManager;
    _helpManager.openHelp("index.html","html");
}*/

/**
 * @brief This action shows the PDF documentation of the tool
 *
 */
void MainWindow::on_actionDeveloper_Guide_2_triggered()
{
    HelpManager _helpManager;
    _helpManager.openHelp("developerGuide.pdf","pdf");

}

/**
 * @brief This action shows the PDF user guide documentation
 *
 */
void MainWindow::on_actionUser_GuideLine_2_triggered()
{
    HelpManager _helpManager;
    _helpManager.openHelp("userGuideline.pdf","pdf");
}

/**
 * @brief This action shows the html help
 *
 */
void MainWindow::on_actionOnline_Help_triggered()
{
    HelpManager _helpManager;
    _helpManager.openHelp("index.html","html");
}

/**
 * @brief About us
 *
 */
void MainWindow::on_actionAbout_triggered()
{
    AboutUsDialog _aboutDialog;
    _aboutDialog.exec();
}


