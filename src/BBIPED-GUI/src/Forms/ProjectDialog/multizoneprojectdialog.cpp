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
#include "multizoneprojectdialog.h"
#include "ui_multizoneprojectdialog.h"
#include <QFileDialog>
#include <QTableWidgetItem>
#include <QHash>
#include <QDir>
#include <QHashIterator>
#include <QStringList>

#include "Utility/qtableutil.h"
#include "Defines/General/GeneralDefines.h"
#include "Defines/General/MultizoneDefines.h"
#include "Utility/utilfiles.h"
#include "Utility/showmessagehelper.h"
#include "Parser/Solver/configurationfileparser.h"
#include "Parser/Solver/meshfileparser.h"
#include "WindowHelper/Dialog/SolverDialog/solvermarkermenuhelper.h"
#include "Forms/WarningDialog/warningmultizonedialog.h"

#include <stdio.h>
#include <iostream>
using namespace std;
/**
 * @brief Constructor
 *
 * @param parent
 */
MultizoneProjectDialog::MultizoneProjectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MultizoneProjectDialog)
{
    ui->setupUi(this);
    _projectPath = "";
    _noZones = 0;
    _isMultizoneProjectCreated = false;

}

/**
 * @brief Desctructor
 *
 */
MultizoneProjectDialog::~MultizoneProjectDialog()
{
    delete ui;
}


/********************************************* CREATE/LAUNCH/BUILD *****************************************/
/**
 * @brief Create so many rows as configurationf iles in the table
 *
 * @param _noFiles
 */
void MultizoneProjectDialog::createRowInConfigFileTable(int _noFiles)
{
    int _totalRows = ui->configTable->rowCount();

    for (int i = 0; i < _noFiles;i++)
        ui->configTable->insertRow(_totalRows+i);

}

/**
 * @brief Create the cells of the row according the content and the zone (Configuration file)
 *
 * @param _content
 * @param _idRow
 * @param _idZone
 */
void MultizoneProjectDialog::createConfigurationRow(QString _content, int _idRow,int _idZone)
{
    QtableUtil _tableHelper;
    ui->configTable->insertRow(_idRow);
    ui->configTable->setItem(_idRow,ColumnZoneNo,_tableHelper.addNonEditableText(QString::number(_idZone)) );
    ui->configTable->setItem(_idRow,ColumnFileName,_tableHelper.addNonEditableText(_content) );
    ui->configTable->setCellWidget(_idRow,ColumnAddFile,_tableHelper.addIcon(IconAdd) );
    ui->configTable->setCellWidget(_idRow,ColumnImportFile,_tableHelper.addIcon(IconImport) );
    ui->configTable->setCellWidget(_idRow,ColumnDeleteFile,_tableHelper.addIcon(IconDelete) );
}

/**
 * @brief Create the cells of the row according the content and the zone (Mesh file)
 *
 * @param _content
 * @param _idRow
 * @param _idZone
 */
void MultizoneProjectDialog::createMeshFileRow(QString _content, int _idRow,int _idZone)
{
    QtableUtil _tableHelper;
    ui->configTable->insertRow(_idRow);
    ui->configTable->setItem(_idRow,ColumnZoneNo,_tableHelper.addNonEditableText(""));
    ui->configTable->setItem(_idRow,ColumnFileName,_tableHelper.addNonEditableText(_content) );
    ui->configTable->setCellWidget(_idRow,ColumnImportFile,_tableHelper.addIcon(IconImport) );

    ui->configTable->item(_idRow-1,ColumnZoneNo)->setTextAlignment(Qt::AlignCenter);
    ui->configTable->setSpan(_idRow-1,ColumnZoneNo,2,1);
    ui->configTable->setSpan(_idRow-1,ColumnDeleteFile,2,1);


}




/**
 * @brief Function that launches to create a new file
 *
 * @param _row
 */
void MultizoneProjectDialog::launchCreateNewFile(int _row)
{
    QString _newFile = QFileDialog::getSaveFileName(this, tr("Create Configuration File"),
                               QDir(_multizoneProjectData.getProjectPath()).filePath(TOOL4),
                               tr("Solver Configuration File (*.cfg)"));
    if (_newFile.isNull())
        return;
    QFileInfo _fileInfo = QFileInfo(_newFile);
    QString _fileName = _fileInfo.baseName() + DEFAULT_CONFIG_FILE_NAME_SUFFIX;

    ui->configTable->item(_row, ColumnFileName)->setText(_fileName);
    _newFilesList.append(_fileName);
}

/**
 * @brief Function that launches to import a  file
 *
 * @param _row
 */
