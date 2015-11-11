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
#include "projectdata.h"


#include <QFileInfo>

/*********************************************************************************************************/

/**
 * @brief Constructor
 *
 */
ProjectData::ProjectData()
{
    _projectPath = "";
    _projectFileName = "";
}

/**
 * @brief Destructor
 *
 */
ProjectData::~ProjectData()
{

}
/*************************************** IS *******************************************************/

/**
 * @brief Returns true if the object has both config file and mesh file paths
 *
 * @return bool
 */
bool ProjectData::solverDataComplete()
{
    if ((!_solverData.isConfigFileEmpty()) && (!_solverData.isMeshFileEmpty()))
        return true;
    else
        return false;
}

/**
 * @brief Returns true if _projectPath and project file name are empty
 *
 * @return bool
 */
bool ProjectData::isEmpty()
{
    if (!_projectPath.isEmpty())
        return false;
    if (!_projectFileName.isEmpty())
        return false;
    return true;

}
/**************************************** SAVE *******************************************************/


/**
 * @brief This function stores the path of the project
 *
 * @param _fullPath
 */
void ProjectData::saveProjectInformation(QString _fullPath)
{
    QFileInfo _qFileInfo(_fullPath);
    setProjectFileName(_qFileInfo.fileName());
    setProjectPath(_qFileInfo.absolutePath());
}

/**
 * @brief This function saves all the project information
 *
 * @param _fileName
 * @param _projectPath
 * @param _solverVO
 */
void ProjectData::saveProjectInformation(QString _fileName, QString _projectPath,SolverVO _solverVO)
{

    setProjectFileName(_fileName);
    setProjectPath(_projectPath);
    setSolverDataInformation(_solverVO);
}
/******************************************** COPY *****************************************************/

/**
 * @brief This function makes a hard (in depth) copy from one object to the other
 *
 * @param _newProjectOBJ
 */
void ProjectData::copy(ProjectData _newProjectOBJ)
{
    setProjectFileName(_newProjectOBJ.getProjectFileName());
    setProjectPath(_newProjectOBJ.getProjectPath());
    setSolverDataInformation(_newProjectOBJ.getSolverDataInformation());
}

/************************************ Getters/Setters************************************/
/**
 * @brief Establish the path of the project
 *
 * @param _path
 */
void ProjectData::setProjectPath(QString _path)
{
    _projectPath = _path;
}

/**
 * @brief Returns the absolute path of the project
 *
 * @return QString
 */
QString ProjectData::getProjectPath()
{
    return _projectPath;
}

/**
 * @brief Establish the file path of the project file
 *
 * @param _fileName
 */
void ProjectData::setProjectFileName(QString _fileName)
{
    _projectFileName = _fileName;
}

/**
 * @brief returns the path of the project file
 *
 * @return QString
 */
QString ProjectData::getProjectFileName()
{
    return _projectFileName;
}

/**
 * @brief Returns the Solver VO object with the stored information
 *
 * @return SolverVO
 */
SolverVO ProjectData::getSolverDataInformation()
{
    return _solverData;
}

/**
 * @brief Sets a new Solver VO object
 *
 * @param _newSolverInfo
 */
void ProjectData::setSolverDataInformation(SolverVO _newSolverInfo)
{
    _solverData.copy(_newSolverInfo);
}

/*************************************** CLEAR ******************************************************/

/**
 * @brief This function is to clear all the data contained in the object
 *
 */
void ProjectData::clear()
{
    _projectPath = "";
    _projectFileName = "";
    _solverData.clear();
}



/**
 * @brief This function is to update the data from another object
 *
 * @param _newProjectData
 */
void ProjectData::updateProjectData(ProjectData _newProjectData)
{
    setProjectFileName(_newProjectData.getProjectFileName());
    setProjectPath(_newProjectData.getProjectPath());
    setSolverDataInformation(_newProjectData.getSolverDataInformation());
}

