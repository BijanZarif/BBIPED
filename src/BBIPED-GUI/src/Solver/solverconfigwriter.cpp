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
#include "solverconfigwriter.h"

#include <QFile>
#include <QString>
#include <QDate>

// DOM includes
#include <QDomDocument>
#include <QDomElement>
#include <stdio.h>
#include <iostream>


#include "Defines/General/GeneralDefines.h"
#include "Defines/Tool/Solver/SolverDefines.h"
using namespace std;




/**
 * @brief Constructor
 *
 */
SolverConfigWriter::SolverConfigWriter()
{
}


/************************************** Write ************************************************/


/**
 * @brief Function to write the values of the configuration values provided by the user
 *  in the correspondent configuration file
 *
 * @param _filePath
 * @param _configTree
 */
void SolverConfigWriter::writeToConfigurationFile(QString _filePath, QDomDocument *  _configTree,
                                                  ProjectDetails _projectDetails)
{
    QFile _file(_filePath);

    if (_file.exists())
        _file.remove();

    bool _fileCreated = _file.open(QIODevice::ReadWrite | QIODevice::Text);
    if (_fileCreated)
    {
       QTextStream * _fileWrite = new QTextStream(&_file);
       writeHeaderFile(_fileWrite, _projectDetails);
       writeDOMTree(_configTree,_fileWrite);
       delete _fileWrite;

    }
    _file.close();
}


/**
 * @brief Function that writes the Header File of the Configuration File
 *
 * @param _fileOutput
 */
void SolverConfigWriter::writeHeaderFile(QTextStream * _fileOutput, ProjectDetails _projectDetails)
{
    QDate date = QDate::currentDate();
    QString dateString = date.toString();
    *_fileOutput << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;
    *_fileOutput << "% CONFIGURATION FILE                                                                %" << endl;
    *_fileOutput << "% Project Title:   "<< _projectDetails.getProjectTile()  <<"     %" << endl;
    *_fileOutput << "% Author:      " << _projectDetails.getAuthor() <<"     %" << endl;
    *_fileOutput << "% Date of the project:" << _projectDetails.getStartingDate() << "     %" << endl;
    *_fileOutput << "% Date of the file:" << dateString << "     %" << endl;
    *_fileOutput << "% Other Details:         %" << _projectDetails.getOtherDetails() << "     %" << endl;
    *_fileOutput << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;
}


/**
 * @brief Function that writes the DOM tree within the textStream into the config file
 *
 * @param _configTree
 * @param _fileOutput
 */
void SolverConfigWriter::writeDOMTree(QDomDocument *  _configTree, QTextStream * _fileOutput)
{
    // Get the root element
    QDomElement _rootElement = _configTree->documentElement();

    if (QString::compare(_rootElement.tagName(),XML_ROOT_NODE_NAME) == StrEqual)
        writeSectionList(_rootElement,_fileOutput);

}

/**
 * @brief Function that writes out a "section" list tag element into the Qtextstream
 *
 * @param _sectionList
 * @param _fileOutput
 */
void SolverConfigWriter::writeSectionList(QDomElement _sectionList, QTextStream * _fileOutput)
{
    QDomElement _sectionListElement = _sectionList.firstChild().toElement();
    if (!_sectionListElement.isNull())
        if (QString::compare(_sectionListElement.tagName(),XML_SECTION_LIST_NAME) == StrEqual)
        {
            QDomNodeList _childList = _sectionListElement.childNodes();
            if (!_childList.isEmpty())
                for (int i = 0; i < _childList.count();i++)
                {
                    QDomElement _sectionEl = _childList.at(i).toElement();
                    if ( (!_sectionEl.isNull()) && (QString::compare(_sectionEl.tagName(),XML_SECTION_NODE_NAME)== StrEqual))
                        writeSectionElement(_sectionEl,_fileOutput);
                }
        }

}

/**
 * @brief Function that writes out a "section" tag element into the Qtextstream
 *
 * @param _sectionElement
 * @param _fileOutput
 */
