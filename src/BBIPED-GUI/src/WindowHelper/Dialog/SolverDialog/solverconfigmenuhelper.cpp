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
#include "solverconfigmenuhelper.h"

#include "Data/solversectiondao.h"

// DEFINES
#include "Defines/General/GeneralDefines.h"
#include "Defines/Tool/Solver/SolverDefines.h"
#include "WindowHelper/Dialog/SolverDialog/solvermarkermenuhelper.h"
// IO WRITING
#include <stdio.h>
#include <iostream>

#include <QHeaderView>

// VALIDATION
//#include <QValidator>
#include <QDoubleValidator>
//#include <QIntValidator>
#include "Utility/qtableutil.h"

#include <QListWidget>
#include <QListWidgetItem>

using namespace std;

/**
 * @brief Constructor
 *
 */
SolverConfigMenuHelper::SolverConfigMenuHelper()
{
}
/******************************* Update *********************************************************/

/**
 * @brief Function that updates the table with the user values
 *
 * @param _solverSecvo
 * @param _table
 * @param QHash<QString
 * @param _newMapValues
 */

void SolverConfigMenuHelper::updateTableWithUserValues(SolverSectionVO _solverSecvo,
                                                       QTableWidget* _table,
                                                       QHash<QString,QString> _newMapValues)
{

   QList<SolverVariableVO> _varList = _solverSecvo.getSubsectionVarList();
   if( (!_varList.isEmpty()) && (!_newMapValues.isEmpty()))
   {
       for (int i = 0; i< _varList.count() ; i++)
       {
           SolverVariableVO _solverVO = _varList.at(i);
           QString _varNameKey = _solverVO.getVariableFileName();
           QString _defaultValue = _solverVO.getDefaultValue();

           if (_newMapValues.contains(_varNameKey))
                   if  ((!_varNameKey.contains(MARKER_PREFIX)) && (!_varNameKey.contains(BC_PREFIX)))
                       if (QString::compare(_newMapValues.value(_varNameKey),_defaultValue) != StrEqual)
                            updateVariableValueinTable(_solverVO,_table,_newMapValues.value(_varNameKey));

       }
   }

}


/**
 * @brief Function that updates the value in the table
 *
 * @param _solverVO
 * @param _table
 * @param _newValue
 */
void SolverConfigMenuHelper::updateVariableValueinTable( SolverVariableVO _solverVO,
                                                         QTableWidget* _table,
                                                         QString _newValue)
{
    int _tableRow = getRowofVariable(_solverVO.getVariableName(),_table);
    if (_tableRow != -1)
    {
        QLineEdit * _lineEdit = qobject_cast<QLineEdit * > ( _table->cellWidget(_tableRow,ColumnSelVal));
        if (_lineEdit)
            _lineEdit->setText(_newValue);
        else
        {
             QComboBox * _combo = qobject_cast<QComboBox * > ( _table->cellWidget(_tableRow,ColumnSelVal));
             if (_combo)
             {
                 QString _valueToShow = getAssociatedFileNameValue(_solverVO,_newValue);
                 int _index = _combo->findText(_valueToShow);
                 if (_index != -1)
                     _combo->setCurrentIndex(_index);
                 else
                 {
                     _combo->insertItem(0,_newValue);
                     _combo->setCurrentIndex(0);
                 }
             }
             else
             {
                 QListWidget * _checkList = qobject_cast<QListWidget * > ( _table->cellWidget(_tableRow,ColumnSelVal));
                 if (_checkList)
                 {
                    QStringList _listValues = _newValue.split(",");
                    for (int i = 0; i < _listValues.size();i++)
                    {
                        QString _valueToUptade = _listValues.at(i);
                        QString _valueToShow = getAssociatedFileNameValue(_solverVO,_valueToUptade);
                        QList<QListWidgetItem * > _itemList =  _checkList->findItems(_valueToShow,Qt::MatchExactly);
                       if (!_itemList.empty())
                       {
                           QList<QListWidgetItem * > _itemFound =  _checkList->findItems(_valueToShow,Qt::MatchExactly);
                           if (!_itemFound.isEmpty())
                               _itemFound.at(0)->setCheckState(Qt::Checked);
                       }
                     }
                 }else //This is the case for the specific windows (TO BE CHECKED)
                 {
                     QTableWidgetItem * _item = _table->item(_tableRow,ColumnSelVal);
                     if (_item)
                         _item->setText(_newValue);
                 }
             }
        }
        // Set check status
        _table->item(_tableRow,ColumnCheck)->setCheckState(Qt::Checked);
    }
}


