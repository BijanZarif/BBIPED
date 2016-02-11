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

#include <QPushButton>
#include <QTabWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QList>
#include <QWidget>
#include <QFileInfo>
#include <QDir>
#include <QDomDocument>

#include "Utility/qtableutil.h"
#include "Utility/showmessagehelper.h"
#include "Forms/ProjectDialog/multizoneselectfilesdialog.h"
#include "Defines/General/GeneralDefines.h"
#include "Utility/utilfiles.h"
#include "WindowHelper/Multizone/multizoneindividualtabform.h"
#include "Solver/solverconfigwriter.h"
#include "Forms/SummaryDialog/multizonesummarydialog.h"



enum positionMultizoneColumns{
    configFileSelected =  0,
    configFileName = 1,
    meshFileName =2
};



/**
 * @brief This function register all the elements of the multizone interaface (buttons, editors, etc)
 *
 */
void MainWindow::registerUIMultizoneElements()
{
    _mManager.registerUIElements(ui->runSimulationButton,ui->setMeshPropertiesButton,ui->confFileLabel,ui->meshFileLabel,
                                 ui->actionRun_Project,ui->actionCompare_Configuration_Files,
                                 //ui->currentFilesGroupBox,
                                 ui->viewGroupBox,
                                 ui->setMeshPropertiesGroup,ui->configFileLineLabel,ui->meshFileLineLabel,ui->projectTreeView);
}

/*************** MENU ELEMENTS *********************************/
/**
 * @brief This function is to control the action of defining a new Multizone project
 *
 */
void MainWindow::on_actionNew_Multizone_Project_triggered()
{
    if (!_mManager.isUIRegistered())
        registerUIMultizoneElements();
    bool _isMultizoneProjectCreated = _mManager.launchMultizoneDialog(&_multizoneProjectData,&_userPreferencesData,&_projectData);
    if (_isMultizoneProjectCreated)
    {
        resetMultizoneTable();
        enableMultizoneElements();
        fillMultizoneTable();
    }

}



/**
 * @brief This funciton opens a Multizone project
 *
 */
void MainWindow::on_actionOpen_Multizone_Project_triggered()
{
    closeExistentProject();
    resetMultizoneTable();

    if (!_mManager.isUIRegistered())
        registerUIMultizoneElements();

    QString _projectFilePath = QFileDialog::getOpenFileName(this, tr("Open Multizone Project File"),
                                                            QCoreApplication::applicationDirPath(),//QDir::currentPath(),
                                                            tr("Multizone Solver Project (*.mprj)"));

    if (_projectFilePath.isNull())    
        // Button Cancel Pressed
        return;

    _status_NeedToBeSaved =  _mManager.openMultizoneProject(_projectFilePath,&_multizoneProjectData,&_userPreferencesData,&_projectData,&_projectDetailsData);

    enableMultizoneElements();
    fillMultizoneTable();
}

/**
 * @brief This function controls the enabling of all the elements associated with the Multizone interface
 *
 */
void MainWindow::enableMultizoneElements()
{
    ShowMessageHelper _showHelper;
    _showHelper.showMessage("Open Multizone Project:"+ _multizoneProjectData.getProjectPath() , statusBar());
    _projectExists = true;
    _isMultizoneProject = true;
    setEnableCloseProjectIcons(true);
    enableAllMainButtons();
    if(_status_NeedToBeSaved)
        setEnableSaveProjectIcons(true);
    ui->restartButton->setEnabled(false);

    // Disable the Input Tab
    ui->solverTabwidget->setTabEnabled(0,false);
    ui->solverTabwidget->setTabEnabled(1,true);
}




/***************************** Buttons **********************************************/
void MainWindow::fillMultizoneTable()
{

    int _noFiles = _multizoneProjectData.getListConfigFileNames().size();
    QStringList _configFileNames = _multizoneProjectData.getListConfigFileNames();
    QStringList _meshFileNames = _multizoneProjectData.getListMeshFileNames();

    for (int i = 0; i < _noFiles;i++)
    {
        QString _meshName("");
        if (_meshFileNames.size()> i)
          _meshName = _meshFileNames.at(i);
         insertMultizoneRow(i,_configFileNames.at(i), _meshName);
    }

    ui->multizoneFilesTable->resizeColumnsToContents();
    ui->multizoneFilesTable->resizeRowsToContents();


}

void MainWindow::insertMultizoneRow(int _pos, QString _configName, QString _meshName)
{
    QtableUtil _tableUtil;
    ui->multizoneFilesTable->insertRow(_pos);
    ui->multizoneFilesTable->setItem(_pos,configFileSelected,_tableUtil.addSelectableCheckColumn());
    ui->multizoneFilesTable->setItem(_pos,configFileName,_tableUtil.addNonEditableText(_configName));
    ui->multizoneFilesTable->setItem(_pos,meshFileName,_tableUtil.addNonEditableText(_meshName));
}

