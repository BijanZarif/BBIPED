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
#include "solverconfigreader.h"

#include <stdio.h>
#include <iostream>

#include "Data/solversectiondao.h"
#include "Defines/General/GeneralDefines.h"
#include "Defines/Tool/Solver/SolverDefines.h"

using namespace std;

/**
 * @brief Constructor
 *
 */
SolverConfigReader::SolverConfigReader()
{
}

/************************************** Create ************************************************/

/**
 * @brief Function that creates the SolverSection list from a Section element of the XML template
 *
 * @param _element
 * @return QList<SolverSectionVO>
 */
QList<SolverSectionVO> SolverConfigReader::createSectionNode(QDomElement _element)
{
    QList<SolverSectionVO> _resultList;
    QString _sectionTitle;
    QDomNodeList _titleList = _element.elementsByTagName(XML_TITLE_SECTION_NAME);
    if (!_titleList.isEmpty())
    {
        _sectionTitle = _titleList.at(0).toElement().text();
    }
    QDomNodeList _sectionList = _element.elementsByTagName(XML_SUBSECTION_NAME);
    if (!_sectionList.isEmpty())
    {
        for (int i = 0; i <_sectionList.count();i++)
        {
            QDomElement _child = _sectionList.at(i).toElement();
            SolverSectionVO _createdObjVO;
            _createdObjVO.setSectionName(_sectionTitle);
            QDomNodeList _titleSubsecList = _child.elementsByTagName(XML_SUBSECTION_TITLE_NAME);
            if (!_titleSubsecList.isEmpty())
                 _createdObjVO.setSubsectionName(_titleSubsecList.at(0).toElement().text());
            QList<SolverVariableVO> _variableList = createVariableSection(_child);
            _createdObjVO.setSubsectionVarList(_variableList);
            _resultList.append(_createdObjVO);
        }

    }
    return _resultList;
}


/**
 * @brief Function that creates the SolverSection list from a Variable element of the XML template
 *
 * @param _element
 * @return QList<SolverVariableVO>
 */
QList<SolverVariableVO> SolverConfigReader::createVariableSection(QDomElement _element)
{
    QList<SolverVariableVO> _resultList;
    QDomNodeList _variableList = _element.elementsByTagName(XML_VARIABLE_NAME);
    if (!_variableList.isEmpty())
    {
        for (int i = 0;i < _variableList.count();i++)
        {
            SolverVariableVO _createdObj;
            QDomElement _child = _variableList.at(i).toElement();
            // Check if it is repeatable
            if (_child.hasAttribute(XML_VARIABLE_ISREPEATABLE_ATT))
                if (QString::compare(_child.attribute(XML_VARIABLE_ISREPEATABLE_ATT),YES_VALUE) == StrEqual)
                        _createdObj.setRepeatable(true);
            // Check if it is optional
            if (_child.hasAttribute(XML_VARIABLE_ISOPTIONAL_ATT))
                if (QString::compare(_child.attribute(XML_VARIABLE_ISOPTIONAL_ATT),YES_VALUE) == StrEqual)
                        _createdObj.setOptional(true);
            // Check if it is selectable by the users
             if (_child.hasAttribute(XML_VARIABLE_SELECTABLE_ATT))
                 if (QString::compare(_child.attribute(XML_VARIABLE_SELECTABLE_ATT),"YES") == StrEqual)
                         _createdObj.setVariableSelectable(true);
                 else
                     _createdObj.setVariableSelectable(false);

            QDomNodeList _nameList = _child.elementsByTagName(XML_VARIABLE_NAME_NAME);

            // Variable Name
            if (!_nameList.isEmpty())
                 _createdObj.setVariableName(_nameList.at(0).toElement().text());
            QDomNodeList _helpList = _child.elementsByTagName(XML_VARIABLE_HELP_NAME);
            // Help Value
            if (!_helpList.isEmpty())
                 _createdObj.setHelpContents(createHelpElement(_helpList.at(0).toElement()));

            QDomNodeList _valueList = _child.elementsByTagName(XML_VARIABLE_VALUE_NAME);
            if (!_valueList.isEmpty())
            {
                QDomElement _valueEl = _valueList.at(0).toElement();
                // Variable File Name
                if (_valueEl.hasAttribute(XML_VARIABLE_VALUE_NAME_ATT))
                    _createdObj.setVariableFileName(_valueEl.attribute(XML_VARIABLE_VALUE_NAME_ATT));
                // Default Value
                if (_valueEl.hasAttribute(XML_VARIABLE_VALUE_DEFAULT_VALUE_ATT))
                {
                    _createdObj.setDefaultValue(_valueEl.attribute(XML_VARIABLE_VALUE_DEFAULT_VALUE_ATT));
                    _createdObj.setCurrentValue(_valueEl.text());
                }
                // Unit Value
                if (_valueEl.hasAttribute(XML_VARIABLE_VALUE_UNIT_ATT))
                {
                    QString _unitValue = _valueEl.attribute(XML_VARIABLE_VALUE_UNIT_ATT);
                    if (QString::compare(_unitValue,"NONE") == StrEqual) // If there is no unit applicable, we don't shown anything
                        _unitValue = "";
                    _createdObj.setUnitValue(_unitValue);
                }
                // In case it has Other Variable Names inside
                if (_valueEl.hasAttribute(XML_VARIABLE_VALUE_SUBVAR_NAME_ATT))
                {
                    QString _subVariableNameList = _valueEl.attribute(XML_VARIABLE_VALUE_SUBVAR_NAME_ATT);
                    _createdObj.setSubVariableNames(_subVariableNameList.split(","));
                }
                // Check if it is Visible
                if (_valueEl.hasAttribute(XML_VARIABLE_ISVISIBLEINMENU_ATT))
                    if (QString::compare(_valueEl.attribute(XML_VARIABLE_ISVISIBLEINMENU_ATT),"NO") == StrEqual)
                            _createdObj.setVisibleInMenu(false);
                // Check if it contains associated variables
                if (_valueEl.hasAttribute(XML_VARIABLE_ASSOCIATED_VAR_ATT))
                {
                    QString _associatedVarList = _valueEl.attribute(XML_VARIABLE_ASSOCIATED_VAR_ATT);
                    _createdObj.setAssociatedVariables(_associatedVarList.split(","));
                }
            }

            // Combo Values
            QDomNodeList _comboValuesParent = _child.elementsByTagName(XML_VARIABLE_COMBOVALUES_LIST_NAME);
            if (! _comboValuesParent.isEmpty())
            {
                QDomElement _comboAttElement = _comboValuesParent.at(0).toElement();
                QString _uniqueValue;
                if (_comboAttElement.hasAttribute(XML_VARIABLE_COMBOVALUE_UNIQUESEL_ATT))
                    _uniqueValue = _comboAttElement.attribute(XML_VARIABLE_COMBOVALUE_UNIQUESEL_ATT);

                // If the combo values has selected no unique option, this implies that it allows multiple
                // selection (similar to checkbox
                if ((!_uniqueValue.isEmpty()) && (QString::compare(_uniqueValue,"YES") != StrEqual))
                    _createdObj.setUniqueSelection(false);
            }
            createComboValues(_child, &_createdObj);

            _resultList.append(_createdObj);

        }
    }
    return _resultList;
}

