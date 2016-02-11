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
 *  Author: Dec 2016: Carmen Alonso Montes calonso@bcamath.org
***/
#include "multizonemanager.h"
#include "Defines/General/GeneralDefines.h"

#include "Forms/ProjectDialog/multizoneprojectdialog.h"
#include "Utility/utilfiles.h"
#include "Data/Solver/VO/solvervo.h"

#include <QStringList>
#include <QFileInfo>


/**
 * @brief CONSTRUCTOR
 *
 */
MultizoneManager::MultizoneManager()
{
    _uiRegistered = false;
}

/**
 * @brief This function returns true if the UI elements are registered
 *
 * @return bool
 */
bool MultizoneManager::isUIRegistered()
{
    return _uiRegistered;
}

/**
 * @brief This function register all the elements of the UI
 *
 * @param _button
 * @param _meshButton
  * @param _confLabel
 * @param _meshLabel
 * @param _runAction
 * @param _compareConfigFilesAction
  * @param _viewGB
 * @param _meshProperties
 * @param _configLineEdit
 * @param _meshLineEdit
 * @param _tree
 */
void MultizoneManager::registerUIElements(QPushButton * _button, QPushButton * _meshButton, //QPushButton * _meshRefresh,
                                          QLabel * _confLabel, QLabel * _meshLabel,
                                          QAction * _runAction,
                                          QAction * _compareConfigFilesAction, // QGroupBox * _filesGroupBox,
                                          QGroupBox * _viewGB,
                                          QGroupBox * _meshProperties,
                                          QLabel * _confLineLabel, QLabel * _meshLineLabel,
                                          QTreeView * _tree )
{

    _registeredButton.append(_button);
     _registeredButton.append(_meshButton);
   // _registeredButton.append(_meshRefresh);

    _registeredLabels.append(_confLabel);
    _registeredLabels.append(_meshLabel);
    _registeredLabels.append(_confLineLabel);
    _registeredLabels.append(_meshLineLabel);

    _registeredActions.append(_runAction);
    _registeredActions.append(_compareConfigFilesAction);

   // _registeredGroupBox.append(_filesGroupBox);
    _registeredGroupBox.append(_viewGB);
    _registeredGroupBox.append(_meshProperties);

   /* _registeredLineEdit.append(_configLineEdit);
    _registeredLineEdit.append(_meshLineEdit);
    */
    _treeHelper.registerMenuTree(_tree);
    //_registeredTreeView = _tree;
    _uiRegistered = true;
}


/**
 * @brief This function create the initial data for the multizone project
 *
 * @param _projectData
 * @param _multizoneProjectData
 */
void MultizoneManager::createInitialProjectDataForMultizoneProject(ProjectData *_projectData,
                                                                   MultizoneProjectData *_multizoneProjectData){
    _projectData->setProjectPath(_multizoneProjectData->getProjectPath());
    _projectData->setProjectFileName(_multizoneProjectData->getProjectName());
}

/**
 * @brief This function changes the tab view
 *
 * @param _multizoneProjectData
 */
void MultizoneManager::changeTabViewToMultizoneProject(MultizoneProjectData *_multizoneProjectData)
{
    _registeredButton.at(runSimulation)->setText("Run Multizone Simulation");
    _registeredLabels.at(confPos)->setText("Multizone Configuration Files");
    _registeredLabels.at(meshPos)->setText("Multizone Mesh Files");
    _registeredActions.at(runAction)->setText("Run Multizone Simulation");
    _registeredActions.at(compareConfigFileAction)->setEnabled(true);
    _registeredGroupBox.at(filesGroupBox)->setTitle("Current Multizone Simulation Files");

    refreshSimulationFilesView(_multizoneProjectData);
}
/**
 * @brief This function refresh the Simulation files
 *
 */
void MultizoneManager::refreshSimulationFilesView(MultizoneProjectData *_multizoneProjectData)
{
    if (!_multizoneProjectData->isEmpty())
    {
        _registeredLabels.at(confPos)->setText(_multizoneProjectData->getListConfigFileNames().join(COMMA_SEPARATOR));
        _registeredLabels.at(meshPos)->setText(_multizoneProjectData->getListMeshFileNames().join(COMMA_SEPARATOR));
     /*  _registeredLineEdit.at(confPos)->setText(_multizoneProjectData->getListConfigFileNames().join(COMMA_SEPARATOR));
       _registeredLineEdit.at(meshPos)->setText(_multizoneProjectData->getListMeshFileNames().join(COMMA_SEPARATOR));
       */
    }
}


