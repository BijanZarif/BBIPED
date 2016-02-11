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
#include "solvermarkermenuhelper.h"
#include "Data/solversectiondao.h"
#include "Defines/General/GeneralDefines.h"
#include "Defines/Tool/Solver/SolverDefines.h"
#include "Utility/listhelper.h"

#include "Utility/qtableutil.h"
#include <QHeaderView>
/**
 * @brief Constructor
 *
 */
SolverMarkerMenuHelper::SolverMarkerMenuHelper()
{
}

/************************************ FILL ************************************/

/**
 * @brief This funtcion allow to fill the specific marker menu table in the Solver Basic dialog
 *
 * @param _parentName
 * @param QHash<QString
 * @param _meshVariables
 * @param _table
 * @param _list
 */
void SolverMarkerMenuHelper::fillMarkerMenuTable(QString _parentName,
                                                 QHash<QString,QString> _meshVariables,
                                                 QTableWidget * _table,
                                                 QList<SolverVariableVO> _list)
{
    if ( (_parentName.isEmpty()) || (QString::compare(_parentName,"") == StrEqual) ||
         (_meshVariables.isEmpty()))
        return;
    QStringList _comboItemList;
    SolverSectionDAO _sectionDAO;
    bool _isMultizoneMarker = false;

  /*  if (QString::compare(_parentName,MARKER_MULTIZONE_TITLE) == StrEqual)
    {
        _comboItemList = _sectionDAO.getVariableNameList(_list);
        _isMultizoneMarker = true;

    }else{*/

        if (!_list.isEmpty())
        {
            if ((QString::compare(_parentName,MARKER_CHILD_TITLE) == StrEqual) )
                _comboItemList = _sectionDAO.getVariableNameList(_list);
            else
            {
                // Solo puede tener 1 elemento
                // Que corresponde al MARKER_MONITORING o a MARKER_PLOTTING
                SolverVariableVO _markerVO = _list.at(0);
                _comboItemList = _markerVO.getComboValuesList();
            }
        }
   // }
    if (!_meshVariables.isEmpty())
    {
        QHashIterator<QString,QString> iter(_meshVariables);
        while (iter.hasNext())
        {
            iter.next();
            QString _meshVariableName = iter.value();
            addMarkerVariable(_table,_meshVariableName,_comboItemList,_isMultizoneMarker);
        }
    }

}



/************************************ ADD/UPDATE ************************************/

/**
 * @brief This function creates the Row in the correspondent table for a Marker Variable
 *
 * @param _table
 * @param _varName
 * @param _list
 */
void SolverMarkerMenuHelper::addMarkerVariable( QTableWidget * _table,QString _varName, QStringList _list,
                                                bool _isMultizoneMarker)
{
    int _lastRow =_table->rowCount();
    //-------------------------------------------------------------------------------------------------------------
    // For the marker menu we are assuming that always will be selectable. If not .. then we need to configure it
    QList<bool> _selectable;
    for (int i = 0; i< _list.size(); i++)
        _selectable.append(true);
    //-------------------------------------------------------------------------------------------------------------
    QtableUtil _tableHelper;
    _table->insertRow(_lastRow);
    _table->setItem(_lastRow,ColumnCheck,_tableHelper.addChangedColumn());

    if ((!_list.isEmpty()) && (!_isMultizoneMarker))
    {
        _table->setCellWidget(_lastRow,ColumnSelVal,_tableHelper.addCombo(_list,_selectable));
        _table->setItem(_lastRow,ColumnSelVal,_tableHelper.addEditText2("") );

    }
    else
        if (_isMultizoneMarker)
        {
           _table->setCellWidget(_lastRow,ColumnSelVal,_tableHelper.addCombo(_list,_selectable)); // Antes tenia el AddChecker
           //((QHeaderView*) _table->horizontalHeader())->setResizeMode(ColumnSelVal, QHeaderView::Stretch);
           //((QHeaderView*) _table->verticalHeader())->setResizeMode(_lastRow, QHeaderView::Stretch);
        }
    _table->setItem(_lastRow,ColumnVarName,_tableHelper.addNonEditableText(_varName));
    _table->setItem(_lastRow,MarkerColumnHelp,_tableHelper.addIcon());
}


/**
 * @brief This function adds an extra column
 *
 * @param _table
 * @param _column
 * @param _defaultVal
 */
