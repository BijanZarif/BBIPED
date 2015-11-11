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
#include <stdio.h>
#include <iostream>


// Qt includes
#include <QFileDialog>
#include <QMessageBox>
#include <QString>
#include <QComboBox>
#include <QPixmap>
#include <QLabel>
#include <Qt/qhelpengine.h>
#include <Qt/qhelpcontentwidget.h>
#include <Qt/qhelpindexwidget.h>

// Own classes includes
#include "Utility/utilfiles.h"
#include "Solver/solverutil.h"
#include "Solver/solverconfigmanager.h"
#include "Solver/solverconfigwriter.h"
#include "WindowHelper/Dialog/SolverDialog/solvermarkermenuhelper.h"
#include "Parser/parserfiles.h"
#include "Parser/Solver/configurationfileparser.h"
#include "Parser/Solver/meshfileparser.h"
#include "Utility/showmessagehelper.h"
#include "Forms/SolverMenuDialog/filecomparisondialog.h"
#include "Forms/WarningDialog/warningclosingdialog.h"
#include "Data/solversectiondao.h"
#include "Preferences/preferencesmanager.h"

// GRAPHICAL Includes
//#include "myprocess.h"
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_symbol.h>
#include <qwt_legend.h>


// Namespaces
using namespace std;


// Defines
#include "Defines/General/GeneralDefines.h"
#include "Defines/Tool/Solver/SolverDefines.h"
#include "Forms/EditMenuDialog/preferencedialog.h"
#include "Forms/ProjectDialog/multizoneprojectdialog.h"
#include "Forms/EditMenuDialog/projectsettingsdialog.h"
#include "Forms/WarningDialog/warningmultizonedialog.h"
#include "Forms/aboutusdialog.h"
#include "Help/helpmanager.h"

#include "Forms/SolverMenuDialog/templatedialog.h"

#include "Forms/ToolsDialog/salometemplatedialog.h"
#include "Forms/ToolsDialog/salomeoptimizationdialog.h"

#include <QProcessEnvironment>
#include "Forms/WarningDialog/warningrestartdialog.h"

#include "Data/Solver/MeshInfo/meshinfofilereader.h"

//#include "MenuHelpers/toolshelper.h"
//#include "MenuHelpers/solverhelper.h"
//#include "MenuHelpers/edithelper.h"
//-----------------------------------------------------------------------------------
/**
 * @brief Constructor
 *
 * @param parent
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Initialize all Elements
    initializeHelper();
    initializeUserPreferences();
    initializeManagers();

    // Disable all elements
  _inputDataTabHelper.hideAllSolverMenus();
   disableAllMainButtons();
   initializeStructures();
   ui->actionSave_Temporal->setEnabled(false);
   ui->actionConfigurator->setEnabled(false);
   ui->actionResidual->setEnabled(false);

   // Set all the status
   setResidualPlot();
   setUpStatusBar();

   // set Boolean values
   _stopped = false;
   _is3DMesh = false;
  }

/**
 * @brief Initialize Solver manager, user Preferences data manager
 *
 */
void MainWindow::initializeManagers()
{
    // Initialize Solver Manager
    if (!_solverManager.isInitialized())
        _solverManager.initializeManager();
    if (!_userPreferencesData.getTemplateConfigPath().isEmpty())
        _solverManager.loadPersonalizedTemplate(_userPreferencesData.getTemplateConfigPath());
}



/**
 * @brief Destructor
 *
 */
MainWindow::~MainWindow()
{
    delete _projectTextEditor;
    delete _myprocess;
    delete _residualVO;
    delete _outputSolverParser;
    delete ui;

}

/************************************ MENU ACTIONS ************************************/

/************************************ FILE ACTIONS */


/**
 * @brief Disable the close project icons (at the moment only one)
 *
 * @param _val
 */
void MainWindow::setEnableCloseProjectIcons(bool _val)
{
    ui->actionClose_Project->setEnabled(_val);
}

/**
 * @brief Enable all the Save actions fo the project
 *
 * @param _val
 */
void MainWindow::setEnableSaveProjectIcons(bool _val)
{
    ui->actionSave->setEnabled(_val);
    ui->actionSave_As->setEnabled(_val);
}




/**
 * @brief This function does all teh actions required to close an existent project, reseting to default values, as well as
 * stopping any existent simulation run
 *
 */
void MainWindow::closeExistentProject()
{
    if (_projectExists)
    {
        stopSolverProcess();
        closeProject();
        resetSolverTabEnabledElements();
        _projectExists = false;
    }
}


/**
 * @brief This function checks whether the path of the open project and the current project path is equal, if not, it updates it.
 *      It is used when we opened a project that has been created in a different computer (similar to import Project)
 *
 * @param _openedProject
 * @param _currentFilePath
 */
void MainWindow::checkProjectBasePathConsistency(ProjectData * _openedProject,QString _currentFilePath)
{
    QString _projectPath = _openedProject->getProjectPath();
    QString _cRootPath = QFileInfo(QFileInfo(_currentFilePath).path()).path();
    if (_projectPath.isNull())
        return;
    QFileInfo _basePathInfo(_projectPath);

    QString _rootPath = _basePathInfo.path();
    QString _projectBaseName = _basePathInfo.fileName();
    if (QString::compare(_rootPath, _cRootPath) != StrEqual)
    {
        QString _updatedProjectPath = QDir(_cRootPath).filePath(_projectBaseName);
        _openedProject->setProjectPath(_updatedProjectPath);

        // Update the values of the solver files (config and mesh files)
        SolverVO _solverVO = _openedProject->getSolverDataInformation();
        if (!_solverVO.getSolverConfigFilePath().isEmpty())
        {
            QString _configFileName = QFileInfo(_solverVO.getSolverConfigFilePath()).fileName();
            QString _updatedConfigRootPath = QDir(QDir(_updatedProjectPath).filePath(TOOL4)).filePath(_configFileName);
            _solverVO.setSolverConfigFilePath(_updatedConfigRootPath);
        }
        if (!_solverVO.getSolverFilePath().isEmpty())
        {
            QString _meshFileName = QFileInfo(_solverVO.getSolverFilePath()).fileName();
            QString _updatedMeshRootPath = QDir(QDir(_updatedProjectPath).filePath(TOOL4)).filePath(_meshFileName);
            _solverVO.setSolverFilePath(_updatedMeshRootPath);
        }
        _openedProject->setSolverDataInformation(_solverVO);
        _status_NeedToBeSaved = true;
    }

}





/**
 * @brief This function update all the elements of the main window, restore the icons, etc
 *
 * @param _projectPath
 * @param _isNewProject
 * @param _finalPath
 */
void MainWindow::updateMainWindowElements(QString _projectPath, bool _isNewProject, QString _finalPath)
{
   // ProjectMenuTreeHelper _treeHelper;
    _treeHelper.fillProjectTreeView(_projectPath);//,ui->projectTreeView);
    enableAllMainButtons();
    _projectExists = true;
    if (_isNewProject)
        _status_NeedToBeSaved = true;

    setEnableCloseProjectIcons(true);
    setEnableSaveProjectIcons(true);
    _userPreferencesData.addProjectToList(_finalPath);
    addProjectToRecentProjectsSection(_finalPath);
    ui->menuSalome_Tools->setEnabled(true);
}





