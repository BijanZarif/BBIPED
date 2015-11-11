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
#include "meshdialog.h"
#include "ui_meshdialog.h"

#include "Utility/Model/DomModel/dommodel.h"
#include "WindowHelper/Dialog/SolverDialog/solverconfigmenuhelper.h"
#include "Defines/General/GeneralDefines.h"
#include "WindowHelper/Dialog/SolverDialog/solvermarkermenuhelper.h"
#include "Defines/Tool/Solver/SolverDefines.h"
#include "Data/solversectiondao.h"
#include "Forms/SolverMenuDialog/specificboundaryvariabledialog.h"
#include "Forms/helpdialog.h"
#include <QListWidget>
#include <QListWidgetItem>



/**
 * @brief Constructor
 *
 * @param parent
 */
MeshDialog::MeshDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MeshDialog)

{
    ui->setupUi(this);
    _signalMapper = new QSignalMapper(this);
    _signalMapperCheckList = new QSignalMapper(this);
    loadTableHeader();
    _markerManager = new MarkerMenuManager();
    _needToSave = false;
    _isNSProblem = false;
    _isMultizoneProject = false;
    _userAcceptChanges = false;
}

/**
 * @brief Destructor
 *
 */
MeshDialog::~MeshDialog()
{
    delete _markerManager;
    delete ui;
}

/**
 * @brief Function that performs the functinality when the user clicks accept button
 *
 */
void MeshDialog::on_meshButtonBox_accepted()
{
    buildUserBndValueMap();
    removeBoundaryNamesInAssociatedVariables();
    _userAcceptChanges = true;
    close();
}

void MeshDialog::removeBoundaryNamesInAssociatedVariables()
{
    QStringList _associatedVariables;
    for (int i = 0; i<_userMeshVariablesList.size();i++)
    {
        UserMeshVO _meshVar = _userMeshVariablesList.at(i);
        if (_meshVar.hasAssociatedMeshVariables())
            _associatedVariables.append(_meshVar.getAssociatedMeshVariables());
    }
    if (_associatedVariables.isEmpty())
        return;
    for (int j = 0; j< _associatedVariables.size();j++)
    {
        int _pos = findMeshVariableToUpdate(_associatedVariables.at(j),MARKER_CHILD_TITLE);
        if (_pos != -1)
        {
            UserMeshVO _associatedMeshVar =_userMeshVariablesList.at(_pos);
            QStringList _bndNamesToRemove = _associatedMeshVar.getBoundaryNameList();
            QString _keyName = _associatedMeshVar.getFilename();
            QStringList _storedValue =_userBndVariableValues.value(_keyName).split(COMMA_SEPARATOR);
            for (int k=0;k<_bndNamesToRemove.size();k++)
            {
                QString _bndName = _bndNamesToRemove.at(k);
                if (!_bndName.isEmpty())
                    _storedValue.removeAll(_bndName);
            }
           _userBndVariableValues.insert(_keyName,_storedValue.join(COMMA_SEPARATOR));
        }
    }

}

/*
 * for (int i = 0; i < _userMeshVariablesList.size();i++)
    {
        UserMeshVO _userMeshVar = _userMeshVariablesList.at(i);
        QString _parentMenu = _userMeshVar.getParentMenu();
        if (_selectedSubMenu.contains(_parentMenu))
        {
            QString _userVarName =_userMeshVar.getUserName();
            QStringList _bndNames = _userMeshVar.getBoundaryNameList();
            QStringList _bndValues = _userMeshVar.getBoundaryAssociatedValues();

            if (_selectedSubMenu.contains(MARKER_CHILD_TITLE))
            {
                if ((_userMeshVar.hasAssociatedMeshVariables()) && (isAllowedTheAssociatedMeshVariables(_userVarName)))
                {
                    QStringList _associatedMeshNameList = _userMeshVar.getAssociatedMeshVariables();
                    // Notice: At the moment we only consider the option of having ONLY ONE associated Mesh
                    // This is done as QStringList for the future, although the logic was not implemented
                    // .... In case of this need, it is needed to do the full logic (TODO)
                    int _pos = findMeshVariableToUpdate(_associatedMeshNameList.at(0),_selectedSubMenu);
                    if (_pos != -1)
                    {
                        UserMeshVO _associatedMeshVar = _userMeshVariablesList.at(_pos);
                        _bndValues =_associatedMeshVar.getBoundaryAssociatedValues();
                    }
                }
                updateTable(_userVarName,_bndNames,_bndValues);
            }
            else
                updateTableWithYesNoValues(_bndNames,_bndValues);
        }
    }
    */

