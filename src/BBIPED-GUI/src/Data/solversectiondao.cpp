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
#include "solversectiondao.h"
#include "Defines/General/GeneralDefines.h"

/*********************************************************************************************/
/**
 * @brief Constructor
 *
 */
SolverSectionDAO::SolverSectionDAO()
{
}

/**************************************** FIND ************************************************/
/**
 * @brief Find a section in a list
 *
 * @param _sectionName
 * @param _list
 * @return SolverSectionVO
 */
SolverSectionVO SolverSectionDAO::findSection(QString _sectionName, QList<SolverSectionVO> _list)
{
    if (_list.isEmpty())
        return SolverSectionVO();
    for(int i = 0; i < _list.count() ;i++)
    {
        SolverSectionVO _element = _list.at(i);
        if (QString::compare(_element.getSectionName(),_sectionName) == StrEqual)
            return _element;
    }

    return SolverSectionVO();
}


/**
 * @brief Find the subsectionNames associated to a section name
 *
 * @param _sectionName
 * @param _list
 * @return QStringList
 */
QStringList SolverSectionDAO::findSubSectionNamesBySectionName(QString _sectionName,QList<SolverSectionVO> _list)
{
   QStringList _result;
    if (_list.isEmpty())
        return _result;
    for (int i = 0; i < _list.size(); i++)
    {
        SolverSectionVO _section = _list.at(i);
        if (QString::compare(_sectionName,_section.getSectionName()) == StrEqual)
            _result.append(_section.getSubsectionName());
    }
    return _result;
}


/**
 * @brief Find the section name associated to a subsection name
 *
 * @param _subSectionName
 * @param _list
 * @return QString
 */
QString SolverSectionDAO::findSectionNameBySubSectionName(QString _subSectionName, QList<SolverSectionVO> _list)
{
    QString _foundName;

    if ( (_list.isEmpty()) || (_subSectionName.isEmpty()) )
        return _foundName;

    for (int i = 0; i < _list.size();  i++)
    {
        SolverSectionVO _element =  _list.at(i);
        if (QString::compare(_element.getSubsectionName(),_subSectionName) == StrEqual)
            return _element.getSectionName();
    }
    return _foundName;
}

/**
 * @brief find subsection in a list
 *
 * @param _subSectionName
 * @param _list
 * @return SolverSectionVO
 */
SolverSectionVO SolverSectionDAO::findSubSection(QString _subSectionName, QList<SolverSectionVO> _list)
{
    if (_list.isEmpty())
        return SolverSectionVO();
    for(int i = 0; i < _list.count() ;i++)
    {
        SolverSectionVO _element = _list.at(i);
        if (QString::compare(_element.getSubsectionName(),_subSectionName) == StrEqual)
            return _element;
    }

    return SolverSectionVO();
}


/**
 * @brief find the Value of a variable
 *
 * @param _solverVO
 * @param _varName
 * @return QString
 */
QString SolverSectionDAO::findValueNameByVariableName(SolverSectionVO _solverVO, QString _varName)
{
    QString _result;

    QList<SolverVariableVO> _varList = _solverVO.getSubsectionVarList();
    if (!_varList.isEmpty())
    {

        for (int i = 0; i < _varList.count(); i++)
        {
            SolverVariableVO _varVO = _varList.at(i);
            QString _variable = _varVO.getVariableName();
            if (QString::compare(_variable, _varName) == StrEqual)
            {
                return (_varVO.getVariableFileName());
            }
        }
    }


    return _result;
}

/**
 * @brief Find the value name associated to a variable name
 *
 * @param _solverList
 * @param _varName
 * @return QString
 */
QString SolverSectionDAO::findValueNameByVariableName(QList<SolverSectionVO> _solverList, QString _varName)
{
    QString _result;
    if (!_solverList.isEmpty())    
        for (int j= 0; j < _solverList.size();j++)
        {
            SolverSectionVO _solverVO = _solverList.at(j);
            QList<SolverVariableVO> _varList = _solverVO.getSubsectionVarList();
            if (!_varList.isEmpty())
                for (int i = 0; i < _varList.count(); i++)
                {
                    SolverVariableVO _varVO = _varList.at(i);
                    QString _variable = _varVO.getVariableName();
                    if (QString::compare(_variable, _varName) == StrEqual)                    
                        return (_varVO.getVariableFileName());                    
                }
        }

    return _result;
}

/**
 * @brief Find a variable by its value name
 *
 * @param _list
 * @param _fileName
 * @return SolverVariableVO
 */
SolverVariableVO SolverSectionDAO::findVariableByFileName(QList<SolverSectionVO> _list, QString _fileName)
{
    if (!_list.isEmpty())
        for (int i = 0; i<_list.count();i++)
        {
            SolverSectionVO _sectionVO = _list.at(i);
            QList<SolverVariableVO> _varList = _sectionVO.getSubsectionVarList();
            SolverVariableVO _foundVariable = findVariableByFileName(_varList,_fileName);
            if (!_foundVariable.isEmpty())
                return _foundVariable;            
        }

    return SolverVariableVO();
}

