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
#include "configurationfileparser.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include "Defines/General/GeneralDefines.h"

/**
 * @brief Constructor
 *
 */
ConfigurationFileParser::ConfigurationFileParser()
{
}

QList<VirtualMrfVO> ConfigurationFileParser::getParsedVirtualMRFData()
{
    return _parsedVirtualMRFData;
}

/**
 * @brief Main function to parse the file
 *
 */
void ConfigurationFileParser::parseFileProcess()
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
void ConfigurationFileParser::analyseLine(QString _line)
{
    if (!isCommentedLine(_line))
        saveTokenElements(_line);

}

/**
 * @brief Checks if the line is a commented line to skip it
 *
 * @param _line
 * @return bool
 */
bool ConfigurationFileParser::isCommentedLine(QString _line)
{
    if (_line.startsWith("%"))
        return true;
    else
        return false;
}

/**
 * @brief Save the parsed element
 *
 * @param _line
 */
void ConfigurationFileParser::saveTokenElements(QString _line)
{
    QStringList _values = _line.split("=");
    if ((!_values.isEmpty()) && (_values.size() == 2))
    {
        QString _keyName =  removeSpaces(_values.at(0));
        QString _valueVal = _values.at(1);
        if (_valueVal.contains("%"))
           _valueVal = removeCommentedPart(_valueVal);
        if ((!_keyName.contains("VMRF")) && (!_keyName.contains("PARTIAL_OUT")))
            _currentParsedValues.insert(_keyName, removeSpaces(_valueVal));
        else
        {
            if (_keyName.contains("VMRF"))
                saveVirtualMRFValue(_keyName,_valueVal);
            if (_keyName.contains("PARTIAL_OUT"))
                saveDomainClippingValue(_keyName,_valueVal);
         }
    }
}


 void ConfigurationFileParser::saveDomainClippingValue(QString _keyName,QString _valueVal)
 {
     if ((_valueVal.isEmpty()) || (_valueVal.isNull()))
         return;
     _valueVal = _valueVal.replace("(","");
     _valueVal = _valueVal.replace(")","");
     if (_currentParsedValues.contains(_keyName))
     {
         QStringList _oldValueList = _currentParsedValues.value(_keyName).split(";");
         _oldValueList.append(_valueVal);
         _currentParsedValues.insert(_keyName,_oldValueList.join(";"));

     }else
         _currentParsedValues.insert(_keyName,_valueVal);
 }

/**
 * @brief
 *
 * @param _keyName
 * @param _valueVal
 */
void ConfigurationFileParser::saveVirtualMRFValue(QString _keyName,QString _valueVal)
{
    VirtualMrfVO _newVirtualData;
    _valueVal = _valueVal.replace("(","");
    _valueVal = _valueVal.replace(")","");
    QStringList _values = _valueVal.split(",");
    if (_values.isEmpty())
        return;

    QString _valueName = _values.at(0);
    if (QString::compare(_keyName,VMRF_AXIS_KEYNAME) == StrEqual )
    {
        if (_values.size() == 8) // before == 8 but, now the number of parameters is 7
        {
            _newVirtualData.setZoneName(_valueName);
            _newVirtualData.setOmega(_values.at(1));
            QString _axisCenter = _values.at(2) + COMMA_SEPARATOR + _values.at(3) + COMMA_SEPARATOR + _values.at(4);
            _newVirtualData.setAxisCenter(_axisCenter.split(COMMA_SEPARATOR));
            QString _axisNormal = _values.at(5) + COMMA_SEPARATOR + _values.at(6) + COMMA_SEPARATOR + _values.at(7);
            _newVirtualData.setAxisNormal(_axisNormal.split(COMMA_SEPARATOR));
            _parsedVirtualMRFData.append(_newVirtualData);
        }
    }
    if (QString::compare(_keyName,VMRF_INNER_CURVE_KEYNAME)== StrEqual )
    {
        int _objPos = findVirtualMRFPositionByZoneName(_valueName);
        if ((_objPos != -1) && (_objPos < _parsedVirtualMRFData.size()))
        {
            _newVirtualData = _parsedVirtualMRFData.at(_objPos);
            _values.removeAt(1);
            _values.removeAt(0);
            _newVirtualData.setInnerCurvePoints(_values);
            _parsedVirtualMRFData[_objPos]  = _newVirtualData;
        }

    }
    if  (QString::compare(_keyName,VMRF_OUTER_CURVE_KEYNAME)== StrEqual )
    {
        int _objPos = findVirtualMRFPositionByZoneName(_valueName);
        if ((_objPos != -1) && (_objPos < _parsedVirtualMRFData.size()))
         {
            _newVirtualData = _parsedVirtualMRFData.at(_objPos);
            _values.removeAt(1);
            _values.removeAt(0);
            _newVirtualData.setOuterCurvePoints(_values);
            _parsedVirtualMRFData[_objPos]  = _newVirtualData;

        }
    }

}

/**
 * @brief
 *
 * @param _name
 * @return int
 */
int ConfigurationFileParser::findVirtualMRFPositionByZoneName(QString _name)
{
    int _result = -1;
    for (int i = 0; i < _parsedVirtualMRFData.size();i++)
    {
        VirtualMrfVO _virtualData = _parsedVirtualMRFData.at(i);
        QString _cName = _virtualData.getZoneName();
        if (QString::compare(_name,_cName,Qt::CaseSensitive) == StrEqual)
            return i;
    }

    return _result;
}

/**
 * @brief
 *
 * @param _text
 * @return QString
 */
QString ConfigurationFileParser::removeCommentedPart(QString _text)
{
    QString _result;
    QStringList _list = _text.split("%");
    if (!_list.isEmpty())
        return _result.at(0);
    else
        return _result;
}