/**
 * @brief Launch Multizone Dialog
 *
 * @param _multizoneProjectData
 * @param _userPreferencesData
 * @param _projectData
 * @return bool
 */
bool MultizoneManager::launchMultizoneDialog(MultizoneProjectData *_multizoneProjectData, UserPreferencesData * _userPreferencesData,
                                             ProjectData * _projectData)
{

    MultizoneProjectDialog _multizoneDiag;
    _multizoneDiag.exec();
    bool _multizoneProjectCreated = false;
    if (_multizoneDiag.isMultizoneProjectCreated())
    {
        // Updating the data
        (* _multizoneProjectData) =  _multizoneDiag.getMultizoneProjectData();
        _userPreferencesData->addProjectToList(_multizoneProjectData->getProjectPath());

        changeTabViewToMultizoneProject(_multizoneProjectData);
        createInitialProjectDataForMultizoneProject(_projectData,_multizoneProjectData);

        // Filling the Tree
        //ProjectMenuTreeHelper _treeHelper;
        _treeHelper.fillProjectTreeView(_multizoneProjectData->getProjectPath());//,_registeredTreeView);
        _multizoneProjectCreated = true;

    }
    return _multizoneProjectCreated;
}

/**
 * @brief Open Multizone Project Dialog
 *
 * @param _projectFilePath
 * @param _multizoneProjectData
 * @param _userPreferencesData
 * @param _projectData
 * @param _projectDetails
 * @return bool
 */
bool MultizoneManager::openMultizoneProject(QString _projectFilePath, MultizoneProjectData *_multizoneProjectData,
                                            UserPreferencesData * _userPreferencesData,
                                            ProjectData * _projectData, ProjectDetails * _projectDetails)
{
    UtilFiles _utilFiles;
    (*_multizoneProjectData) = _utilFiles.openMultizoneProjectStructure(_projectFilePath);
    bool _statusNeedToBeSaved = checkProjectBasePathConsistency(_multizoneProjectData,_projectFilePath);
    (* _projectDetails) = _utilFiles.openProjectDetails(_projectFilePath);
    _userPreferencesData->addProjectToList(_multizoneProjectData->getProjectPath());
    // Show Tree menu elements
   // ProjectMenuTreeHelper _treeHelper;
    _treeHelper.fillProjectTreeView(_multizoneProjectData->getProjectPath());//,_registeredTreeView);
    // Enabling all the buttons
   changeTabViewToMultizoneProject(_multizoneProjectData);
   createInitialProjectDataForMultizoneProject(_projectData,_multizoneProjectData);
   return _statusNeedToBeSaved;
}


/**
 * @brief Check for the paths of the project (import project) and update it if needed
 *
 * @param _multizoneProjectData
 * @param _currentFilePath
 * @return bool
 */
bool MultizoneManager::checkProjectBasePathConsistency(MultizoneProjectData * _multizoneProjectData,QString _currentFilePath)
{
    bool _statusNeedToBeSaved = false;
    QString _projectPath = _multizoneProjectData->getProjectPath();
    QString _cRootPath = QFileInfo(QFileInfo(_currentFilePath).path()).path();
    if (_projectPath.isNull())
        return _statusNeedToBeSaved;
    QFileInfo _basePathInfo(_projectPath);

    QString _rootPath = _basePathInfo.path();
    QString _projectBaseName = _basePathInfo.fileName();
    if (QString::compare(_rootPath, _cRootPath) != StrEqual)
    {
        QString _updatedProjectPath = QDir(_cRootPath).filePath(_projectBaseName);
        _multizoneProjectData->setProjectPath(_updatedProjectPath);

        QString _newProjectPathFiles = QDir(_updatedProjectPath).filePath(TOOL4);
        // Update Configuration Files
        _multizoneProjectData->setListConfigFiles(updateFilePaths(_multizoneProjectData->getListConfigFileNames(), _newProjectPathFiles));
        _multizoneProjectData->setListMeshFiles(updateFilePaths(_multizoneProjectData->getListMeshFileNames(), _newProjectPathFiles));

        _statusNeedToBeSaved = true;
    }
    return _statusNeedToBeSaved;
}