void SolverMarkerMenuHelper::addExtraColumn(QTableWidget * _table, int _column,QString _defaultVal)
{
    QtableUtil _tableHelper;
    for (int i = 0; i < _table->rowCount();i++)
        _table->setItem(i,_column,_tableHelper.addNonEditableText(_defaultVal));
}

/**
 * @brief This function adds a Edit Icon to the correspondent column in the row
 *
 * @param _table
 * @param row
 * @param _column
 */
void SolverMarkerMenuHelper::addEditIconToColumn(QTableWidget * _table, int row, int _column)
{
     QtableUtil _tableHelper;
        _table->setItem(row,_column,_tableHelper.addEditIcon());
}

/**
 * @brief This function adds the correspondent values in the tree view for the Marker Menu
 *
 * @param _menuToFill
 */
void SolverMarkerMenuHelper::addMarkerMenu(QDomDocument * _rootTree,QDomDocument * _menuToFill)
{
    SolverConfigMenuHelper _menuHelper;
    QDomElement _rootElement = _rootTree->documentElement();
    if (!_rootElement.isNull())
    {
        QDomNode _sectionsNode = _rootElement.firstChild();
        QDomNodeList _sectionList = _sectionsNode.childNodes();
        QDomElement _boundarySection = _menuHelper.findSectionElementByName(MARKER_TITLE,_sectionList);
        _menuHelper.createSectionElement(_boundarySection, _menuToFill);
    }
}


/**
 * @brief This function removes the markers elements according to the map values
 *
 * @param QHash<QString
 * @param _mapValues
 */
void SolverMarkerMenuHelper::removeMarkerElementsFromConfigFile(QHash<QString,QString> * _mapValues)
{
    QHashIterator<QString,QString> _iter(*_mapValues);
    QStringList _valuesToRemove;
    while (_iter.hasNext())
    {
        _iter.next();
        QString _keyName = _iter.key();
        if  ((_keyName.contains("MARKER_")) || (_keyName.contains("BC_")))
            _valuesToRemove.append(_keyName);
    }

    if (!_valuesToRemove.isEmpty())
    {
        for (int i = 0; i < _valuesToRemove.count(); i++)
            _mapValues->remove(_valuesToRemove.at(i));
    }
}


/**
 * @brief Function that cheks the Marker mesh variables
 *
 * @param QHash<QString
 * @param _userMarkers
 * @param QHash<QString
 * @param _meshFileMarkers
 * @param QHash<QString
 * @param _markersConfig
 * @return bool
 */
bool SolverMarkerMenuHelper::checkMarkerMeshVariables(QHash<QString,QString>  _userMarkers,
                                          QHash<QString,QString>  _meshFileMarkers,
                                          QHash<QString,QString> * _markersConfig )
{

    QHashIterator<QString,QString> iter(_userMarkers);
    ListHelper _listHelper;

    QStringList _list = _listHelper.getListFromQHash(_meshFileMarkers);
    int _count = 0;
    bool _result = true;
    while(iter.hasNext())
    {
        iter.next();
        QString _keyName = iter.key();
        if  ((_keyName.contains("MARKER_")) || (_keyName.contains("BC_")))
        {
            QString _value = iter.value();
            if (!_value.contains("NONE"))
            {
                _markersConfig->insert(QString::number(_count),_value);
                _count++;
                if(!(_listHelper.isContainedInList(_list,_value)) )
                    _result = false;
            }
        }
    }
    return _result;
}

/**
 * @brief Function that cheks the Marker mesh variables
 *
 * @param QHash<QString
 * @param _userMarkers
 * @param QHash<QString
 * @param _meshFileMarkers
 * @return bool
 */
bool SolverMarkerMenuHelper::checkMarkerMeshVariables(QHash<QString,QString>  _userMarkers,
                                          QHash<QString,QString>  _meshFileMarkers)
{
    ListHelper _listHelper;
    QStringList _list = _listHelper.getListFromQHash(_meshFileMarkers);
    int _count = 0;
    bool _result = true;

    QHashIterator<QString,QString> iter(_userMarkers);
    while(iter.hasNext())
    {
        iter.next();
        QString _keyName = iter.key();
        if ((_keyName.contains("MARKER_")) || (_keyName.contains("BC_")))
        {
            QString _value = iter.value();
            if (!_value.contains("NONE"))
            {
                _count++;
                if(!(_listHelper.isContainedInList(_list,_value)) )
                    _result = false;
            }
        }
    }
    return _result;
}