void SolverConfigWriter::writeSectionElement(QDomElement _sectionElement, QTextStream * _fileOutput)
{
   *_fileOutput << "%**********************************************************************%%" << endl;
    QDomNode _childNode = _sectionElement.firstChild();
    while (!_childNode.isNull())
    {
        QDomElement _nodeElement = _childNode.toElement();
        if (!_nodeElement.isNull())
        {
            if (QString::compare(_nodeElement.tagName(),XML_TITLE_SECTION_NAME) == StrEqual)
            {

                *_fileOutput << "%%####################################################################################" << endl;
                *_fileOutput << "%%                    " <<  _nodeElement.text() << endl;
                *_fileOutput << "%%####################################################################################" << endl;
            }
            if (QString::compare(_nodeElement.tagName(),XML_SUBSECTION_LIST_NAME) == StrEqual)
                writeSubsectionList(_nodeElement,_fileOutput);

        }
        _childNode = _childNode.nextSibling();
    }
}

/**
 * @brief Function that writes out a "subsection" list tag element into the Qtextstream
 *
 * @param _subSectionList
 * @param _fileOutput
 */
void SolverConfigWriter::writeSubsectionList(QDomElement _subSectionList, QTextStream * _fileOutput)
{
    if (!_subSectionList.isNull())
    {
        QDomNodeList _childList = _subSectionList.childNodes();
        if (!_childList.isEmpty())
        {
            for (int i = 0; i < _childList.count();i++)
            {
                QDomElement _subsectionEl = _childList.at(i).toElement();
                if ((!_subsectionEl.isNull()) && (QString::compare(_subsectionEl.tagName(),XML_SUBSECTION_NAME) == StrEqual) )
                    writeSubsectionElement(_subsectionEl,_fileOutput);
            }

        }
    }
}

/**
 * @brief Function that writes out a "subsection"  tag element into the Qtextstream
 *
 * @param _subSectionElement
 * @param _fileOutput
 */
void SolverConfigWriter::writeSubsectionElement(QDomElement _subSectionElement, QTextStream * _fileOutput)
{
    if (!_subSectionElement.isNull())
    {
        QDomNodeList _childList = _subSectionElement.childNodes();
        bool _isVirtualMRF = false;

        if (!_childList.isEmpty())
            for (int i = 0;i < _childList.count();i++)
            {                
                QDomElement _subsectionEl = _childList.at(i).toElement();
                if (!_subsectionEl.isNull())
                {
                    if (QString::compare(_subsectionEl.tagName(),XML_SUBSECTION_TITLE_NAME) == StrEqual)
                    {
                        *_fileOutput << "%%--------------------------------------------------------------------------------------" << endl;
                        *_fileOutput << "%%                    " << _subsectionEl.text() << endl;
                        *_fileOutput << "%%--------------------------------------------------------------------------------------" << endl;
                        // If is Virtual MRF section a customized output must be done
                        if (QString::compare(_subsectionEl.text(),VIRTUAL_SUBSECTION_NAME) == StrEqual)                        
                            _isVirtualMRF = true;
                    }
                    if (QString::compare(_subsectionEl.tagName(),XML_VARIABLE_LIST_NAME) == StrEqual)
                        writeVariableList(_subsectionEl,_fileOutput,_isVirtualMRF);
                }
            }

        // In the case ov Virtual MRF -- Add hidden Variables
            if ((_isVirtualMRF) && (!_virtualMRFUserValues.isEmpty()))
            {
                // Write the hidden Variables
                 QString _value;
                _value=VMRF_GRID_MOVEMENT_KEYNAME+"=(YES)";
                _value.simplified();
                _value.replace(" ","");
                *_fileOutput << _value  << endl;
                _value=VMRF_GRID_MOVEMENT_KIND_KEYNAME+"=(ROTATING_FRAME)";
                _value.simplified();
                _value.replace(" ","");
                *_fileOutput << _value  << endl;

            }
    }
}