void MultizoneProjectDialog::launchImportNewFile(int _row)
{
    QString _importedFile;
    bool _isConfigFile = false;
    if (_row%2 == 0)
    {
         _importedFile = QFileDialog::getOpenFileName(this, tr("Import Existing Configuration File"),
                               QDir(_multizoneProjectData.getProjectPath()).filePath(TOOL4),
                               tr("Solver Configuration File (*.cfg)"));
        _isConfigFile = true;
    }
    else
    {
         _importedFile = QFileDialog::getOpenFileName(this, tr("Import Existing Mesh File"),
                                QDir(_multizoneProjectData.getProjectPath()).filePath(TOOL4),
                               tr("Mesh File (*.su2)"));
        _isConfigFile = false;
    }
    if ( (_importedFile.isNull()) || (_importedFile.isEmpty()))
        return;

    ui->configTable->item(_row, ColumnFileName)->setText(_importedFile);
    _importedFilesList.append(_importedFile);
}

/**
 * @brief Function that actually build the project files into the correspondent project folder
 *
 * @return bool
 */
bool MultizoneProjectDialog::buildFilesInProject()
{
    UtilFiles _fileHelper;
    int _rowConfigFile;
    int _rowMeshFile;
    bool _buildSuccess = true;
    QString _fileConfigName;
    QString _fileMeshName;
    SolverConfigManager _solverManager;
    QHash<QString,QString> _userVal;

    QStringList _listConfigFiles;
    QStringList _listMeshFiles;

    if (!_solverManager.isInitialized())
               _solverManager.initializeManager();

    int _lastRow = ui->configTable->rowCount();
    int _processedRow = 0;


    while(_processedRow < _lastRow )
    {
        _rowConfigFile = _processedRow;
        _rowMeshFile = _processedRow+1;

        _fileConfigName = ui->configTable->item(_rowConfigFile, ColumnFileName)->text();
        _fileMeshName = ui->configTable->item(_rowMeshFile, ColumnFileName)->text();

        if (!isMeshFileNameValid(_fileMeshName))
        {
           ShowMessageHelper _messageHelper;
           bool _result =  _messageHelper.showWarningMessage("Mesh File Name not Valid", "Please, choose a valid mesh file to continue");
            return (!_buildSuccess);
        }
        else{
            // Import Mesh File
            QString _newMeshPath = _fileHelper.importFile(_projectPath,_fileMeshName, TOOL4);
            _listMeshFiles.append(_newMeshPath);
            _userVal.insert("MESH_FILENAME",_newMeshPath);
            // Process the Config file
             QString _finalPath = QDir(_projectPath).filePath(TOOL4);
            if (_newFilesList.contains(_fileConfigName))
            {
                // Saving with all the values by default and the mesh file name associated to it
                _solverManager.saveConfigFile(QDir(_finalPath).filePath(_fileConfigName),_userVal, _projectDetailsData,_virtualMRFData);
                 _listConfigFiles.append(QDir(_finalPath).filePath(_fileConfigName));
            }else
            {
                QString _newPath;
                if (_importedFilesList.contains(_fileConfigName))
                {
                   _newPath = _fileHelper.importFile(_projectPath,_fileConfigName, TOOL4);
                   bool _canContinue = checkConsistency(_newPath, _newMeshPath, _solverManager);
                   if (!_canContinue)
                   {
                       _buildSuccess = false;
                       return _buildSuccess;
                   }

                }else // The user has not done anything --> Create a new one with default name
                {
                    _newPath = QDir(_finalPath).filePath("default_config_"+QString::number(_rowConfigFile)+".cfg");
                    _solverManager.saveConfigFile(_newPath,_userVal,_projectDetailsData,_virtualMRFData);
                }
                 _listConfigFiles.append(_newPath);
            }
            // Update the counter
            _processedRow = _processedRow+2;
            _userVal.clear();
        }
        _multizoneProjectData.setListConfigFiles(_listConfigFiles);
        _multizoneProjectData.setListMeshFiles(_listMeshFiles);

        }
    return _buildSuccess;
}

/********************************************* SLOT / vALUE CHANGED *****************************************/

/**
 * @brief Function that controls the Spin Box, increasing create rows, decreasing remove rows
 *
 * @param arg1
 */
