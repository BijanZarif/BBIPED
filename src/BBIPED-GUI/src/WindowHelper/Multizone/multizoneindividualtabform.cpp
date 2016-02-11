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
#include "multizoneindividualtabform.h"
#include "ui_multizoneindividualtabform.h"
#include "Defines/General/GeneralDefines.h"

#include <QFileInfo>
#include <QTabWidget>
#include <QStackedWidget>
#include <QHash>
#include <QHashIterator>
#include "Defines/General/GeneralDefines.h"
#include "Defines/Tool/Solver/SolverDefines.h"
#include "Utility/showmessagehelper.h"
#include "Utility/utilfiles.h"
//#include "Solver/TabHelper/inputdatatabhelper.h"
#include "WindowHelper/MainWindow/inputdatatabhelper.h"
#include "Forms/SolverMenuDialog/meshdialog.h"

MultizoneIndividualTabForm::MultizoneIndividualTabForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MultizoneIndividualTabForm)
{
    ui->setupUi(this);

    _configFile ="";
    _meshFile ="";
    _needToSave = false;
    _configVal = new QHash<QString,QString>();
    _bndMeshVal = new QHash<QString,QString>();
    _virtualMRFList = new QList<VirtualMrfVO>();
    _bndValuesSolver = new QHash<QString,QString>();

    // set up the initial view
    ui->saveButton->setEnabled(false);

}

MultizoneIndividualTabForm::~MultizoneIndividualTabForm()
{
    delete ui;
}

void MultizoneIndividualTabForm::on_editConfigFileButton_clicked()
{
    // TODO --> The 3D configuration for VMRF is disabled, only 2D accepted at the moment
    // MUst check how is the behaviour in Multizone
    _needToSave = _regTabHelper->launchSolverConfigDialog(_parentWindow,true,
                                                               _configVal,_solverManager,_virtualMRFList,false);
    if (_needToSave)
    {
        ui->saveButton->setEnabled(true);
        addSaveIconTab(iconSaveSepia);
    }
}

void MultizoneIndividualTabForm::addSaveIconTab(int _option)
{
    QTabWidget * _tabWidget = qobject_cast<QTabWidget *> (parent()->parent());

    if (_tabWidget)
    {
        int _index = _tabWidget->indexOf(this);

        QIcon _icon;
        switch (_option)
        {
            case iconSaveSepia:
                _icon.addFile(":/new/toolbarIcons/images/disk_sepia.png");
                break;
            otherwise:
                _icon.addFile("");
                break;
        }
        _tabWidget->setTabIcon(_index,_icon);
    }
}

void MultizoneIndividualTabForm::on_importMeshFileButton_clicked()
{

    if (!_meshFile.isEmpty())
    {
        ShowMessageHelper _msgHelper;
        bool _overWrite = _msgHelper.showWarningMessage("Mesh File Overwrite!","If you import a new file, you will loose all your previous configuration. Do you want to continue?");
        if (!_overWrite)
            return;
    }
    QString _meshFilePath = QFileDialog::getOpenFileName(_parentWindow,"Import SU2 Mesh File",
                                                            QDir(_projectPath).filePath(TOOL4),
                                                            "SU2 Mesh File (*.su2)");
    if (_meshFilePath.isNull())
         return;

    // Step 1. Import the mesh File into the project folder
    UtilFiles _utilHelper;
    _meshFile = _utilHelper.importFile(QFileInfo(_projectPath).path(),_meshFilePath,TOOL4);

    // Step 2. Reset previous Bnd Values
    _bndMeshVal->clear();
    _bndValuesSolver->clear();
     removeAssociatedMeshValues();

    // Step 2. Parse the mesh file to get the bnd Variables
    _regTabHelper->parseFile(ParserSolverMeshFile,_meshFile,_configVal,_bndMeshVal,_virtualMRFList);
    _configVal->insert("MESH_FILENAME",_meshFile);
    // Step 4. Update the view
    ui->currentMeshFileLabel->setText(QFileInfo(_meshFile).fileName());
    _needToSave = true;
    ui->saveButton->setEnabled(true);
    addSaveIconTab(iconSaveSepia);
}

void MultizoneIndividualTabForm::removeAssociatedMeshValues()
{
    if (!_configVal->isEmpty())
    {
        QHashIterator<QString,QString> _iter(*_configVal);
        while(_iter.hasNext())
        {
            _iter.next();
            QString _keyName = _iter.key();
            if( (_keyName.contains(MARKER_PREFIX)) || (_keyName.contains(BC_PREFIX)) )
                 _configVal->insert(_keyName,"NONE");
        }
    }
}