void MeshDialog::buildUserBndValueMap()
{
    for (int i = 0; i<_userMeshVariablesList.size();i++)
    {
        bool _printAssociatedVal = true;
        UserMeshVO _meshVar = _userMeshVariablesList.at(i);
        QString _keyName = _meshVar.getFilename();
        QString _valuesToSave;
        QString _parentMenu = _meshVar.getParentMenu();


      if ((_parentMenu.contains(MARKER_PLOTTING_TITLE)) || (_parentMenu.contains(MARKER_MONITORING_TITLE))
                || (_keyName.contains("MULTIZONE")))
                _printAssociatedVal = false;
        QStringList _bndList = _meshVar.getBoundaryNameList();
        QStringList _bndValues = _meshVar.getBoundaryAssociatedValues();

    //    QString _kk1 = _bndList.join(";");
    //    QString _kk2 = _bndValues.join(";");

        // Remove empty QString
        _bndList = removeEmptyStrings(_bndList);
        _bndValues = removeEmptyStrings(_bndValues);

        if (_bndList.isEmpty())
            _valuesToSave = "NONE";
        else
        {
            if (_bndValues.isEmpty())
                _valuesToSave = _bndList.join(COMMA_SEPARATOR);
            else
            {
                QStringList _tmpList;
                for (int j=0; j<_bndList.size();j++)
                {
                    _tmpList.append(_bndList.at(j));
                    if ( (_printAssociatedVal) && (j < _bndValues.size()))
                        _tmpList.append(_bndValues.at(j));
                }
                _tmpList.removeAll("NONE");
                _valuesToSave = _tmpList.join(COMMA_SEPARATOR);

            }
        }
         _userBndVariableValues.insert(_keyName,_valuesToSave);
    }
}

QStringList MeshDialog::removeEmptyStrings(QStringList _list)
{
    QStringList _result;
    for (int i =0; i< _list.size();i++)
    {
        QString _val = _list.at(i);
        _val.simplified();
        _val.replace(" ","");
        if ( (!_val.isEmpty()) && (!_val.isNull()))
            _result.append(_val);
    }
    return _result;
}

/******************************** MENU FUNCTIONS ************************************/

/**
 * @brief Function that loads the main menu elements
 *
 */
void MeshDialog::loadMenu()
{
    DomModel * _model;
    if (!_solverManager->isInitialized())
        _solverManager->initializeManager();
    if (_isMultizoneProject)
        _model = new DomModel(*_solverManager->getBoundaryMenuList(),this);
    else
        _model = new DomModel(*_solverManager->getBoundaryMenuListWithoutMultizoneChildren(),this);

    // Setting and showing the model tree
    ui->meshTreeView->setModel(_model);
    ui->meshTreeView->hideColumn(1);
    ui->meshTreeView->hideColumn(2);
    ui->meshTreeView->show();

    // Creating the new list of objects containing the essential information of the mesh list
    initializeMeshValues();
}

void MeshDialog::createMeshVariables(QString _menu, QList<SolverVariableVO> _varList)
{

    for (int i = 0; i < _varList.size(); i++)
    {
        SolverVariableVO _element = _varList.at(i);
        QString _key = _element.getVariableFileName();//BC_WALL
        QString _userKeyName = _element.getVariableName(); //Wall
        UserMeshVO _meshVO;
        _meshVO.setUserName(_userKeyName);
        _meshVO.setFilename(_key);
        _meshVO.setParentMenu(_menu);

        // Checking if it has associated Variables
        QStringList _associatedVariables = _element.getAssociatedVariables();
       if( (!_associatedVariables.isEmpty()) && (_associatedVariables.size() > 0) )
        {
            _meshVO.setHasAssociatedMeshVariables(true);
            QStringList _keyNameAssociatedMeshVariable = _element.getAssociatedVariables();
            _meshVO.setAssociatedMeshVariables(_keyNameAssociatedMeshVariable);
        }
        // Initialize with the user values
        if (!_configUserMeshValues.empty())
        {
            QStringList _value = _configUserMeshValues.value(_key).split(COMMA_SEPARATOR);

            bool _allBndNames = checkIfAllBoundaryNames(_value);

            if ((!_value.isEmpty()) && (!_allBndNames))
            {                
                QStringList _boundValList = splitBndNameAndValues(_value);
                if( (!_boundValList.isEmpty()) && (_boundValList.size() == 2))
                {
                    _meshVO.setBoundaryNameList(_boundValList.at(0).split(COMMA_SEPARATOR));
                    _meshVO.setBoundaryAssociatedValues(_boundValList.at(1).split(SEMICOLON_SEPARATOR));

                }
            }else
            {
                // In this case is purely a set of Boundary Names
                _meshVO.setBoundaryNameList(_value);
                if ((_menu.contains(MARKER_PLOTTING_TITLE)) || (_menu.contains(MARKER_MONITORING_TITLE)) ||
                   (_key.contains("MULTIZONE")) )
                {
                    QStringList _yesValues;
                    for (int j=0;j< _value.size();j++)
                        _yesValues.append("YES");
                    _meshVO.setBoundaryAssociatedValues(_yesValues);
                }
            }

        }
        _userMeshVariablesList.append(_meshVO);

    }
}

