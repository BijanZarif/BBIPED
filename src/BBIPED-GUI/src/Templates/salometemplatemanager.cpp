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
#include "salometemplatemanager.h"
#include <QFile>
#include <QDomNode>
#include <QDomNodeList>
#include <QXmlStreamWriter>

#include <stdio.h>
#include <iostream>
using namespace std;

#include "SalomeTemplate_Defines.h"
#include "../Defines/General/GeneralDefines.h"
#include <QDir>
#include <QCoreApplication>
/**
 * @brief Constructor
 *
 */
SalomeTemplateManager::SalomeTemplateManager()
{
    _bbipedHomePath = QCoreApplication::applicationDirPath();
    _templatePath = QDir(_bbipedHomePath).filePath(".tools/Automatization/Templates/Salome/templates_salome.xml");
    _isInitialized = false;
}

/**
 * @brief
 *
 */
void SalomeTemplateManager::loadTemplate()
{
    // Loading the file from the resource
    QFile * _template = new QFile(_templatePath);
    QDomDocument * _domTree = new QDomDocument();

    if( !_template->open( QIODevice::ReadOnly | QIODevice::Text ) )
        qDebug( "Failed to open file for reading." );
    else
    {
        if( !_domTree->setContent( _template ) )
            qDebug( "Failed to parse the file into a DOM tree." );

        buildTemplateMenus(_domTree);
        _isInitialized = true;
    }
    _template->close();
}

/**
 * @brief
 *
 * @param _templates
 */
void SalomeTemplateManager::updateTemplateValues(QList<SalomeTemplateData> _templates)
{
    _templatesList = _templates;
    updateTemplateNameList(selectPredefinedTemplates);
      saveTemplate();
}

/**
 * @brief
 *
 * @param _option
 */
void SalomeTemplateManager::updateTemplateNameList(int _option)
{
    if (_option == selectPredefinedTemplates)
    {
        _templateNameList.clear();
        for (int i = 0; i < _templatesList.size();i++)
        {
            SalomeTemplateData _selTemplate = _templatesList.at(i);
            _templateNameList.push_back(_selTemplate.getTemplateName());
        }
    }
    if (_option == selectUserDefinedTemplates)
    {
        _userTemplateNameList.clear();
        for (int i = 0; i < _userTemplatesList.size();i++)
        {
            SalomeTemplateData _selTemplate = _userTemplatesList.at(i);
            _userTemplateNameList.push_back(_selTemplate.getTemplateName());
        }
    }
}

/**
 * @brief
 *
 * @param _templates
 */
void SalomeTemplateManager::updateUserTemplateValues(QList<SalomeTemplateData> _templates)
{
    _userTemplatesList = _templates;
    updateTemplateNameList(selectUserDefinedTemplates);
    saveTemplate();
}
/**************************************************** Build TEMPLATE **************************************************/

/**
 * @brief
 *
 * @param _domTree
 */
void SalomeTemplateManager::buildTemplateMenus(QDomDocument * _domTree)
{
    QDomElement _rootElement = _domTree->documentElement();
    if (!_rootElement.isNull())
    {
        //cout << "TAG " << _rootElement.tagName().toStdString() << endl;
        if (QString::compare(_rootElement.tagName(),XML_TEMPLATE_SALOME_NODE) == StrEqual)
        {
            QDomElement _templatesNode = _rootElement.firstChild().toElement();
            if (!_templatesNode.isNull())
                buildPredefinedTemplateMenus(_templatesNode);
            QDomElement _userTemplatesNode = _rootElement.firstChild().nextSibling().toElement();
            if (!_userTemplatesNode.isNull())
                buildUserTemplateMenus(_userTemplatesNode);
        }

    }

}

/**
 * @brief
 *
 * @param _templateListNode
 */
void SalomeTemplateManager::buildPredefinedTemplateMenus(QDomElement _templateListNode)
{
    if ((!_templateListNode.isNull() ) && (QString::compare(_templateListNode.tagName(),XML_TEMPLATE_LIST_NODE) == StrEqual))
    {
        QDomNodeList _templateList = _templateListNode.childNodes();
        for (int i = 0; i < _templateList.size(); i++)
        {
            QDomElement _templateEl = _templateList.at(i).toElement();
            buildTemplateData(_templateEl,TEMPLATE_LIST);
        }
    }

}


