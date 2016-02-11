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
#include "multizonesummarydialog.h"
#include "ui_multizonesummarydialog.h"

#include <QFileInfo>
#include <QDir>
#include <QHashIterator>

#include "Utility/qtableutil.h"
#include "Defines/General/GeneralDefines.h"
#include "Data/solversectiondao.h"
#include "WindowHelper/MainWindow/inputdatatabhelper.h"


MultizoneSummaryDialog::MultizoneSummaryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MultizoneSummaryDialog)
{
    ui->setupUi(this);
    _selectedRow = -1;
}

MultizoneSummaryDialog::~MultizoneSummaryDialog()
{
    delete ui;
}


void MultizoneSummaryDialog::on_okButton_clicked()
{
    close();
}

void MultizoneSummaryDialog::on_selectConfigFilesTable_cellClicked(int row, int column)
{
    if (column == checkBoxColumn)
    {
        if (_selectedRow != -1)
            ui->selectConfigFilesTable->item(_selectedRow,checkBoxColumn)->setCheckState(Qt::Unchecked);
        QString _configFile = ui->selectConfigFilesTable->item(row,configFileNameCol)->text();
        resetTable();
        addConfigData(QDir(QDir(_projectPath).filePath(TOOL4)).filePath(_configFile));
        ui->fileName1Label->setText(_configFile);
        _selectedRow = row;
    }
}

void MultizoneSummaryDialog::resetTable()
{
    int _lastRow = ui->summaryFile1QTable->rowCount();
    for (int i = _lastRow-1; i >= 0; i--)
        ui->summaryFile1QTable->removeRow(i);

}

void MultizoneSummaryDialog::addConfigData(QString _file)
{
    InputDataTabHelper _helper;
    QHash<QString,QString> * _configVal = new QHash<QString,QString>();
    QHash<QString,QString> * _bndMeshVal = new QHash<QString,QString>();
    QList<VirtualMrfVO> * _virtualMRFList = new  QList<VirtualMrfVO>();

    _helper.parseFile(ParserSolverConfigFile,_file,_configVal,_bndMeshVal,_virtualMRFList);
    removeDefaultValues(_configVal);
    fillDataTable(_configVal,_virtualMRFList);

}

void MultizoneSummaryDialog::removeDefaultValues(QHash<QString,QString> * _configVal)
{
    QHash<QString,QString> _cleanedConfigValues;
    QHashIterator<QString,QString> iter(* _configVal);
    SolverSectionDAO _dao;
    if (!_solverManager->isInitialized())
        _solverManager->isInitialized();
    QList<SolverSectionVO> _menuList = _solverManager->getAllSectionMenuList();
    while(iter.hasNext())
    {
        iter.next();
        if (!iter.value().contains("NONE"))
            if ( (!_dao.isDefaultValueByFileName(_menuList, iter.key(), iter.value()))
                || (iter.key().contains("BC_")) || (iter.key().contains("MARKER")) )
                _cleanedConfigValues.insert(iter.key(),iter.value());
    }
    (* _configVal) = _cleanedConfigValues;
}

void MultizoneSummaryDialog::fillDataTable(QHash<QString,QString> * _mapValues, QList<VirtualMrfVO> * _virtualList)
{
    int _indxRow = ui->summaryFile1QTable->rowCount();
    if (!_mapValues->isEmpty())
    {
        QHashIterator<QString,QString> _iter (*_mapValues);
        while(_iter.hasNext())
        {
            _iter.next();
            QString _keyName =_iter.key();
            QString _value = _iter.value();
            insertRow(_indxRow,_keyName, _value, "");
            _indxRow++;
        }
    }

    if (!_virtualList->isEmpty())
    {
        for (int i = 0; i < _virtualList->size();i++)
        {
            VirtualMrfVO _virtualData = _virtualList->at(i);
            QString _keyName =_virtualData.getZoneName();
            QString _value = "Omega ="+_virtualData.getOmega() + ";Axis center ="+_virtualData.getAxisCenter().join(COMMA_SEPARATOR)
                    +";Axis normal =" +_virtualData.getAxisNormal().join(COMMA_SEPARATOR);
            insertRow(_indxRow,_keyName, _value, "");
            _indxRow++;
        }
    }
    ui->summaryFile1QTable->resizeColumnsToContents();
    ui->summaryFile1QTable->resizeRowsToContents();
}


void MultizoneSummaryDialog::insertRow(int _pos, QString _key, QString _value, QString _unit)
{
    QtableUtil _tableHelper;
    ui->summaryFile1QTable->insertRow(_pos);
    ui->summaryFile1QTable->setItem(_pos,variableNameCol,_tableHelper.addNonEditableText(_key));
    ui->summaryFile1QTable->setItem(_pos,variableValueCol,_tableHelper.addNonEditableText(_value));
    ui->summaryFile1QTable->setItem(_pos,variableUnitCol,_tableHelper.addNonEditableText(_unit));
}

void MultizoneSummaryDialog::fillConfigTable()
{
    int _noFiles = _configFileList.size();
    QtableUtil _tableUtil;
    for (int i=0; i<_noFiles;i++)
    {
        ui->selectConfigFilesTable->insertRow(i);
        ui->selectConfigFilesTable->setItem(i,checkBoxColumn,_tableUtil.addSelectableCheckColumn());
        ui->selectConfigFilesTable->setItem(i,configFileNameCol,_tableUtil.addNonEditableText(QFileInfo(_configFileList.at(i)).fileName()));
    }
    ui->selectConfigFilesTable->resizeColumnsToContents();
    ui->selectConfigFilesTable->resizeRowsToContents();
}

/***************************************************************************/
void MultizoneSummaryDialog::setConfigFileList(QStringList _list)
{
    _configFileList = _list;
    fillConfigTable();
}

void MultizoneSummaryDialog::setProjectPath(QString _path)
{
    _projectPath = _path;
}
void MultizoneSummaryDialog::setSolverManager(SolverConfigManager * _manager)
{
    _solverManager = _manager;
}
