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
#include "Forms/WarningDialog/meshingtoolconfigurationwarningdialog.h"
/**
 * @brief This action is the Menu action for lauching Salome
 *
 */
void MainWindow::on_actionSalome_Mesh_triggered()
{
    launchSalomeMeshTool();
}

/** @brief This internal function controls how to launch the Salome tool, specially due to the path of this tool (not in
* traditional Linux paths like usr/bin). So, with this function we locate the path to Salome to run it
*
*/
void MainWindow::launchSalomeMeshTool()
{
   // Step 1. Set up the working directory in the project

   QString _workingDir = QDir(_projectData.getProjectPath()).filePath(TOOL4);
   _salomeLocationProccess->setWorkingDirectory(_workingDir);

   // Esto no funciona --> Revisar ...
   QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
   env.insert("WORKINGDIR", _workingDir); // Add an environment variable
   _salomeLocationProccess->setProcessEnvironment(env);

   // Step 2. Launch Mesh Tool
 //  QString _command = "export WORKINGDIR=\"" +_workingDir +"\"; echo $WORKINGDIR";//+
   QString _command = _userPreferencesData.getMeshViewerPath();
   // By default for Salome tools
   if ( (_command.isNull()) || (_command.isEmpty()) )
   {
       // There is no configuration for the Salome Mesher. The user shall provide the location of the executable
        MeshingToolConfigurationWarningDialog _meshingWarning;
        _meshingWarning.initializeStructures(&_userPreferencesData,&_solverManager);
        _meshingWarning.exec();
        return;
   }


    QProcess * _runSalome = new QProcess(this) ;
    connect(_runSalome, SIGNAL(error(QProcess::ProcessError)),
             this, SLOT(slotProcessError(QProcess::ProcessError)));
    _runSalome->start(_command);

}
/**
 * @brief This function is the one capable of locating the Salome path in the client side
 *
 */
void MainWindow::getSalomePathSlot()
{
    QString _pathSalomeBin = _salomeLocationProccess->readAllStandardOutput();
    _userPreferencesData.setMeshViewerPath(_pathSalomeBin);
    QProcess * _runSalome = new QProcess(this) ;

    connect(_runSalome, SIGNAL(error(QProcess::ProcessError)),
            this, SLOT(slotProcessError(QProcess::ProcessError)));

    _runSalome->start(_pathSalomeBin);
}

/**
 * @brief  Launching the paraview application
 *
 */
void MainWindow::on_actionParaview_File_triggered()
{
    seeParaviewResults();
}

/**
 * @brief This internal function controls if the Paraview is opened passing a file or to open a new
 * instance of Paraview
 *
 */
void MainWindow::seeParaviewResults()
{
    QString _currentPath;
    if (_projectData.getProjectPath().isEmpty())
        _currentPath = QCoreApplication::applicationDirPath();//QDir::currentPath();
    else
        _currentPath = QDir(_projectData.getProjectPath()).filePath(TOOL4);

    QString _viewFile = QFileDialog::getOpenFileName(this, tr("Select the file to see with Paraview"),
                                                            _currentPath,
                                                            tr("Paraview Files (*.vtk)"));

    if (!_viewFile.isEmpty())
    {
        QString _postProcessorPath = _userPreferencesData.getPostProcessorViewerPath();
        QString _command;
        if ( (!_postProcessorPath.isNull()) && (!_postProcessorPath.isEmpty()) )
            _command = _postProcessorPath;
        else
            _command = "paraview";

        QProcess * _viewProcess = new QProcess(this);
        _command = _command + " --data="+_viewFile;
        connect(_viewProcess, SIGNAL(error(QProcess::ProcessError)),
                this, SLOT(slotProcessError(QProcess::ProcessError)));
        _viewProcess->start(_command);
    }
}
