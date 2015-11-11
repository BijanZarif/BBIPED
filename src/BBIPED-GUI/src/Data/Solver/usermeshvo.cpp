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
#include "usermeshvo.h"

/**
 * @brief Constructor
 *
 */
UserMeshVO::UserMeshVO()
{
    _filename = "";
    _userName ="";
    _parentMenu = "";
    _hasAssociatedMeshVariables = false;

}
/**
 * @brief Destructor
 *
 * @param _menu
 */
void UserMeshVO::setParentMenu(QString _menu)
{
    _parentMenu = _menu;
}

/**
 * @brief Get the name of the parent menu
 *
 * @return QString
 */
QString UserMeshVO::getParentMenu()
{
    return _parentMenu;
}

/**
 * @brief Get the name of the file
 *
 * @return QString
 */
QString UserMeshVO::getFilename()
{
    return _filename;
}

/**
 * @brief Get the user name
 *
 * @return QString
 */
QString UserMeshVO::getUserName()
{
    return _userName;
}

/**
 * @brief the the list of boundary names
 *
 * @return QStringList
 */
QStringList UserMeshVO::getBoundaryNameList()
{
    return _boundaryNameList;

}
/**
 * @brief Get the list of the associated values
 *
 * @return QStringList
 */
QStringList UserMeshVO::getBoundaryAssociatedValues()
{
    return _boundaryAssociatedValues;
}

/**
 * @brief Set the file name
 *
 * @param _name
 */
void UserMeshVO::setFilename(QString _name)
{
    _filename = _name;
}

/**
 * @brief set the user name
 *
 * @param _name
 */
void UserMeshVO::setUserName(QString _name)
{
    _userName = _name;
}

/**
 * @brief Set the list of the boundary names
 *
 * @param _list
 */
void UserMeshVO::setBoundaryNameList(QStringList _list)
{
    _boundaryNameList = _list;
}

/**
 * @brief Set the list of associated values
 *
 * @param _list
 */
void UserMeshVO::setBoundaryAssociatedValues(QStringList _list)
{
    _boundaryAssociatedValues = _list;
}

/**
 * @brief Update the boundary value
 *
 * @param _boundName
 * @param _newVal
 */
void UserMeshVO::updateBndValue(QString _boundName, QString _newVal)
{
    int _pos = _boundaryNameList.indexOf(_boundName);
    if ((_pos != -1) && (_pos < _boundaryAssociatedValues.size()))
        _boundaryAssociatedValues.replace(_pos, _newVal);
    else
    {
        for (int i = _boundaryAssociatedValues.size(); i <= _pos;i++)
        {
            _boundaryAssociatedValues.append("0");
        }
        _boundaryAssociatedValues.replace(_pos, _newVal);
    }
}

/**
 * @brief Add a Boundary and its value
 *
 * @param _boundName
 * @param _val
 */
void UserMeshVO::addBoundary(QString _boundName, QString _val)
{
    // Firts check if it is already in the name list

            if ( (!_boundaryNameList.isEmpty()) && (_boundaryNameList.contains(_boundName)) )
    {
       updateBndValue(_boundName,_val);
    }else
    {
        _boundaryNameList.append(_boundName);
        _boundaryAssociatedValues.append(_val);
    }

}

/**
 * @brief Reset all lis values
 *
 */
void UserMeshVO::resetValues()
{
    _boundaryAssociatedValues.clear();
    _boundaryNameList.clear();
}

/**
 * @brief Checks if a boundary name is present
 *
 * @param _bndName
 * @return bool
 */
bool UserMeshVO::containsBoundary(QString _bndName)
{
    return _boundaryNameList.contains(_bndName);

}
/**
 * @brief Delete a boundary by its name
 *
 * @param _bndName
 */
void UserMeshVO::deleteBoundary(QString _bndName)
{
    if (_boundaryNameList.isEmpty())
        return;
    int _pos = _boundaryNameList.indexOf(_bndName);
    if (_pos == -1)
        return;
    _boundaryNameList.removeAt(_pos);
    if ( (!_boundaryAssociatedValues.isEmpty()) && (_pos <= _boundaryAssociatedValues.size()))
        _boundaryAssociatedValues.removeAt(_pos);
}
/**
 * @brief Set the list of associated mesh variables
 *
 * @param _list
 */
void UserMeshVO::setAssociatedMeshVariables(QStringList _list)
{
    _associatedMeshVariables = _list;
}

/**
 * @brief Get the list of associated mesh variables
 *
 * @return QStringList
 */
QStringList UserMeshVO::getAssociatedMeshVariables()
{
    return _associatedMeshVariables;
}

/**
 * @brief Check if it has associated Mesh variables (by default false)
 *
 * @return bool
 */
bool UserMeshVO::hasAssociatedMeshVariables()
{
    return _hasAssociatedMeshVariables;
}

/**
 * @brief Set true if it has associated mesh variables, false otherwise
 *
 * @param _val
 */
void UserMeshVO::setHasAssociatedMeshVariables(bool _val)
{
    _hasAssociatedMeshVariables = _val;
}
/**
 * @brief Initialize the boundary associated values to a default value (_val)
 *
 * @param _val
 * @param _noElements
 */
void UserMeshVO::initializeBndAssociatedValuesToDefault(QString _val,int _noElements)
{
    for (int i = 0; i < _noElements;i++)
        _boundaryAssociatedValues.append(_val);
}