/**
 * @brief Update file paths
 *
 * @param _listNames
 * @param _cPathRoot
 * @return QStringList
 */
QStringList MultizoneManager::updateFilePaths(QStringList _listNames, QString _cPathRoot)
{
    QStringList _updatedList;
    if (!_listNames.isEmpty())
    {
        for (int i= 0; i < _listNames.size();i++)
        {
            QString _fileName = _listNames.at(i);
            QString _newPath = QDir(_cPathRoot).filePath(_fileName);
            _updatedList.append(_newPath);
        }
    }
    return _updatedList;
}

/**
 * @brief This function builds the project data for a specific config file in a multizone project
 *
 * @param _newConfigFile
 */
void MultizoneManager::buildProjectDataForMultizoneProject(QString _newConfigFile, MultizoneProjectData *_multizoneProjectData,
                                                            ProjectData * _projectData)
{
    SolverVO _newSolverVO;
    _newSolverVO.setSolverConfigFilePath(_newConfigFile);
    if (!_multizoneProjectData->isEmpty())
    {
        QStringList _listFiles = _multizoneProjectData->getListConfigFiles();
        if (_listFiles.contains(_newConfigFile))
        {
            int _posFile = _listFiles.indexOf(_newConfigFile,0);
            if (_posFile != -1)
            {
                QStringList _listMeshFiles = _multizoneProjectData->getListMeshFiles();
                _newSolverVO.setSolverFilePath(_listMeshFiles.at(_posFile));
            }
        }
    }
     _projectData->setSolverDataInformation(_newSolverVO);
}
/**
 * @brief This function update the data in Multizone Projects
 *
 * @param _filePath
 * @param _option
 */
void MultizoneManager::updateMultiZoneValues(QString _filePath, int _option, MultizoneProjectData *_multizoneProjectData,
                                             ProjectData * _projectData, bool _isNewConfigFile)
{
    switch(_option)
    {
    case OptionMzConfigPath:
    {
        QStringList _listConfig = _multizoneProjectData->getListConfigFiles();
        if (!_listConfig.contains(_filePath))
        {
            int _posToAdd = getPositionToAddFileMultizone(_multizoneProjectData);
            if (_posToAdd != -1)
            {
                if (_posToAdd >= _listConfig.size())
                    _listConfig.append(_filePath);
                else
                    _listConfig[_posToAdd] = _filePath;
                _multizoneProjectData->setListConfigFiles(_listConfig);
            }
        }

    }
        break;
    case OptionMzMeshPath:
    {
        QStringList _listMesh = _multizoneProjectData->getListMeshFiles();
        int _posToAdd = -1;
        if (_isNewConfigFile)
            _posToAdd = getPositionToAddFileMultizone(_multizoneProjectData);
        else
            _posToAdd = getConfigurationFilePosition(_multizoneProjectData,_projectData);

        if (_posToAdd != -1)
        {
            if (_posToAdd >= _listMesh.size())
                _listMesh.append(_filePath);
            else
                _listMesh[_posToAdd] = _filePath;
            _multizoneProjectData->setListMeshFiles(_listMesh);
        }

    }
        break;
    }
}

/**
 * @brief This function gets the position for the lists to add a new file
 * controlling the fact that the list can have different sizes
 *
 * @return int
 */
int MultizoneManager::getPositionToAddFileMultizone( MultizoneProjectData *_multizoneProjectData)
{
    int _result = -1;
    QStringList _listConfig = _multizoneProjectData->getListConfigFiles();
    QStringList _listMesh = _multizoneProjectData->getListMeshFiles();
    if (_listConfig.size() == _listMesh.size())
        return (_listConfig.count());

    if (_listConfig.size() > _listMesh.size())
        return (_listConfig.count() - 1);

    if (_listConfig.size() < _listMesh.size())
        return (_listMesh.count() - 1);

    return _result;
}

/**
 * @brief This function gets the id (position in the list) of the configuration file
 *
 * @return int
 */
int MultizoneManager::getConfigurationFilePosition(MultizoneProjectData *_multizoneProjectData,ProjectData * _projectData)
{
    QString _name =  _projectData->getSolverDataInformation().getSolverConfigFilePath();
    if (_multizoneProjectData->getListConfigFiles().contains(_name))
        return _multizoneProjectData->getListConfigFiles().indexOf(_name,0);

    return -1;
}

