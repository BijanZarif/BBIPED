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
#include "solvervo.h"

#include "Defines/General/GeneralDefines.h"

/*********************************************************************************************/
/**
 * @brief Constructor
 *
 */
SolverVO::SolverVO()
{
    _solverFilePath = "";
    _solverConfigFilePath = "";
    _solverEngine = SOLVER_DEFAULT_ENGINE;

}

/**
 * @brief Destructor
 *
 */
SolverVO::~SolverVO()
{

}

/************************************* SETTER/GETTER/IS *********************************************/
/**
 * @brief Checks if the Mesh File Path is empty
 *
 * @return bool
 */
bool SolverVO::isMeshFileEmpty()
{
    if (_solverFilePath.isEmpty())
        return true;
    else
        return false;
}

/**
 * @brief Checks if the values of the config file are empty
 *
 * @return bool
 */
bool SolverVO::isConfigFileEmpty()
{
    if (_solverConfigFilePath.isEmpty())
        return true;
    else
        return false;
}

/**
 * @brief Set the Solver File Path infor
 *
 * @param _path
 */
void SolverVO::setSolverFilePath(QString _path)
{
    _solverFilePath = _path;
}

/**
 * @brief Returns the Solver File Path info
 *
 * @return QString
 */
QString SolverVO::getSolverFilePath()
{
    return _solverFilePath;
}

/**
 * @brief Set the Solver Config File Path
 *
 * @param _path
 */
void SolverVO::setSolverConfigFilePath(QString _path)
{
    _solverConfigFilePath = _path;
}

/**
 * @brief Returns the Solver Config File Path
 *
 * @return QString
 */
QString SolverVO::getSolverConfigFilePath()
{
    return _solverConfigFilePath;
}

/**
 * @brief Set the values for the configuration file
 *
 * @param QHash<QString
 * @param _newMapValues
 */
void SolverVO::setSolverConfigFileMapValues(QHash<QString,QString> _newMapValues)
{
    _solverConfigFileMapValues = _newMapValues;
}

/**
 * @brief Returns the solver map values
 *
 * @return QHash<QString, QString>
 */
QHash<QString,QString> SolverVO::getSolverConfigFileMapValues()
{
    return _solverConfigFileMapValues;
}

/**************************** UPDATE/COPY ********************************************/
/**
 * @brief Update the solver map values
 *
 * @param _varName
 * @param _varValue
 */
void SolverVO::updateSolverConfigFileMapValues(QString _varName,QString _varValue)
{
    // If the variable does not exist, create it, otherwise, the variable is updated
    _solverConfigFileMapValues.insert(_varName,_varValue);
}

/**
 * @brief Deep copy of one object to other
 *
 * @param _objToCopy
 */
void SolverVO::copy(SolverVO _objToCopy)
{
    this->setSolverConfigFilePath(_objToCopy.getSolverConfigFilePath());
    this->setSolverFilePath(_objToCopy.getSolverFilePath());

}
/********************************* RESET/CLEAR ***********************************************/
/**
 * @brief Reset map valeus
 *
 */
void  SolverVO::resetMapValues()
{
    this->_solverConfigFileMapValues.clear();
}

/**
 * @brief Clear values
 *
 */
void SolverVO::clear()
{
    _solverFilePath = "";
    _solverConfigFilePath = "";
    _solverConfigFileMapValues.clear();

}

