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


#include "Utility/showmessagehelper.h"
#include "Utility/utilfiles.h"
#include "Forms/WarningDialog/warningclosingdialog.h"
#include "Preferences/preferencesmanager.h"
#include "Defines/General/GeneralDefines.h"
#include <QAction>
#include "Data/Solver/MeshInfo/meshinfofilereader.h"

/**
 * @brief Create a New Project
 *
 */
void MainWindow::on_actionNew_Project_triggered()
{
    bool _fileCreated = false;
    ShowMessageHelper _showHelper;

    closeExistentProject();

    while (!_fileCreated)
    {
        // Creating the dialog for asking the user for the project name
        QString projectPath = QFileDialog::getSaveFileName(this, tr("Create Project File"),QCoreApplication::applicationDirPath(),
                                   //QDir::currentPath(),
                                   tr("Solver Project (*.prj)"));


        // Conditions: (1) Check that this project does not exist --> Otherwise warning
        //             (2) If everything is ok --> Create project folder structure
        //                  projectName --> Folder
        //                  projectName.prj --> Project File Name
        //                  Other folders --> Tool1, Tool2, Tool3, Solver

        if (projectPath.isNull())
        {
            // Button Cancel is pressed, so whe go out from this function
            return;
        }


        UtilFiles _utilFiles;

        // Creating all the project structure
        _fileCreated = _utilFiles.createProjectStructure(projectPath);
        if (!_fileCreated)
        {

            QString _title(tr("Error Creating the new project") );
            QString _message(tr("If this project already exists. Please, select a different name. Otherwise, check your writing rights in the folder"));
            _showHelper.showMessageWindow(_title,_message,this);
        }
        else
        {
            QFileInfo _qFileInfo( _utilFiles.getFinalPath());
            this->_projectData.saveProjectInformation(_utilFiles.getFinalPath());
            _showHelper.showMessage("Project:"+ _utilFiles.getFinalPath(),statusBar());
            // Updating the values in the GUI
            updateMainWindowElements(_qFileInfo.absolutePath(),true,_utilFiles.getFinalPath());
        }

        // We need to create a New Configuration File
        createNewConfigurationFile();

    }

}


/**
 * @brief Open a Project
 *
 */
void MainWindow::on_actionOpen_Project_triggered()
{
    closeExistentProject();
    QString _projectFilePath = QFileDialog::getOpenFileName(this, tr("Open Project File"),QCoreApplication::applicationDirPath(),
                                                            tr("Solver Project (*.prj)"));
    if (_projectFilePath.isNull())
            return;

    openProject(_projectFilePath);

}

void MainWindow::loadMesh3DProperties()
{
   if(_projectData.getSolverDataInformation().isMeshFileEmpty())
       return;
   QString _meshFileName = _projectData.getSolverDataInformation().getSolverFilePath();

   if ((!_meshFileName.isEmpty()) || (!_meshFileName.isNull()))
   {
        QString _meshConfigFileName = QString(QFileInfo(_meshFileName).completeBaseName());
        QString _completeMeshConfigFileName = QDir(QDir(_projectData.getProjectPath()).filePath(TOOL4)).filePath(_meshConfigFileName+"."+MESH_CONFIG_SUFFIX);

        importMeshInfoFile(_completeMeshConfigFileName);
        _meshData.setMeshFileName(_meshFileName); // We update the correspondent su2 filename
        return;
   }

}

/**
 * @brief Save a Project
 *
 */
void MainWindow::on_actionSave_triggered()
{
    // Note: In this case, we are going to save only the associated project
    // documents of each tool.

    if((_projectExists) && (_status_NeedToBeSaved))
    {
        if ((!_isMultizoneProject))
        {
         bool _savedFile = saveConfigFile();
         if (_savedFile)
            setEnableSaveProjectIcons(false);
        }else
        {
            if (!_projectData.isEmpty())
            {
                bool _savedFile = saveConfigFile();
                if (_savedFile)
                   setEnableSaveProjectIcons(false);
            }else{
                updateProjectDataToFile();
                _status_NeedToBeSaved = false;
            }
        }

    }

}
/**
 * @brief SAVE AS a Project
 *
 */