void MultizoneIndividualTabForm::getLoadedBoundaryValues()
{
    if (!_configVal->isEmpty())
    {
        QHashIterator<QString,QString> _iter(*_configVal);

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
                    _bndValuesSolver->insert(_keyVal,_val);
                }
        }

    }

}
void MultizoneIndividualTabForm::on_editBoundConfigButton_clicked()
{
    if (_bndValuesSolver->isEmpty())
        getLoadedBoundaryValues();
    MeshDialog * _meshWindow = new MeshDialog(this);
    _meshWindow->setMeshValues(*_bndMeshVal);
    _meshWindow->setSolverConfigManager(_solverManager);
    _meshWindow->setUserConfiguredMeshValues(*_bndValuesSolver); //TBR
    _meshWindow->setIsMultizoneProject(true);
    bool _isNSProblem = false;
    if (!_configVal->isEmpty())
    {
        QString _physicalPbVal = _configVal->value("PHYSICAL_PROBLEM");
        if (QString::compare(_physicalPbVal,"NAVIER_STOKES") == StrEqual)
            _isNSProblem = true;
        else _isNSProblem = false;
    }
    _meshWindow->setIsNSProblem(_isNSProblem);
    _meshWindow->loadMenu();
    _meshWindow->exec();
    if (_meshWindow->hasUserAcceptChanges())
    {
        (* _bndValuesSolver) = _meshWindow->getUserBndVariableValues();
         updateBoundaryValues();
        _needToSave = true;
        ui->saveButton->setEnabled(true);
        addSaveIconTab(iconSaveSepia);
    }

}

void MultizoneIndividualTabForm::updateBoundaryValues()
{
    removeAssociatedMeshValues();
    if (!_bndValuesSolver->isEmpty())
    {
        QHashIterator<QString,QString> _iter(*_bndValuesSolver);
        while (_iter.hasNext())
        {
            _iter.next();
            QString _keyname = _iter.key();
            QString _value = _iter.value();
            _configVal->insert(_keyname,_value);
        }
    }
}

void MultizoneIndividualTabForm::on_saveButton_clicked()
{
    _parentWindow->saveMultizoneData(_configFile, _meshFile, _configVal,_virtualMRFList);
    savedData();
}

void MultizoneIndividualTabForm::savedData()
{
    ui->saveButton->setEnabled(false);
    addSaveIconTab(noIcon);
    _needToSave = false;
}

void MultizoneIndividualTabForm::on_closeButton_clicked()
{
  if (_needToSave)
  {
      ShowMessageHelper _warningMsg;
      bool _saveResults = _warningMsg.showWarningMessage("Unsaved Changes!","You need to save your modifications. Do you want to save?");
      if (_saveResults)
          _parentWindow->saveMultizoneData(_configFile, _meshFile, _configVal,_virtualMRFList);

  }
  // Closing the tab
  closeTab();
}

void MultizoneIndividualTabForm::closeTab()
{
    QTabWidget * _tabWidget = qobject_cast<QTabWidget *> (parent()->parent());

    if (_tabWidget)
    {
        int _index = _tabWidget->indexOf(this);
        _tabWidget->removeTab(_index);
        close();
    }
}

void MultizoneIndividualTabForm::on_cancelButton_clicked()
{
    if (_needToSave)
    {
        ShowMessageHelper _warningMsg;
        bool _saveResults = _warningMsg.showWarningMessage("Unsaved Changes!","You need to save your modifications. Do you want to save?");
        if (_saveResults)
            _parentWindow->saveMultizoneData(_configFile, _meshFile, _configVal,_virtualMRFList);
    }

    closeTab();
}


/*********************************************/
void MultizoneIndividualTabForm::setConfigFile(QString _filePath)
{
    _configFile = _filePath;
    ui->currentConfigFileLabel->setText(QFileInfo(_configFile).fileName());
    if (_configFile.isEmpty())
        return;
    _regTabHelper->parseFile(ParserSolverConfigFile,_configFile,_configVal,_bndMeshVal,_virtualMRFList);
}

void MultizoneIndividualTabForm::setMeshFile(QString _filePath)
{
    _meshFile = _filePath;
    ui->currentMeshFileLabel->setText(QFileInfo(_meshFile).fileName());
    if (_meshFile.isEmpty())
        return;
    _regTabHelper->parseFile(ParserSolverMeshFile,_meshFile,_configVal,_bndMeshVal,_virtualMRFList);

}

void MultizoneIndividualTabForm::registerInitialStructures(MainWindow * _object, SolverConfigManager * _manager)
{
     _parentWindow = _object;
     _solverManager = _manager;
}
void MultizoneIndividualTabForm::setProjectPath(QString _path)
{
    _projectPath = _path;
}

bool  MultizoneIndividualTabForm::isDataModified()
{
    return _needToSave;
}

QString MultizoneIndividualTabForm::getConfigFile()
{
    return _configFile;
}

QString MultizoneIndividualTabForm::getMeshFile()
{
    return _meshFile;
}

QHash<QString, QString> * MultizoneIndividualTabForm::getConfigValues()
{
    return _configVal;
}

 QList<VirtualMrfVO> * MultizoneIndividualTabForm::getVirtualValues()
{
    return _virtualMRFList;
}

