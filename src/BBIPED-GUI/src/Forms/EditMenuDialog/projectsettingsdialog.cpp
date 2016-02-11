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
 *
 *  Author: Dec 2016: Carmen Alonso Montes calonso@bcamath.org
***/
#include "projectsettingsdialog.h"
#include "ui_projectsettingsdialog.h"

#include <QFileInfo>
#include "Utility/qtableutil.h"
#include "Defines/General/GeneralDefines.h"
#include "Utility/showmessagehelper.h"

#define PROJECT_TYPE_NORMAL "Solver project"
#define PROJECT_TYPE_MULTIZONE "Solver MULTIZONE project"


/**
 * @brief Constructor
 *
 * @param parent
 */
ProjectSettingsDialog::ProjectSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProjectSettingsDialog)
{
    ui->setupUi(this);
    _userAcceptChanges = false;
}

/**
 * @brief Destructor
 *
 */
ProjectSettingsDialog::~ProjectSettingsDialog()
{
    delete ui;

}

/*********************************** UPDATE ***********************/
/**
 * @brief This fucntion updates the project information in the Dialog
 *
 */
void ProjectSettingsDialog::updateProjectInformationInSettingsView()
{
    QString _projectName;
    QString _projectFileType;
    if (!_projectData.isEmpty())
    {
        _projectName = _projectData.getProjectFileName();
        _projectFileType = PROJECT_TYPE_NORMAL;

    }else{
        if (!_multizoneData.isEmpty())
        {
            _projectName = _multizoneData.getProjectName();
            _projectFileType = PROJECT_TYPE_MULTIZONE;
        }
    }
    if ((!_projectName.isEmpty()) && (!_projectFileType.isEmpty()))
    {
        ui->projectNameLabel->setText(_projectName);
        ui->projectTypeLabel->setText(_projectFileType);
    }
}

/**
 * @brief This function updates the information in teh table
 *
 */
void ProjectSettingsDialog::updateFileTableInSettingsView()
{
    QStringList _listConfigFiles;
    QStringList _listMeshFiles;
    if (!_projectData.isEmpty())
    {
        QString _configFile = _projectData.getSolverDataInformation().getSolverConfigFilePath();
        QString _meshFile = _projectData.getSolverDataInformation().getSolverFilePath();
        _listConfigFiles.append(QFileInfo(_configFile).fileName());
        _listMeshFiles.append(QFileInfo(_meshFile).fileName());
    }else
    {
        if (!_multizoneData.isEmpty())
        {
            _listConfigFiles = _multizoneData.getListConfigFileNames();
            _listMeshFiles = _multizoneData.getListMeshFileNames();
        }

    }
    addValuesToTable(_listConfigFiles,_listMeshFiles);
    ui->associatedFileTable->resizeColumnsToContents();

}



/**
 * @brief This function add the values to the table
 *
 * @param _list1
 * @param _list2
 */
void ProjectSettingsDialog::addValuesToTable(QStringList _list1, QStringList _list2)
{
    bool _isDifSize = false;
    int _listSize = _list1.size();
    // Check if the lists are not equal
    if ((_list1.size() != _list2.size()))
        _isDifSize = true;
    if ( (_isDifSize) && (_list1.size()>_list2.size()) )
      _listSize = _list2.size();

    for(int i= 0; i < _listSize; i++)
    {
        QString _configFileName = _list1.at(i);
        QString _meshFileName = _list2.at(i);
        int _cRow = i*2;
        addRowToTable(_cRow,_cRow/2, _configFileName);
        _cRow++;
        addRowToTable(_cRow,_cRow/2, _meshFileName);
    }
    // Completing the
    if (_isDifSize)
    {
        bool _addToConfig =false;
        bool _addToMesh = false;
        int _difSizeVal = 0;
        int _startPoint = _listSize;
        if (_list1.size() > _list2.size() )
        {
            _difSizeVal = _list1.size() - _list2.size();
            _addToConfig = true;
        }else
        {
            _difSizeVal = _list2.size() - _list1.size();
            _addToMesh = true;
        }
        for (int j = _startPoint; j < (_difSizeVal+_startPoint);j++)
        {
            QString _configFileName("Not Defined");
            if (_addToConfig)
                _configFileName = _list1.at(j);
            QString _meshFileName("Not Defined");
            if (_addToMesh)
                _meshFileName = _list2.at(j);
            int _cRow = j*2;
            addRowToTable(_cRow,_cRow/2, _configFileName);
            _cRow++;
            addRowToTable(_cRow,_cRow/2, _meshFileName);
        }

    }

}