/************************************ SOLVER ACTIONS */


/**
 * @brief This function changes the disable/enable nature of the Physics button
 *
 * @param _val
 */
void MainWindow::setEnabledInputTabSaveButtons(bool _val)
{
    ui->savePhysicsButton->setEnabled(_val);
}


/**
 * @brief Establish the main contents of the status bar (logo, labels, etc)
 *
 */
void MainWindow::setUpStatusBar()
{
    QPixmap *pixmap = new QPixmap (":/logo/images/icon_8x16.png");
    QLabel *iconLbl = new QLabel;
    iconLbl->setPixmap(*pixmap);
    statusBar()->addPermanentWidget(iconLbl);
    delete pixmap;
}


/************************************ BUTTON ACTIONS ************************************/





/************************************ Solver Configuration Tab ACTIONS ***********/

/**
 * @brief Implementation to Close the tabs
 *
 * @param index
 */

void MainWindow::on_solverTabwidget_tabCloseRequested(int index)
{
    ShowMessageHelper _showHelper;
    if (index > 2)
        ui->solverTabwidget->removeTab(index);
    else
        _showHelper.showMessageWindow("CLosing Tabs", "These tabs cannot be closed",this);
}


/******* ACTIONS Area*/



/**
 * @brief Activate the save Congifuration elements and save config as from the menu
 *
 * @param _val
 */
void MainWindow::setEnabledSaveConfigIcons(bool _val)
{
    ui->actionSave_Configuration_File->setEnabled(_val);
   // ui->actionSave_Configuration_File_As->setEnabled(_val);
}
/**
 * @brief This function enables the GUI mesh button and actions
 *
 */
void MainWindow::enableMeshOptions()
{
    ui->actionImport_Mesh_File->setEnabled(true);
    if ( (!_projectData.isEmpty()) && (!_projectData.getSolverDataInformation().getSolverFilePath().isEmpty()) )
    {
        ui->setMeshPropertiesButton->setEnabled(true);
//        ui->meshRefreshButton->setEnabled(true);
    }
}

/**
 * @brief Activate the run elemenst (button and menu element9
 *
 * @param _val
 */
void MainWindow::setEnabledRunIcon(bool _val)
{
    ui->actionRun_Project->setEnabled(_val);
    ui->runSimulationButton->setEnabled(_val);
    if (!_isMultizoneProject)
        ui->restartButton->setEnabled(_val);
    ui->actionStop_Execution->setEnabled(!_val);
    ui->stopButton->setEnabled(!_val);
}



/******* Change Values config field Area*/



/**
 * @brief Save helper function to save the configuration file and all the associated info
 *
 */
bool MainWindow::saveConfigFile()
{
    QString _fileConfigPathName = _projectData.getSolverDataInformation().getSolverConfigFilePath();
    QFileInfo _fileInfo(_fileConfigPathName);
    QString _cConfigPath = _fileInfo.absolutePath();
    QString _projecSolvertPath = QDir(_projectData.getProjectPath()).filePath(TOOL4);
    bool _savedFile = true;

    // Updating the Tree View
    _solverManager.updateDOMTree(this->_projectData.getSolverDataInformation().getSolverConfigFileMapValues());
    // Updating the Mesh file Name
    if (!_projectData.getSolverDataInformation().getSolverFilePath().isEmpty())
        _userConfigValuesSolver.insert("MESH_FILENAME",QDir(_meshData.getMeshPath()).filePath(_meshData.getMeshFileName()));

    if ( (_projectExists) && ( (_fileConfigPathName.isEmpty()) || (_fileConfigPathName.isNull())
         || (QString::compare(_projecSolvertPath ,_cConfigPath) != StrEqual) ) )
    {
        QString filename = QFileDialog::getSaveFileName(this,tr("Save Solver Configuration File As"),
                                                        QDir(_projectData.getProjectPath()).filePath(TOOL4),
                                                        tr("Configuration File name (*.cfg)") );
        if (!filename.isEmpty())
        {
            _inputDataTabHelper.saveAsConfigFile(filename,&_projectData,&_solverManager,_userConfigValuesSolver, _projectDetailsData,
                                                 _userVirtualMRFDataList);
            updateProjectDataToFile();
            _status_NeedToBeSaved = false;
        }else
           _savedFile = false;

    }
    else
    {
        _solverManager.saveConfigFile(_fileConfigPathName,_userConfigValuesSolver, _projectDetailsData,_userVirtualMRFDataList);
        updateProjectDataToFile();
        _status_NeedToBeSaved = false;
    }
    return _savedFile;
}


/**
 * @brief SOLVER: Shows the user input for the configuration file
 *
 */
void MainWindow::on_seeSummaryChangesButton_clicked()
{

    _summaryDialog = new SummaryDialog(this);
    removeDefaultValuesReadFromConfigFile();
    _summaryDialog->setValuesToShow(&_userConfigValuesSolver);
    _summaryDialog->setSectionMenuList(_solverManager.getAllSectionMenuList());
    _summaryDialog->buildSummaryTable();
    _summaryDialog->exec();
    if (_summaryDialog->userHasMadeChanges())
    {
        _status_NeedToBeSaved = true;
        setEnabledSaveConfigIcons(_status_NeedToBeSaved);
        setEnableSaveProjectIcons(_status_NeedToBeSaved);
        ui->runSimulationButton->setEnabled(_status_NeedToBeSaved);

    }

}

/******* Current Files Area*/
/*** TOOL BUTTONS*/


/***  BUTTONS*/
/**
 * @brief SOLVER: If restart option is allowed, restart simulation
 *
 */
void MainWindow::on_restartButton_clicked()
{

    plotResiduals->clearCanvas();
    _residualVO->clearAllData();

    if (_userConfigValuesSolver.isEmpty())
        return;
    // Checking if RESTART_SOL is enabled
    // If not --> Warning Message to know if it is worthy to continue

    if ( (!_userConfigValuesSolver.contains("RESTART_SOL")) ||
        ( (_userConfigValuesSolver.contains("RESTART_SOL")) &&
         (QString::compare(_userConfigValuesSolver.value("RESTART_SOL"),"YES") != StrEqual) ) )
    {
        WarningRestartDialog * _warningRestartDialog = new WarningRestartDialog(this);
        _warningRestartDialog->exec();

        if (_warningRestartDialog->userAcceptToRestart())
        {
            _userConfigValuesSolver.insert("RESTART_SOL","YES");
            _status_NeedToBeSaved = true;
        }else
            return;

    }
    // Ask for the file
    QString _selectedRestartFile = QFileDialog::getOpenFileName(this, tr("Select the file to be used to restart the simulation."),
                                                                _projectData.getProjectPath(),
                                                            tr("All files(*)"));
    if (_selectedRestartFile.isNull())
            return;

    // Parse the file name
    QFileInfo _fileInfo(_selectedRestartFile);
    QStringList _noIteration = (_fileInfo.completeBaseName()).split("-");
    QString _restartValue("");
    // We need to get the number of iteration that it is in the shape of
    // solution-flow.dat-10.dat --> And we need the 10
    if (_noIteration.size() > 0)
        _restartValue =  _noIteration.at(_noIteration.size()-1);
     _userConfigValuesSolver.insert("RESTART_SOL",QString("YES").append(",").append(_restartValue));
     _status_NeedToBeSaved = true;
    // Running the simulation
     runSimulation();


}


