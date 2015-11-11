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
#include "parserfiles.h"

/**
 * @brief Constructor
 *
 */
ParserFiles::ParserFiles()
{
    _filePath = "";
    _parsedValues.empty();
}

/**
 * @brief Returns the path of the file
 *
 * @return QString
 */
QString ParserFiles::getFilePath()
{
    return _filePath;
}

/**
 * @brief Establish the file path
 *
 * @param _newPath
 */
void ParserFiles::setFilePath (QString _newPath)
{
    _filePath = _newPath;
}

/**
 * @brief Returns the map of the parsed values
 *
 * @return QHash<QString, QString>
 */
QHash<QString,QString> ParserFiles::getParsedValues()
{
    return _parsedValues;
}

/**
 * @brief Sets the parsed values
 *
 * @param QHash<QString
 * @param _newValues
 */
void ParserFiles::setParsedValues(QHash<QString,QString> _newValues)
{
    _parsedValues = _newValues;
}

/**
 * @brief Function to remove spaces in the values
 *
 * @param _value
 * @return QString
 */
QString ParserFiles::removeSpaces(QString _value)
{
    _value = _value.simplified();
    _value.replace(" ","");
    return _value;
}
