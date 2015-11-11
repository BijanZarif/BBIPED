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
#include "inputdatatabhelper.h"
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QMessageBox>
#include <QFileDialog>
#include <QObject>

#include "Defines/General/GeneralDefines.h"
#include "Parser/parserfiles.h"
#include "Parser/Solver/configurationfileparser.h"
#include "Parser/Solver/meshfileparser.h"
#include "WindowHelper/Dialog/SolverDialog/solvermarkermenuhelper.h"
#include "Utility/utilfiles.h"

#include "Utility/showmessagehelper.h"

/**
 * @brief Constructor
 *
 */
InputDataTabHelper::InputDataTabHelper()
{
}

/********************************************* Initialize ************************************/

/**
 * @brief Initialize Solver Basic Dialog Window
 *
 *
 * @param SolverBasicDialog * _dialog, bool _basicView
 */

void InputDataTabHelper::initializeSolverBasicDialog(SolverBasicDialog * _dialog, bool _basicView,
                                                     QHash<QString,QString> _userConfigVal,
                                                     SolverConfigManager * _manager, QList<VirtualMrfVO> _virtualVOList,
                                                     bool _is3DMesh)
{
    _dialog->resetDialog();
    _dialog->setVirtualMRFData(_virtualVOList);
    _dialog->setConfigurationValues(_userConfigVal);
    _dialog->setBasicView(_basicView);
    _dialog->set3DMeshType(_is3DMesh);
    // Establishing the new data
    _dialog->setData(_manager);
    // Loading the correspondent menu
    _dialog->loadMenu();
    if (_manager->isPersonalizedMenuListAvailable())
        _dialog->enablePersonalizedView();

}
/********************************************* Parse ************************************/


/**
 * @brief Parse the correspondent files to get the values to load into the project variables
 *
 * @param _option
 * @param _pathFile
 */
void InputDataTabHelper::parseFile(int _option,QString _pathFile,
                                   QHash<QString,QString> * _userConfigVal,
                                   QHash<QString,QString> * _boundaryMeshVal,
                                   QList<VirtualMrfVO> * _virtualVOList)
{
    switch(_option)
    {

        case ParserSolverConfigFile:
           {
                ConfigurationFileParser _configParser;
                _configParser.setFilePath(_pathFile);
                _configParser.parseFileProcess();
                *_userConfigVal = _configParser.getParsedValues();
                *_virtualVOList = _configParser.getParsedVirtualMRFData();
            }
            break;
        case ParserSolverMeshFile:
            {
                MeshFileParser _meshParser;
                _meshParser.setFilePath(_pathFile);
                _meshParser.parseFileProcess();
                *_boundaryMeshVal = _meshParser.getParsedValues();
                bool _isMesh3D = _meshParser.is3DMesh();
                if (_isMesh3D)
                    *_boundaryMeshVal->insert(IS_3D_MESH_TAG,"YES");
            }
        break;
    }
}


/********************************************* Save ************************************/

/**
 * @brief Dialog to save with a different name a configuration file
 *
 */
void InputDataTabHelper::saveAsConfigFile(QString _fileName,ProjectData * _projectData, SolverConfigManager * _manager,
                                          QHash<QString,QString> _userVal, ProjectDetails _projectDetails,
                                          QList<VirtualMrfVO> _virtualVOList)
{

          if( !_fileName.isNull() )
           {
               UtilFiles _utilFiles;
               if (!_utilFiles.hasFileExtension(_fileName,"cfg"))
                   _fileName =  _fileName + ".cfg";
              _manager->saveConfigFile(_fileName,_userVal, _projectDetails,_virtualVOList);

             // Updatear el solverConfigFile
                SolverVO _solver = _projectData->getSolverDataInformation();
                _solver.setSolverConfigFilePath(_fileName);
                _projectData->setSolverDataInformation(_solver);
                updateConfigurationFilePathInActionGroup(_solver.getSolverConfigFilePath());
           }

}