void MainWindow::resetMultizoneTable()
{
    int _totalRows = ui->multizoneFilesTable->rowCount();
    for (int i = _totalRows; i>=0;i--)
          ui->multizoneFilesTable->removeRow(i);

}


/***************************** Buttons **********************************************/
void MainWindow::on_multizoneNewButton_clicked()
{
    // Launch Menu Dialog for selecting the Configuration File and the Mesh File

    MultizoneSelectFilesDialog _multiZoneSelectFilesDialog;
    _multiZoneSelectFilesDialog.setProjectPath(_multizoneProjectData.getProjectPath());
    _multiZoneSelectFilesDialog.exec();

    if (!_multiZoneSelectFilesDialog.hasSelectedFiles())
        return;
    QString _configFile = _multiZoneSelectFilesDialog.getConfigFilePath();
    QString _meshFile = _multiZoneSelectFilesDialog.getMeshFilePath();
    if (_configFile.isEmpty()) return;
    // Update the MultizoneObject
    updateMultizoneUserData(_configFile,_meshFile);

    // Add the data into the table
    insertMultizoneRow(ui->multizoneFilesTable->rowCount(),QFileInfo(_configFile).fileName(),
                       QFileInfo(_meshFile).fileName());
}

void MainWindow::updateMultizoneUserData(QString _configFile,QString _meshFile)
{
    QFileInfo _configFileInfo(_configFile);
    QFileInfo _meshFileInfo(_meshFile);
    // Checking the paths of the files, if they are different to the project path they must be copied here
    QString _projectPath = QDir(_multizoneProjectData.getProjectPath()).filePath(TOOL4);
    QString _configPath = _configFileInfo.absolutePath();
    QString _meshFilePath = _meshFileInfo.absolutePath();
    UtilFiles _utilFilesHelper;
    if (!_configFile.isEmpty())
    {
            //_inputDataTabHelper->parseFile(ParserSolverConfigFile,_configFile,_configVal,_bndMeshVal,_virtualMRFList);
            if ((QString::compare(_projectPath,_configPath) != StrEqual))
                _configFile = _utilFilesHelper.importFile(_multizoneProjectData.getProjectPath(),_configFile,TOOL4);
            else
            {
                // Create the _configFile
                 SolverConfigWriter _writer;
                 _writer.writeToConfigurationFile(_configFile, new QDomDocument(), _projectDetailsData);

            }

            if (!_meshFile.isEmpty())// && ( QString::compare(_projectPath,_meshFilePath) != StrEqual))
            {
                _meshFile =  _utilFilesHelper.importFile(_multizoneProjectData.getProjectPath(),_meshFile,TOOL4);
                QHash<QString,QString> _configValues;
                _configValues.insert("MESH_FILENAME",_meshFile);
                _solverManager.saveConfigFile(_configFile,_configValues,_projectDetailsData,_userVirtualMRFDataList);
            }
     }


    // Update Multizone Information
    _multizoneProjectData.addConfigFile(_configFile);
    _multizoneProjectData.addMeshFile(_meshFile);
    _status_NeedToBeSaved = true;
}


void MainWindow::on_multizoneOpenEditButton_clicked()
{
    QList<int> _selRows = getSelectedFiles();
    if (_selRows.isEmpty())
        return;
    QString _projectPath =  QDir(_multizoneProjectData.getProjectPath()).filePath(TOOL4);
    for (int i = 0; i < _selRows.size();i++)
    {
        QString _configFileName = ui->multizoneFilesTable->item(_selRows.at(i),configFileName)->text();
        QString _meshFileName = ui->multizoneFilesTable->item(_selRows.at(i),meshFileName)->text();

        MultizoneIndividualTabForm * _newTab = new MultizoneIndividualTabForm(ui->solverTabwidget);
        _newTab->setProjectPath(_projectPath);
        _newTab->setConfigFile(QDir(_projectPath).filePath(_configFileName));
        _newTab->setMeshFile(QDir(_projectPath).filePath(_meshFileName));
        // TODO --> Tengo que cargar y leer cada configuration file y su correspondientes boundary variables
        _newTab->registerInitialStructures(this,&_solverManager);
        ui->solverTabwidget->addTab(_newTab,_configFileName);
        ui->solverTabwidget->setTabsClosable(true);       
    }
    // Put the focus on the last tab open
    int _lastTab = ui->solverTabwidget->count();
    ui->solverTabwidget->setCurrentIndex(_lastTab-1);

}


