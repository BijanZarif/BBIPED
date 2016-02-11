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
#include "templatemanager.h"

#include <QFileInfo>
#include <QDir>
#include <QFile>
#include <QHashIterator>
#include "../Defines/Tool/Solver/SolverDefines.h"
#include "../Defines/General/GeneralDefines.h"
#include "../Data/solversectiondao.h"
#include "../Data/Solver/VO/solvervarhelpvo.h"


/**
 * @brief Constructor
 *
 */
TemplateManager::TemplateManager()
{
    _sectionId = 0;
    _subsectionId = 0;
    _variableId = 0;
}

/************************************** Save ************************************************/

/**
 * @brief Save the template values in a file
 *
 * @param _templatePath
 * @param QHash<QString
 * @param _selVar
 */
void TemplateManager::saveTemplate(QString _templatePath,QHash<QString,QString> _selVar)
{
    if ( (_templatePath.isEmpty()) || (_selVar.isEmpty()) )
        return;
    _selectedVariables = _selVar;

    QString _suffix = QFileInfo(_templatePath).suffix();
    if (_suffix.isEmpty())
        _templatePath = _templatePath + ".xml";
    QFile _file(_templatePath);
    if (_file.exists())
        _file.remove();
    bool _fileCreated = _file.open(QIODevice::ReadWrite);
    if ( (_fileCreated) && (!_selectedVariables.isEmpty()) )
    {
        QXmlStreamWriter xmlWriter(&_file);
        xmlWriter.setAutoFormatting(true);
        xmlWriter.writeStartDocument();

        QStringList _sectionNames = getSectionNames();
        if  (!_sectionNames.isEmpty())
            createTemplateFile(_sectionNames,&xmlWriter);

        xmlWriter.writeEndDocument();
    }
    _file.close();

}

/************************************** Create ************************************************/

/**
 * @brief Write the template file according the list of variables
 *
 * @param _sectionList
 * @param _xmlWriter
 */
void TemplateManager::createTemplateFile(QStringList _sectionList, QXmlStreamWriter * _xmlWriter)
{
    _xmlWriter->writeStartElement(XML_ROOT_NODE_NAME);
    _xmlWriter->writeStartElement(XML_SECTION_LIST_NAME); // sections

    for (int i = 0; i < _sectionList.size(); i++)
        createSectionElement(_sectionList.at(i),_xmlWriter);
    // End sections list
    _xmlWriter->writeEndElement();
    // End of the root element
    _xmlWriter->writeEndElement();


}

/**
 * @brief Writes the section element
 *
 * @param _sectionName
 * @param _xmlWriter
 */
void TemplateManager::createSectionElement(QString _sectionName, QXmlStreamWriter * _xmlWriter)
{
     // Create Section Element
    _xmlWriter->writeStartElement(XML_SECTION_NODE_NAME);// section

    // Create the title section
    //createXmlElement(XML_TITLE_SECTION_NAME,_sectionName,_xmlWriter);
    _xmlWriter->writeStartElement(XML_TITLE_SECTION_NAME);
    _xmlWriter->writeAttribute(XML_SECTION_TITLE_IN_BASIC_VIEW_ATT,"yes");
    _xmlWriter->writeAttribute(XML_SECTION_TITLE_VISIBLE_CHILDREN_ATT,"yes");
    _xmlWriter->writeCharacters(_sectionName);
    _xmlWriter->writeEndElement();

    // Create the id
    createXmlElement(XML_SECTION_ID_NAME,QString::number(_sectionId),_xmlWriter);
    _sectionId++;

    // Create the subsectionList Element
    createSubsectionList(_sectionName,_xmlWriter);

    // End of section element
    _xmlWriter->writeEndElement();

}

/**
 * @brief  Writes the subsection element
 *
 * @param _sectionName
 * @param _xmlWriter
 */
void TemplateManager::createSubsectionList(QString _sectionName,  QXmlStreamWriter * _xmlWriter)
{
    // Create the SubsectionList Name
    _xmlWriter->writeStartElement(XML_SUBSECTION_LIST_NAME); // subsections

    QStringList _selSubSectionList = getSubSectionNamesBySectionName(_sectionName);
    if (!_selSubSectionList.isEmpty())
    {
        for (int i = 0; i < _selSubSectionList.size(); i++)
        {
            _xmlWriter->writeStartElement(XML_SUBSECTION_NAME); // subsection
            _xmlWriter->writeAttribute(XML_SUBSECTION_VISIBLE_IN_BASIC_VIEW_ATT,"yes");
            // Create the title
            createXmlElement(XML_SUBSECTION_TITLE_NAME,_selSubSectionList.at(i),_xmlWriter);
            // Create the id
            createXmlElement(XML_SUBSECTION_ID_NAME,QString::number(_subsectionId),_xmlWriter);
            _subsectionId++;
            createVariableList(_selSubSectionList.at(i), _xmlWriter);
            // End subsection name
            _xmlWriter->writeEndElement();
        }

    }

    // End SubsectionList Name
    _xmlWriter->writeEndElement();
    _subsectionId = 0; // reset the counter
}