void MeshDialog::initializeMeshValues()
{
     SolverSectionVO _sectionVO;

    // Building the complete list of Variables for the whole Boundary Menu
    _sectionVO =  _solverManager->getVariableListofSubsections(MARKER_CHILD_TITLE);
    createMeshVariables(MARKER_CHILD_TITLE,_sectionVO.getSubsectionVarList());
    _sectionVO =  _solverManager->getVariableListofSubsections(MARKER_PLOTTING_TITLE);
     createMeshVariables(MARKER_PLOTTING_TITLE,_sectionVO.getSubsectionVarList());
    _sectionVO =  _solverManager->getVariableListofSubsections(MARKER_MONITORING_TITLE);
     createMeshVariables(MARKER_MONITORING_TITLE,_sectionVO.getSubsectionVarList());
    if (_isMultizoneProject)
    {
        _sectionVO =  _solverManager->getVariableListofSubsections(MARKER_MULTIZONE_TITLE);
         createMeshVariables(MARKER_MULTIZONE_TITLE,_sectionVO.getSubsectionVarList());
    }
    initBndNamesInAssociatedMeshVar();

}

void MeshDialog::initBndNamesInAssociatedMeshVar()
{
    for (int i = 0; i < _userMeshVariablesList.size();i++)
    {
        UserMeshVO _userMeshVO =_userMeshVariablesList.at(i);
        if(_userMeshVO.hasAssociatedMeshVariables())
        {
            bool _removeAssociatedValues = false;
            if (!isAllowedTheAssociatedMeshVariables(_userMeshVO.getAssociatedMeshVariables().at(0)))
                _removeAssociatedValues = true;
            QStringList  _bndList = _userMeshVO.getBoundaryNameList();
            if (!_bndList.isEmpty())
            {
                int _pos = findMeshVariableToUpdate(_userMeshVO.getAssociatedMeshVariables().at(0),MARKER_CHILD_TITLE);
                if (_pos != -1)
                {
                    UserMeshVO _assocMeshVO = _userMeshVariablesList.at(_pos);
                    if (!_removeAssociatedValues)
                    {
                        QString _listVal = _assocMeshVO.getBoundaryAssociatedValues().join(",");
                        _assocMeshVO.setBoundaryAssociatedValues(_listVal.split(","));
                        _assocMeshVO.setBoundaryNameList(_userMeshVO.getBoundaryNameList());
                    }else
                    {
                        QStringList _emptyList;
                        _assocMeshVO.setBoundaryAssociatedValues(_emptyList);
                    }
                    _userMeshVariablesList.replace(_pos,_assocMeshVO);
                }
            }
         }

    }
}

bool  MeshDialog::checkIfAllBoundaryNames(QStringList _list)
{
    for (int i = 0; i< _list.size();i++)
    {
        QString _item = _list.at(i);
        QString _keyVal = _meshValues.key(_item);
        bool _isBnd = _meshValues.contains(_keyVal);

        //QString
        if (!_isBnd)
            return false;
    }

    return true;

}
QStringList MeshDialog::splitBndNameAndValues(QStringList _list)
{
    QStringList _result;
    QStringList _bndNames;
    QStringList _bndValues;
    QStringList _tmpBndValues;
    for (int i = 0; i < _list.size();i++)
    {
        //QString _valToCompare = _list.at(i);
        QString _item = _list.at(i);
        QString _keyVal = _meshValues.key(_item);
        bool _isBnd = _meshValues.contains(_keyVal);
        if (_isBnd)
        {
            _bndNames.append(_item);
            if (!_tmpBndValues.isEmpty())
            {
                _bndValues.append(_tmpBndValues.join(COMMA_SEPARATOR));
                _tmpBndValues.clear();
            }
         }else
            _tmpBndValues.append(_item);

        if (i == _list.size()-1)
            _bndValues.append(_tmpBndValues.join(COMMA_SEPARATOR));
    }

    _result.append(_bndNames.join(COMMA_SEPARATOR));
    _result.append(_bndValues.join(SEMICOLON_SEPARATOR));
    return _result;
}


/******************************** INITIALIZE FUNCTIONS ************************************/



/******************************** TABLE FUNCTIONS ************************************/



/**
 * @brief Function that loads the table Header
 *
 */

void MeshDialog::loadTableHeader()
{
    if (_selectedSubMenu.isEmpty())
        return;

    ui->meshTableWidget->setColumnCount(4);

    if( (!_selectedSubMenu.isEmpty()) &&
            ( (QString::compare(_selectedSubMenu,MARKER_TITLE) == StrEqual) ||
            (QString::compare(_selectedSubMenu,MARKER_CHILD_TITLE) == StrEqual ) ) )
    {
        ui->meshTableWidget->setColumnCount(6);
        ui->meshTableWidget->setHorizontalHeaderLabels( QString("Changed;Boundary Name;Boundary Type;Help;Current Value;Edit").split(";"));
    }
    else
        ui->meshTableWidget->setHorizontalHeaderLabels( QString("Changed;Boundary Name;Select?;Help").split(";"));

    ui->meshTableWidget->horizontalHeader()->setVisible(true);
}

/**
 * @brief Function that resets the table contents
 *
 */

