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
#include "solvervarhelpvo.h"
/************************************************************************************************/
/**
 * @brief
 *
 */
SolverVarHelpVO::SolverVarHelpVO()
{
     _definitionTxt ="";
     _valuesTxt ="";
     _detailsTxt ="";
}

/**********************************SETTER/GETTER/iS*****************************************/

/**
 * @brief This function saves the Definition of the variable for the help section
 *
 * @param _txt
 */
void SolverVarHelpVO::setDefinitionText(QString _txt)
{
    _definitionTxt = _txt;
}

/**
 * @brief This function returns the definition content
 *
 * @return QString
 */
QString SolverVarHelpVO::getDefinitionText()
{
    return _definitionTxt;
}

/**
 * @brief This function saves the values of the variable
 *
 * @param _txt
 */
void SolverVarHelpVO::setValuesText(QString _txt)
{
    _valuesTxt = _txt;
}

/**
 * @brief This function returns these values
 *
 * @return QString
 */
QString SolverVarHelpVO::getValuesText()
{
    return _valuesTxt;
}

/**
 * @brief This function stores the details associated in the help of the variable
 *
 * @param _txt
 */
void SolverVarHelpVO::setDetailsText(QString _txt)
{
    _detailsTxt = _txt;
}

/**
 * @brief This function returns the details associated in the help of the variable
 *
 * @return QString
 */
QString SolverVarHelpVO::getDetailsText()
{
    return _detailsTxt;
}
