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
#include "solvervariablevo.h"

#include "Html/htmlformatter.h"
#include "Defines/General/GeneralDefines.h"
/**
 * @brief Constructor
 *
 */
SolverVariableVO::SolverVariableVO()
{
    _name = "";
    _defaultValue= "";
    _fileName= "";
    _currentValue= "";
    _isEmpty = true;
    _isVisible = true;
    _unitTxt = "";
    _isUniqueSelection = true;
    _isRepeatable = false;
    _isOptional = false;
    _isVisibleInMenu = true;
    _isSelectable = true;
}

/**
 * @brief Destructor
 *
 */
SolverVariableVO::~SolverVariableVO()
{

}
/************************************ Getters/Setters************************************/

/**
 * @brief Set the Variable Name
 *
 * @param _newName
 */
void SolverVariableVO::setVariableName(QString _newName)
{
    _name = _newName;
    _isEmpty = false;
}

/**
 * @brief Get the Associated variable name
 *
 * @return QString
 */
QString SolverVariableVO::getVariableName()
{
    return _name;
     _isEmpty = false;
}

/**
 * @brief set the Associated default value
 *
 * @param _newDefaultValue
 */
void SolverVariableVO::setDefaultValue(QString _newDefaultValue)
{
    _defaultValue = _newDefaultValue;
     _isEmpty = false;
}

/**
 * @brief get the Associated default value
 *
 * @return QString
 */
QString SolverVariableVO::getDefaultValue()
{
    return _defaultValue;
}

/**
 * @brief Set the associated Help Contents
 *
 * @param _newHelpContents
 */
void SolverVariableVO::setHelpContents(SolverVarHelpVO * _newHelpContents)
{
    _helpContents = _newHelpContents;
     _isEmpty = false;
}

/**
 * @brief  Get the associated Help Contents
 *
 * @return SolverVarHelpVO
 */
QString SolverVariableVO::getHelpContents(bool _withFormat)
{
     QString _text;
     if (_withFormat)
     {
         HtmlFormatter _htmlFormatter;
         _htmlFormatter.startFormatting();
         _htmlFormatter.appendTitle(this->getVariableName());
         _htmlFormatter.appendTitleParagraph("Definition");
         _htmlFormatter.appendParagraph(_helpContents->getDefinitionText());
         _htmlFormatter.appendTitleParagraph("Possible values and default Values");
         _htmlFormatter.appendParagraph( _helpContents->getValuesText());
         _htmlFormatter.appendTitleParagraph("Other Details");
         _htmlFormatter.appendParagraph(_helpContents->getDetailsText());

         // Adding the values in a table
         _htmlFormatter.appendTitleParagraph("Quick Summary");
         QStringList _headerTable;
         QList<QStringList> _contentTable;
         QString _row1("Possible Values;");
         _row1+=(this->getComboValuesList()).join(COMMA_SEPARATOR);
         QString _row2("Default Value;");
         _row2+= this->getDefaultValue();
         QString _row3("Variable Name in SU2 file;");
         _row3+=this->getVariableFileName();

         _contentTable.append(_row1.split(SEMICOLON_SEPARATOR));
         _contentTable.append(_row2.split(SEMICOLON_SEPARATOR));
         _contentTable.append(_row3.split(SEMICOLON_SEPARATOR));
        _htmlFormatter.appendTable(_headerTable,_contentTable);
         _htmlFormatter.endFormatting();
        _text = _htmlFormatter.getFormatedText();

     }else
     {
         _text = _helpContents->getDefinitionText();
     }

    return _text;


}

/**
 * @brief Set the associated combo values list
 *
 * @param _newComboValuesList
 */
void SolverVariableVO::setComboValuesList(QStringList _newComboValuesList)
{
    _comboValuesList = _newComboValuesList;
     _isEmpty = false;
}

void SolverVariableVO::setSelectableComboValues( QList<bool> _list)
{
    _selectableComboValues =_list;
}

QList<bool> SolverVariableVO::getSelectableComboValues()
{
    return _selectableComboValues;
}

/**
 * @brief Get the associated combo values list
 *
 * @return QStringList
 */
QStringList SolverVariableVO::getComboValuesList()
{
    return _comboValuesList;
}

/**
 * @brief Set the names of the combos
 *
 * @param _list
 */
void SolverVariableVO::setComboValueNamesList(QStringList _list)
{
    _comboValueNamesList = _list;
    _isEmpty = false;
}


/**
 * @brief Get the names of the combos
 *
 * @return QStringList
 */
QStringList SolverVariableVO::getComboValueNamesList()
{
    return _comboValueNamesList;
}

/**
 * @brief Set the variable name as it appears in the configuration file
 *
 * @param _newName
 */
void SolverVariableVO::setVariableFileName(QString _newName)
{
    _fileName = _newName;
     _isEmpty = false;
}