/**
 * @brief This function create the SolverVAriable value object with the content for the
 * combo values to be shown in the view
 *
 * @param _element
 * @param _solverVO
 */
void SolverConfigReader::createComboValues(QDomElement _element,SolverVariableVO * _solverVO)
{
    QDomNodeList _comboValList = _element.elementsByTagName(XML_VARIABLE_COMBOVALUE_ELEMENT_NAME);

    if (!_comboValList.isEmpty())
    {
        QStringList _listVal;
        QStringList _listNames;
        QList<bool> _selectableValList;

        for (int i = 0;i < _comboValList.count();i++)
        {
            QDomElement _valueEl = _comboValList.at(i).toElement();
            _listVal.append(_valueEl.text() );

            if (_valueEl.hasAttribute(XML_VARIABLE_COMBOVALUE_NAME_ATT))
                _listNames.append(_valueEl.attribute(XML_VARIABLE_COMBOVALUE_NAME_ATT));
            if (_valueEl.hasAttribute(XML_VARIABLE_COMBOVALUE_SELECTABLE_ATT))
            {
                QString _selVal= _valueEl.attribute(XML_VARIABLE_COMBOVALUE_SELECTABLE_ATT);
                if (QString::compare(_valueEl.attribute(XML_VARIABLE_COMBOVALUE_SELECTABLE_ATT),"YES") == StrEqual)
                    _selectableValList.append(true);
                else
                    _selectableValList.append(false);
            }

        }
        if (!_listNames.isEmpty())
            _solverVO->setComboValueNamesList(_listNames);
        if (!_listVal.isEmpty())
            _solverVO->setComboValuesList(_listVal);
        if (!_selectableValList.isEmpty())
            _solverVO->setSelectableComboValues(_selectableValList);
    }
}



/**
 * @brief This fucntion allows to create the help element from the xml file
 *
 * @param _element
 * @return SolverVarHelpVO
 */
SolverVarHelpVO * SolverConfigReader::createHelpElement(QDomElement _element)
{
    SolverVarHelpVO * _helpObj = new SolverVarHelpVO();

    QDomNodeList _definition = _element.elementsByTagName("definition");
    QDomNodeList _values = _element.elementsByTagName("values");
    QDomNodeList _details =_element.elementsByTagName("otherdetails");
    if( (!_definition.isEmpty()) && (_definition.size() >= 1))
            _helpObj->setDefinitionText(_definition.at(0).toElement().text());
    if( (!_values.isEmpty()) && (_values.size() >= 1))
        _helpObj->setValuesText(_values.at(0).toElement().text());
    if( (!_details.isEmpty()) && (_details.size() >= 1))
        _helpObj->setDetailsText(_details.at(0).toElement().text());

    return _helpObj;
}