void MainWindow::on_actionSave_As_triggered()
{
    if (_projectExists)
    {
        QString filename;
        if (!_isMultizoneProject)
            filename = QFileDialog::getSaveFileName( this, tr("Save Project As"),QCoreApplication::applicationDirPath(),// QDir::currentPath(),
                               tr("Project Document name (*.prj)") );
        else
            filename = QFileDialog::getSaveFileName( this, tr("Save Multizone Project As"),QCoreApplication::applicationDirPath(),// QDir::currentPath(),
                               tr("Project Document name (*.mprj)") );

        if( filename.isNull() )
         return;

        UtilFiles _utilFiles;
        bool _projectCreated;
        if (!_isMultizoneProject)
            _projectCreated =  _utilFiles.createProjectStructure(filename);
        else
           _projectCreated =  _utilFiles.createMultizoneProjectStructure(filename);

        // Copy all the files from origin to new location and update in accordance
        if (_projectCreated)
        {
            QFileInfo _qFileInfo(_utilFiles.getFinalPath());
            if (!_isMultizoneProject)
            {
                ProjectData _newProjectData;
                _newProjectData.saveProjectInformation(_utilFiles.getFinalPath());

                _utilFiles.copyAllToolData(this->_projectData,_newProjectData);
                _projectData = _utilFiles.updateToolPaths(_projectData,_newProjectData);
                _utilFiles.updateProjectDataToFile(_projectData, _projectDetailsData);

            }else{
                // TO BE REVIEWED
                MultizoneProjectData _newMultizoneProjectData ;
                _newMultizoneProjectData.setProjectPath(_qFileInfo.path());
                _newMultizoneProjectData.setProjectName(_qFileInfo.baseName());

                _utilFiles.copyAllToolData(_multizoneProjectData,_newMultizoneProjectData);
               _multizoneProjectData = _utilFiles.updateMultizoneToolPaths(_multizoneProjectData,_newMultizoneProjectData);
                _utilFiles.updateMultizoneProjectDataToFile(_newMultizoneProjectData,_projectDetailsData);
            }
            _status_NeedToBeSaved = false;
            setEnableSaveProjectIcons(false);
            // Updating the Status Bar
            ShowMessageHelper _showHelper;
           _showHelper.showMessage("Project:"+ _utilFiles.getFinalPath(),statusBar());
           // Fill the Project tree view
          _treeHelper.fillProjectTreeView(_qFileInfo.absolutePath());//,ui->projectTreeView);
        }
    }
}


/**
 * @brief SAVE TEMPORAL --> TODO
 *
 */
void MainWindow::on_actionSave_Temporal_triggered()
{
    ShowMessageHelper _showHelper;
    if (_projectExists)
    {
        QString filename = QFileDialog::getSaveFileName( this, tr("Save Project in a Temporal"), QCoreApplication::applicationDirPath(),//QDir::currentPath(),
               tr("Temporal Project (*.tmp.prj)") );
           if( !filename.isNull() )
             _showHelper.showMessageWindow("Temporal Saving","Pending Implementation",this);
    }
     _showHelper.showMessageWindow("Temporal Saving","Pending Implementation",this);
}
/**
 * @brief CLOSE a Project
 *
 */
void MainWindow::on_actionClose_Project_triggered()
{
    if (_projectExists)
    {
        bool _needToClose = true;
        if (_userPreferencesData.showCloseProjectWarning())
        {
           WarningClosingDialog _closingDialog;
           _closingDialog.setTitleMessage("You are about to close the current Project");
           _closingDialog.exec();
           _needToClose = _closingDialog.needsToCloseApplication();
           _userPreferencesData.setShowCloseProjectWarning(_closingDialog.getShowAgainValue());
        }
        if (_needToClose)
        {
           closeExistentProject();
           setEnableSaveProjectIcons(false);
           setEnableCloseProjectIcons(false);
           ui->solverTabwidget->setCurrentIndex(0);
           if (ui->solverTabwidget->count()>3)
               for (int i = ui->solverTabwidget->count();i>=3; i--)
                   ui->solverTabwidget->removeTab(i);

           ui->menuSalome_Tools->setEnabled(false);
           setEnableMenuSolver(false);
        }
    }

}


/**
 * @brief Function that controls the exit of the application
 *
 */
void MainWindow::on_actionExit_triggered()
{
    exitApplication();
}
/**
 * @brief This function controls the exit of the Application, to close everything and to save the data to the user preferences file if needed
 *
 * @return bool
 */
bool MainWindow::exitApplication()
{
   bool _closeApplication = true;

   if (_userPreferencesData.showCloseWarning())
   {
       WarningClosingDialog _closingDialog;
       _closingDialog.exec();
       _closeApplication = _closingDialog.needsToCloseApplication();
       _userPreferencesData.setShowCloseWarning(_closingDialog.getShowAgainValue());
   }

   if (_closeApplication)
   {
       closeExistentProject();
        close();
   }

   PreferencesManager _prefManager;
   _prefManager.saveUserPreferences(_userPreferencesData);

   return _closeApplication;
}
/******************************************** RECENT PROJECTS *********************/
/**
 * @brief This function update the project list of the "recent project list" of the menu
 *
 */