/**
 * @brief  Writes the list of variables element
 *
 * @param _subSectionName
 * @param _xmlWriter
 */
void TemplateManager::createVariableList(QString _subSectionName, QXmlStreamWriter * _xmlWriter)
{

    _xmlWriter->writeStartElement(XML_VARIABLE_LIST_NAME);
    QList<SolverVariableVO> _varList = getVariables(_subSectionName);

    for (int i = 0; i <_varList.size();i++)
        createVariable(_varList.at(i),_xmlWriter);
    // End of the variableList
    _xmlWriter->writeEndElement();
    _variableId = 0; // reset the Counter
}

/**
 * @brief  Writes the variable element
 *
 * @param _variableVO
 * @param _xmlWriter
 */
void TemplateManager::createVariable(SolverVariableVO _variableVO, QXmlStreamWriter * _xmlWriter)
{
    _xmlWriter->writeStartElement(XML_VARIABLE_NAME);
    _xmlWriter->writeAttribute(XML_VARIABLE_ID_ATT,QString::number(_variableId));
    _variableId++;
    // Write Name
    createXmlElement(XML_VARIABLE_NAME_NAME, _variableVO.getVariableName(),_xmlWriter);
    // Write Help
    createHelpElement(_variableVO, _xmlWriter);
    // Write ComboValues
    createComboValues(_variableVO,_xmlWriter);
    // Write Value
    createValueTag(_variableVO,_xmlWriter);

    _xmlWriter->writeEndElement();

}

/**
 * @brief  Writes the help content element
 *
 * @param _variableVO
 * @param _xmlWriter
 */
void TemplateManager::createHelpElement(SolverVariableVO _variableVO, QXmlStreamWriter * _xmlWriter)
{
    _xmlWriter->writeStartElement(XML_VARIABLE_HELP_NAME);
    SolverVarHelpVO _help = _variableVO.getHelperContents();
    _xmlWriter->writeStartElement(XML_VARIABLE_HELP_DEF_ATT);
    _xmlWriter->writeCharacters(_help.getDefinitionText());
    _xmlWriter->writeEndElement();


    _xmlWriter->writeStartElement(XML_VARIABLE_HELP_VAL_ATT);
    _xmlWriter->writeCharacters(_help.getValuesText());
    _xmlWriter->writeEndElement();

    _xmlWriter->writeStartElement(XML_VARIABLE_HELP_OTHERDETAILS_ATT);
    _xmlWriter->writeCharacters(_help.getDetailsText());
    _xmlWriter->writeEndElement();

    _xmlWriter->writeEndElement();

}

/**
 * @brief  Writes the combo values element
 *
 * @param _variableVO
 * @param _xmlWriter
 */
void TemplateManager::createComboValues(SolverVariableVO _variableVO, QXmlStreamWriter * _xmlWriter)
{
    _xmlWriter->writeStartElement(XML_VARIABLE_COMBOVALUES_LIST_NAME);
    QStringList _comboValList = _variableVO.getComboValuesList();
    QStringList _comboValNameList = _variableVO.getComboValueNamesList();
    if (!_comboValNameList.isEmpty())
        for (int i = 0; i < _comboValNameList.size();i++)
        {
            _xmlWriter->writeStartElement(XML_VARIABLE_COMBOVALUE_ELEMENT_NAME);
            _xmlWriter->writeAttribute(XML_VARIABLE_COMBOVALUE_NAME_ATT,_comboValNameList.at(i));
            _xmlWriter->writeCharacters(_comboValList.at(i));
            _xmlWriter->writeEndElement();
        }

    _xmlWriter->writeEndElement();
}
/**
 * @brief  Writes the value tag element
 *
 * @param _variableVO
 * @param _xmlWriter
 */