/******************************* Getter/Setter *********************************************************/
/**
 * @brief Function that gets the associated variable name in the file
 *
 * @param _solverVO
 * @param _value
 * @return QString
 */
QString SolverConfigMenuHelper::getAssociatedFileNameValue(SolverVariableVO _solverVO, QString _value)
{
    QString _result;

    if (_solverVO.getComboValuesList().contains(_value))
    {
        QStringList _valList = _solverVO.getComboValueNamesList();
        int _pos = _solverVO.getComboValuesList().indexOf(_value);
        _result = _valList.at(_pos);
    }

   return _result;
}

/**
 * @brief Function that gets the number of the table's row of the correspondent variableName
 *
 * @param _variableName
 * @param _table
 * @return int
 */
int SolverConfigMenuHelper::getRowofVariable(QString _variableName, QTableWidget * _table)
{
    for (int i = 0; i < _table->rowCount(); i++)
    {
        QString _tableVarName = _table->item(i,ColumnVarName)->text();
        if (( !_tableVarName.isEmpty() &&
                QString::compare(_tableVarName,_variableName) == StrEqual) )
        {
            return i;
        }
    }
    return -1;
}

/**
 * @brief Function that gets the correspondent help text of a variable Name
 *
 * @param _variableName
 * @param _list
 * @return QString
 */
QString SolverConfigMenuHelper::getAssociatedHelpText(QString _variableName,
                                                      QList<SolverVariableVO> _list, bool _withFormat)
{
    if (!_list.isEmpty())
    {
        for (int i = 0; i <_list.count() ; i++)
        {
            SolverVariableVO _child = _list.at(i);
            if (QString::compare(_child.getVariableName(),_variableName) == StrEqual)
                return _child.getHelpContents(_withFormat);
        }
    }
    return QString();
}


/**
 * @brief Function that creates those non visible section elements
 *
 * @param _sectionName
 * @param _confgiDomTree
 * @param _menuToFill
 */
void SolverConfigMenuHelper::getSectionMenuElements(QString _sectionName, QDomDocument * _confgiDomTree, QDomDocument * _menuToFill)
{
    QDomElement _rootElement = _confgiDomTree->documentElement();
    if (!_rootElement.isNull())
    {
        QDomNodeList _sectionList = _rootElement.elementsByTagName(XML_SECTION_NODE_NAME);
        QDomElement _foundSection = findSectionElementByName(_sectionName,_sectionList);
        if (!_foundSection.isNull())
            createNotVisibleSectionElement(_foundSection, _menuToFill);
    }
}

/**
 * @brief function that gets section title from an XML DOM element
 *
 * @param _element
 * @return QString
 */
QString SolverConfigMenuHelper::getSectionTitle(QDomElement _element)
{
    QDomNodeList _children = _element.childNodes();
    if (!_children.isEmpty())
    {
        for (int i = 0;i < _children.count();i++)
        {
            QDomElement _child = _children.at(i).toElement();
            if (QString::compare(_child.tagName(),XML_TITLE_SECTION_NAME) == StrEqual)
                return _child.text();
        }
    }
    return QString();
}

/**
 * @brief Function that gets the subsection title
 *
 * @param _element
 * @return QStringList
 */