/**
 * @brief Function that writes out a "variable" list tag element into the Qtextstream
 *
 * @param _variableList
 * @param _fileOutput
 */
void SolverConfigWriter::writeVariableList(QDomElement _variableList, QTextStream * _fileOutput,bool _isVirtualMRF)
{
    if (!_variableList.isNull())
    {
        QDomNodeList _childList = _variableList.childNodes();
        if (!_childList.isEmpty())
        {
            for (int i = 0; i < _childList.count();i++)
            {
                QDomElement _variableEl = _childList.at(i).toElement();
                if (!_variableEl.isNull())
                    if (QString::compare(_variableEl.tagName(),XML_VARIABLE_NAME) == StrEqual)
                        writeVariableElement(_variableEl,_fileOutput,_isVirtualMRF);
            }
        }
    }

}



/**
 * @brief Function that writes out a "variable"  tag element into the Qtextstream
 *
 * @param _varElement
 * @param _fileOutput
 */
void SolverConfigWriter::writeVariableElement(QDomElement _varElement, QTextStream * _fileOutput,bool _isVirtualMRF)
{
    QDomNode _childNode = _varElement.firstChild();
    while (!_childNode.isNull())
    {
        QDomElement _nodeElement = _childNode.toElement();
        if (!_nodeElement.isNull())
        {
            if (QString::compare(_nodeElement.tagName(),XML_VARIABLE_NAME_NAME) == StrEqual)
                *_fileOutput << "%%---**" << _nodeElement.text() << endl;

            if (QString::compare(_nodeElement.tagName(),XML_VARIABLE_VALUE_NAME) == StrEqual)
                writeVariableValue(_nodeElement,_fileOutput,_isVirtualMRF);
        }
        _childNode = _childNode.nextSibling();
    }
}

/**
 * @brief Function that writes out a "value"  tag element into the Qtextstream
 *
 * @param _valueElement
 * @param _fileOutput
 */
void SolverConfigWriter::writeVariableValue(QDomElement _valueElement, QTextStream * _fileOutput,bool _isVirtualMRF)
{
    QString _value;
    if (_valueElement.hasAttribute(XML_VARIABLE_VALUE_NAME_ATT))    {

        if (!_isVirtualMRF)
        {

            QString _keyValue = _valueElement.attribute(XML_VARIABLE_VALUE_NAME_ATT);
            bool _saveValue = true;
            if ((!_optionalVariables.isEmpty()) && (_optionalVariables.contains(_keyValue)))
                _saveValue = false;

            if ((!_userValues.isEmpty()) &&  (_userValues.contains(_keyValue)))
            {
                _value = _valueElement.attribute(XML_VARIABLE_VALUE_NAME_ATT);
                QString _cValue = _userValues.value(_keyValue);
                if (QString::compare(_keyValue,DOMAIN_CLIPPING_VARIABLE_FILENAME) == StrEqual)
                {
                    if (_cValue.isEmpty())
                        _value ="";
                    else
                    {
                        // Print each value in several Domain clip variables in the file
                        QStringList _valueList = _cValue.split(";");
                        for (int i =0; i < _valueList.size();i++)
                        {
                            _value = DOMAIN_CLIPPING_VARIABLE_FILENAME+ "=("+_valueList.at(i)+")";
                            _value.simplified();
                            _value.replace(" ","");
                            *_fileOutput << _value  << endl;
                            _value="";
                        }
                    }

                }else
                {
                    if ( ((_value.contains(MARKER_PREFIX)) || (_value.contains(BC_PREFIX)))
                         && (!_cValue.contains("(")) && (!_cValue.contains(")")))
                        _value = _value  +"= ("+  _cValue+ ")";
                    else
                        _value = _value +"="+ _cValue;
                }
            }else
                if (_saveValue)
                    _value =  _valueElement.attribute(XML_VARIABLE_VALUE_NAME_ATT) +"="+_valueElement.text();

            _value.simplified();
            _value.replace(" ","");
            *_fileOutput << _value  << endl;
        }else
            printVirtualMRFValues(_valueElement,_fileOutput);
    }
}
/************************************** Setter ************************************************/