/** This function add the row to the table for hte correspondent file
 * @brief
 *
 * @param _idRow
 * @param _idZone
 * @param _fileName
 */
void ProjectSettingsDialog::addRowToTable(int _idRow, int _idZone, QString _fileName)
{
    QtableUtil _tableHelper;
    ui->associatedFileTable->insertRow(_idRow);
    ui->associatedFileTable->setItem(_idRow,SettingsColZoneNo,_tableHelper.addNonEditableText(QString::number(_idZone)) );
    ui->associatedFileTable->setItem(_idRow,SettingsColFileType,_tableHelper.addNonEditableText(_fileName) );
    // Only the configuration files have the possibility to be deleted
    // The mesh files must be deleted or changed in the normal Solver Menu
    if (_idRow%2 == 0)
        ui->associatedFileTable->setCellWidget(_idRow,SettingsColDelFile,_tableHelper.addIcon(IconDelete) );
    else
    {
        ui->associatedFileTable->item(_idRow-1,SettingsColZoneNo)->setTextAlignment(Qt::AlignCenter);

        ui->associatedFileTable->setSpan(_idRow-1,SettingsColDelFile,2,1);
        ui->associatedFileTable->setSpan(_idRow-1,SettingsColZoneNo,2,1);
    }
}

/**
 * @brief This function updates the project data information according user changes
 *
 */
void ProjectSettingsDialog::updateProjectData()
{
    if (!_projectData.isEmpty())
    {
        SolverVO _solverData = _projectData.getSolverDataInformation();
        QString _configFileName = QFileInfo(_solverData.getSolverConfigFilePath()).fileName();
        QString _meshFileName = QFileInfo(_solverData.getSolverFilePath()).fileName();
        if ( (QString::compare(_configFileName,_removedConfigFiles.at(0)) == StrEqual)
            && (QString::compare(_meshFileName,_removedMeshFiles.at(0)) == StrEqual) )
            _solverData.clear();
        _projectData.setSolverDataInformation(_solverData);

    }else
    {
        if (!_multizoneData.isEmpty())
        {
            QStringList _configFileList = _multizoneData.getListConfigFiles();
            QStringList _configFileListNames = _multizoneData.getListConfigFileNames();
            QStringList _meshFileList = _multizoneData.getListMeshFiles();
            QStringList _meshFileListNames = _multizoneData.getListMeshFileNames();

            for (int i = 0; i <  _removedConfigFiles.size();i++)
            {
                int _posToRemove = _configFileListNames.indexOf(_removedConfigFiles.at(i));
                _configFileList.removeAt(_posToRemove);
                _configFileListNames.removeAt(_posToRemove);
            }

            for (int i = 0; i <  _removedMeshFiles.size();i++)
            {
                int _posToRemove = _meshFileListNames.indexOf(_removedMeshFiles.at(i));
                _meshFileList.removeAt(_posToRemove);
                _meshFileListNames.removeAt(_posToRemove);
            }
            _multizoneData.setListConfigFiles(_configFileList);
            _multizoneData.setListMeshFiles(_meshFileList);
        }

    }


}

/**
 * @brief Update the values of the project details in the tab view
 *
 */