QStringList SolverConfigMenuHelper::getSubsectionTitle(QDomElement _element)
{
    QStringList _resultList;
    QDomNodeList _children = _element.elementsByTagName(XML_SUBSECTION_NAME);
    if (!_children.isEmpty())
    {
        for (int i = 0;i < _children.count();i++)
        {
            QDomElement _child = _children.at(i).toElement();
            QDomNodeList _childNodes = _child.childNodes();
            if (!_childNodes.isEmpty())
            {
                for (int j = 0; j < _childNodes.count();j++)
                {
                    QDomElement _childElement  = _childNodes.at(j).toElement();
                    if (QString::compare(_childElement.tagName(),XML_TITLE_SECTION_NAME) == StrEqual)
                        _resultList.append(_childElement.text());
                }
            }

        }
    }
    return _resultList;
}
/******************************* Remove *********************************************************/
/**
 * @brief Function that removes a Section from the Tree
 *
 * @param _sectionName
 * @param _tree
 */
void SolverConfigMenuHelper::removeSectionNode(QString _sectionName, QDomDocument * _tree)
{
    // Note: In the view, we only show root elements, so we need to look in the siblings
    if( !_tree->isNull())
    {

        QDomElement  _elementToRemove = _tree->documentElement();
        while (!_elementToRemove.isNull())
        {
            if (QString::compare(_sectionName,_elementToRemove.tagName()) == StrEqual)
            {
                _tree->removeChild(_elementToRemove);
                return;
            }
            _elementToRemove = _elementToRemove.nextSibling().toElement();
        }

    }

}


/**
 * @brief  Function that removes an elemente name by its name
 *
 * @param _name
 * @param _tree
 */
void SolverConfigMenuHelper::removeMenuElementByName(QString _name, QDomDocument * _tree )
{
    if( !_tree->isNull())
    {

        QDomElement  _elementToRemove = _tree->documentElement();
        while (!_elementToRemove.isNull())
        {
            if (QString::compare(_name,_elementToRemove.tagName()) == StrEqual)
            {
                _tree->removeChild(_elementToRemove);
                return;
            }
            _elementToRemove = _elementToRemove.nextSibling().toElement();
        }
    }
}


/**
 * @brief Removes teh correspondent subsection element
 *
 * @param _parentName
 * @param _childName
 * @param _tree
 */
void SolverConfigMenuHelper::removeSubsectionElementByName(QString _parentName, QString _childName,
                                                           QDomDocument * _tree)
{
    if( !_tree->isNull())
    {
        QDomElement  _parentElement = _tree->documentElement();
        while (!_parentElement.isNull())
        {
            if (QString::compare(_parentName,_parentElement.tagName()) == StrEqual)
            {
                QDomNodeList _childNodeList = _parentElement.childNodes();
                if (!_childNodeList.isEmpty())
                {
                    for (int i = 0; i < _childNodeList.count(); i++)
                    {
                        QDomElement _element = _childNodeList.at(i).toElement();
                        if (QString::compare(_childName,_element.tagName()) == StrEqual)
                            _parentElement.removeChild(_element);
                    }
                }
                return;
            }
            _parentElement = _parentElement.nextSibling().toElement();
        }
    }
}

/******************************* Fill/Add *********************************************************/

/**
 * @brief Function that fills the menu table
 *
 * @param _parentName
 * @param _childName
 * @param _table
 * @param _list
 */
void SolverConfigMenuHelper::fillMenuTable(QString _parentName, QString _childName,
                                           QTableWidget * _table,QList<SolverVariableVO> _list)
{
    if ( (_parentName.isEmpty()) || (QString::compare(_parentName,"") == StrEqual) )
        return;

    for (int i= 0; i < _list.count();i++)
    {
        SolverVariableVO _vo = _list.at(i);
        /* This is to check if the variable must be shown according to
         * the existing rules applied to them
         * Added by calonso: 09/09/2013
        */
        if (_vo.isVisible())
            addVariableToTable(_vo,_table,i); // Pb. Tengo que respetar el numero de filas
        else
        {
            SolverVariableVO _emptyVO; // TODO--> Hacer un customized method para dejar la fila pero no selectable y grayed out
            addVariableToTable(_emptyVO,_table,i);
        }

     }

}