void SolverConfigWriter::printVirtualMRFValues(QDomElement _valueElement, QTextStream * _fileOutput)
{
    QString _value;
    bool _printValue=true;

    if (_virtualMRFUserValues.isEmpty())
        return;
    QString _keyValue = _valueElement.attribute(XML_VARIABLE_VALUE_NAME_ATT);
    _value=_keyValue+"=(";
    for (int i = 0; i < _virtualMRFUserValues.size();i++)
    {
        _value=_keyValue+"=(";
        _printValue=true;
        VirtualMrfVO _virtualMRF_obj = _virtualMRFUserValues.at(i);
        QString _name =_virtualMRF_obj.getZoneName();

        // Create VMRF_AXIS
        if (QString::compare(_keyValue,VMRF_AXIS_KEYNAME) == StrEqual)
            _value = _value + _name + COMMA_SEPARATOR + _virtualMRF_obj.getOmega() + COMMA_SEPARATOR +
                    _virtualMRF_obj.getAxisCenter().join(COMMA_SEPARATOR) + COMMA_SEPARATOR +
                    _virtualMRF_obj.getAxisNormal().join(COMMA_SEPARATOR) +")";


        // Create INNER CURVE
        if (QString::compare(_keyValue, VMRF_INNER_CURVE_KEYNAME) == StrEqual)
            if ( (!_virtualMRF_obj.getInnerCurvePoints().isEmpty()) && (_virtualMRF_obj.getInnerCurvePoints().count() >0))
            {
                int _noPoints = 0;
              /*  if ( _virtualMRF_obj.isCoordinatesIn3D())
                    _noPoints = _virtualMRF_obj.getInnerCurvePoints().count()/3;
                else */
                    _noPoints = _virtualMRF_obj.getInnerCurvePoints().count();

                _value = _value + _name + COMMA_SEPARATOR + QString::number(_noPoints) + COMMA_SEPARATOR +
                        _virtualMRF_obj.getInnerCurvePoints().join(COMMA_SEPARATOR)+")";

            }
            else
                _value=_value +"NONE)";

        // Create Outer Curve
        if (QString::compare(_keyValue,VMRF_OUTER_CURVE_KEYNAME) == StrEqual)
            if ( (!_virtualMRF_obj.getOuterCurvePoints().isEmpty()) && (_virtualMRF_obj.getOuterCurvePoints().count() >0))
            {
                int _noPoints = 0;
               /* if ( _virtualMRF_obj.isCoordinatesIn3D())
                    _noPoints = _virtualMRF_obj.getOuterCurvePoints().count()/3;
                else*/
                    _noPoints = _virtualMRF_obj.getOuterCurvePoints().count();

                _value = _value + _name + COMMA_SEPARATOR + QString::number(_noPoints) + COMMA_SEPARATOR +
                        _virtualMRF_obj.getOuterCurvePoints().join(COMMA_SEPARATOR)+")";

            }
            else
                _value=_value +"NONE)";
        if ((QString::compare(_keyValue,VMRF_GRID_MOVEMENT_KEYNAME) == StrEqual) || (QString::compare(_keyValue,VMRF_GRID_MOVEMENT_KIND_KEYNAME) == StrEqual))
            _printValue = false;

        if (_printValue)
        {
            _value.simplified();
            _value.replace(" ","");
            *_fileOutput << _value  << endl;
        }
    }

}


/**
 * @brief Function that set up the user value map
 *
 * @param QHash<QString
 * @param _values
 */
void SolverConfigWriter::setUserValues(QHash<QString,QString> _values)
{
    _userValues = _values;
}

void SolverConfigWriter::setVirtualMRFUserValues(QList<VirtualMrfVO> _values)
{
    _virtualMRFUserValues = _values;
}
void SolverConfigWriter::setOptionalVariableList(QStringList _list)
{
    _optionalVariables = _list;
}