/**
 * @brief
 *
 * @param _templateNode
 * @param _option
 */
void SalomeTemplateManager::buildTemplateData(QDomElement _templateNode,int _option)
{
    if ( (!_templateNode.isNull()) && (QString::compare(_templateNode.tagName(),XML_TEMPLATE_NODE) == StrEqual))
    {
        QDomNodeList _templateChildren= _templateNode.childNodes();
        SalomeTemplateData _templateData;
        for (int i = 0; i < _templateChildren.size(); i++)
        {
            QDomElement _templateEl = _templateChildren.at(i).toElement();

            if ((_templateEl.isNull()) || (_templateEl.tagName().isEmpty()) )
                continue;
            if (QString::compare(_templateEl.tagName(),XML_TEMPLATE_NAME_NODE) == StrEqual)
                _templateData.setTemplateName(_templateEl.text());

            if (QString::compare(_templateEl.tagName(),XML_TEMPLATE_FILE_NAME_NODE) == StrEqual)
                _templateData.setTemplateFileName(_templateEl.text());
            if (QString::compare(_templateEl.tagName(),XML_VARIABLE_LIST_NODE) == StrEqual)
                buildVariableList(_templateEl,&_templateData);

        }
        // Saving the data of the template
        if (_option == TEMPLATE_LIST)
        {
            _templatesList.push_back(_templateData);
            _templateNameList.push_back(_templateData.getTemplateName());
        }
        else
        {
            _userTemplatesList.push_back(_templateData);
            _userTemplateNameList.push_back(_templateData.getTemplateName());
        }
    }
}

/**
 * @brief
 *
 * @param _variableListNode
 * @param _template
 */
void SalomeTemplateManager::buildVariableList(QDomElement _variableListNode, SalomeTemplateData * _template)
{
    if ( (!_variableListNode.isNull()) && (QString::compare(_variableListNode.tagName(),XML_VARIABLE_LIST_NODE) == StrEqual))
    {
        QDomNodeList _varChildren= _variableListNode.childNodes();
        QList<SalomeVariableData> _varList;
        for (int i=0; i<_varChildren.size();i++)
        {
            QDomElement _varEl = _varChildren.at(i).toElement();
            if (_varEl.isNull())
                continue;
           SalomeVariableData _salomeVariable =  buildVariableData(_varEl);
           _varList.push_back(_salomeVariable);
        }
        _template->setVariableList(_varList);
    }
}

/**
 * @brief
 *
 * @param _variableNode
 * @return SalomeVariableData
 */
SalomeVariableData SalomeTemplateManager::buildVariableData(QDomElement _variableNode)
{
    SalomeVariableData _variableData;
    if ( (!_variableNode.isNull()) && (QString::compare(_variableNode.tagName(),XML_VARIABLE_NODE) == StrEqual))
    {
        QDomNodeList _varChildren= _variableNode.childNodes();
        for (int i=0; i<_varChildren.size();i++)
        {
            QDomElement _varEl = _varChildren.at(i).toElement();

            if (_varEl.isNull())
                continue;
            if (QString::compare(_varEl.tagName(),XML_VARIABLE_NAME_NODE) == StrEqual)
               _variableData.setName(_varEl.text());
            if (QString::compare(_varEl.tagName(),XML_VARIABLE_FILE_NAME_NODE) == StrEqual)
               _variableData.setFileName(_varEl.text());
            if (QString::compare(_varEl.tagName(),XML_VARIABLE_DEFAULT_VALUE_NODE) == StrEqual)
               _variableData.setDefaultValue(_varEl.text());
            if (QString::compare(_varEl.tagName(),XML_VARIABLE_CURRENT_VALUE_NODE) == StrEqual)
               _variableData.setCurrentValue(_varEl.text());
            if (QString::compare(_varEl.tagName(),XML_VARIABLE_ISOPTIMIZED_NODE) == StrEqual)
               _variableData.setOptimizedValue(_varEl.text());
        }
    }
    return _variableData;
}


/**
 * @brief
 *
 * @param _templateListNode
 */
