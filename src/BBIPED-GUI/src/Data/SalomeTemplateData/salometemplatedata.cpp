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
#include "salometemplatedata.h"
#include "Defines/General/GeneralDefines.h"

/**
 * @brief Constructor
 *
 */
SalomeTemplateData::SalomeTemplateData()
{
    _templateName ="";
    _templateFileName ="";
    _variableList.clear();
}
/**
 * @brief Gets the template Name (the one shown to the user)
 *
 * @return QString
 */
QString SalomeTemplateData::getTemplateName()
{
    return _templateName;
}

/**
 * @brief Sets the name of the template (the one shown to the user)
 *
 * @param _name
 */
void SalomeTemplateData::setTemplateName(QString _name)
{
    _templateName = _name;
}

/**
 * @brief Gets the name of the file associated to the template
 *
 * @return QString
 */
QString SalomeTemplateData::getTemplateFileName()
{
    return _templateFileName;
}

/**
 * @brief Sets the name of the file associated to the template
 *
 * @param _name
 */
void SalomeTemplateData::setTemplateFileName(QString _name)
{
    _templateFileName = removeWhiteSpaces(_name);
}


/**
 * @brief Get the list of variables of the template
 *
 * @return QList<SalomeVariableData>
 */
QList<SalomeVariableData> SalomeTemplateData::getVariableList()
{
    return _variableList;
}

/**
 * @brief Set the list of variables within the template
 *
 * @param _list
 */
void SalomeTemplateData::setVariableList(QList<SalomeVariableData> _list)
{
    _variableList = _list;
}


/**
 * @brief Checks if the template does not exist (template name, file name and variablelist empty or null)
 *
 * @return bool
 */
bool SalomeTemplateData::isEmpty()
{
    if ((_templateName.isEmpty()) && (_templateFileName.isEmpty()) && (_variableList.isEmpty()))
        return true;
    else
        return false;
}
/**
 * @brief Remove wite spaces of any QString
 *
 * @param _str
 * @return QString
 */
QString SalomeTemplateData::removeWhiteSpaces(QString _str)
{
    _str = _str.simplified();
    _str.replace( " ", "" );
    return _str;
}


/**
 * @brief Find any SalomeVariable Data by his name
 *
 * @param _name
 * @return SalomeVariableData
 */
SalomeVariableData SalomeTemplateData::findByName(QString _name)
{
    SalomeVariableData _foundResult;
    if (!_variableList.isEmpty())
    {
        for (int i = 0; i < _variableList.size();i++)
        {
            SalomeVariableData _cVar = _variableList.at(i);
            QString _varName = _cVar.getName();
            if (QString::compare(_varName,_name) == StrEqual)
                return _cVar;
        }
    }
    return _foundResult;
}