/**
 * @brief This function update the fields of the configuration file
 *
 * @param _file
 */
void MultizoneManager::updateConfigFileNameinTabView(QString _file)
{
    QFileInfo _fileInfo(_file);
    QString _fileName = _fileInfo.fileName();
    _registeredGroupBox.at(viewGroupBox)->setTitle("Physics Conditions: ["+_fileName+"]");

}
/**
 * @brief This function updata the fields of the mehs file
 *
 * @param _file
 */
void MultizoneManager::updateMeshFileNameinTabView(QString _file)
{
    QFileInfo _fileInfo(_file);
    QString _fileName = _fileInfo.fileName();
    _registeredGroupBox.at(meshProperties)->setTitle("Boundary Conditions: ["+_fileName+"]");
    _registeredButton.at(meshProp)->setEnabled(true);
    //_registeredButton.at(meshRefresh)->setEnabled(true);
}
/**
 * @brief This funciton restores the GUI to the single view
 *
 */
void MultizoneManager::restoreTabViewToGeneralProject()
{
    _registeredButton.at(runSimulation)->setText("Run Simulation");
    _registeredButton.at(meshProp)->setEnabled(false);
    //_registeredButton.at(meshRefresh)->setEnabled(false);

    _registeredLabels.at(confPos)->setText("Configuration File");
    _registeredLabels.at(meshPos)->setText("Mesh File");

    _registeredActions.at(runAction)->setText("Run");
    _registeredActions.at(compareConfigFileAction)->setEnabled(false);

    _registeredGroupBox.at(filesGroupBox)->setTitle("Current Simulation Files");
    _registeredGroupBox.at(viewGroupBox)->setTitle("Physics Conditions");
    _registeredGroupBox.at(meshProperties)->setTitle("Boundary Conditions");

    _registeredLabels.at(confPos)->clear();
    _registeredLabels.at(meshPos)->clear();
 /*   _registeredLineEdit.at(confPos)->clear();
    _registeredLineEdit.at(meshPos)->clear();
    */


}


/**
 * @brief Update the view for a new file
 *
 * @param _newFile
 * @param _optionMultizone
 * @param _multizoneProjectData
 * @param _projectData
 * @param _isNewConfigFile
 */
void  MultizoneManager::updateViewForNewFile(QString _newFile, int _optionMultizone, MultizoneProjectData *_multizoneProjectData,
                                ProjectData * _projectData, bool _isNewConfigFile)
{
    if (_optionMultizone == OptionMzConfigPath)
    {
      //  bool _existMeshInfo = checkOldMeshConfiguration();
       updateConfigFileNameinTabView(_newFile);
    }


    if (_optionMultizone == OptionMzMeshPath)
       updateMeshFileNameinTabView(_newFile);

    updateMultiZoneValues(_newFile, _optionMultizone,_multizoneProjectData,_projectData, _isNewConfigFile);
    refreshSimulationFilesView(_multizoneProjectData);
}




/**
 * @brief Update the Open Edit config file view
 *
 * @param _newConfigFile
 * @param _multizoneProjectData
 * @param _projectData
 * @param _isNewConfigFile
 */
void MultizoneManager::updateOpenEditConfigFile(QString _newConfigFile, MultizoneProjectData *_multizoneProjectData,
                              ProjectData * _projectData, bool _isNewConfigFile)
{
    updateConfigFileNameinTabView(_newConfigFile);
    updateMultiZoneValues(_newConfigFile, OptionMzConfigPath,_multizoneProjectData,_projectData, _isNewConfigFile);
    SolverVO _solverVO = _projectData->getSolverDataInformation();
    if (!_solverVO.getSolverFilePath().isEmpty())
    {
        updateMeshFileNameinTabView(_solverVO.getSolverFilePath());
        QStringList _listConfigFiles = _multizoneProjectData->getListConfigFileNames();
        if (!_listConfigFiles.contains(_newConfigFile))
            updateMultiZoneValues(_projectData->getSolverDataInformation().getSolverFilePath(), OptionMzMeshPath,
                                            _multizoneProjectData,_projectData, _isNewConfigFile);
    }
    refreshSimulationFilesView(_multizoneProjectData);
}