void MainWindow::updateRecentProjectsSection()
{
    if (!_userPreferencesData.getRecentProjectsList().isEmpty())
    {
       if (ui->action_Empty->isVisible())
            ui->action_Empty->setVisible(false);
       createProjectActions(_userPreferencesData.getRecentProjectsList());
    }

}

/**
 * @brief This function add a new project to the "recent project list"
 *
 * @param _projectPathName
 */
void MainWindow::addProjectToRecentProjectsSection(QString _projectPathName)
{
    if (_projectPathName.isEmpty())
        return;
    if (ui->action_Empty->isVisible())
        ui->action_Empty->setVisible(false);

    // Update According
    bool _projectExist = false;
    int _pos = -1;
    for (int i = 0; (i < _recentProjectsList.size()) && (!_projectExist);i++)
    {
        QString _projectActionName = _recentProjectsList.at(i)->data().toString();
        if (QString::compare(_projectActionName,_projectPathName) == StrEqual)
        {
            _projectExist = true;
            _pos = i;
        }
    }

    QAction * _newRecentProjectAction = createRecentProjectAction(_projectPathName);

    if (!_projectExist)
    {
        if (_recentProjectsList.size() >= 10)
        {
            ui->menuRecent_Projects->removeAction(_recentProjectsList.at(0));
            _recentProjectsList.removeFirst();
        }
    }else
    {
        if (_pos != -1)
        {
            ui->menuRecent_Projects->removeAction(_recentProjectsList.at(_pos));
            _recentProjectsList.removeAt(_pos);
        }
    }

    _recentProjectsList.append(_newRecentProjectAction);
    ui->menuRecent_Projects->addAction(_newRecentProjectAction);
}

/**
 * @brief This function creates a new recent project action per each element of the list
 *
 * @param _list
 */
void MainWindow::createProjectActions(QStringList _list)
{
    for (int i = 0; i < _list.size();i++)
    {
        QAction * _newRecentProjectAction = createRecentProjectAction(_list.at(i));
        _recentProjectsList.append(_newRecentProjectAction);
        ui->menuRecent_Projects->addAction(_newRecentProjectAction);
    }

}

/**
 * @brief This function creates the QAction for the "recent project list"
 *
 * @param _name
 * @return QAction
 */
QAction *  MainWindow::createRecentProjectAction(QString _name)
{
    QAction * _newRecentProjectAction = new QAction(this);
    _newRecentProjectAction->setText(_name );
    _newRecentProjectAction->setData(_name);
    _newRecentProjectAction->setVisible(true);
    connect(_newRecentProjectAction, SIGNAL(triggered()),this, SLOT(openRecentFile()));
    return _newRecentProjectAction;
}

/**
 * @brief This action opens a project selected in the recent project list
 *
 */
void MainWindow::openRecentFile()
{
    closeExistentProject();

    QString _file;
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
        _file = action->data().toString();
    if (_file.isEmpty())
        return;
    int _pos = _userPreferencesData.getRecentProjectsList().indexOf(_file);
    if (_pos == -1 )
        return;

    openProject(_file);

}

/**
 * @brief This function controls the opening of a project
 *
 * @param _projectFilePath
 */
void MainWindow::openProject(QString _projectFilePath)
{
    UtilFiles _utilFiles;

      // Update Project Data
    ProjectData _openedProject = _utilFiles.openProjectStructure(_projectFilePath);

    // Update Paths, if necessary
     checkProjectBasePathConsistency(&_openedProject,_projectFilePath);

    _projectDetailsData = _utilFiles.openProjectDetails(_projectFilePath);


    // Update Status Bar
    QString _openedProjectPath = _openedProject.getProjectPath() + QDir::separator() + _openedProject.getProjectFileName();
    ShowMessageHelper _showHelper;
    _showHelper.showMessage("Open Project:"+ _openedProjectPath, statusBar());

     // Getting the associated info
    _projectData.copy(_openedProject);


     // Update Variables according the Solver
    bool _needToBeSaved = _inputDataTabHelper.updateSolverUserValues(_projectData,&_userConfigValuesSolver,
                                                                         &_boundaryMeshValues,&_boundaryConfigValues,
                                                                     &_userVirtualMRFDataList);
    if (_needToBeSaved)
        _status_NeedToBeSaved = _needToBeSaved;
    // Update configuration values removing those that corresponds with default values
     removeDefaultValuesReadFromConfigFile();

    _inputDataTabHelper.enableConfigurationViewMenu(_projectData);
    updateMainWindowElements(_openedProject.getProjectPath(),false,_openedProjectPath);
     if (_status_NeedToBeSaved)
     {
         setEnableSaveProjectIcons(true);
         ui->savePhysicsButton->setEnabled(true);
     }

     loadMesh3DProperties();
}