void MultizoneProjectDialog::on_multizoneNoSpinBox_valueChanged(const QString &arg1)
{
    int _totalRows = ui->configTable->rowCount();
    int _newZoneNo = arg1.toInt();
    int _cZoneNo = _totalRows/2;
    if (_newZoneNo > _cZoneNo) // We need to add new rows
    {
        int _noNewRows = _newZoneNo - _cZoneNo;
        int _rowPos = _totalRows;
        for (int i = 0;i < _noNewRows;i++)
        {
            QString _defaultConfig = DEFAULT_CONFIG_FILE_NAME_PREFIX+QString::number(_cZoneNo+i)+DEFAULT_CONFIG_FILE_NAME_SUFFIX;
            QString _defaultMesh = DEFAULT_MESH_FILE_NAME_PREFIX+QString::number(_cZoneNo+i)+DEFAULT_MESH_FILE_NAME_SUFFIX;
            createConfigurationRow(_defaultConfig,_rowPos,_cZoneNo+i);
            createMeshFileRow( _defaultMesh, _rowPos+1,_cZoneNo+i);
            _rowPos = _rowPos+2;

        }
    }else{
        if (_newZoneNo < _cZoneNo) // We need to delete rows
        {
            int _noDeletedRows = ( _cZoneNo -_newZoneNo)*2;
            for (int i = 0; i <= _noDeletedRows;i++)
                  ui->configTable->removeRow(_totalRows - i);
            //updateZoneNumberInCell();
        }
    }

}
/**
 * @brief Action that controls the tool button for creation of the project
 *
 */
void MultizoneProjectDialog::on_projectSelectionToolButton_clicked()
{
    _projectPath = QFileDialog::getSaveFileName(this, tr("Create Project File"),
                               QCoreApplication::applicationDirPath(),//QDir::currentPath(),
                               tr("Solver Project (*.prj)"));
    if (_projectPath.isNull())
        return;
    ui->projectLineEdit->setText(_projectPath);
    enableMultizoneParameters();
    // Creating the project structure
    UtilFiles _utilFiles;

    // Creating all the project structure
    bool _fileCreated = _utilFiles.createMultizoneProjectStructure(_projectPath); // TODO -- cambiar para incluir los tags the multizone
    if (!_fileCreated)
    {
        QString _title(tr("Error Creating the new project") );
        QString _message(tr("If this project already exists. Please, select a different name. Otherwise, check your writing rights in the folder"));
        ShowMessageHelper _showHelper;
        _showHelper.showMessageWindow(_title,_message,this);
        return;
    }
    _multizoneProjectData.setProjectPath(_projectPath);
    _multizoneProjectData.setProjectName(QFileInfo(_projectPath).baseName());
}

/**
 * @brief Action that controls what to do depending on the click (add new, import file, delete file)
 *
 * @param row
 * @param column
 */
void MultizoneProjectDialog::on_configTable_cellClicked(int row, int column)
{

    switch(column)
    {
    case ColumnAddFile:
        launchCreateNewFile(row);
        break;
    case ColumnImportFile:
        launchImportNewFile(row);
        break;
    case ColumnDeleteFile:
        deleteFile(row);
        break;
    }
}

/**
 * @brief Action when the user rejects the changes
 *
 */
void MultizoneProjectDialog::on_buttonBox_rejected()
{
    _isMultizoneProjectCreated = false;
    close();
}



/**
 * @brief
 *
 */
void MultizoneProjectDialog::accept()
{
    if (!_multizoneProjectData.isEmpty())
     {
         UtilFiles _fileHelper;
         bool _buildSuccess = false;
         _buildSuccess = buildFilesInProject();
         if (_buildSuccess)
         {
             _fileHelper.updateMultizoneProjectDataToFile(_multizoneProjectData,_projectDetailsData);
             _isMultizoneProjectCreated = true;
             close();
         }

     }
}

/*********************************************ENABLE/DISABLE *****************************************/

/**
 * @brief Function that enables the different parameters for the multizone
 *
 */
void MultizoneProjectDialog::enableMultizoneParameters()
{
    ui->MainTitle->setEnabled(true);
    ui->multizoneNoLabel->setEnabled(true);
    ui->multizoneNoSpinBox->setEnabled(true);
    ui->configFilesTitle->setEnabled(true);
    ui->configTable->setEnabled(true);
}





/********************************************* UPDATE *****************************************/

/**
 * @brief Update the number of zones in the correspondent cell
 *
 */
void MultizoneProjectDialog::updateZoneNumberInCell()
{
    int _totalRows = ui->configTable->rowCount();
    for (int _idRow = 0; _idRow < _totalRows;_idRow++)
        if (_idRow%2 == 0)
        {
            QTableWidgetItem * _zoneItem = (QTableWidgetItem  * ) ui->configTable->item(_idRow,ColumnZoneNo);
            if (_zoneItem)
            {
                int _cPos = _idRow/2;
                _zoneItem->setText(QString::number(_cPos));
                ui->configTable->setItem(_idRow,ColumnZoneNo,_zoneItem);
            }
        }

}