void ProjectSettingsDialog::updateProjectDetailsInProjectDetailsTabView()
{
    ui->projectTitleLineEdit->setText(_projectDetailsData.getProjectTile());
    ui->authorLineEdit->setText(_projectDetailsData.getAuthor());
    ui->startingDateLineEdit->setText(_projectDetailsData.getStartingDate());
    ui->otherDetailsTexEdit->setPlainText(_projectDetailsData.getOtherDetails());
}

/***************************************** SLOTS/Signals *************************/

/**
 * @brief This action controls the actions when the user clicks in teh cell to remove a file (removing means remove
 * both configuration file and mesh file)
 *
 * @param row
 * @param column
 */
void ProjectSettingsDialog::on_associatedFileTable_cellClicked(int row, int column)
{
    if  (column == SettingsColDelFile)
    {
        ShowMessageHelper _msgHelper;
        bool _userWantsToDelete =_msgHelper.showWarningMessage("Remove Config & Associated Mesh File from Project","You are about to remove the link of the configuration file in this project and its corresponding mesh file. You will still keep them in the folder but not for the simulation. Are you sure?");

        if (_userWantsToDelete)
        {
            QString _configFile = ui->associatedFileTable->item(row,SettingsColFileType)->text();
            QString _meshFile =  ui->associatedFileTable->item(row+1,SettingsColFileType)->text();
            _removedConfigFiles.append(_configFile);
            _removedMeshFiles.append(_meshFile);

            ui->associatedFileTable->removeRow(row+1);
            ui->associatedFileTable->removeRow(row);

        }

    }
}

/**
 * @brief Action when the user accepts the changes
 *
 */
void ProjectSettingsDialog::on_buttonBox_accepted()
{
     _userAcceptChanges = true;
     if( (!_removedConfigFiles.isEmpty()) && (!_removedMeshFiles.isEmpty()))
        updateProjectData();

     // Update the Project details
     _projectDetailsData.setProjectTile(ui->projectTitleLineEdit->text());
     _projectDetailsData.setAuthor(ui->authorLineEdit->text());
     _projectDetailsData.setStartingDate(ui->startingDateLineEdit->text());
     _projectDetailsData.setOtherDetails(ui->otherDetailsTexEdit->toPlainText());

     close();
}

/**
 * @brief Action when the user rejects the changes
 *
 */
void ProjectSettingsDialog::on_buttonBox_rejected()
{
    _userAcceptChanges = false;
    close();
}


/****************************** GETTER /SETTER *****************************/
/**
 * @brief Set the project data  and  update the correspondent views
 *
 * @param _data
 */
void ProjectSettingsDialog::setProjectData(ProjectData _data)
{
    _projectData = _data;
    updateProjectInformationInSettingsView();
    updateFileTableInSettingsView();
}

/**
 * @brief Returns the project data
 *
 * @return ProjectData
 */
ProjectData ProjectSettingsDialog::getProjectData()
{
    return _projectData;
}

/**
 * @brief Set the multizone project data  and  update the correspondent views
 *
 * @param _data
 */
void ProjectSettingsDialog::setMultizoneProjectData(MultizoneProjectData _data)
{
    _multizoneData = _data;
    updateProjectInformationInSettingsView();
    updateFileTableInSettingsView();
}

/**
 * @brief Returns the multizone data
 *
 * @return MultizoneProjectData
 */
MultizoneProjectData ProjectSettingsDialog::getMultizoneProjectData()
{
    return _multizoneData;
}
/**
 * @brief Returns true if the user accept the changes
 *
 * @return bool
 */
bool ProjectSettingsDialog::userAcceptChanges()
{
    return _userAcceptChanges;
}

/**
 * @brief Store the project Details
 *
 * @param _prjDetails
 */
void ProjectSettingsDialog::setProjectDetailsData(ProjectDetails _prjDetails)
{
    _projectDetailsData = _prjDetails;
    updateProjectDetailsInProjectDetailsTabView();
}


/**
 * @brief Returns  the project Details
 *
 * @return ProjectDetails
 */
ProjectDetails ProjectSettingsDialog::getProjectDetailsData()
{
    return _projectDetailsData;
}