/**
 * @brief Function that adds the variable elements to each row in the table
 *
 * @param _variable
 * @param _table
 * @param _row
 */
void SolverConfigMenuHelper::addVariableToTable(SolverVariableVO _variable, QTableWidget * _table, int _row )
{
    QtableUtil _tableHelper;
    bool _isSelectable = _variable.isVariableSelectable();
    _table->insertRow(_row);
   _table->setItem(_row,ColumnVarName,_tableHelper.addVariable(_variable.getVariableName(),_isSelectable));

    if (!_variable.getComboValueNamesList().isEmpty())
    {
        if (_variable.isUniqueSelection())
        {
            _table->setCellWidget(_row,ColumnSelVal,_tableHelper.addCombo(_variable.getComboValueNamesList(),
                                                                          _variable.getSelectableComboValues(),_isSelectable));
            _table->setItem(_row,ColumnSelVal,_tableHelper.addEditText2("") );
        }else
        {
            _table->setCellWidget(_row,ColumnSelVal,_tableHelper.addCheckBox(_variable.getComboValueNamesList(),
                                                                             _variable.getDefaultValue(),_isSelectable));
             _table->setItem(_row,ColumnSelVal,_tableHelper.addEditText2("") );
             ((QHeaderView*) _table->horizontalHeader())->setResizeMode(ColumnSelVal, QHeaderView::Stretch);
             ((QHeaderView*) _table->verticalHeader())->setResizeMode(_row, QHeaderView::Stretch);
        }

    }else
    {
            if (_variable.getSubVariableNames().isEmpty())
            {
                _table->setCellWidget(_row,ColumnSelVal,_tableHelper.addEditText(_variable.getCurrentValue(),_isSelectable));
                _table->setItem(_row,ColumnSelVal,_tableHelper.addEditText2("") );
            }
            else
                _table->setItem(_row,ColumnSelVal,_tableHelper.addNonEditableText(_variable.getCurrentValue(),_isSelectable));
    }
    _table->setItem(_row, ColumnHelp, _tableHelper.addIcon(_isSelectable));
    _table->setItem(_row, ColumnUnit, _tableHelper.addNonEditableText(_variable.getUnitValue(),_isSelectable));
    _table->setItem(_row,ColumnCheck,_tableHelper.addChangedColumn(_isSelectable));

}

/**
 * @brief Function that add elements to the menu
 *
 * @param _basicView
 * @param _configDomTree
 * @param _menuToFill
 */
void SolverConfigMenuHelper::addMenuElements(bool _basicView, QDomDocument * _configDomTree, QDomDocument * _menuToFill)
{
    QDomElement _rootElement = _configDomTree->documentElement();
    if (!_rootElement.isNull())
    {
        QDomNode _sectionsNode = _rootElement.firstChild();
        QDomNodeList _sectionList = _sectionsNode.childNodes();
        if ( !_sectionList.isEmpty())
        {
            for (int i= 0;i<_sectionList.count();i++)
            {
                QDomElement _sectionEl = _sectionList.at(i).toElement();
                if (QString::compare(_sectionEl.tagName(),XML_SECTION_NODE_NAME ) == StrEqual)
                {
                    if (_basicView)
                        createSectionElement(_sectionEl,_basicView,_menuToFill);
                    else
                        createSectionElement(_sectionEl,_menuToFill);
                }
            }
        }

    }
}
/*******************************  Create *********************************************************/

/**
 * @brief Function that creates not visible section elements
 *
 * @param _element
 * @param _tree
 */