void MeshDialog::resetTable()
{
    if (ui->meshTableWidget->rowCount() > 0)
    {
        //  Delete all rows
        int _totalRows = ui->meshTableWidget->rowCount();
        for (int i = _totalRows; i >= 0; i--)
            ui->meshTableWidget->removeRow(i);
    }
}


QList<SolverVariableVO>  MeshDialog::removeNonVisibleVariables(QList<SolverVariableVO> _list)
{
    QList<int> _positionsToRemove;
    for (int i = 0; i< _list.size();i++)
    {
        SolverVariableVO _var = _list.at(i);
        if (!_var.isVisibleInMenu())
            _positionsToRemove.append(i);
    }
    if (!_positionsToRemove.isEmpty())
    {
        int _totalPosToRemove = _positionsToRemove.size();
        for (int j=_totalPosToRemove-1; j>= 0;j--)
            _list.removeAt(_positionsToRemove.at(j));
    }
    return _list;
}

/**
 * @brief Function that fills the table according user clicks
 *
 * @param _parent
 */

void MeshDialog::fillTable(QString _parent)
{
   // ui->meshTableWidget->blockSignals(true);
    if (QString::compare(_parent,"") != StrEqual)
    {

        SolverSectionVO _sectionVO;
        SolverMarkerMenuHelper _markerHelper;

        _sectionVO =  _solverManager->getVariableListofSubsections(_selectedSubMenu);

        QList<SolverVariableVO> _listVariables = _sectionVO.getSubsectionVarList();
        // Removing the non visible variables
        _listVariables = removeNonVisibleVariables(_listVariables);

        _markerHelper.fillMarkerMenuTable(_selectedSubMenu,_meshValues,ui->meshTableWidget,_listVariables);


        if (QString::compare(_selectedSubMenu,MARKER_CHILD_TITLE) == StrEqual)
        {
            _markerHelper.addExtraColumn(ui->meshTableWidget,CurrentValueColumn,"");
            _markerHelper.addExtraColumn(ui->meshTableWidget,EditIconColum,"");
        }

        addSignals();

       if (!_userMeshVariablesList.isEmpty())
           updateTableWithUserValues();
        if (QString::compare(_selectedSubMenu,MARKER_MULTIZONE_TITLE) != StrEqual)
            buildAllToolTips(); // FALLA CON EL MULTIZONE MIRAR PORQUE

    }
}


void MeshDialog::updateTableWithUserValues()
{
    for (int i = 0; i < _userMeshVariablesList.size();i++)
    {
        UserMeshVO _userMeshVar = _userMeshVariablesList.at(i);
        QString _parentMenu = _userMeshVar.getParentMenu();
        if (_selectedSubMenu.contains(_parentMenu))
        {
            QString _userVarName =_userMeshVar.getUserName();
            QStringList _bndNames = _userMeshVar.getBoundaryNameList();
            QStringList _bndValues = _userMeshVar.getBoundaryAssociatedValues();

            if (_selectedSubMenu.contains(MARKER_CHILD_TITLE))
            {
                if ((_userMeshVar.hasAssociatedMeshVariables()) && (isAllowedTheAssociatedMeshVariables(_userVarName)))
                {
                    QStringList _associatedMeshNameList = _userMeshVar.getAssociatedMeshVariables();
                    // Notice: At the moment we only consider the option of having ONLY ONE associated Mesh
                    // This is done as QStringList for the future, although the logic was not implemented
                    // .... In case of this need, it is needed to do the full logic (TODO)
                    int _pos = findMeshVariableToUpdate(_associatedMeshNameList.at(0),_selectedSubMenu);
                    if (_pos != -1)
                    {
                        UserMeshVO _associatedMeshVar = _userMeshVariablesList.at(_pos);
                        _bndValues =_associatedMeshVar.getBoundaryAssociatedValues();
                    }
                }
                updateTable(_userVarName,_bndNames,_bndValues);
            }
            else
                updateTableWithYesNoValues(_bndNames,_bndValues);
        }
    }
}

bool MeshDialog::isAllowedTheAssociatedMeshVariables(QString _variableName)
{
    if( (QString::compare(_variableName,"Wall") == StrEqual) && (_isNSProblem))
        return true;

    return false;
}

void MeshDialog::updateTableWithYesNoValues(QStringList _bndNames,QStringList _bndValues)
{
    for (int i=0 ; i < ui->meshTableWidget->rowCount();i++)
    {
        QString _cBndName = ui->meshTableWidget->item(i,BndNameColumn)->text();
        int _pos = _bndNames.indexOf(_cBndName);
        if ( (_pos != -1) && (_pos < _bndValues.size()))
             updateComboValue(i,_bndValues.at(_pos));
        else
            updateComboValue(i,"NO");
    }

}



void MeshDialog::updateTable(QString _userVarName,QStringList _bndNames,QStringList _bndValues)
{
    for (int i = 0; i < ui->meshTableWidget->rowCount();i++)
    {
        QString _bndTableName = ui->meshTableWidget->item(i,1)->text();
        if (_bndNames.contains(_bndTableName))
        {
             updateComboValue(i,_userVarName);
            if (!_bndValues.isEmpty())
             {
               int _pos = _bndNames.indexOf(_bndTableName);
               if( (_pos != -1) && (_pos < _bndValues.size()) )
                    showValuesInValueCol(i, _bndValues.at(_pos));
            }
        }
    }
}

