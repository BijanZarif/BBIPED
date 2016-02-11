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
 * Author: Dec 2016: Carmen Alonso Montes calonso@bcamath.org
***/
#include "userpreferencesdata.h"

/***************************************************************************************************/
/**
 * @brief Constructor
 *
 */
UserPreferencesData::UserPreferencesData()
{
    _isDefaultSolverEngineSelected = true;
    _isVirtualSolverEngineSelected = false;
    _isMultizoneSolverEngineSelected = false;
    _userDefinedSolverPath = "";
    _meshViewerPath = "";
    _postProcessorViewerPath = "";
    _showCloseWarning = true;
    _showCloseProjectWarning = true;
    _templateConfigPath = "";

    // Parallel Configuration
    _customizedParallelEnginePath = "";
    _customizedPartitionEnginePath = "";
    _isDefaultParallelEngineSelected = true;
    _isDefaultPartitionEngineSelected = true;
}

/********************************GETTER/SETTER/IS*************************************/

/**
 * @brief Returns true if the default Solver Engine is selected (SU2_CFD)
 *
 * @return bool
 */
bool UserPreferencesData::isDefaultSolverEngineSelected()
{
    return  _isDefaultSolverEngineSelected;
}

/**
 * @brief Returns true if the Virtual Solver Engine is selected (SU2_CFD_VIRTUAL)
 *
 * @return bool
 */
bool UserPreferencesData::isVirtualSolverEngineSelected()
{
    return _isVirtualSolverEngineSelected;
}


/**
 * @brief Returns true if the Virtual Solver Engine is selected (SU2_CFD_MULTIZONE)
 *
 * @return bool
 */
bool UserPreferencesData::isMultizoneSolverEngineSelected()
{
    return _isMultizoneSolverEngineSelected;

}


/**
 * @brief Set the default Solver Engine to true
 *
 * @param _val
 */
void UserPreferencesData::setDefaultSolverEngineSelected(bool _val)
{
    _isDefaultSolverEngineSelected = _val;
    _isVirtualSolverEngineSelected = !_val;
    _isMultizoneSolverEngineSelected  = !_val;
}


/**
 * @brief Set the VIRTUAL Solver Engine to true
 *
 * @param _val
 */
void UserPreferencesData::setVirtualSolverEngineSelected(bool _val)
{
    _isVirtualSolverEngineSelected = _val;
    _isDefaultSolverEngineSelected =! _val;
    _isMultizoneSolverEngineSelected  = !_val;
}


/**
 * @brief Set the MULTIZONE Solver Engine to true
 *
 * @param _val
 */
void UserPreferencesData::setMultizoneSolverEngineSelected(bool _val)
{
    _isMultizoneSolverEngineSelected  =_val;
    _isDefaultSolverEngineSelected = !_val;
    _isVirtualSolverEngineSelected = !_val;

}

/**
 * @brief Returns the path of the user defined Solver Engine
 *
 * @return QString
 */
QString UserPreferencesData::getUserDefinedSolverPath()
{
    return _userDefinedSolverPath;
}
/**
 * @brief Set the path of the user defined Solver Engine
 *
 * @param _path
 */
void UserPreferencesData::setUserDefinedSolverPath(QString _path)
{
    _userDefinedSolverPath = _path;
    _isDefaultSolverEngineSelected = false;
    _isVirtualSolverEngineSelected = false;
    _isMultizoneSolverEngineSelected  = false;
}


/**
 * @brief Returns the mesh viewer path  (e.g. Salome)
 *
 * @return QString
 */
QString UserPreferencesData::getMeshViewerPath()
{
    return _meshViewerPath;
}

/**
 * @brief Set the mesh viewer path (e.g. Salome)
 *
 * @param _path
 */
void UserPreferencesData::setMeshViewerPath(QString _path)
{
    _meshViewerPath = _path;
}

/**
 * @brief get the postProcessor viewer path (e.g. Paraview)
 *
 * @return QString
 */
QString UserPreferencesData::getPostProcessorViewerPath()
{
    return _postProcessorViewerPath;
}

/**
 * @brief Set the postProcessor viewer path (e.g. Paraview)
 *
 * @param _path
 */
void UserPreferencesData::setPostProcessorViewerPath(QString _path)
{
    _postProcessorViewerPath = _path;
}

/**
 * @brief Get the current list of already opened projects
 *
 * @return QStringList
 */
QStringList UserPreferencesData::getRecentProjectsList()
{
    return _recentProjectsList;
}

/**
 * @brief Set the current list of already opened projects
 *
 * @param _list
 */
void UserPreferencesData::setRecentProjectsList(QStringList _list)
{
   _recentProjectsList = _list;
}

/**
 * @brief Set whether the user wants to see the close window/application warning
 *
 * @param _val
 */
void UserPreferencesData::setShowCloseWarning(bool _val)
{
    _showCloseWarning = _val;
}

