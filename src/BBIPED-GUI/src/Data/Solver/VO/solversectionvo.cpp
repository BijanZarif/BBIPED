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
 *   along with Foobar. If not, see <http://opensource.org/licenses/LGPL-3.0>
 *
 *  Author: Dec 2016: Carmen Alonso Montes calonso@bcamath.org.
***/
#include "solversectionvo.h"

/**
 * @brief Constructor
 *
 */
SolverSectionVO::SolverSectionVO()
{
    _sectionName = "";
    _subsectionName =  "";
}

/**
 * @brief Destructor
 *
 */
SolverSectionVO::~SolverSectionVO()
{
}

/************************************ Getters/Setters************************************/

/**
 * @brief Set the Section Name
 *
 * @param _name
 */
void SolverSectionVO::setSectionName(QString _name)
{
    _sectionName = _name;
}

/**
 * @brief Get associated Section Name
 *
 * @return QString
 */
QString SolverSectionVO::getSectionName()
{
    return _sectionName;
}

/**
 * @brief Set Subsection name
 *
 * @param _name
 */
void SolverSectionVO::setSubsectionName(QString _name)
{
    _subsectionName = _name;
}

/**
 * @brief Get Associated Subsection Name
 *
 * @return QString
 */
QString SolverSectionVO::getSubsectionName()
{
    return _subsectionName;
}

/**
 * @brief Set Associated list of variables of a Subsection
 *
 * @param _list
 */
void SolverSectionVO::setSubsectionVarList(QList<SolverVariableVO> _list)
{
    _variableList = _list;
}

/**
 * @brief returns the associated variable list
 *
 * @return QList<SolverVariableVO>
 */
QList<SolverVariableVO> SolverSectionVO::getSubsectionVarList()
{
    return _variableList;
}