/******************************** TREE VIEW FUNCTIONS ************************************/

/**
 * @brief Function that performs the functionality when the user click on a element of the tree view
 *
 * @param index
 */
void MeshDialog::on_meshTreeView_clicked(const QModelIndex &index)
{

   QVariant _data = index.data(Qt::DisplayRole);
   _selectedSubMenu = _data.toString();
   _isLoadingTable = true;
   QString _parent = index.parent().data(Qt::DisplayRole).toString();
   resetTable();
   loadTableHeader();
   fillTable(_parent);
   _isLoadingTable = false;
   ui->meshTableWidget->resizeColumnsToContents();
   ui->meshTableWidget->show();
}

/******************************** SAVE/UPDATE FUNCTIONS ************************************/
/**
 * @brief Function to update the combo value with previous values
 *
 * @param _rowToUpdate
 * @param _selVarName
 */
void MeshDialog::updateComboValue(int _rowToUpdate,QString _selVarName)
{
   QComboBox * _combo = qobject_cast<QComboBox * > ( ui->meshTableWidget->cellWidget(_rowToUpdate,ColumnSelVal));
   int _index = -1;
   if (_combo)
   {
           _index = _combo->findText(_selVarName);
           if (_index != -1)
               _combo->setCurrentIndex(_index);

   }
   if (_index >0)
    ui->meshTableWidget->item(_rowToUpdate,ColumnCheck)->setCheckState(Qt::Checked);
}


QString MeshDialog::getBndTypeName()
{
    QString _result;

    if (_selectedSubMenu.contains("Monitoring"))
        _result = "MARKER_MONITORING";

    if (_selectedSubMenu.contains("Plotting"))
        _result = "MARKER_PLOTTING";

    if (_selectedSubMenu.contains("Multizone"))
        _result = "BC_MULTIZONE_INTERFACE";


    return _result;
}

/**
 * @brief Function to save the user changes
 *
 * @param _value
 * @param _itemRow
 */
void MeshDialog::saveUserChanges(QString _value,int _itemRow)
{
    UserMeshVO _meshToUpdate;
    QString _bndNameInRow = ui->meshTableWidget->item(_itemRow,BndNameColumn)->text();
    QString _bndTypeInRow = _value;
    int _posToRemove = findMeshVariableToUpdateByBndValue(_bndNameInRow,_selectedSubMenu);
    int _posToUpdate;
    if (_selectedSubMenu.contains(MARKER_CHILD_TITLE))
        _posToUpdate = findMeshVariableToUpdate(_bndTypeInRow,_selectedSubMenu);
    else
        _posToUpdate = findMeshVariableToUpdateByFileNameVar(getBndTypeName(),_selectedSubMenu);

    if (_posToRemove!=-1)
    {
        _meshToUpdate = _userMeshVariablesList.at(_posToRemove);
        _meshToUpdate.deleteBoundary(_bndNameInRow);
        _userMeshVariablesList.replace(_posToRemove,_meshToUpdate);
    }
    if (_posToUpdate != -1)
    {
        QString _bndVal;
        if (ui->meshTableWidget->columnCount() == 6)
            _bndVal = ui->meshTableWidget->item(_itemRow,CurrentValueColumn)->text();
        else            
            _bndVal = _value; // TBR

        // Update the main MeshVariable
        _meshToUpdate = _userMeshVariablesList.at(_posToUpdate);

        // Now update the Associated Mesh Variables accordingly
        if ((_meshToUpdate.hasAssociatedMeshVariables()) && (isAllowedTheAssociatedMeshVariables(_meshToUpdate.getUserName())))
        {
            QStringList _associatedMeshName = _meshToUpdate.getAssociatedMeshVariables();
            int _pos = findMeshVariableToUpdate(_associatedMeshName.at(0),_selectedSubMenu);
            if ((_pos != -1) && (_pos < _userMeshVariablesList.size()))
            {
                UserMeshVO _associatedMeshVO = _userMeshVariablesList.at(_pos);
                QStringList _associatedValList = _associatedMeshVO.getBoundaryAssociatedValues();
                if ((_associatedValList.isEmpty()) || (_associatedValList.size() <= 0))
                {    // We need to create an initial structure with the default values and the same number of Boundaries than the parent Var
                    int _noElements = _meshToUpdate.getBoundaryNameList().size();
                    _associatedMeshVO.initializeBndAssociatedValuesToDefault("0",_noElements);
                    _associatedMeshVO.setBoundaryNameList(_meshToUpdate.getBoundaryNameList());
                }

                _associatedMeshVO.addBoundary(_bndNameInRow,_bndVal);
                _userMeshVariablesList.replace(_pos,_associatedMeshVO);
                _bndVal.clear(); // We donot need to store it
            }

        }

        // Update the values of the mesh with the new Boundary and boundary values
        if (_bndVal.compare("NO")!= StrEqual)
            _meshToUpdate.addBoundary(_bndNameInRow,_bndVal);
        _userMeshVariablesList.replace(_posToUpdate,_meshToUpdate);

    }
}

