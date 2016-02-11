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
#include "markermenumanager.h"

#include "Solver/markermenureader.h"
#include "Defines/General/GeneralDefines.h"

/**
 * @brief Constructor
 *
 */
MarkerMenuManager::MarkerMenuManager()
{
  MarkerMenuReader _reader;
  _reader.buildAllMarkerMenuNodes();
  this->_markerMenuList = _reader.getMarkerMenuList();
}

/**
 * @brief Function that returns the marker menu list to be shown in the tree
 *
 * @return QList<SolverMarkerMenu>
 */
QList<SolverMarkerMenu> MarkerMenuManager::getAllMarkerMenuList()
{
    return _markerMenuList;
}

/**
 * @brief Function that obtains the Marker element by the node name
 *
 * @param _nodeName
 * @return SolverMarkerMenu
 */
SolverMarkerMenu MarkerMenuManager::getMarkerNodeByName(QString _nodeName)
{
    SolverMarkerMenu _result;

    for (int i = 0; i < _markerMenuList.count(); i++)
    {
        SolverMarkerMenu _nameToComapre = _markerMenuList.at(i);
        if (QString::compare(_nodeName,_nameToComapre.getMarkerTitle()) == StrEqual)
            return _markerMenuList.at(i);
    }

    return _result;
}