/**
 * @brief Get the variable name as it appears in the configuration file
 *
 * @return QString
 */
QString SolverVariableVO::getVariableFileName()
{
    return _fileName;
}

/**
 * @brief Set the current value of the variable
 *
 * @param _newValue
 */
void SolverVariableVO::setCurrentValue(QString _newValue)
{
    _currentValue = _newValue;
     _isEmpty = false;
}

/**
 * @brief  Get the current value of the variable
 *
 * @return QString
 */
QString SolverVariableVO::getCurrentValue()
{
    return _currentValue;
}

/**
 * @brief Returns true if the object is empty
 *
 * @return bool
 */
bool  SolverVariableVO::isEmpty()
{
    return _isEmpty;
}


/**
 * @brief This function gets the value of the variable to be visible or not
 *
 * @return bool
 */
bool SolverVariableVO::isVisible()
{
    return _isVisible;
}

/**
 * @brief This function allows to change the visibility of a variable
 *
 * @param _val
 */
void SolverVariableVO::setVisibility(bool _val)
{
    _isVisible = _val;
}

/**
 * @brief This function allows to store a rule associated to a variable
 *
 * @param _newRule
 */
void SolverVariableVO::setRule(VariableRule _newRule)
{
    _rule = _newRule;
}

/**
 * @brief This function returns the associated rule
 *
 * @return VariableRule
 */
VariableRule SolverVariableVO::getRule()
{
    return _rule;
}


/**
 * @brief Set the value of the (metric) units
 *
 * @param _newValue
 */
void SolverVariableVO::setUnitValue(QString _newValue)
{
    _unitTxt = _newValue;
}

/**
 * @brief Get the value of the units (metric units)
 *
 * @return QString
 */
QString SolverVariableVO::getUnitValue()
{
    return _unitTxt;
}

/**
 * @brief Get the object that contains all the help contents
 *
 * @return SolverVarHelpVO
 */
SolverVarHelpVO SolverVariableVO::getHelperContents()
{
    return (* _helpContents);
}


/**
 * @brief This function sets the option of unique selection (COMBOBOX mode) for the
 * values (true) otherwise multiple selection in the combobox values are set
 * By default is TRUE
 * @return bool
 */
bool SolverVariableVO::isUniqueSelection()
{
    return _isUniqueSelection;
}

/**
 * @brief This function is used to set the value for unique selection or multiple selection
 * of the COMBOBOX values
 *
 * @param _val
 */
void SolverVariableVO::setUniqueSelection(bool _val)
{
    _isUniqueSelection = _val;
}

/**
 * @brief Set the list of associated subvariables for this SolverVariableVO
 *
 * @param _newValue
 */
void SolverVariableVO::setSubVariableNames(QStringList _newValue)
{
    _subVariableNames = _newValue;
}

/**
 * @brief Get the name list of associated subvariables for this SolverVariableVO
 *
 * @return QStringList
 */
QStringList SolverVariableVO::getSubVariableNames()
{
    return _subVariableNames;
}


/**
 * @brief Check if this SolverVariable can be repeated
 *
 * @return bool
 */
bool SolverVariableVO::isRepeatable()
{
    return _isRepeatable;
}

/**
 * @brief Set the possibility to repeat a variable in the configuration file. By default is false;
 *
 * @param _val
 */
void SolverVariableVO::setRepeatable(bool _val)
{
    _isRepeatable = _val;
}

/**
 * @brief Check if a variable can be optional (if optional and the user does not give a value, it is not written in the configuration file)
 *
 * @return bool
 */
bool SolverVariableVO::isOptional()
{
    return _isOptional;
}

/**
 * @brief Set the possibility for a variable to be optional in the configuration file. By default is false;
 *
 * @param _val
 */
void SolverVariableVO::setOptional(bool _val)
{
    _isOptional = _val;
}


/**
 * @brief Check if a variable is visible in the Menu (by default true)
 *
 * @return bool
 */
bool SolverVariableVO::isVisibleInMenu()
{
    return _isVisibleInMenu;
}

/**
 * @brief Set the visibility of a variable in the configuration file. By default is true;
 *
 * @param _val
 */
void SolverVariableVO::setVisibleInMenu(bool _val)
{
    _isVisibleInMenu = _val;
}

/**
 * @brief Get the list of associated variables
 *
 * @return QStringList
 */
QStringList SolverVariableVO::getAssociatedVariables()
{
    return _associatedVariables;
}

/**
 * @brief Set the list of associated variables
 *
 * @param _list
 */
void SolverVariableVO::setAssociatedVariables(QStringList _list)
{
    _associatedVariables = _list;
}

bool SolverVariableVO::isVariableSelectable()
{
    return _isSelectable;
}

void SolverVariableVO::setVariableSelectable(bool _val)
{
    _isSelectable = _val;
}