void SalomeTemplateManager::buildUserTemplateMenus(QDomElement _templateListNode)
{
    if ((!_templateListNode.isNull() ) && (QString::compare(_templateListNode.tagName(),XML_USER_TEMPLATE_LIST_NODE) == StrEqual))
    {
        QDomNodeList _templateList = _templateListNode.childNodes();
        for (int i = 0; i < _templateList.size(); i++)
        {
            QDomElement _templateEl = _templateList.at(i).toElement();
            buildTemplateData(_templateEl,USER_TEMPLATE_LIST);
        }
    }
}

/**
 * @brief
 *
 */
void SalomeTemplateManager::saveTemplate()
{
    QFile _file(_templatePath);
    bool _fileCreated = false;

    if (_file.exists())
        _file.remove();

    _fileCreated = _file.open(QIODevice::ReadWrite);
    if (_fileCreated)
    {
        QXmlStreamWriter xmlWriter(&_file);
        xmlWriter.setAutoFormatting(true);
        xmlWriter.writeStartDocument();

        xmlWriter.writeStartElement(XML_TEMPLATE_SALOME_NODE);

        xmlWriter.writeStartElement(XML_TEMPLATE_LIST_NODE);
        saveTemplateData(&xmlWriter, _templatesList);
        xmlWriter.writeEndElement();
        xmlWriter.writeStartElement(XML_USER_TEMPLATE_LIST_NODE);
        saveTemplateData(&xmlWriter,_userTemplatesList);
        xmlWriter.writeEndElement();

        xmlWriter.writeEndDocument();

    }
}

/**
 * @brief
 *
 * @param _xmlWriter
 * @param _template
 */
void SalomeTemplateManager::saveTemplateData(QXmlStreamWriter * _xmlWriter, QList<SalomeTemplateData> _template)
{
    for (int i = 0; i < _template.size(); i++)
    {
        SalomeTemplateData _templateData = _template.at(i);
        _xmlWriter->writeStartElement(XML_TEMPLATE_NODE);
            // Name of the template
            _xmlWriter->writeStartElement(XML_TEMPLATE_NAME_NODE);
            _xmlWriter->writeCharacters(_templateData.getTemplateName());
            _xmlWriter->writeEndElement();
            // Name of the file name template
            _xmlWriter->writeStartElement(XML_TEMPLATE_FILE_NAME_NODE);
            _xmlWriter->writeCharacters(_templateData.getTemplateFileName());
            _xmlWriter->writeEndElement();
            // Variables Section
            _xmlWriter->writeStartElement(XML_VARIABLE_LIST_NODE);
            saveVariableData(_xmlWriter,_templateData.getVariableList());
            _xmlWriter->writeEndElement();

        _xmlWriter->writeEndElement();
    }
}

/**
 * @brief
 *
 * @param _xmlWriter
 * @param _varList
 */
void SalomeTemplateManager::saveVariableData(QXmlStreamWriter * _xmlWriter, QList<SalomeVariableData> _varList)
{
    for (int i = 0; i < _varList.size();i++)
    {
        SalomeVariableData _var = _varList.at(i);
        _xmlWriter->writeStartElement(XML_VARIABLE_NODE);
        _xmlWriter->writeStartElement(XML_VARIABLE_NAME_NODE);
        _xmlWriter->writeCharacters(_var.getName());
        _xmlWriter->writeEndElement();

        _xmlWriter->writeStartElement(XML_VARIABLE_FILE_NAME_NODE);
        _xmlWriter->writeCharacters(_var.getFileName());
        _xmlWriter->writeEndElement();

        _xmlWriter->writeStartElement(XML_VARIABLE_DEFAULT_VALUE_NODE);
        _xmlWriter->writeCharacters(_var.getDefaultValue());
        _xmlWriter->writeEndElement();

        _xmlWriter->writeStartElement(XML_VARIABLE_CURRENT_VALUE_NODE);
        _xmlWriter->writeCharacters(_var.getCurrentValue());
        _xmlWriter->writeEndElement();

        _xmlWriter->writeStartElement(XML_VARIABLE_ISOPTIMIZED_NODE);
        _xmlWriter->writeCharacters(_var.getOptimizedValue());
        _xmlWriter->writeEndElement();

        _xmlWriter->writeEndElement();
    }
}

/**************************************************** GETTER /SETTER **************************************************/
/**
 * @brief
 *
 * @return QList<SalomeTemplateData>
 */