void TemplateManager::createValueTag(SolverVariableVO _variableVO, QXmlStreamWriter * _xmlWriter)
{
    _xmlWriter->writeStartElement(XML_VARIABLE_VALUE_NAME);
    _xmlWriter->writeAttribute(XML_VARIABLE_VALUE_NAME_ATT,_variableVO.getVariableFileName());
    _xmlWriter->writeAttribute(XML_VARIABLE_VALUE_DEFAULT_VALUE_ATT, _variableVO.getDefaultValue());
    _xmlWriter->writeAttribute(XML_VARIABLE_VALUE_UNIT_ATT,_variableVO.getUnitValue());
    _xmlWriter->writeCharacters(_variableVO.getDefaultValue());
    _xmlWriter->writeEndElement();
}


/**
 * @brief  Writes a Xml node element
 *
 * @param _tagName
 * @param _value
 * @param _xmlWriter
 */
void TemplateManager::createXmlElement(QString _tagName, QString _value,QXmlStreamWriter * _xmlWriter)
{
    _xmlWriter->writeStartElement(_tagName);
    _xmlWriter->writeCharacters(_value);
    _xmlWriter->writeEndElement();
}
/************************************** Getter/Setter/Is ************************************************/

/**
 * @brief Sets the tree list
 *
 * @param _list
 */
void TemplateManager::setSectionTreeList( QList<SolverSectionVO> _list)
{
    _sectionTreeList = _list;
}
/**
 * @brief Gets teh list of solver variables associated to a subSection
 *
 * @param _subSectionName
 * @return QList<SolverVariableVO>
 */
QList<SolverVariableVO> TemplateManager::getVariables(QString _subSectionName)
{
    SolverSectionDAO _solverDAO;
    QList<SolverVariableVO> _result;
    SolverSectionVO _section = _solverDAO.findSubSection(_subSectionName,_sectionTreeList);
    QList<SolverVariableVO> _fullList = _section.getSubsectionVarList();
    QStringList _varNameList = getVariableNames(_subSectionName);

    if (!_fullList.isEmpty())
        for (int i = 0 ; i < _fullList.size(); i++)
        {
            SolverVariableVO _varVO = _fullList.at(i);
            QString _varName = _varVO.getVariableName();
            if (_varNameList.contains(_varName))
                _result.append(_varVO);
        }

    return _result;
}


/**
 * @brief Gets the list of sections
 *
 * @return QStringList
 */
QStringList TemplateManager::getSectionNames()
{
    QStringList _result;
    QStringList _subSectionNameList = getSubSectionNames();
    SolverSectionDAO _solverDAO;
    for (int i = 0; i < _subSectionNameList.size();i++)
    {
        QString _sectionName = _solverDAO.findSectionNameBySubSectionName(_subSectionNameList.at(i),_sectionTreeList);
        if( (!_sectionName.isEmpty()) && (!_result.contains(_sectionName)) )
            _result.append(_sectionName);
    }

    return _result;

}

/**
 * @brief Gets the list of subsections
 *
 * @return QStringList
 */
QStringList TemplateManager::getSubSectionNames()
{
        QStringList _result;
    QHashIterator<QString, QString> _iter(_selectedVariables);

    while (_iter.hasNext())
    {
        _iter.next();
        QString _subSectionName = _iter.value();
        if ( (!_subSectionName.isEmpty()) && (!_result.contains(_subSectionName)) )
            _result.append(_subSectionName);
    }

    return _result;

}
/**
 * @brief get the SubSectionames associated to a section
 *
 * @param _sectionName
 * @return QStringList
 */
QStringList TemplateManager::getSubSectionNamesBySectionName(QString _sectionName)
{
    QStringList _result;

    SolverSectionDAO _solverDAO;
    QStringList _subSectionList = _solverDAO.findSubSectionNamesBySectionName(_sectionName,_sectionTreeList);
    QStringList _cSelectedSubSection = getSubSectionNames();

    if ( (_subSectionList.isEmpty()) && (_cSelectedSubSection.isEmpty()) )
       return _result;

    for (int i = 0; i < _subSectionList.size();i++ )
    {
        if (_cSelectedSubSection.contains(_subSectionList.at(i)))
            _result.append(_subSectionList.at(i));
    }

    return _result;

}


/**
 * @brief Gets the list of variables of a subsection
 *
 * @param _subSectionName
 * @return QStringList
 */
QStringList TemplateManager::getVariableNames( QString _subSectionName)
{

    QStringList _result;
    if (_subSectionName.isEmpty())
            return _result;
    QHashIterator<QString, QString> _iter(_selectedVariables);
    while (_iter.hasNext())
    {
        _iter.next();
        if (QString::compare(_iter.value(), _subSectionName) == StrEqual)
             _result.append(_iter.key());
    }

    return _result;
}