int MeshDialog::findMeshVariableToUpdate(QString _nameToFind,QString _menu)
{
    for (int i = 0; i < _userMeshVariablesList.size();i++)
    {
        UserMeshVO _cMeshVariable = _userMeshVariablesList.at(i);
        if (_cMeshVariable.getParentMenu().contains(_menu))
            if (QString::compare(_cMeshVariable.getUserName(),_nameToFind) == StrEqual)
                 return i;
    }
    return -1;
 }


int MeshDialog::findMeshVariableToUpdateByFileNameVar(QString _nameToFind,QString _menu)
{
    for (int i = 0; i < _userMeshVariablesList.size();i++)
    {
        UserMeshVO _cMeshVariable = _userMeshVariablesList.at(i);
        if (_cMeshVariable.getParentMenu().contains(_menu))
            if (QString::compare(_cMeshVariable.getFilename(),_nameToFind) == StrEqual)
                 return i;
    }
    return -1;
}

int MeshDialog::findMeshVariableToUpdateByBndValue(QString _bndToFind,QString _menu)
{
    for (int i = 0; i < _userMeshVariablesList.size();i++)
    {
        UserMeshVO _cMeshVariable = _userMeshVariablesList.at(i);
        if (_cMeshVariable.getParentMenu().contains(_menu))
            if (_cMeshVariable.containsBoundary(_bndToFind))
                 return i;
    }
    return -1;
}
/********************************************** PRIVATE SLOT FUNCTIONS ***************************/

/**
 * @brief Slot that controls the change of the combo value by the user
 *
 * @param position
 */
void MeshDialog::changedComboValueByUser(QString position)
{

    if (!_isLoadingTable)
    {
        QStringList coordinates = position.split("-");
        int row = coordinates[0].toInt();
        int col = coordinates[1].toInt();
        QComboBox* combo = (QComboBox*) ui->meshTableWidget->cellWidget(row, col);
        ui->meshTableWidget->item(row,ColumnCheck)->setCheckState(Qt::Checked);
        QString _value = combo->currentText();

        if (!needToOpenSpecificConfigWindow(_value))
        {

           if (QString::compare(_value,"Wall") == StrEqual)
                  showValuesInValueCol(row,"0"); // If Euler, Heatflux = 0;
            else
                if (QString::compare(_selectedSubMenu,MARKER_CHILD_TITLE) == StrEqual)
                     showValuesInValueCol(row,"");


            saveUserChanges(_value,row);
            _needToSave = true;
        }else
            launchSpecificBoundaryConfigWindow(row,_value,combo,false);
        }


    }

/**
 * @brief Function to launch the Specific Boundary config window
 *
 * @param row
 * @param _value
 * @param _combo
 */
void MeshDialog::launchSpecificBoundaryConfigWindow(int row,QString _value, QComboBox * _combo,
                                                    bool _isEdit)
{
    SpecificBoundaryVariableDialog * _boundaryDialog = new SpecificBoundaryVariableDialog(this);
    _boundaryDialog->setHelpContent(buildHelpContent(row));
    QStringList _cValues;
    if (_isEdit)
        _cValues = ui->meshTableWidget->item(row,CurrentValueColumn)->text().split(",");
    QString _bndName = ui->meshTableWidget->item(row,BndNameColumn)->text();
    _boundaryDialog->setBoundaryName(_bndName);
    _boundaryDialog->setCurrentValues(_cValues);
    _boundaryDialog->setMarkerMenuManager(_markerManager);
    _boundaryDialog->setMeshVariables(_meshValues);

    _boundaryDialog->setBoundaryType(_value);
    _boundaryDialog->setStoredUserConfigValues(_storedSpecificBoundValues);
    _boundaryDialog->loadValues();
    _boundaryDialog->exec();
    if (_boundaryDialog->userAcceptChanges())
    {        
        QString _keyname = _combo->currentText();
        QStringList  _result =_boundaryDialog->getConfiguredBoundary();
        QString _value = _result.join(COMMA_SEPARATOR);
        showValuesInValueCol(row,_value);
        saveUserChanges(_keyname,row);
        _needToSave = true;
    }else
       restoreValues(row);

}

/**
 * @brief Function to restore the values in the row
 *
 * @param row
 */
void MeshDialog::restoreValues(int row)
{
    if (QString::compare(_selectedSubMenu,MARKER_CHILD_TITLE) == StrEqual)
    {
        QComboBox * _combo = qobject_cast<QComboBox * > ( ui->meshTableWidget->cellWidget(row,ColumnSelVal) );
        if (_combo)
        {
            QString _key =  ui->meshTableWidget->item(row,ColumnVarName)->text();
            QStringList _prevValList = _previousUserMeshValues.value(_key).split(COMMA_SEPARATOR);
            if (!_prevValList.isEmpty())
            {
                QString _prevValue = _prevValList.at(0);

                SolverSectionVO _sectionVO =  _solverManager->getVariableListofSubsections(_selectedSubMenu);
                QList<SolverVariableVO> _list = _sectionVO.getSubsectionVarList();
                SolverSectionDAO _solverDAO ;
                SolverVariableVO _prevVar = _solverDAO.findVariableByFileName(_list, _prevValue);

                int _index = _combo->findText(_prevVar.getVariableName());
                if (_index != -1)
                    _combo->setCurrentIndex(_index);
            }


        }
    }


}

