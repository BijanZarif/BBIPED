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
#include "solvermarkermenu.h"

/**************************************************************************************/
/**
 * @brief Constructor
 *
 */
SolverMarkerMenu::SolverMarkerMenu()
{
    _markerTitle = "";
    _markerFileName = "";
    _help = "";

}
/******************************* GETTER/SETTER/IS************************************/
/**
 * @brief Checks if the object is empty
 *
 * @return bool
 */
bool SolverMarkerMenu::isEmpty()
{
    if  ((_markerTitle.isEmpty()) && (_markerFileName.isEmpty()) && (_help.isEmpty()))
         return true;
    else
        return false;
}

/**
 * @brief Get the marker title
 *
 * @return QString
 */
QString SolverMarkerMenu::getMarkerTitle()
{
    return _markerTitle;
}

/**
 * @brief SEt the marker title
 *
 * @param _newTitle
 */
void SolverMarkerMenu::setMarkerTitle(QString _newTitle)
{
   _markerTitle = _newTitle;
}

/**
 * @brief Get marker value name (marker tag in the file)
 *
 * @return QString
 */
QString SolverMarkerMenu::getMarkerFileName()
{
    return _markerFileName;
}

/**
 * @brief returns marker value name (marker tag in the file)
 *
 * @param _newFileName
 */
void SolverMarkerMenu::setMarkerFileName(QString _newFileName)
{
    _markerFileName = _newFileName;
}

/**
 * @brief Get the parameter list
 *
 * @return QStringList
 */
QStringList SolverMarkerMenu::getParametersList()
{
    return _parameterList;
}

/**
 * @brief set the parameter list
 *
 * @param _newList
 */
void SolverMarkerMenu::setParametersList(QStringList _newList)
{
    _parameterList = _newList;
}

/**
 * @brief Set the type list
 *
 * @param _newType
 */
void SolverMarkerMenu::setTypeList(QStringList _newType)
{
    _typeList = _newType;
}

/**
 * @brief gets the type list
 *
 * @return QStringList
 */
QStringList SolverMarkerMenu::getTypeList()
{
    return _typeList;
}

/**
 * @brief Returns the associated help
 *
 * @return QString
 */
QString SolverMarkerMenu::getHelp()
{
    return _help;
}

/**
 * @brief  Sets the help text
 *
 * @param _helpText
 */
void SolverMarkerMenu::setHelp(QString _helpText)
{
    _help = _helpText;
}

/**
 * @brief get the list of units associated to the different values
 *
 * @return QStringList
 */
QStringList SolverMarkerMenu::getUnitList()
{
    return _unitList;
}


/**
 * @brief Set the list of units associated to the different values
 *
 * @param _list
 */
void SolverMarkerMenu::setUnitList(QStringList _list)
{
    _unitList = _list;
}


QStringList SolverMarkerMenu::getDefaultValueList()
{
    return _defaultValueList;
}

void SolverMarkerMenu::setDefaultValueList(QStringList _list)
{
    _defaultValueList = _list;
}
