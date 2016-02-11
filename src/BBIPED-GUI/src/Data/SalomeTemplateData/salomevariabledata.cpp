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
 *
 * * Author: Dec 2016: Carmen Alonso Montes calonso@bcamath.org
***/
#include "salomevariabledata.h"

/**
 * @brief Constructor
 *
 */
SalomeVariableData::SalomeVariableData()
{
    _name = "";
    _fileName = "";
    _defaultValue = "";
    _currentValue = "";
    _optimizedValue = "NO";
}
/**
 * @brief Destructor
 *
 * @return QString
 */
QString SalomeVariableData::getName()
{
    return _name;
}

/**
 * @brief Set the template name
 *
 * @param _newName
 */
void SalomeVariableData::setName(QString _newName)
{
    _name =_newName;
}

/**
 * @brief get the file name of the template
 *
 * @return QString
 */
QString SalomeVariableData::getFileName()
{
   return _fileName;
}
/**
 * @brief set the file name of the template
 *
 * @param _newName
 */
void SalomeVariableData::setFileName(QString _newName)
{
    _fileName = removeWhiteSpaces(_newName);
}

/**
 * @brief get the default value
 *
 * @return QString
 */
QString SalomeVariableData::getDefaultValue()
{
    return _defaultValue;
}
/**
 * @brief set the default value
 *
 * @param _val
 */
void SalomeVariableData::setDefaultValue(QString _val)
{
    _defaultValue = removeWhiteSpaces(_val);
}
/**
 * @brief get the current value of the variable
 *
 * @return QString
 */
QString SalomeVariableData::getCurrentValue()
{
    return _currentValue;
}

/**
 * @brief Set the current value of the variable
 *
 * @param _val
 */
void SalomeVariableData::setCurrentValue(QString _val)
{
    _currentValue =  removeWhiteSpaces(_val);
}

/**
 * @brief get the optimized values
 *
 * @return QString
 */
QString SalomeVariableData::getOptimizedValue()
{
    return _optimizedValue;
}

/**
 * @brief Set the optimized values
 *
 * @param _val
 */
void SalomeVariableData::setOptimizedValue(QString _val)
{
    _optimizedValue = removeWhiteSpaces(_val);
}


/**
 * @brief Remove white spaces
 *
 * @param _str
 * @return QString
 */
QString SalomeVariableData::removeWhiteSpaces(QString _str)
{
    _str = _str.simplified();
    _str.replace( " ", "" );
    return _str;
}
