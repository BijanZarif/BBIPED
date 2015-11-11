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
#include "abstractrule.h"

/**
 * @brief Constructor
 *
 */
AbstractRule::AbstractRule()
{
}

/**
 * @brief Set the variables that must be disabled in case a value is provided to this one
 *
 * @param _list
 */
void AbstractRule::setVariablesToBeDisabled(QStringList _list)
{
    _listVariablesToBeDisabled = _list;
}

/**
 * @brief Get the variables that must be disabled in case a value is provided to this one
 *
 * @return QStringList
 */
QStringList AbstractRule::getVariablesToBeDisabled()
{
    return _listVariablesToBeDisabled;
}

/**
 * @brief set the variables that must be enabled in case a value is provided to this one
 *
 * @param _list
 */
void AbstractRule::setVariablesToBeEnabled(QStringList _list)
{
   _listVariablesToBeEnabled = _list;
}

/**
 * @brief get the variables that must be enabled in case a value is provided to this one
 *
 * @return QStringList
 */
QStringList AbstractRule::getVariablesToBeEnabled()
{
    return _listVariablesToBeEnabled;
}