/********************************************* DELETE/DECREASE *****************************************/


/**
 * @brief Function that launches to delete a file (when a config. file is deleted, the correspondent
 * mesh file is also deleted
 *
 * @param _row
 */
void MultizoneProjectDialog::deleteFile(int _row)
{
    int _lastRow = ui->configTable->rowCount();
    if (_row < _lastRow)
    {
        QString _meshFileName = ui->configTable->item(_row+1,ColumnFileName)->text();
        if (!_meshFileName.isEmpty())
            if (_importedFilesList.contains(_meshFileName))
                _importedFilesList.removeOne(_meshFileName);
        QString _configFileName = ui->configTable->item(_row,ColumnFileName)->text();
        if (!_configFileName.isEmpty())
            if (_importedFilesList.contains(_configFileName))
                _importedFilesList.removeOne(_configFileName);

        ui->configTable->removeRow(_row+1);
        ui->configTable->removeRow(_row);
        decreaseZoneNumber();
        updateZoneNumberInCell();
    }


}


/**
 * @brief Action that controls the decreasing number in the spinbox
 *
 */
void MultizoneProjectDialog::decreaseZoneNumber()
{
    int _cvalue = ui->multizoneNoSpinBox->value();
    ui->multizoneNoSpinBox->setValue(_cvalue-1);
}


/********************************************* GETTER/SETTER/IS *****************************************/


/**
 * @brief Function that checks if the mesh file name exists or not
 *
 * @param _file
 * @return bool
 */
bool MultizoneProjectDialog::isMeshFileNameValid(QString _file)
{
    QFile _fileName(_file);
    if (_fileName.exists())
        return true;
    else
        return false;

}

/**
 * @brief This fucntion checks if there are differences among the boundary names read in the mesh file and those read
 *  from the configuration file. If there are differences a warning dialog is launched.
 *
 * @param _configFilePath
 * @param _meshFilePath
 * @param _solverManager
 */
bool MultizoneProjectDialog::checkConsistency(QString _configFilePath, QString _meshFilePath,
                                              SolverConfigManager _solverManager)
{
    bool _isConsistent = true;
    // Load Config File
    ConfigurationFileParser _configParser;
   _configParser.setFilePath(_configFilePath);
    _configParser.parseFileProcess();
    QHash<QString,QString>  _userConfigVal = _configParser.getParsedValues();
    _virtualMRFData = _configParser.getParsedVirtualMRFData();


    // Load Mesh File Values
   MeshFileParser _meshParser;
    _meshParser.setFilePath(_meshFilePath);
    _meshParser.parseFileProcess();
    QHash<QString,QString> _meshBoundaryVal = _meshParser.getParsedValues();

    // Check if there are differences
    SolverMarkerMenuHelper _markerHelper;
    if ((!_userConfigVal.isEmpty()) && (! _meshBoundaryVal.isEmpty()) )
    {
        bool _areEqual = _markerHelper.checkMarkerMeshVariables(_userConfigVal,_meshBoundaryVal);
        if (!_areEqual)
        {
            WarningMultizoneDialog  _warningDialog;
            _warningDialog.initializeFileNames(_configFilePath,_meshFilePath);
            _warningDialog.setConfigValues(_userConfigVal);
            _warningDialog.setMeshValueNames(_meshBoundaryVal);
            _warningDialog.exec();
            if (!_warningDialog.userAcceptChanges())
                _isConsistent = false;
            else{
                if ( (_warningDialog.userAcceptChanges()) && (_warningDialog.isNeedToUpdate()) )
                {                     
                     _userConfigVal = _warningDialog.getConfigValues();
                     _userConfigVal.insert("MESH_FILENAME",_meshFilePath);
                }
                // Saving the file
                _solverManager.saveConfigFile(_configFilePath,_userConfigVal,_projectDetailsData,_virtualMRFData);
            }
        }
    }
    return _isConsistent;

}

/**
 * @brief Returns if the multizone project is created or not
 *
 * @return bool
 */
bool MultizoneProjectDialog::isMultizoneProjectCreated()
{
    return _isMultizoneProjectCreated;
}

/**
 * @brief Get the associated information of the multizone project
 *
 * @return MultizoneProjectData
 */
MultizoneProjectData MultizoneProjectDialog::getMultizoneProjectData()
{
    return _multizoneProjectData;
}

/**
 * @brief  Set project details
 *
 * @param _projectDetails
 */
void MultizoneProjectDialog::setProjectDetails(ProjectDetails _projectDetails)
{
    _projectDetailsData = _projectDetails;
}