/**
 * @brief Function to show the values that comes from the specific window
 *
 * @param row
 * @param _value
 */
void MeshDialog::showValuesInValueCol(int row, QString _value)
{

    ui->meshTableWidget->item(row,CurrentValueColumn)->setText(_value);
    if (!_value.isEmpty())
    {
        SolverMarkerMenuHelper _markerHelper;
        _markerHelper.addEditIconToColumn(ui->meshTableWidget,row,EditIconColum);
    }
    else
        ui->meshTableWidget->item(row,EditIconColum)->setIcon(QIcon(""));

}



/************************* REMOVE FUNCTION *********************************/




/****************** SIGNALS ***************************/
/**
 * @brief Function to add signals
 *
 */
void MeshDialog::addSignals()
{

    bool _foundCombos = false;
    bool _foundCheckList = false;

    if (ui->meshTableWidget->rowCount() > 0)
    {
        for (int i = 0; i< ui->meshTableWidget->rowCount();i++)
        {
                QWidget * _item;
                _item = ui->meshTableWidget->cellWidget(i,ColumnSelVal);

                if (_item)
                {
                    QComboBox *_comboItem = qobject_cast<QComboBox *> (_item);
                    if (_comboItem)
                    {
                        connect(_comboItem, SIGNAL(currentIndexChanged(int)), _signalMapper, SLOT(map()),Qt::UniqueConnection);
                       _signalMapper->setMapping(_comboItem, QString("%1-%2").arg(i).arg(ColumnSelVal));
                        _foundCombos = true;
                    }else
                    {
                        QListWidget * _checkList = qobject_cast<QListWidget *> (_item);
                        if (_checkList)
                        {
                            connect(_checkList,SIGNAL(itemClicked(QListWidgetItem*)),_signalMapperCheckList,SLOT(map()),Qt::UniqueConnection);
                           _signalMapperCheckList->setMapping(_checkList,QString("%1").arg(i));
                           _foundCheckList = true;
                        }
                    }
                }

        }
    }
    if (_foundCombos)
        connect(_signalMapper, SIGNAL(mapped(const QString &)),this, SLOT(changedComboValueByUser(const QString &)),Qt::UniqueConnection);
    if (_foundCheckList)
        connect(_signalMapperCheckList, SIGNAL(mapped(const QString &)),this, SLOT(handleItem(const QString &)),Qt::UniqueConnection);
}

/****************** GETTER/SETTERS ***************************/
/**
 * @brief Function that checks if it is needed to open the Specific boundary dialog
 *
 * @param _varName
 * @return bool
 */
bool MeshDialog::needToOpenSpecificConfigWindow(QString _varName)
{
    QString _valueName;
    SolverSectionDAO _solverDAO;
    if (QString::compare(_selectedSubMenu,MARKER_CHILD_TITLE) == StrEqual)
    {
        SolverSectionVO _sectionVO =  _solverManager->getVariableListofSubsections(MARKER_CHILD_TITLE);
        _valueName = _solverDAO.findValueNameByVariableName(_sectionVO,_varName);
    }

    if (
         (QString::compare(_valueName,MARKER_PERIODIC_NAME) == StrEqual) ||
         (QString::compare(_valueName,MARKER_INLET_NAME) == StrEqual) ||
         (QString::compare(_valueName,MARKER_OUTLET_NAME) == StrEqual) ||
         (QString::compare(_valueName,"BC_FARFIELD") == StrEqual) ||
            (QString::compare(_valueName,"BC_WALL") == StrEqual)
         )
        return true;

    return false;
}

/**
 * @brief Function to set the mesh values
 *
 * @param QHash<QString
 * @param _val
 */
#include <QHashIterator>
void MeshDialog::setMeshValues( QHash<QString,QString> _val)
{
    if (_val.contains(IS_3D_MESH_TAG))
        _val.remove(IS_3D_MESH_TAG);
        _meshValues = _val;

 }

/**
 * @brief Returns the mesh values
 *
 * @return QHash<QString, QString>
 */
QHash<QString,QString> MeshDialog::getMeshValues()
{
    return _meshValues;
}
/**
 * @brief Set the solver config manager
 *
 * @param _manager
 */
void MeshDialog::setSolverConfigManager(SolverConfigManager * _manager)
{
    _solverManager = _manager;
}


/**
 * @brief Returns the row where the boundary is placed
 *
 * @param _boundaryName
 * @return int
 */