/**
 * @brief Set whether the user wants to see the close project warning
 *
 * @param _val
 */
void UserPreferencesData::setShowCloseProjectWarning(bool _val)
{
    _showCloseProjectWarning = _val;
}

/**
 * @brief Get the template configuration folder path
 *
 * @return QString
 */
QString UserPreferencesData::getTemplateConfigPath()
{
    return _templateConfigPath;
}

/**
 * @brief Set the template configuration folder path
 *
 * @param _file
 */
void UserPreferencesData::setTemplateConfigPath(QString _file)
{
    _templateConfigPath = _file;
}
/********************************ADD*************************************/

/**
 * @brief This function allows to add a new project to the recentProjectList. A max of 10 projects are considered
 *
 * @param _pathProject
 */
void UserPreferencesData::addProjectToList(QString _pathProject)
{
    if (_recentProjectsList.contains(_pathProject))
        _recentProjectsList.removeOne(_pathProject);

    if( (_recentProjectsList.isEmpty()) || (_recentProjectsList.size() < 10) )
       _recentProjectsList.append(_pathProject);
    else
    {
       _recentProjectsList.removeAt(0);
       _recentProjectsList.append(_pathProject);
    }
}
/********************************SHOW*************************************/

/**
 * @brief Returns true if the Close Application warning window shall appear
 *
 * @return bool
 */
bool UserPreferencesData::showCloseWarning()
{
    return _showCloseWarning;
}



/**
 * @brief Returns true if the Close Project warning window shall appear
 *
 * @return bool
 */
bool UserPreferencesData::showCloseProjectWarning()
{
   return _showCloseProjectWarning ;
}

/**
 * @brief This function is to update the data from another object
 *
 * @param _newData
 */
void UserPreferencesData::updateData(UserPreferencesData _newData)
{
    setDefaultSolverEngineSelected(_newData.isDefaultSolverEngineSelected());
    //setVirtualSolverEngineSelected(_newData.isVirtualSolverEngineSelected());
    //setMultizoneSolverEngineSelected(_newData.isMultizoneSolverEngineSelected());
    setUserDefinedSolverPath(_newData.getUserDefinedSolverPath());
    setMeshViewerPath(_newData.getMeshViewerPath());
    setPostProcessorViewerPath(_newData. getPostProcessorViewerPath());
    setRecentProjectsList(_newData.getRecentProjectsList());
    setShowCloseWarning(_newData.showCloseWarning());
    setShowCloseProjectWarning(_newData.showCloseProjectWarning());
    setTemplateConfigPath(_newData.getTemplateConfigPath());
    // Parallel Solver Engine Configuration

    if (!_newData.isDefaultParallelEngineSelected())
    {
        setCustomizedParallelEnginePath(_newData.getCustomizedParallelEnginePath());
        setDefaultParallelEngineSelected(false);
    }else
    {
        setDefaultParallelEngineSelected(true);
        setCustomizedParallelEnginePath("");
    }

    if (!_newData.isDefaultPartitionEngineSelected())
    {
        setCustomizedPartitionEnginePath(_newData.getCustomizedPartitionEnginePath());
        setDefaultPartitionEngineSelected(false);
    }else
    {
        setDefaultPartitionEngineSelected(true);
        setCustomizedPartitionEnginePath("");
    }
}

QString  UserPreferencesData::getDefaultParallelEnginePath()
{
    return _defaultParallelEnginePath;
}

void  UserPreferencesData::setDefaultParallelEnginePath(QString _path)
{
    _defaultParallelEnginePath = _path;
}

QString   UserPreferencesData::getDefaultPartitionEnginePath()
{
    return _defaultPartitionEnginePath;
}

void  UserPreferencesData::setDefaultPartitionEnginePath(QString _path)
{
    _defaultPartitionEnginePath = _path;
}

QString  UserPreferencesData::getCustomizedParallelEnginePath()
{
    return _customizedParallelEnginePath;
}

void  UserPreferencesData::setCustomizedParallelEnginePath(QString _path)
{
    _customizedParallelEnginePath = _path;
}

QString  UserPreferencesData::getCustomizedPartitionEnginePath()
{
    return _customizedPartitionEnginePath;
}

void  UserPreferencesData::setCustomizedPartitionEnginePath(QString _path)
{
    _customizedPartitionEnginePath = _path;
}

bool  UserPreferencesData::isDefaultParallelEngineSelected()
{
    return _isDefaultParallelEngineSelected;
}

void  UserPreferencesData::setDefaultParallelEngineSelected(bool _val)
{
    _isDefaultParallelEngineSelected = _val;
}

bool  UserPreferencesData::isDefaultPartitionEngineSelected()
{
    return _isDefaultPartitionEngineSelected;
}

bool  UserPreferencesData::setDefaultPartitionEngineSelected(bool _val)
{
    _isDefaultPartitionEngineSelected = _val;
}
