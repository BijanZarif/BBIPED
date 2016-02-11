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
 *  *  Author: Dec 2016: Carmen Alonso Montes calonso@bcamath.org

***/
#include "searchedpair.h"

/**
 * @brief
 *
 */
SearchedPair::SearchedPair()
{
}
/**
 * @brief Gets the Id of the parent (read from the xml file)
 *
 * @return int
 */
int SearchedPair::getIdParent()
{
    return _idParent;
}

/**
 * @brief Set the Id of the parent
 *
 * @param _id
 */
void SearchedPair::setIdParent(int _id)
{
    _idParent = _id;
}

/**
 * @brief Gets the Id of the child (read from the xml file)
 *
 * @return int
 */
int SearchedPair::getIdChild()
{
    return _idChild;
}

/**
 * @brief sets the Id of the child (read from the xml file)
 *
 * @param _id
 */
void SearchedPair::setIdChild(int _id)
{
    _idChild = _id;
}


/**
 * @brief Get the children list
 *
 * @return QList<int>
 */
QList<int> SearchedPair::getChildrenList()
{
    return _listChildren;
}

/**
 * @brief Set the children list
 *
 * @param _list
 */
void SearchedPair::setChildrenList(QList<int> _list)
{
    _listChildren = _list;
}

/**
 * @brief Returns true if the id of the parent and the child are the same as stored in the object
 *
 * @param _nidParent
 * @param _nidChild
 * @return bool
 */
bool SearchedPair::contains(int _nidParent, int _nidChild)
{
    if ((_idParent ==_nidParent) && (_idChild == _nidChild))
            return true;
    else
        return false;
}