void SolverConfigMenuHelper::createNotVisibleSectionElement(QDomElement _element, QDomDocument * _tree)
{
    QDomElement _rootElement = _tree->documentElement();
    if (!_tree->isNull())
    {
        QDomNode _sectionEl = _tree->createElement(getSectionTitle(_element));
        _tree->appendChild(_sectionEl);

        QStringList _subSectionTitleList = getSubsectionTitle(_element);
        if (!_subSectionTitleList.isEmpty())
        {
            for (int i = 0;i < _subSectionTitleList.count();i++)
            {
                 QDomElement _subsectionEl = _tree->createElement(_subSectionTitleList.at(i));
                _sectionEl.appendChild(_subsectionEl);

            }
        }

    }

}



/**
 * @brief Function that creates section elements
 *
 * @param _sectionElement
 * @param _basicView
 * @param _menuList
 */
void SolverConfigMenuHelper::createSectionElement(QDomElement _sectionElement,
                                                  bool _basicView,
                                                  QDomDocument * _menuList)
{
    QDomNodeList _childNodes = _sectionElement.childNodes();
    bool _needToCreateChildren = false;
    QDomElement _createdMenuElement;

    if (!_childNodes.isEmpty())
    {

        for (int i = 0; i < _childNodes.count() ; i++)
        {
            QDomNode _nodeEl = _childNodes.at(i);
            if (!_nodeEl.isNull())
            {
                QDomElement _element = _nodeEl.toElement();
                if (QString::compare(_element.tagName(),XML_TITLE_SECTION_NAME) == StrEqual)
                {
                    if( (_basicView) && (_element.hasAttributes()))
                    {
                        // We need to check which elements shall appear in the basic view
                        if ( (_element.hasAttribute(XML_SECTION_TITLE_IN_BASIC_VIEW_ATT))
                             && (QString::compare(_element.attribute(XML_SECTION_TITLE_IN_BASIC_VIEW_ATT),YES_VALUE)
                                 == StrEqual) )
                       {
                            _createdMenuElement = _menuList->createElement(_element.text());
                            _menuList->appendChild(_createdMenuElement);

                           // Check if the children shall appear to create them
                           if  ( (_element.hasAttribute(XML_SECTION_TITLE_VISIBLE_CHILDREN_ATT))
                               && (QString::compare(_element.attribute(XML_SECTION_TITLE_VISIBLE_CHILDREN_ATT),YES_VALUE)
                                   == StrEqual) )
                                _needToCreateChildren = true;

                        }
                    }
                }
                if (  (QString::compare(_element.tagName(),XML_SUBSECTION_LIST_NAME) == StrEqual)
                      && (_needToCreateChildren) )
                {
                        createSubSectionElements(_element,&_createdMenuElement,true,_menuList);
                        _needToCreateChildren = false;
                }
            }
          }
        }
    }


/** Fucntion that creates a Section Element
 * @brief
 *
 * @param _sectionElement
 * @param _menuList
 */
void SolverConfigMenuHelper::createSectionElement(QDomElement _sectionElement,
                                                  QDomDocument * _menuList)
{
    QDomNodeList _childNodes = _sectionElement.childNodes();
    bool _needToCreateChildren = false;
    QDomElement _createdMenuElement;

    if (!_childNodes.isEmpty())
    {
        for (int i = 0; i < _childNodes.count() ; i++)
        {
            QDomNode _nodeEl = _childNodes.at(i);
            if (!_nodeEl.isNull())
            {
                QDomElement _element = _nodeEl.toElement();
                if (QString::compare(_element.tagName(),XML_TITLE_SECTION_NAME) == StrEqual)
                {
                    if (_element.hasAttributes())
                    {
                        // We need to check which elements shall appear in the basic view

                            _createdMenuElement = _menuList->createElement(_element.text());
                            _menuList->appendChild(_createdMenuElement);

                           // Check if the children shall appear to create them
                           if  ( (_element.hasAttribute(XML_SECTION_TITLE_VISIBLE_CHILDREN_ATT))
                               && (QString::compare(_element.attribute(XML_SECTION_TITLE_VISIBLE_CHILDREN_ATT),YES_VALUE)
                                   == StrEqual) )
                                _needToCreateChildren = true;
                    }
                }
                if (  (QString::compare(_element.tagName(),XML_SUBSECTION_LIST_NAME) == StrEqual)
                      && (_needToCreateChildren) )
                {
                        createSubSectionElements(_element,&_createdMenuElement,false,_menuList);
                        _needToCreateChildren = false;

                }
            }
          }
        }
    }