/**
 * @brief SOLVER: Run the current simulation
 *
 */
void MainWindow::on_runSimulationButton_clicked()
{
    if (!_isMultizoneProject)
    {
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
 * @brief SOLVER: run the simulation
 *
 */
void MainWindow::runSimulation()
{
    clearConsoleText();
    setEnabledRunIcon(false);
    ui->solverTabwidget->setCurrentIndex(tabResidual);

    if (_status_NeedToBeSaved)
    {
        ShowMessageHelper _showHelper;
        bool _saveResults = _showHelper.showMessageForSaving();
        if (_saveResults)
        {
            bool _savedFile = true;
            if (_status_NeedToBeSaved)
                _savedFile = saveConfigFile();
            if (_savedFile)
                runSolverProject();
        }else
        {
           setEnabledRunIcon(true);
           ui->solverTabwidget->setCurrentIndex(tabInputData);
        }

    }
    else
        runSolverProject();


}

/************************************ CONSOLE SLOT ***********************************/


/**
 * @brief Slot that allows to capture the standard output and redirect to the console text entry
 *
 */
void MainWindow::outlog()
{
    QString _solverOutput =_myprocess->readAllStandardOutput() ;
    qDebug ( _myprocess->readAllStandardError());
   if (!_solverOutput.contains("Press any key to exit"))
    {
        if (!_outputSolverParser->getStartParsing())
            _outputSolverParser->checkWhenDataToPaintAvailable(_solverOutput);
        else
        {
            _outputSolverParser->parseOutput(_residualVO,_solverOutput);
            plotResiduals->showPoints(_residualVO);
        }
    }else
   {
       ui->textEdit->append(_solverOutput);
        stopSolverProcess();
    }

    ui->textEdit->append(_solverOutput);
}


//---------------------------------------------------------------------------------------
//                              PRIVATE FUNCTIONS
//---------------------------------------------------------------------------------------

/************************************ Initialize Functions ************************************/

/**
 * @brief Initialize Main Window structures
 *
 */
void MainWindow::initializeStructures()
{
    // Editor & QProcess
     _projectTextEditor = new QTextEdit();
     _myprocess = new QProcess(this);
     _salomeLocationProccess = new QProcess(this);
     _residualVO = new SolverResidualVO();
     _outputSolverParser = new OutputSolverParser();

     resetBooleanValues();

   // Connecting signals
      connectProcessSignalSlots();
     connect(_salomeLocationProccess, SIGNAL(readyReadStandardOutput()),this,SLOT(getSalomePathSlot()));
     connect(_salomeLocationProccess, SIGNAL(error(QProcess::ProcessError)),this, SLOT(slotProcessError(QProcess::ProcessError)));


}

/**
 * @brief  This function resets all the boolean variables of mainwindow
 *
 */
void MainWindow::resetBooleanValues()
{
    // Boolean Values
     _projectExists = false;
     _status_NeedToBeSaved = false;
     _isMultizoneProject = false;
     _isNewConfigFile = false;
}

/********** Functios for Dialogs */

/**
 * @brief Dialog for open a configuration file
 *
 */

bool MainWindow::openSolverConfigurationFileDialog()
{
    bool _successOpeningFile = false;
    QString _projectFilePath = QFileDialog::getOpenFileName(this, tr("Open Configuration File"),
                                                            QDir(_projectData.getProjectPath()).filePath(TOOL4),
                                                            tr("Configuration File (*.cfg)"));

    if (!_projectFilePath.isNull())
    {

        if (!_isMultizoneProject)
        {
            _inputDataTabHelper.parseFile(ParserSolverConfigFile,_projectFilePath,&_userConfigValuesSolver,
                                  &_boundaryMeshValues, &_userVirtualMRFDataList);

            SolverVO _oldSolverData = _projectData.getSolverDataInformation();
            _oldSolverData.setSolverConfigFilePath(_projectFilePath);
            _projectData.setSolverDataInformation(_oldSolverData);
            _inputDataTabHelper.updateConfigurationFilePathInActionGroup(_oldSolverData.getSolverConfigFilePath());
            if (!_projectData.getSolverDataInformation().getSolverFilePath().isEmpty())
               _userConfigValuesSolver.insert("MESH_FILENAME",QDir(_meshData.getMeshPath()).filePath(_meshData.getMeshFileName()));
          }else{
            // A Multizone project needs to build the projectData with the correspondent values
             _mManager.buildProjectDataForMultizoneProject(_projectFilePath,&_multizoneProjectData,&_projectData);
            _inputDataTabHelper.updateSolverUserValues(_projectData,&_userConfigValuesSolver,
                                                       &_boundaryMeshValues,&_boundaryConfigValues,
                                                       &_userVirtualMRFDataList);
            checkMeshNames();
            _mManager.updateOpenEditConfigFile(_projectFilePath,&_multizoneProjectData,&_projectData, _isNewConfigFile);

           }
         removeDefaultValuesReadFromConfigFile();
        _successOpeningFile = true;
    }
    return _successOpeningFile;
}


/**
 * @brief This function removes those variables loaded from the configuration file
 * which value corresponds to the default value
 *
 */
void MainWindow::removeDefaultValuesReadFromConfigFile()
{
    if (!_userConfigValuesSolver.isEmpty())
    {
        QHash<QString,QString> _cleanedConfigValues;
        QHashIterator<QString,QString> iter(_userConfigValuesSolver);
        SolverSectionDAO _dao;
        if (!_solverManager.isInitialized())
            _solverManager.isInitialized();
        QList<SolverSectionVO> _menuList = _solverManager.getAllSectionMenuList();
        while(iter.hasNext())
        {
            iter.next();
            if ( (!_dao.isDefaultValueByFileName(_menuList, iter.key(), iter.value()))
                || (iter.key().contains("BC_")) || (iter.key().contains("MARKER")) )
                _cleanedConfigValues.insert(iter.key(),iter.value());
        }
        _userConfigValuesSolver = _cleanedConfigValues;
    }
}

void MainWindow::importMeshInfoFile(QString _file)
{
    QString _meshFileInfoPath = QDir(QDir(_projectData.getProjectPath()).filePath(TOOL4)).filePath(
                QString(QFileInfo(_file).baseName() + "." + MESH_CONFIG_SUFFIX)  );
    MeshInfoFileReader _meshHelper;
    _meshHelper.readMeshInfo(_file,&_meshData);
    _is3DMesh = _meshData.is3DMesh();
    _boundaryMeshValues = _meshData.getBoundaryNameListMap();
    _meshData.setMeshDetailsFileName(_meshFileInfoPath);
    QString _meshPath = QDir(_meshData.getMeshPath()).filePath(_meshData.getMeshFileName());

    // Creating the link to the mesh
    SolverUtil _solverUtil;
    _solverUtil.uiImportSolverMeshFile(_meshPath,_projectData);
    _meshData.saveMeshInfoFile();

    checkMeshNames();
    //________________________________________________________________________________________________________________
        // Saving Mesh information
        _meshData.set3DMesh(_is3DMesh);
        _meshData.setBoundaryNameList(_boundaryMeshValues);
        _meshData.setMeshPath(QFileInfo(_file).absolutePath());
        _meshData.setMeshFileName(QFileInfo(_file).fileName());
        QString _meshfileName =  QFileInfo(_file).baseName() + "." + MESH_CONFIG_SUFFIX;
        QString _meshDetailsFileName = QDir(QDir(_projectData.getProjectPath()).filePath(SOLVER_DIR_NAME)).filePath(_meshfileName);
        _meshData.setMeshDetailsFileName(_meshDetailsFileName);
        _meshData.saveMeshInfoFile();
        //________________________________________________________________________________________________________________
    updateMeshView(_projectData.getSolverDataInformation().getSolverFilePath());
}

/**
 * @brief Dialog for importing a mesh file
 *
 */
void MainWindow::importMeshFile(QString _meshConfigFile)
{
    SolverUtil _solverUtil;
    SolverVO _resultSolver = _solverUtil.uiImportSolverMeshFile(_meshConfigFile,_projectData);
    SolverVO _storedSolverData = _projectData.getSolverDataInformation();
    _storedSolverData.setSolverFilePath(_resultSolver.getSolverFilePath());
    this->_projectData.setSolverDataInformation(_storedSolverData);

    // Loading the Mesh Values
    _inputDataTabHelper.parseFile(ParserSolverMeshFile,_resultSolver.getSolverFilePath(),&_userConfigValuesSolver,
              &_boundaryMeshValues,&_userVirtualMRFDataList);
    if (_boundaryMeshValues.contains(IS_3D_MESH_TAG))
    {
        _is3DMesh = true;
        _boundaryMeshValues.remove(IS_3D_MESH_TAG);
    }

    checkMeshNames();
    //________________________________________________________________________________________________________________
    // Saving Mesh information
    _meshData.set3DMesh(_is3DMesh);
    _meshData.setBoundaryNameList(_boundaryMeshValues);
    _meshData.setMeshPath(QFileInfo(_meshConfigFile).absolutePath());
    _meshData.setMeshFileName(QFileInfo(_meshConfigFile).fileName());
    QString _meshfileName =  QFileInfo(_meshConfigFile).baseName() + "." + MESH_CONFIG_SUFFIX;
    QString _meshDetailsFileName = QDir(QDir(_projectData.getProjectPath()).filePath(SOLVER_DIR_NAME)).filePath(_meshfileName);
    _meshData.setMeshDetailsFileName(_meshDetailsFileName);
    _meshData.saveMeshInfoFile();
    //________________________________________________________________________________________________________________

    updateMeshView(_resultSolver.getSolverFilePath());

}

void MainWindow::updateMeshView(QString _solverFilePath)
{
    // Updating View
    if (!_isMultizoneProject)
        _inputDataTabHelper.updateMeshFileNameInActionGroup(_solverFilePath, &_userConfigValuesSolver);
    else
        _mManager.updateViewForNewFile(_solverFilePath,OptionMzMeshPath,&_multizoneProjectData,&_projectData, _isNewConfigFile);

    // Enabling and disabling
    _inputDataTabHelper.enableConfigurationViewMenu(_projectData);
    _inputDataTabHelper.enableChangeConfigActionMenu();
    setEnabledRunIcon(false);

}

/**
 * @brief This function checks the mesh boundary names
 *
 */
void MainWindow::checkMeshNames()
{
    SolverMarkerMenuHelper _markerHelper;
    QString _configFilePath = _projectData.getSolverDataInformation().getSolverConfigFilePath();
    QString _meshFilePath = QDir(_meshData.getMeshPath()).filePath(_meshData.getMeshFileName());
            //_projectData.getSolverDataInformation().getSolverFilePath();

    if ((!_userConfigValuesSolver.isEmpty()) && (! _boundaryMeshValues.isEmpty()) )
    {
        bool _areEqual = _markerHelper.checkMarkerMeshVariables(_userConfigValuesSolver,_boundaryMeshValues);
        if (!_areEqual)
        {
            WarningMultizoneDialog  _warningDialog;
            _warningDialog.initializeFileNames(_configFilePath,_meshFilePath);
            _warningDialog.setConfigValues(_userConfigValuesSolver);
            _warningDialog.setMeshValueNames(_boundaryMeshValues);
            _warningDialog.exec();
            if (!_warningDialog.userAcceptChanges())
            {
                resetOldMarkerValues();
            }
            else{
                if ( (_warningDialog.userAcceptChanges()) && (_warningDialog.isNeedToUpdate()) )
                {
                    _userConfigValuesSolver.clear();
                    _userConfigValuesSolver.insert("MESH_FILENAME",_meshFilePath);
                    _userConfigValuesSolver = _warningDialog.getConfigValues();
                }else
                    resetOldMarkerValues();
            }
        }
    }
}

/************************************ Save/Update Functions ************************************/



/**
 * @brief Update Project Data Paths
 *
 */
void MainWindow::updateProjectDataToFile()
{

    UtilFiles _utilFiles;
    if (!_isMultizoneProject)
        _utilFiles.updateProjectDataToFile(_projectData, _projectDetailsData);
    else
        _utilFiles.updateMultizoneProjectDataToFile(_multizoneProjectData, _projectDetailsData);

}



/************************************ Plotting Functions ************************************/



/************************************ disable/enable Functions ************************************/


/**
 * @brief Disable all the Tool Buttons
 *
 */
void MainWindow::disableAllMainButtons()
{
    ui->salomeButton->setEnabled(false);
    ui->paraviewButton->setEnabled(false);
    ui->solverButton->setEnabled(false);
}

/**
 * @brief Enable all the Tool Buttons
 *
 */
void MainWindow::enableAllMainButtons()
{
    ui->salomeButton->setEnabled(true);
    ui->paraviewButton->setEnabled(true);
    ui->solverButton->setEnabled(true);
}



/************************************ clear Functions ************************************/

/**
 * @brief Clear the console text and removes all the text
 *
 */
void MainWindow::clearConsoleText()
{
    ui->textEdit->clear();
}


/**
 * @brief Reset the current values of the Solver configuration made by the user
 *
 */
void MainWindow::resetUserConfigurationValues()
{
    _userConfigValuesSolver.clear();
}

/************************************ close/stop Functions */

/**
 * @brief Close Project function
 *
 */
void MainWindow::   closeProject()
{
    //ProjectMenuTreeHelper _treeHelper;
    bool _savedFile = true;
    if (_status_NeedToBeSaved)
    {
       ShowMessageHelper _showHelper;
       bool _saveResults = _showHelper.showMessageForSaving();
       if (_saveResults)
       {
           if (_isMultizoneProject)
           {
               if (!_projectData.getSolverDataInformation().getSolverConfigFilePath().isEmpty())
                   _savedFile = saveConfigFile();
               else
               {
                   updateProjectDataToFile();
                   _savedFile = true;
               }
            }else
               if ((_saveResults) && (_status_NeedToBeSaved))
                 _savedFile = saveConfigFile();
       }

    }
    if (_savedFile)
    {
        // Reset Multizone View
        if (_isMultizoneProject)
            _mManager.restoreTabViewToGeneralProject();

        // Clear All Data
        clearConsoleText();
        clearAllData();
        _treeHelper.clearProjectTreeView();//ui->projectTreeView);

        // Hide Disable
        _inputDataTabHelper.hideAllSolverMenus();
        _inputDataTabHelper.disableAllInputDataTab();
        disableAllMainButtons();

        // Set up boolean values

        resetBooleanValues();
        // Plot Actions
        plotResiduals->resetLegend(_residualVO);
        plotResiduals->clearCanvas();
        _residualVO->clearAllData();
        setEnabledRunIcon(false);
        resetSolverMenu();

        if (_salomeLocationProccess->pid() > 0)
            _salomeLocationProccess->kill();
    }

}


/**
 * @brief Reset the values of the menus of Solver to the default values
 *
 */
void MainWindow::resetSolverMenu()
{
    ui->actionImport_Mesh_File->setEnabled(false);
    ui->actionSave_Configuration_File->setEnabled(false);
    ui->actionSave_Configuration_File_As->setEnabled(false);
    setEnabledRunIcon(false);
}

/**
 * @brief Clear all the data to the default values
 *
 */
void MainWindow::clearAllData()
{
    // Project Data
    _projectData.clear();
    _projectDetailsData.clear();
    _meshData.clear();

    delete _projectTextEditor; // TO BE REVIEWED
    _projectTextEditor = new QTextEdit();
    _status_NeedToBeSaved = false;
    _projectExists = false;

    // Solver
     _userConfigValuesSolver.clear();
     resetBoundaryValues();
     /*_userBoundaryValuesSolver.clear();
     _boundaryMeshValues.clear();*/
     delete _outputSolverParser; // TO BE REVIEWED
     _outputSolverParser = new OutputSolverParser();
     _userVirtualMRFDataList.clear();
}

/**
 * @brief Stop current solver running process
 *
 */
void MainWindow::stopSolverProcess()
{
    if (_myprocess->pid() > 0)
        _myprocess->kill();
    setEnabledRunIcon(true);
}

/************************ Configuration File Tab */

/**
 * @brief Run the current Solver project
 *
 */
void MainWindow::runSolverProject()
{
    bool _serialExecution = true;
    if (!ui->noProcessorslineEdit->text().isEmpty())
    {
        bool isInt;
        int noProcessors = ui->noProcessorslineEdit->text().toInt(&isInt);

        if ((isInt) && (noProcessors > 1))
            _serialExecution = false;
    }

    // Clear data just in case
    //plot->resetLegend(_residualVO);
     _residualVO->clearAllData();
     plotResiduals->clearCanvas();
    _updatedLegend = false;
    _outputSolverParser = new OutputSolverParser();
    // SOL: http://stackoverflow.com/questions/11921004/cannot-read-output-using-readyreadstandardoutput-in-qt

    // Setting the working directory

    QString _workingDir = QDir(_projectData.getProjectPath()).filePath(TOOL4);
    _myprocess->setWorkingDirectory(_workingDir);
    QString _processCommand;
    if (_serialExecution)
    {
        _processCommand = buildSolverCommand();
        if (!_processCommand.isEmpty())
        {
            ui->textEdit->append( "EXECUTING ***********************");
            ui->textEdit->append("PATH--> "+_workingDir);
            ui->textEdit->append("COMMAND--> " +_processCommand);
            _myprocess->start(_processCommand);
        }else
            ui->textEdit->append( "ERROR running the solver program" );
    }
    else
    {
         QString _partitionPath;
        if (_userPreferencesData.isDefaultPartitionEngineSelected())
            _partitionPath = QDir(QDir(QCoreApplication::applicationDirPath()).filePath(SOLVER_DEFAULT_FOLDER)).filePath(PARALLEL_BBIPED_PARTITION_COMMAND);
        else
            _partitionPath = _userPreferencesData.getCustomizedPartitionEnginePath();

        _processCommand = MPI_EXEC_COMMAND + SPACE_TAB + ui->noProcessorslineEdit->text() + SPACE_TAB + _partitionPath +
                SPACE_TAB + _projectData.getSolverDataInformation().getSolverConfigFilePath();
        if (!_processCommand.isEmpty())
        {
        ui->textEdit->append( "EXECUTING ***********************");
        ui->textEdit->append("PATH--> "+_workingDir);
        ui->textEdit->append("COMMAND--> " +_processCommand);
        _myprocess->start(_processCommand);
        }else
            ui->textEdit->append( "ERROR running the solver program" );

        QString _parallelPath;
        if (_userPreferencesData.isDefaultParallelEngineSelected())
            _parallelPath = QDir(QDir(QCoreApplication::applicationDirPath()).filePath(SOLVER_DEFAULT_FOLDER)).filePath(PARALLEL_BBIPED_RUN_COMMAND);
        else
            _parallelPath = _userPreferencesData.getCustomizedParallelEnginePath();


        _processCommand = MPI_EXEC_COMMAND + SPACE_TAB + ui->noProcessorslineEdit->text() + SPACE_TAB + _parallelPath +
                SPACE_TAB + _projectData.getSolverDataInformation().getSolverConfigFilePath();
        if (!_processCommand.isEmpty())
        {
            ui->textEdit->append( "EXECUTING ***********************");
            ui->textEdit->append("PATH--> "+_workingDir);
            ui->textEdit->append("COMMAND--> " +_processCommand);
            _myprocess->start(_processCommand);
        }else
        ui->textEdit->append( "ERROR running the solver program" );

    }

}

void MainWindow::runSolverProject(QString _projectPath)
{

    // Clear data just in case
    //plot->resetLegend(_residualVO);
     _residualVO->clearAllData();
     plotResiduals->clearCanvas();
    _updatedLegend = false;
    _outputSolverParser = new OutputSolverParser();
    // SOL: http://stackoverflow.com/questions/11921004/cannot-read-output-using-readyreadstandardoutput-in-qt

    // Setting the working directory

    QString _workingDir = QDir(_projectPath).filePath(TOOL4);
    _myprocess->setWorkingDirectory(_workingDir);
    QString _processCommand = buildSolverCommand();
    if (!_processCommand.isEmpty())
    {
        ui->textEdit->append( "EXECUTING ***********************");
        ui->textEdit->append("PATH--> "+_workingDir);
        ui->textEdit->append("COMMAND--> " +_processCommand);
        _myprocess->start(_processCommand);
    }else
        ui->textEdit->append( "ERROR running the solver program" );
}
/**
 * @brief  Connecting the Process Signal Slots
 *
 */
void MainWindow::connectProcessSignalSlots()
{
    connect(_myprocess, SIGNAL(readyReadStandardOutput()), this, SLOT(outlog()));
    connect(_myprocess, SIGNAL(error(QProcess::ProcessError)),
            this, SLOT(slotProcessError(QProcess::ProcessError)));
    connect(_myprocess,SIGNAL(finished(int,QProcess::ExitStatus)),this,
            SLOT(slotFinishResult(int, QProcess::ExitStatus)));
    connect(_myprocess,SIGNAL(started()),this,SLOT(slotStarted()));    
    //connect(_myprocess, SIGNAL(readyReadStandardError()), this, SLOT(outlog()));
}

/**
 * @brief  Function to build the solver Command according project conditions and user preferences
 *
 * @return QString
 */
QString MainWindow::buildSolverCommand()
{
    QString _processCommand;
    _processCommand += getSolverCommand();

    if (_isMultizoneProject)
    {
       QStringList _listConfig = _multizoneProjectData.getListConfigFileNames();
       QStringList _listMesh = _multizoneProjectData.getListMeshFileNames();
       // Choose the exact number of Config Files that matchs with the number of mesh files
       int _availableConfigFiles = _listConfig.size();
       if (_listConfig.size() > _listMesh.size())
            _availableConfigFiles = _listMesh.size();

       // Build the command with the Config file names
       for (int i = 0; i < _availableConfigFiles ;i++)
       {
           _processCommand += SPACE_TAB;
           _processCommand += _listConfig.at(i);
       }
    }else
    {
        QString _configFilePath = _projectData.getSolverDataInformation().getSolverConfigFilePath();
        if (QString::compare(_configFilePath,"") != StrEqual)
        {
            QFileInfo _configFileInfo(_configFilePath);
            QString _configFileName = _configFileInfo.fileName();            
            _processCommand += SPACE_TAB;
            _processCommand += _configFileName;
         }
    }
    return _processCommand;
}

/**
 * @brief This function decides which Solver running executable is going to be called. At
 * the moment there are 3 options: SU2_CFD, SU2_CFD_VMRF, SU2_CFD_MULTIZONE
 *
 * @param _solver
 * @return QString
 */
QString MainWindow::getSolverCommand()
{
    QString _command;

    _command = QDir(QDir(QCoreApplication::applicationDirPath()).filePath(SOLVER_DEFAULT_FOLDER)).filePath(SOLVER_RUN_COMMAND);

    // If the user chooses a local Engine, this is the priority
    if  ( (!_userPreferencesData.isDefaultSolverEngineSelected()) &&
          (!_userPreferencesData.getUserDefinedSolverPath().isEmpty()) )
        return _userPreferencesData.getUserDefinedSolverPath();

    // Now the rest of the options can be analised

    if (_userPreferencesData.isDefaultSolverEngineSelected())
        return _command;

    return _command;
}

/**
 * @brief Slot function that shows when the process has started
 *
 */
void MainWindow::slotStarted()
{
    ui->textEdit->append("Process Started");
// TODO --> Read if hte history file is written

 /*   QString _fileName;
    if (_userConfigValuesSolver.contains("CONV_FILENAME"))
        _fileName = QDir(QDir(_projectData.getProjectPath()).filePath(TOOL4)).filePath(_userConfigValuesSolver.value("CONV_FILENAME"));

    if( (!_fileName.isEmpty()) && (!_fileName.isNull()))
    {
        ui->textEdit->append("Process Started" +_fileName);
      }*/
}

/**
 * @brief Slot function that performs some operations as soon as the process finish
 *
 * @param val
 * @param status
 */
void MainWindow::slotFinishResult(int _val,QProcess::ExitStatus status)
{
    ui->textEdit->append("-------------------------------- END PROCESSS -------------------------");
    setEnabledRunIcon(true);
    stopSolverProcess();
}

/**
 * @brief Slot function that captures the error if something goes wrong in the process
 *
 * @param error
 */
void MainWindow::slotProcessError(QProcess::ProcessError error)
{

    switch (error) {
        case QProcess::FailedToStart :
        ui->textEdit->append("FAILED to start the application: Check the executable configuration in the Preferences Menu");
            break;
    case QProcess::Crashed:
         ui->textEdit->append( "STOP Running simulation" );
        break;

       case QProcess::Timedout:
         ui->textEdit->append("TimedOut process: Contact Support");
        break;
    case QProcess::WriteError:
         ui->textEdit->append("Write Error: Check the permissions of the folder");
        break;
    case QProcess::ReadError:
         ui->textEdit->append( "Read Error: Check the permissions of the files ");
        break;
    case QProcess::UnknownError:
         ui->textEdit->append("Unknown Error: Contact Support ");
        break;

    }

 stopSolverProcess();
}

/************************************ Other Utility ************************************/


/**
 * @brief Function that controls the close event
 *
 * @param event
 */
void MainWindow::closeEvent(QCloseEvent *event)
{
   bool _needToClose = exitApplication();
   if (!_needToClose)
       event->ignore();
}




/************************************ RESIDUAL Tab ACTIONS */

/*** BUTTONS */

/**
 * @brief RESIDUAL: restart the current simulation --> TODO
 *
 */
#include "Plotter/plottermanager.h"
void MainWindow::setResidualPlot()
{
    PlotterManager _plotManager;

    QWidget * vBox = new QWidget(this);
    plotResiduals = new SolverPlotter( vBox );

    _plotManager.createPlotTab(vBox, ui->solverTabwidget,plotResiduals, "Residual Plot","Press the legend to en/disable a curve");

/*
    vBox->setWindowTitle( "Residual Plot" );
    plot = new SolverPlotter( vBox );
    const int margin = 5;
    plot->setContentsMargins( margin, margin, margin, margin );

    QString info( "Press the legend to en/disable a curve" );

    QLabel *label = new QLabel( info, vBox );
    QVBoxLayout *layout = new QVBoxLayout( vBox );
    layout->addWidget( plot );
    layout->addWidget( label );
    vBox->show();

    ui->solverTabwidget->addTab(vBox,"Residual");
*/
}



// NEW FEATURES

/**
 * @brief SOLVER: Main functionality to show the basic configuration view for the Physics config menu
 *
 */
void MainWindow::on_basivViewEditButton_clicked()
{
   bool _needToSave =  _inputDataTabHelper.launchSolverConfigDialog(this,true,&_userConfigValuesSolver,
                                            &_solverManager,&_userVirtualMRFDataList,_is3DMesh);
   if (_needToSave)
   {
       _status_NeedToBeSaved = true;
       setEnableSaveProjectIcons(_status_NeedToBeSaved);
       setEnabledSaveConfigIcons(_status_NeedToBeSaved);
       ui->savePhysicsButton->setEnabled(_status_NeedToBeSaved);
   }

}

/**
 * @brief SOLVER: Import mesh file button
 *
 */
void MainWindow::on_importMeshFileButton_clicked()
{
    importMeshFile();
}

void MainWindow::importMeshFile()
{
    // Check if mesh config information exists, otherwise import file and create mesh config information

    QString _meshConfigFile = QFileDialog::getOpenFileName(this, tr("Import SU2 Mesh File or Configuration Mesh File"),
                                                            QDir(_projectData.getProjectPath()).filePath(TOOL4),
                                                            tr("Mesh File s(*.su2 *.meshcfg)"));
    if (_meshConfigFile.isNull())
         return;


   QString _meshConfigPath = QFileInfo(_meshConfigFile).absolutePath();
   if (QString::compare(_meshConfigPath,QDir(_projectData.getProjectPath()).filePath(TOOL4)) != StrEqual) // If we are taking from the project. We donot need to remove the files
   {
        resetOldMarkerValues();
        removeOldMeshFiles();
        bool _isMeshInfo = false;
        if (QString::compare(QFileInfo(_meshConfigFile).completeSuffix(),MESH_CONFIG_SUFFIX) == StrEqual)
            _isMeshInfo = true;
         if (_isMeshInfo)
             importMeshInfoFile(_meshConfigFile);
         else
             importMeshFile(_meshConfigFile);
         saveMeshVariables();
         _status_NeedToBeSaved = true;
   }
   setEnabledProjectIconButtons(true);
   enableMeshOptions();
}

void MainWindow::removeOldMeshFiles()
{
    QString _oldMeshFileName= QFileInfo(_projectData.getSolverDataInformation().getSolverFilePath()).fileName();
    QString _projectPath = QDir(_projectData.getProjectPath()).filePath(TOOL4);

    QFile _oldMeshName (QDir(_projectPath).filePath(_oldMeshFileName));
    QFile _oldMeshName_ConfigSuffix (QDir(_projectPath).filePath(QString(QFileInfo(_oldMeshFileName).baseName() + "." + MESH_CONFIG_SUFFIX)));

    if (_oldMeshName.exists())
        _oldMeshName.remove();
    if (_oldMeshName_ConfigSuffix.exists())
        _oldMeshName_ConfigSuffix.remove();
}

/**
 * @brief SOLVER: Save Mesh variables values, changed by the user
 *
 */
void MainWindow::saveMeshVariables()
{
    QString _meshFile = QDir(_meshData.getMeshPath()).filePath(_meshData.getMeshFileName());
    _userConfigValuesSolver.insert("MESH_FILENAME",_meshFile);
    QString _ext = QFileInfo(_meshFile).suffix();
    if (QString::compare(_ext.toLower(), "su2") == StrEqual)
        _userConfigValuesSolver.insert("MESH_FORMAT",_ext.toUpper());
    else
        _userConfigValuesSolver.insert("MESH_FORMAT",_ext);
    SolverVO _oldData = _projectData.getSolverDataInformation();
    _oldData.setSolverFilePath(_meshFile);
    _projectData.setSolverDataInformation(_oldData);
}

/**
 * @brief SOLVER: Function to load the menu for configuring the boundaries by the user
 *
 */
void MainWindow::on_setMeshPropertiesButton_clicked()
{
    loadMeshDialog();
}

/**
 * @brief SOLVER: funtion to load the MESH dialog
 *
 */
void MainWindow::loadMeshDialog()
{
    if (_userBoundaryValuesSolver.isEmpty())
        getLoadedBoundaryValues();
    _meshWindow = new MeshDialog(this);
    _meshWindow->setMeshValues(_boundaryMeshValues);
    _meshWindow->setSolverConfigManager(&_solverManager);
    _meshWindow->setUserConfiguredMeshValues(_userBoundaryValuesSolver); //TBR
    _meshWindow->setIsMultizoneProject(_isMultizoneProject);
    bool _isNSProblem = false;
    if (!_userConfigValuesSolver.isEmpty())
    {
        QString _physicalPbVal = _userConfigValuesSolver.value("PHYSICAL_PROBLEM");
        if (QString::compare(_physicalPbVal,"NAVIER_STOKES") == StrEqual)
            _isNSProblem = true;
        else _isNSProblem = false;
    }
    _meshWindow->setIsNSProblem(_isNSProblem);
    _meshWindow->loadMenu();
    _meshWindow->exec();
    if (_meshWindow->hasUserAcceptChanges())
    {
        _userBoundaryValuesSolver = _meshWindow->getUserBndVariableValues();
        updateBoundaryValues();
        _status_NeedToBeSaved = true;
        setEnabledProjectIconButtons(true);
    }
}

/**
 * @brief SOLVER: function to update the values changed by the user in the mesh variables
 *
 */
void MainWindow::updateBoundaryValues()
{
    resetOldMarkerValues();
    if (!_userBoundaryValuesSolver.isEmpty())
    {
        QHashIterator<QString,QString> _iter(_userBoundaryValuesSolver);
        while (_iter.hasNext())
        {
            _iter.next();
            QString _keyname = _iter.key();
            QString _value = _iter.value();
            _userConfigValuesSolver.insert(_keyname,_value);
        }
    }


}


/**
 * @brief Reset the marker values to their old values
 *
 */
void MainWindow::resetOldMarkerValues()
{
    if (!_userConfigValuesSolver.isEmpty())
    {
        QHashIterator<QString,QString> _iter(_userConfigValuesSolver);
        while(_iter.hasNext())
        {
            _iter.next();
            QString _keyName = _iter.key();
            if( (_keyName.contains(MARKER_PREFIX)) || (_keyName.contains(BC_PREFIX)) )
                 _userConfigValuesSolver.insert(_keyName,"NONE");
        }
    }
}

/**
 * @brief load the Boundary values
 *
 */
void MainWindow::getLoadedBoundaryValues()
{
    if (!_userConfigValuesSolver.isEmpty())
    {
        QHashIterator<QString,QString> _iter(_userConfigValuesSolver);

        while (_iter.hasNext())
        {
            _iter.next();
            QString _keyVal = _iter.key();
            QString _val = _iter.value();
            if( (!_keyVal.isEmpty()) && (!_val.isEmpty()) )
                if (( (_keyVal.contains(MARKER_PREFIX)) ||
                     (_keyVal.contains(BC_PREFIX))) &&
                    (!_val.contains("NONE") ) )
                {
                    if (_val.contains("("))
                        _val.remove("(");
                    if (_val.contains(")"))
                        _val.remove(")");
                    _userBoundaryValuesSolver.insert(_keyVal,_val);
                }
        }

    }

}

/**
 * @brief Save Physics button functionality, it saves all the information (including the boundaries into a
 * single configuration file
 *
 */
void MainWindow::on_savePhysicsButton_clicked()
{
    bool _savedFile = saveConfigFile();
    // Remove the default values
    if (_savedFile)
    {
        removeDefaultValuesReadFromConfigFile();
        setEnableSaveProjectIcons(_status_NeedToBeSaved);
        setEnabledSaveConfigIcons(_status_NeedToBeSaved);
        ui->savePhysicsButton->setEnabled(_status_NeedToBeSaved);
        if (!_status_NeedToBeSaved)
             enableSolverRunAction();
    }
}



/**
 * @brief Enable the action buttons for running the simulation according with the current status of the
 * application and the configuration made by the user
 *
 */
void MainWindow::enableSolverRunAction()
{
    if ((_isMultizoneProject) && (!_multizoneProjectData.isEmpty()))
    {
        setEnabledRunIcon(true);
    }else
        if (_projectData.solverDataComplete())
           setEnabledRunIcon(true);

}

/////////////////////
/**
 * @brief Reset of Solver tab elements to their default values
 *
 */
void MainWindow::resetSolverTabEnabledElements()
{
    setEnabledRunIcon(false);
    setEnableMenuSolver(false);
 /*   ui->actionCreate_New_Configuration_File->setEnabled(false);
    ui->actionLoad_Configuration_File->setEnabled(false);
    ui->actionStop_Execution->setEnabled(false); */
    ui->stopButton->setEnabled(false);
 //   ui->actionSave_Residual_Image->setEnabled(false);
//    setEnabledSaveConfigIcons(false);
    ui->savePhysicsButton->setEnabled(false);
    ui->setMeshPropertiesButton->setEnabled(false);
}



void MainWindow::setEnableMenuSolver(bool _val)
{
    ui->actionCreate_New_Configuration_File->setEnabled(_val);
    ui->actionLoad_Configuration_File->setEnabled(_val);
    ui->actionRun_Project->setEnabled(_val);
    ui->actionStop_Execution->setEnabled(false);
    ui->actionSave_Residual_Image->setEnabled(_val);
    setEnabledSaveConfigIcons(_val);
    ui->actionImport_Mesh_File->setEnabled(_val);
    ui->actionSave_Configuration_File_As->setEnabled(_val);
    ui->actionSave_Configuration_File->setEnabled(_val);

}

/**
 * @brief This function stops the current simulation
 *
 */
void MainWindow::on_stopButton_clicked()
{
    stopSolverProcess();
}


/**
 * @brief This function is to enable of the different buttons according to _val for the user
 *
 * @param _val
 */
void MainWindow::setEnabledProjectIconButtons(bool _val)
{
    setEnabledRunIcon(_status_NeedToBeSaved);
    setEnableSaveProjectIcons(_status_NeedToBeSaved);
    setEnabledSaveConfigIcons(_status_NeedToBeSaved);
    setEnabledInputTabSaveButtons(_val);
}


void MainWindow::resetBoundaryValues()
{
    _userBoundaryValuesSolver.clear();
    _boundaryMeshValues.clear();
}







void MainWindow::on_actionCreate_New_Configuration_File_triggered()
{
   createNewConfigurationFile();
}


void MainWindow::createNewConfigurationFile()
{
    bool _fileCreated = false;
    while (!_fileCreated)
    {
        QString _configPath = QFileDialog::getSaveFileName(this, tr("Create New Configuration File"),
                                    QDir(_projectData.getProjectPath()).filePath(TOOL4),
                                   tr("Configuration File (*.cfg)"));
        if (_configPath.isNull())
        {
            // Button Cancel is pressed, so whe go out from this function
            return;
        }

        if (_configPath.contains(" "))
        {
                 QMessageBox::warning(this,"Name contains blank spaces","The name of the project cannot contain blank spaces. They will be substituted by _.");
                 _configPath = _configPath.simplified();
                 _configPath.replace( " ", "_" );
        }
        // Importing the Mesh File is needed
       resetUserConfigurationValues();


        // Writing the initial Configuration File
        SolverConfigWriter _writer;
        QDomDocument * _emptyTree = new QDomDocument();
        UtilFiles _utilFiles;
        if (!_utilFiles.hasFileExtension(_configPath,"cfg"))
                   _configPath =  _configPath + ".cfg";
        _writer.writeToConfigurationFile(_configPath, _emptyTree, _projectDetailsData);

        // Updating the Configuration File info in the project Data

        SolverVO _solverVO = _projectData.getSolverDataInformation();
        _solverVO.setSolverConfigFilePath(_configPath);
        _projectData.setSolverDataInformation(_solverVO);

        // Enabling correspondent bool values
        _fileCreated = true;
        _status_NeedToBeSaved = true;
        _isNewConfigFile = true;
        _inputDataTabHelper.enableConfigurationViewMenu(_projectData);

       setEnabledRunIcon(ui->runSimulationButton->isEnabled());
       setEnabledSaveConfigIcons(true);
       ui->savePhysicsButton->setEnabled(true);
       if (!_userBoundaryValuesSolver.isEmpty())
       {
           _userBoundaryValuesSolver.clear();
            getLoadedBoundaryValues();
       }
       // Updating Values
       if (!_isMultizoneProject)
           _inputDataTabHelper.updateConfigurationFilePathInActionGroup(_configPath);
       else
       {
           _mManager.updateViewForNewFile(_configPath, OptionMzConfigPath,&_multizoneProjectData,
                                          &_projectData, _isNewConfigFile);

           // If a mesh file has been already opened, ask the user to keep the values or not
           if (!_projectData.getSolverDataInformation().getSolverFilePath().isEmpty())
           {
               ShowMessageHelper _showMessage;
               bool _keepMeshValues = _showMessage.showWarningMessage("Mesh Information",
                                       "A boundary configuration already exists. Do you want to keep the old boundary configuration?");
               if (_keepMeshValues)
                  _mManager.updateViewForNewFile(_projectData.getSolverDataInformation().getSolverFilePath(),
                                                  OptionMzMeshPath,&_multizoneProjectData,&_projectData,
                                                  _isNewConfigFile);
               else
               {
                   // Reset the values from Mesh file
                   _projectData.getSolverDataInformation().clear();
                   _projectData.getSolverDataInformation().setSolverConfigFilePath(_configPath);
                   _mManager.updateMeshFileNameinTabView("");

                   // Reset the values for the boundaries
                   resetBoundaryValues();

               }
           }
       }
    }

    QMessageBox::information(this,"Configuration of your simulation","To configure your simulation, please, click on Solver button, and follow the steps. Thanks!");

}

void MainWindow::on_actionLoad_Configuration_File_triggered()
{
    if ( (_status_NeedToBeSaved) && (_isMultizoneProject) && (!_projectData.getSolverDataInformation().isConfigFileEmpty())
         && (!_projectData.getSolverDataInformation().isMeshFileEmpty()) )
           saveConfigFile();

    if (openSolverConfigurationFileDialog())
    {
        removeDefaultValuesReadFromConfigFile();
        _inputDataTabHelper.enableChangeConfigActionMenu();
        //_inputDataTabHelper.enableSimulationFileEdits();
        _status_NeedToBeSaved = true;
        _isNewConfigFile = false; // TBR
        setEnabledSaveConfigIcons(true);
        ui->savePhysicsButton->setEnabled(true);
        if (!_userBoundaryValuesSolver.isEmpty())
        {
            _userBoundaryValuesSolver.clear();
             getLoadedBoundaryValues();
        }

    }
}