/**
 * @brief Find a Variable by its value name
 *
 * @param _list
 * @param _fileName
 * @return SolverVariableVO
 */
SolverVariableVO SolverSectionDAO::findVariableByFileName(QList<SolverVariableVO> _list, QString _fileName)
{
    if (!_list.isEmpty())
        for (int i = 0; i<_list.count();i++)
        {
            SolverVariableVO _variableVO = _list.at(i);
            QString _cFileName = _variableVO.getVariableFileName();
            if (QString::compare(_cFileName,_fileName) == StrEqual)
                return _variableVO;
        }
    return SolverVariableVO();
}


/**
 * @brief Find a Variable by the value name
 *
 * @param _solverVO
 * @param _valueName
 * @return QString
 */
QString SolverSectionDAO::findVariableNameByValueName(SolverSectionVO _solverVO,QString _valueName)
{
    QList<SolverVariableVO> _listVariables = _solverVO.getSubsectionVarList();
    if (!_listVariables.isEmpty())
        for (int i = 0; i < _listVariables.count();i++)
        {
            SolverVariableVO _varVO = _listVariables.at(i);
            QString _varFileName = _varVO.getVariableFileName();
            QString _variableName = _varVO.getVariableName();
            if (QString::compare(_valueName,_varFileName) == StrEqual)
                return _variableName;
        }

    return QString("");
}

SolverVariableVO SolverSectionDAO::findVariableByValueName(SolverSectionVO _solverVO,QString _valueName)
{
    QList<SolverVariableVO> _listVariables = _solverVO.getSubsectionVarList();
    if (!_listVariables.isEmpty())
        for (int i = 0; i < _listVariables.count();i++)
        {
            SolverVariableVO _varVO = _listVariables.at(i);
           // QString _varFileName = _varVO.getVariableFileName();
            QString _variableName = _varVO.getVariableName();
            if (QString::compare(_valueName,_variableName) == StrEqual)
                return _varVO;
        }
    SolverVariableVO _result;
    return _result;
}

/***********************************CREATE********************************************/

/**
 * @brief Creates a section Element with the name
 *
 * @param _sectionName
 * @return SolverSectionVO
 */
SolverSectionVO * SolverSectionDAO::createSectionElement(QString _sectionName)
{
    SolverSectionVO * _createdElement = new SolverSectionVO();
    _createdElement->setSubsectionName(_sectionName);
    return _createdElement;
}

/**
 * @brief creates a section element
 *
 * @param _sectionName
 * @param _listVariables
 * @return SolverSectionVO
 */
SolverSectionVO * SolverSectionDAO::createSectionElement(QString _sectionName, QList<SolverVariableVO> _listVariables)
{
    SolverSectionVO * _createdElement = new SolverSectionVO();
    _createdElement->setSubsectionName(_sectionName);
    _createdElement->setSubsectionVarList(_listVariables);
    return _createdElement;

}

/*********************************** GETTER/SETTER/IS ********************************************/
/**
 * @brief Checks if the current value of a variable is the default value
 *
 * @param _list
 * @param _varFileName
 * @param _cValue
 * @return bool
 */
bool SolverSectionDAO::isDefaultValueByFileName(QList<SolverSectionVO> _list, QString _varFileName, QString _cValue)
{
    if (!_list.isEmpty())
        for (int i = 0; i<_list.count();i++)
        {
            SolverSectionVO _sectionVO = _list.at(i);
            QList<SolverVariableVO> _varList = _sectionVO.getSubsectionVarList();
            SolverVariableVO _foundVariable = findVariableByFileName(_varList,_varFileName);
            if  (!_foundVariable.isEmpty())
            {
                QString _defaultValue = _foundVariable.getDefaultValue().simplified();
                _defaultValue.replace(" ","");
                _cValue = _cValue.simplified();
                _cValue.replace(" ","");
                if (QString::compare(_defaultValue,_cValue) == StrEqual)
                    return true;
            }
        }

    return false;
}



/**
 * @brief Get the list fo variables
 *
 * @param _list
 * @return QStringList
 */
QStringList SolverSectionDAO::getVariableNameList(QList<SolverVariableVO> _list)
{
    QStringList _resultList;
    _resultList.append("NONE");
    if (!_list.isEmpty())
        for (int i = 0; i < _list.count(); i++)
        {
            SolverVariableVO _var = _list.at(i);
            _resultList.append(_var.getVariableName());
        }

    return _resultList;

}


/**
 * @brief
 *
 * @param _listSection
 * @return QStringList
 */
QStringList SolverSectionDAO::getOptionalVariables(QList<SolverSectionVO> _listSection)
{
    QStringList _result;
    for (int i = 0; i < _listSection.size(); i++)
    {
        SolverSectionVO _secVO = _listSection.at(i);
        QList<SolverVariableVO> _varList = _secVO.getSubsectionVarList();
        for (int j = 0; j < _varList.size(); j++)
        {
            SolverVariableVO _varVO = _varList.at(j);
            if (_varVO.isOptional())
                _result.append(_varVO.getVariableFileName());
        }

    }

    return _result;
}