QList<SalomeTemplateData> SalomeTemplateManager::getTemplates()
{
    return _templatesList;
}

/**
 * @brief
 *
 * @return QList<SalomeTemplateData>
 */
QList<SalomeTemplateData> SalomeTemplateManager::getUserTemplates()
{
    return _userTemplatesList;
}
/**
 * @brief
 *
 * @return QStringList
 */
QStringList SalomeTemplateManager::getUserTemplateNameList()
{
    return _userTemplateNameList;
}

/**
 * @brief
 *
 * @return QStringList
 */
QStringList SalomeTemplateManager::getTemplateNameList()
{
    return _templateNameList;
}
/**
 * @brief
 *
 * @return bool
 */
bool SalomeTemplateManager::isInitialized()
{
    return _isInitialized;
}

/**
 * @brief
 *
 * @param _templateName
 * @return QString
 */
QString SalomeTemplateManager::getTemplateFileNameByTemplateName(QString _templateName)
{
    if (_templateName.isEmpty())
        return QString("");
    int _templateNo = _templateNameList.indexOf(_templateName);
    bool _isUserTemplate = false;
    if (_templateNo == -1)
    {
      _templateNo = _userTemplateNameList.indexOf(_templateName);
      if (_templateNo == -1)
          return QString("");
      else
          _isUserTemplate = true;

    }

    SalomeTemplateData _selectedTemplate;
    if( (_isUserTemplate) && (_templateNo < _userTemplatesList.size()) )
    {
        _selectedTemplate = _userTemplatesList.at(_templateNo);

    }else
    {
        if( (!_isUserTemplate) && (_templateNo < _templatesList.size()) )
            _selectedTemplate = _templatesList.at(_templateNo);
    }

    return _selectedTemplate.getTemplateFileName();

}


void SalomeTemplateManager::deleteUserDefinedTempalte(QString _templateName)
{
    if (_templateName.isEmpty())
        return;
    int _pos = _userTemplateNameList.indexOf(_templateName);
    if (_pos == -1)
        return;
    _userTemplatesList.removeAt(_pos);
    _userTemplateNameList.removeAt(_pos);
    saveTemplate();

}

void SalomeTemplateManager::restoreToDefaultTemplate(int i)
{

    SalomeTemplateData _tempTemplate = _templatesList.at(i);
    if (_tempTemplate.isEmpty())
        return;
   QList<SalomeVariableData> _varList =  _tempTemplate.getVariableList();
   QList<SalomeVariableData> _newList;
   if (_varList.isEmpty())
       return;
   for (int i = 0; i < _varList.size(); i++)
   {
       SalomeVariableData _data = _varList.at(i);
       if (QString::compare(_data.getCurrentValue(),_data.getDefaultValue()) != StrEqual)
       {
           QString _defaultVal = _data.getDefaultValue();
           _data.setCurrentValue(_defaultVal);
       }
       _newList.push_back(_data);
   }
   _tempTemplate.setVariableList(_newList);
   _templatesList[i] = _tempTemplate;

}
/*void SalomeTemplateManager::setBBIPEDHome(QString _homePath)
{
    _bbipedHomePath = _homePath;
    // We need to update the BBIPED Template path
    _templatePath = QDir(_bbipedHomePath).file(_templatePath);
}
*/

/*
void SalomeTemplateManager::restoreToDefaultUserTemplate(int i)
{
    SalomeTemplateData _tempTemplate = _userTemplatesList.at(i);
    if (_tempTemplate.isEmpty())
        return;
   QList<SalomeVariableData> _varList =  _tempTemplate.getVariableList();
   QList<SalomeVariableData> _newList;
   if (_varList.isEmpty())
       return;
   for (int i = 0; i < _varList.size(); i++)
   {
       SalomeVariableData _data = _varList.at(i);
       if (QString::compare(_data.getCurrentValue(),_data.getDefaultValue()) != StrEqual)
       {
           QString _defaultVal = _data.getDefaultValue();
           _data.setCurrentValue(_defaultVal);
       }
       _newList.push_back(_data);
   }
   _tempTemplate.setVariableList(_newList);
   _userTemplatesList[i] = _tempTemplate;

}
*/
