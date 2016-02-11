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
 *  Author: Dec 2016: Carmen Alonso Montes calonso@bcamath.org
***/
#include "meshfileparser.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>

/**
 * @brief Constructor
 *
 */
MeshFileParser::MeshFileParser()
{
    _id = 0;
    _is3dMesh = false; // By default consider 2D
    _found3DMesh = false;
}

/**
 * @brief Main function to parse the file
 *
 */
void MeshFileParser::parseFileProcess()
{
    QString _path = this->getFilePath();
    QFile _file(_path);
    bool _fileCreated = false;
    if (_file.exists())
    {
         _fileCreated = _file.open(QIODevice::ReadOnly);

         if (_fileCreated )
         {
             QTextStream _reader(&_file);
             QString _line = _reader.readLine();
             while(!_line.isNull())
             {
                 analyseLine(_line);
                 _line = _reader.readLine();
             }
             setParsedValues(_currentParsedValues);
         }
         _file.close();
    }
}

/**
 * @brief Function that analyses the line to be parsed
 *
 * @param _line
 */
void MeshFileParser::analyseLine(QString _line)
{
    if (containsKeyTagName(_line))
    {
        saveTokenElements(_line);
    }
}

/**
 * @brief Function that checks if the line contains the MARKER_TAG
 *
 * @param _line
 * @return bool
 */
bool MeshFileParser::containsKeyTagName(QString _line)
{
    if (_line.contains("MARKER_TAG"))
        return true;
    else
        if( (!_found3DMesh) && (_line.contains("NDIME")))
        {
            QStringList _listElements = _line.split("=");
            for (int i=0; i < _listElements.size();i++)
            {
                QString _element =_listElements.at(i);
                if (!_element.isEmpty())
                {
                    bool isInt = false;
                    int _dim = _element.toInt(&isInt);
                    if (isInt)
                        if (_dim == 3)
                            _is3dMesh = true;

                }
            }
            _found3DMesh = true;
            return false;

        }else
            return false;
}

/**
 * @brief Function that stores the parsed toked elements
 *
 * @param _line
 */
void MeshFileParser::saveTokenElements(QString _line)
{
    QStringList _values = _line.split("=");
    if ((!_values.isEmpty()) && (_values.size() == 2))
    {
        // Key_name = name of the variable, and the value is Marker_tag
        _currentParsedValues.insert(QString::number(_id), removeSpaces(_values.at(1)));
        _id++;
    }
}


bool MeshFileParser::is3DMesh()
{
    return _is3dMesh;
}