/********************************************* Dialog Launchers ************************************/


/**
 * @brief Launch the Solver Configuration Dialog
 *
 * @param _isBasicView
 */
bool InputDataTabHelper::launchSolverConfigDialog(QWidget * _parent, bool _isBasicView,QHash<QString,QString> * _configValToSave,
                                                    SolverConfigManager * _manager, QList<VirtualMrfVO> * _virtualVOList,
                                                  bool _is3DMesh)
{
    bool _needToSave = false;
    SolverBasicDialog * _dialog = new SolverBasicDialog(_parent);
    initializeSolverBasicDialog(_dialog,_isBasicView,(*_configValToSave),_manager, (* _virtualVOList),_is3DMesh);

    _dialog->exec();
    updateUserConfigValues(_dialog->getConfigurationValues(),_configValToSave, _dialog->isReseted());

    if (_dialog->hasUserMadeChanges())
    {
        _needToSave = true;
        *_virtualVOList = _dialog->getVirtualMRFData(); // Updating the value of the Virtual Values
    }
    delete _dialog;
    return _needToSave;
}

/********************************************* Update ************************************/

/**
 * @brief Update the configuration values of the user in teh correspondent Hash variable
 *
 *
 * @param QHash<QString,QString> _newMap
 */
void InputDataTabHelper::updateUserConfigValues(QHash<QString,QString> _newMap,
                                                QHash<QString,QString> * _configValToSave,
                                                bool _reseted)
{
    if ( (! _newMap.isEmpty()) ||
         ((_newMap.isEmpty()) && (_reseted)) )
      {
            *_configValToSave = _newMap;
      }
}

/**
 * @brief Update the user values for the configuration file and the mesh files of the current project
 *
 */
bool InputDataTabHelper::updateSolverUserValues(ProjectData _projectData,
                                                QHash<QString,QString> * _userConfigVal,
                                                QHash<QString,QString> * _boundaryMeshVal,
                                                QHash<QString,QString> * _boundaryConfigVal,
                                                 QList<VirtualMrfVO> * _virtualVOList)
{

    SolverVO _solverVO = _projectData.getSolverDataInformation();
    if (!_solverVO.getSolverConfigFilePath().isEmpty())
        parseFile(ParserSolverConfigFile,_solverVO.getSolverConfigFilePath(),_userConfigVal,
                  _boundaryMeshVal,_virtualVOList);
    if (!_solverVO.getSolverFilePath().isEmpty())
            parseFile(ParserSolverMeshFile,_solverVO.getSolverFilePath(),_userConfigVal,_boundaryMeshVal,_virtualVOList);

    return false;
}



/**
 * @brief Update the file path in the line edit of the current configuration file
 *
 * @param _newPath
 */
void InputDataTabHelper::updateConfigurationFilePathInActionGroup(QString _newPath)
{
    QFileInfo _fileInfo(_newPath);
    QString _fileName = _fileInfo.fileName();
    _configLineLabel->setText(_fileName);
}


/**
 * @brief Update the file path in the line edit of the current mesh file
 *
 * @param _newMeshPath
 */
void InputDataTabHelper::updateMeshFileNameInActionGroup(QString _newMeshPath,
                                                         QHash<QString, QString> * _userMap)
{
    QFileInfo _fileInfo(_newMeshPath);
    QString _fileName = _fileInfo.fileName();

    _meshLineLabel->setText(_fileName);
    _userMap->insert("MESH_FILENAME",_newMeshPath);
}

/********************************************** Fill *****************************************/


/**
 * @brief Function to fill the names in the line Edit of the Input Data Tab
 *
 */