/**
 * @brief Function that creates a Subsection element
 *
 * @param _element
 * @param _root
 * @param _basicView
 * @param _menuList
 */
void SolverConfigMenuHelper::createSubSectionElements(QDomElement _element, QDomElement * _root, bool _basicView, QDomDocument * _menuList)
{
    QDomNodeList _childNodes = _element.childNodes();

    if (!_childNodes.isEmpty())
    {
        for (int i = 0; i < _childNodes.count(); i++)
        {
            QDomElement _subSectionNodeEl = _childNodes.at(i).toElement();

            if ( (_basicView) && (_subSectionNodeEl.hasAttribute(XML_SUBSECTION_VISIBLE_IN_BASIC_VIEW_ATT))
                 && (QString::compare(_subSectionNodeEl.attribute(XML_SUBSECTION_VISIBLE_IN_BASIC_VIEW_ATT),YES_VALUE) == StrEqual) )
            {
                QDomNodeList _subSectionChildren = _subSectionNodeEl.childNodes();
                for (int j= 0;j < _subSectionChildren.count();j++)
                {
                    QDomElement _nodeElement = _subSectionChildren.at(j).toElement();
                    if ( QString::compare(_nodeElement.tagName(),XML_SUBSECTION_TITLE_NAME) == StrEqual )
                    {

                        QDomElement _createdEl;
                     //   if (_basicView)
                            _createdEl= _menuList->createElement(_nodeElement.text());
                     //   else
                     //      _createdEl = _advancedMenuList->createElement(_nodeElement.text());
                        _root->appendChild(_createdEl);
                    }
                }
            }else
            {
                QDomNodeList _subSectionChildren = _subSectionNodeEl.childNodes();
                for (int j= 0;j < _subSectionChildren.count();j++)
                {
                    QDomElement _nodeElement = _subSectionChildren.at(j).toElement();
                    if ( QString::compare(_nodeElement.tagName(),XML_SUBSECTION_TITLE_NAME) == StrEqual )
                    {

                        QDomElement _createdEl;
                        _createdEl = _menuList->createElement(_nodeElement.text());
                        _root->appendChild(_createdEl);
                    }
                }
            }
        }
    }

}


/******************************* Search / Find  *********************************************************/

/**
 * @brief Function that finds a section element by name
 *
 * @param _sectionName
 * @param _list
 * @return QDomElement
 */
QDomElement SolverConfigMenuHelper::findSectionElementByName(QString _sectionName,QDomNodeList _list)
{
    if (!_list.isEmpty())
    {
        for(int i = 0;i < _list.count();i++)
        {
            QDomElement _element = _list.at(i).toElement();
            if (compareSectionTitle(_sectionName,_element))
                return _element;
        }
    }
    return QDomElement();

}
/******************************* Compare *********************************************************/

/**
 * @brief functions that compares the section titles
 *
 * @param _name
 * @param _element
 * @return bool
 */
bool SolverConfigMenuHelper::compareSectionTitle(QString _name,QDomElement _element)
{
    QDomNodeList _children = _element.childNodes();
    if (!_children.isEmpty())
    {
        for (int i = 0;i < _children.count();i++)
        {
            QDomElement _child = _children.at(i).toElement();
            if (QString::compare(_child.tagName(),XML_TITLE_SECTION_NAME) == StrEqual)
                if (QString::compare(_child.text(),_name)  == StrEqual )
                    return true;
        }
    }


    return false;
}



//-------------