QList<int> MainWindow::getSelectedFiles()
{
    int _totalRows = ui->multizoneFilesTable->rowCount();
    QList<int> _selectedElements;
    for (int i= 0; i< _totalRows;i++)
    {
       if (ui->multizoneFilesTable->item(i,configFileSelected)->checkState() == Qt::Checked)
           _selectedElements.append(i);
    }
    return _selectedElements;
}

void MainWindow::on_multizoneDeleteButton_clicked()
{
    QList<int> _selRows = getSelectedFiles();
    if (_selRows.isEmpty())
        return;
    for (int i = _selRows.size()-1; i>=0;i--)
    {
        // Update the Multizone Project Information
        QString _configFile = QDir(_multizoneProjectData.getProjectPath()).
                filePath(ui->multizoneFilesTable->item(_selRows.at(i),configFileName)->text());
        QString _meshFile  = QDir(_multizoneProjectData.getProjectPath()).
                filePath(ui->multizoneFilesTable->item(_selRows.at(i),meshFileName)->text());
        _multizoneProjectData.removeConfigFile(_configFile);
        _multizoneProjectData.removeMeshFile(_meshFile);
        ui->multizoneFilesTable->removeRow(_selRows.at(i));
    }
    _status_NeedToBeSaved = true;

}

void MainWindow::on_multizoneSummaryAllConfigButton_clicked()
{
    MultizoneSummaryDialog _multizoneSummary;
    _multizoneSummary.setProjectPath(_multizoneProjectData.getProjectPath());
    _multizoneSummary.setConfigFileList(_multizoneProjectData.getListConfigFiles());
    _multizoneSummary.setSolverManager(&_solverManager);
    _multizoneSummary.exec();
}

void MainWindow::on_multizoneSaveAllButton_clicked()
{

   if (notSavedMultizoneData())
        multizoneSaveAll();

}

bool MainWindow::notSavedMultizoneData()
{
    if (_status_NeedToBeSaved)
        return true;
    int _noTabs = ui->solverTabwidget->count();
    if (_noTabs > 3)
    {
        for (int i=3; i < _noTabs;i++)
        {
           MultizoneIndividualTabForm * _indivTab = qobject_cast<MultizoneIndividualTabForm *>(ui->solverTabwidget->widget(i));
           if (_indivTab)
           {
               if (_indivTab->isDataModified())
                   return true;
           }
        }
    }
    return false;
}

void MainWindow::multizoneSaveAll()
{
    int _noTabs = ui->solverTabwidget->count();
    QList<int> _indexTabsToSave;

    if (_noTabs > 3)
    {
        for (int i=3; i < _noTabs;i++)
        {
           MultizoneIndividualTabForm * _indivTab = qobject_cast<MultizoneIndividualTabForm *>(ui->solverTabwidget->widget(i));
           if (_indivTab)
           {
               if (_indivTab->isDataModified())
                   _indexTabsToSave.append(i);
           }
        }
        if (_indexTabsToSave.isEmpty())
            return;
        // Show a message per each Tab
        ShowMessageHelper _msgHelper;
        bool _saveAll = _msgHelper.showWarningMessage("Modified Values in the Zones!","Warning! There unsaved in the files you have open. Do you want to save all of them?");
        if (!_saveAll)
            return;
        for (int i=0; i < _indexTabsToSave.size();i++)
        {
            MultizoneIndividualTabForm * _indivTab = qobject_cast<MultizoneIndividualTabForm *>
                    (ui->solverTabwidget->widget(_indexTabsToSave.at(i)));
            if (_indivTab)
            {
                saveMultizoneData(_indivTab->getConfigFile(), _indivTab->getMeshFile(),_indivTab->getConfigValues(),_indivTab->getVirtualValues());
                _indivTab->savedData();
            }

        }
        _status_NeedToBeSaved = false;
    }

}

void MainWindow::on_multizoneRunSimulationButton_clicked()
{
    clearConsoleText();

    // Check if we need to save the Status

    if (notSavedMultizoneData())
        multizoneSaveAll();

    // Run the project
    runSolverProject(_multizoneProjectData.getProjectPath());

    ui->multizoneRunSimulationButton->setEnabled(false);
    ui->solverTabwidget->setCurrentIndex(tabResidual);
}




void MainWindow::on_multizoneStopSimulationButton_clicked()
{
    if (_myprocess->pid() > 0)
        _myprocess->kill();
}

void MainWindow::on_multizoneRestartSimulation_clicked()
{
    // TODO --> TO BE confirmed before the logic in the restart in BBIPED!

}


void MainWindow::saveMultizoneData(QString _configFile, QString _meshFile, QHash<QString,QString> * _configVal,
                                   QList<VirtualMrfVO> * _virtualMrfList)
{
    _multizoneProjectData.updatePair(_configFile,_meshFile);
    _solverManager.saveConfigFile(_configFile,*_configVal,_projectDetailsData,*_virtualMrfList);
}
