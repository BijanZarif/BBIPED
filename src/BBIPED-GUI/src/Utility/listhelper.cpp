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
#include "listhelper.h"

/**
 * @brief
 *
 */
ListHelper::ListHelper()
{
}
/**
 * @brief Function that searchs the value into the list, returns true if it is found
 *
 * @param _list
 * @param _value
 * @return bool
 */
bool ListHelper::isContainedInList(QStringList _list, QString _value)
{
    for (int i = 0; i < _list.count() ; i++)
    {
        if (_value.contains(_list.at(i)))
            return true;
    }
    return false;

}

/**
 * @brief Function that obtains a Qstringlist formed by the values contained in the map (excluding the keys)
 *
 * @param QHash<QString
 * @param _map
 * @return QStringList
 */
QStringList ListHelper::getListFromQHash(QHash<QString,QString> _map)
{
    QHashIterator<QString,QString> _iter(_map);
    QStringList _list;
    while (_iter.hasNext())
    {
        _iter.next();
        _list.append(_iter.value());
    }
    return _list;
}