void InputDataTabHelper::fillFileNamesInTab(ProjectData _projectData,
                                            QHash<QString,QString> * _userMap)
{
    SolverVO _solverInfo = _projectData.getSolverDataInformation();
    QString _configFilePath = _solverInfo.getSolverConfigFilePath();
    QString _meshFilePath = _solverInfo.getSolverFilePath();
    updateConfigurationFilePathInActionGroup(_configFilePath);
    updateMeshFileNameInActionGroup(_meshFilePath,_userMap);


}





/************************************************ ENABLE/DISABLE/Show **************************************/

/**
 * @brief Show all the Tabs related with the solver Tool
 *
 */
void InputDataTabHelper::showAllSolverMenus(ProjectData _projectData)
{
  _tab->setEnabled(true);
  enableConfigurationViewMenu(_projectData);
}

/**
 * @brief This fucntion enables the configuration action menu according project data information
 *
 * @param _projectData
 */
void InputDataTabHelper::enableConfigurationViewMenu(ProjectData _projectData)
{
    SolverVO _solverData = _projectData.getSolverDataInformation();
    if (!_solverData.getSolverConfigFilePath().isEmpty())
    {
           enableChangeConfigActionMenu();
           enableCurrentSimActionMenu();
    }

}



/**
 * @brief Enable Change Config Action Menu elements
 *
 */
void InputDataTabHelper::enableChangeConfigActionMenu()
{
    _basicV->setEnabled(true);
    _summaryB->setEnabled(true);
}



/**
 * @brief Enable the Simulation Action Menu Elements
 *
 */
void InputDataTabHelper::enableCurrentSimActionMenu()
{
    //enableSimulationFileEdits();
    _runSimB->setEnabled(true);
}


/**
 * @brief Disable the change config action menu elements
 *
 */
void InputDataTabHelper::disableChangeConfigActionMenu()
{
    _basicV->setEnabled(false);
    _summaryB->setEnabled(false);
}


/**
 * @brief Disable All current sim action menu elements
 *
 */
void InputDataTabHelper::disableCurrentSimActionMenu()
{
    _runSimB->setEnabled(false);
}

/**
 * @brief Disable All input data tab elements
 *
 */
void InputDataTabHelper::disableAllInputDataTab( )
{

    disableChangeConfigActionMenu();
    disableCurrentSimActionMenu();
    disableRestartSimButtons();

}


/**
 * @brief Enable the restart simulatio button
 *
 */
void InputDataTabHelper::enableRestartSimButtons()
{
    _restartB->setEnabled(true);
}

/**
 * @brief Disable the restart simulatio button
 *
 */
void InputDataTabHelper::disableRestartSimButtons( )
{

   _restartB->setEnabled(false);


}

/**
 * @brief Hide all the Tabs related with the solver Tool
 *
 */
void InputDataTabHelper::hideAllSolverMenus()
{
     _tab->setEnabled(false);
}
/******************************* REGISTER ***************************************/

/**
 * @brief This function register the main Buttons of the Solver tab to controls their behaviour
 *
 * @param _mybasicV
 * @param _mySummaryB
 * @param _myRunSimB
 * @param _myRestartB
 */
void InputDataTabHelper::registerButtons(QPushButton * _mybasicV,
                     QPushButton * _mySummaryB, QPushButton * _myRunSimB,
                     QPushButton * _myRestartB)
{
    _basicV = _mybasicV;
    _summaryB = _mySummaryB;
    _runSimB= _myRunSimB  ;
   _restartB =  _myRestartB ;
}

/**
 * @brief This function register the main Line Edits of the Solver tab to controls their behaviour
 *
 * @param _myConfigLine
 * @param _myMeshLine
 */
void InputDataTabHelper::registerLineEdits(QLabel * _myConfigLine, QLabel * _myMeshLine)
{
    _configLineLabel = _myConfigLine;
    _meshLineLabel = _myMeshLine;
}

/**
 * @brief This function register the the Solver tab to controls their behaviour
 *
 * @param _myTab
 */
void InputDataTabHelper::registerTab(QTabWidget * _myTab)
{
    _tab = _myTab;

}