int MeshDialog::getBoundaryRow(QString _boundaryName)
{
    int _result = -1;
    for (int _row = 0; _row < ui->meshTableWidget->rowCount();_row++)
    {
        QString _varName = ui->meshTableWidget->item(_row,BndNameColumn)->text();
        if (QString::compare(_varName,_boundaryName) == StrEqual)
            return _row;
    }


    return _result;
}

/**
 * @brief Returns if it is a special boundary
 *
 * @param _keyVariable
 * @return bool
 */
bool MeshDialog::isSpecialBoundary(QString _keyVariable)
{
    if (QString::compare(_keyVariable,MARKER_ROTB_NAME) == StrEqual)
        return true;
    if (QString::compare(_keyVariable,MARKER_INLET_NAME) == StrEqual)
        return true;
    if (QString::compare(_keyVariable,MARKER_OUTLET_NAME) == StrEqual)
        return true;
    if (QString::compare(_keyVariable,MARKER_PERIODIC_NAME)  == StrEqual)
        return true;
    if (QString::compare(_keyVariable,BC_FARFIELD_NAME)  == StrEqual)
        return true;
    if (QString::compare(_keyVariable,BC_WALL_NAME)  == StrEqual)
        return true;
     return false;
}




/**
 * @brief Set up configured mesh values
 *
 * @param QHash<QString
 * @param _userValues
 */
void MeshDialog::setUserConfiguredMeshValues(QHash<QString,QString> _userValues)
{
    _configUserMeshValues =_userValues;

}

/**
 * @brief Returns user configured mesh values
 *
 * @return QHash<QString, QString>
 */
QHash<QString,QString> MeshDialog::getUserConfiguredMeshValues()
{
    return _configUserMeshValues;
}




/**
 * @brief Fucntion that perfomrs the functionality for cancel click button
 *
 */
void MeshDialog::on_meshButtonBox_rejected()
{
    _configUserMeshValues.clear();
    _needToSave = false;
    close();
}

/**
 * @brief Returns true if the status is needed to be saved
 *
 * @return bool
 */
bool MeshDialog::needToSave()
{
    return _needToSave;
}

/**
 * @brief Function to perform actions when a cell is clicked
 *
 * @param row
 * @param column
 */
void MeshDialog::on_meshTableWidget_cellClicked(int row, int column)
{
    if(column == MarkerColumnHelp)
    {
       HelpDialog *  _helpDialog = new HelpDialog();
       QString _helpText = buildHelpContent(row);
        _helpDialog->setHelpText(_helpText);
        _helpDialog->loadHelpText();
        _helpDialog->exec();
        delete _helpDialog;
    }
    else
    {
        if (column == EditIconColum)
        {
            QComboBox * _combo = qobject_cast<QComboBox * > (ui->meshTableWidget->cellWidget(row,ColumnSelVal));
            QString _value = _combo->currentText();
            if (needToOpenSpecificConfigWindow(_value))         
                launchSpecificBoundaryConfigWindow(row,_value,_combo,true);

        }
    }
}


/**
 * @brief Function to build up all the tooltips
 *
 */
void MeshDialog::buildAllToolTips()
{
    for (int i = 0; i < ui->meshTableWidget->rowCount(); i++)
    {
        ui->meshTableWidget->item(i,BndNameColumn)->setToolTip("Mesh Variable Name found in the Mesh file");
        ui->meshTableWidget->item(i,BndTypeColumn)->setToolTip("Select the most suitable option for the corresponding mesh variable");
    }

}


/**
 * @brief Function to build up the help content
 *
 * @param row
 * @return QString
 */
QString MeshDialog::buildHelpContent(int row)
{
    QString _variableName;
    int _pos = 0;

    SolverSectionVO _sectionVO;
     _sectionVO=  _solverManager->getVariableListofSubsections(_selectedSubMenu);

       if (QString::compare(_selectedSubMenu,MARKER_CHILD_TITLE) == StrEqual)
    {
        QComboBox * _combo =  qobject_cast<QComboBox* >(ui->meshTableWidget->cellWidget(row,ColumnSelVal));
        if (_combo)
        {
            _variableName = _combo->currentText();
        }
        if (!_variableName.isEmpty())
        {
            SolverConfigMenuHelper _uiHelper;
            return _uiHelper.getAssociatedHelpText(_variableName,_sectionVO.getSubsectionVarList(),HTML_FORMAT_ON);
        }
    }

    QList<SolverVariableVO> _list  = _sectionVO.getSubsectionVarList();
    if (!_list.isEmpty())
    {
        SolverVariableVO _solverVarVO =  _list.at(_pos);
        return _solverVarVO.getHelpContents(HTML_FORMAT_ON);
    }
    return QString("");
}


void MeshDialog::setIsNSProblem(bool _val)
{
   _isNSProblem = _val;
}
void MeshDialog::setIsMultizoneProject(bool _val)
{
    _isMultizoneProject = _val;
}
QHash<QString,QString> MeshDialog::getUserBndVariableValues()
{
    return _userBndVariableValues;
}
bool MeshDialog::hasUserAcceptChanges()
{
    return _userAcceptChanges;
}