/************************************** Getter/Setter/Is ************************************************/
/**
 * @brief Function that returns the list of SolverSection elements from the XML DOM template
 *
 * @param _tree
 * @return QList<SolverSectionVO>
 */
QList<SolverSectionVO> SolverConfigReader::getAllSectionVariableMenus(QDomDocument * _tree)
{
    QList<SolverSectionVO> _list;
    QDomElement _root = _tree->documentElement();
    QDomNodeList _sectionNodeList = _root.elementsByTagName(XML_SECTION_NODE_NAME);
    if (!_sectionNodeList.isEmpty())
    {
        for (int i = 0; i < _sectionNodeList.count();i++)
        {
            QList<SolverSectionVO> _resultList = createSectionNode(_sectionNodeList.at(i).toElement());
            _list.append(_resultList);
        }
    }

    return _list;
}


/**
 * @brief Function that get the list of Combo values from the XML Tree
 *
 * @param _element
 * @return QStringList
 */
QStringList SolverConfigReader::getComboValues(QDomElement _element)
{
    QStringList _resultList;
    QDomNodeList _comboList = _element.elementsByTagName(XML_VARIABLE_COMBOVALUE_ELEMENT_NAME);
    if (_comboList.isEmpty())
        return _resultList;
    for (int i = 0;i < _comboList.count();i++)
        _resultList.append(_comboList.at(i).toElement().text() );

    return _resultList;
}

/************************************** Update ************************************************/

/**
 * @brief Function tha update the value elements in the Dom TREE
 *
 * @param _valueName
 * @param _newValue
 * @param _domTree
 */
void SolverConfigReader::updateValueElement(QString _valueName,
                                            QString _newValue, QDomDocument * _domTree)
{
    changeValueElement(_valueName,_newValue,_domTree);
}


/**
 * @brief Function taht updates all the values of the DOM tree
 *
 * @param QHash<QString
 * @param _newMapValues
 * @param _domTree
 */
void SolverConfigReader::updateAllDomTree(QHash<QString,QString> _newMapValues,
                                          QDomDocument * _domTree)
{
    QHash<QString, QString>::iterator _iter;

    if( (!_newMapValues.empty()) && (!_domTree->isNull()))
    {
        for (_iter = _newMapValues.begin(); _iter != _newMapValues.end();++_iter)
        {
            // Get the root element
            QString _varName = _iter.key();
            QString _newValue = _iter.value();
            changeValueElement(_varName, _newValue,_domTree);
        }
    }
}
/************************************** Change/Find ************************************************/

/**
 * @brief Function that changels the values of the elements in the DOM tree
 *
 * @param _varName
 * @param _newValue
 * @param _domTree
 */
void SolverConfigReader::changeValueElement(QString _varName, QString _newValue, QDomDocument * _domTree)
{
    QDomElement  _rootElement = _domTree->documentElement();
    QDomNodeList _sectionNodeList = _rootElement.childNodes();
    childIds _foundVarIds = findSectionChildId(_varName,_sectionNodeList);
    _domTree->documentElement().childNodes().at(_foundVarIds._sectionId).childNodes().
            at(_foundVarIds._variableId).childNodes().at(2).firstChild().setNodeValue(_newValue);

}


/**
 * @brief Function that finds a Section node in the XML Tree
 *
 * @param _varName
 * @param _sectionNodeList
 * @return SolverConfigReader::childIds
 */
SolverConfigReader::childIds SolverConfigReader::findSectionChildId(QString _varName, QDomNodeList  _sectionNodeList)
{
    int _varIndex = 0;
    childIds _foundId;
    _foundId._sectionId = -1;
    _foundId._variableId = -1;

    for (int i = 0 ; i < _sectionNodeList.count(); i++)
    {
        QDomElement _sectionEl = _sectionNodeList.at(i).toElement();
        cout << _sectionEl.tagName().toStdString() << endl;
        QDomNode _childNode = _sectionEl.firstChild();
        while  (!_childNode.isNull())
        {
            QDomElement _nodeElement = _childNode.toElement();
            if (!_nodeElement.isNull())
            {
                if (QString::compare(_nodeElement.tagName(),XML_VARIABLE_NAME) == StrEqual)
                {
                      QDomNodeList _valueNode = _nodeElement.elementsByTagName(XML_VARIABLE_VALUE_NAME);
                      if ((!_valueNode.isEmpty()) && (_valueNode.size() == 1))
                      {
                          QString _nameAtt = _valueNode.at(0).toElement().attribute(XML_VARIABLE_VALUE_NAME_ATT);
                          if (QString::compare(_nameAtt,_varName) == StrEqual)
                          {
                                _foundId._sectionId = i;
                                _foundId._variableId = _varIndex;
                                return _foundId;
                          }
                      }
                }
            }
            _childNode = _childNode.nextSibling();
            _varIndex ++;

        }

    }

    return _foundId;
}





