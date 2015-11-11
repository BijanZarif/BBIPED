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
#include "multizoneprojectdata.h"

#include "Defines/General/GeneralDefines.h"
#include <QFileInfo>

#define OPTION_CONFIG_FILE 0
#define OPTION_MESH_FILE 1

/*********************************************************************************************************/
/**
 * @brief Constructor
 *
 */
MultizoneProjectData::MultizoneProjectData()
{
    _projectPath = "";
    _projectName = "";

}

/***************************************SETTER/GETTER/IS*********************************************/

/**
 * @brief Set the project Path
 *
 * @param _path
 */
void MultizoneProjectData::setProjectPath(QString _path)
{
    _projectPath = _path;
}

/**
 * @brief Get the project Path
 *
 * @return QString
 */
QString MultizoneProjectData::getProjectPath()
{
    return _projectPath;
}

/**
 * @brief Set the project name
 *
 * @param _name
 */
void MultizoneProjectData::setProjectName(QString _name)
{
    _projectName = _name;
}

/**
 * @brief Get the project name
 *
 * @return QString
 */
QString MultizoneProjectData::getProjectName()
{
    return _projectName;
}

/**
 * @brief Set the list of configuration files
 *
 * @param _list
 */
void MultizoneProjectData::setListConfigFiles(QStringList _list)
{
    _listConfigFiles = _list;
    _listConfigFileNames.clear();
    buildListofFileNames(_listConfigFiles,OPTION_CONFIG_FILE);
}

/**
 * @brief Get the list of configuration files
 *
 * @return QStringList
 */
QStringList MultizoneProjectData::getListConfigFiles()
{
    return _listConfigFiles;
}

/**
 * @brief Set the list of mesh files
 *
 * @param _list
 */
void MultizoneProjectData::setListMeshFiles(QStringList _list)
{
    _listMeshFiles = _list;
    _listMeshFileNames.clear();
    buildListofFileNames(_listMeshFiles,OPTION_MESH_FILE);

}

/**
 * @brief Get the list of mesh files
 *
 * @return QStringList
 */
QStringList MultizoneProjectData::getListMeshFiles()
{
    return _listMeshFiles;
}
/**
 * @brief Returns true if there is no value in the project path and name
 *
 * @return bool
 */
bool MultizoneProjectData::isEmpty()
{
    if (QString::compare(_projectPath,"") != StrEqual)
        return false;
    if (QString::compare(_projectName,"") != StrEqual)
        return false;
    return true;
}
/**
 * @brief Get the list of the configuration file names (without path)
 *
 * @return QStringList
 */
QStringList MultizoneProjectData::getListConfigFileNames()
{
    return _listConfigFileNames;
}

/**
 * @brief Get the list of the mesh file names (without path)
 *
 * @return QStringList
 */
QStringList MultizoneProjectData::getListMeshFileNames()
{
    return _listMeshFileNames;
}

/*******************************************BUILD ****************************************/

/**
 * @brief This function builds the list of file names (removing paths)
 *
 * @param _list
 * @param _option
 */
void MultizoneProjectData::buildListofFileNames(QStringList _list,int _option)
{
    if (!_list.isEmpty())
    {
        for (int i = 0; i< _list.count();i++)
        {
             QString _fullPath = _list.at(i);
             QFileInfo _fileInfo(_fullPath);
             QString _filename = _fileInfo.fileName();
             switch(_option)
             {
             case OPTION_CONFIG_FILE:
                 _listConfigFileNames.append(_filename);
                 break;
             case OPTION_MESH_FILE:
                 _listMeshFileNames.append(_filename);
                 break;
             }

        }
    }
}


/**
 * @brief This function is to update the data from another object
 *
 * @param _newMultiData
 */
void  MultizoneProjectData::updateMultizoneProjectData(MultizoneProjectData _newMultiData)
{
    setProjectPath(_newMultiData.getProjectPath());
    setProjectName(_newMultiData.getProjectName());
    setListConfigFiles(_newMultiData.getListConfigFiles());
    setListMeshFiles(_newMultiData.getListMeshFiles());
}


/**
 * @brief Add a config file to the lists of the multizone. The file is the full path file
 *
 * @param _file
 */
void MultizoneProjectData::addConfigFile(QString _file)
{
    _listConfigFiles.append(_file);
    setListConfigFiles(_listConfigFiles);
}

void MultizoneProjectData::addMeshFile(QString _file)
{
    _listMeshFiles.append(_file);
    setListMeshFiles(_listMeshFiles);
}

void MultizoneProjectData::removeConfigFile(QString _file)
{
    int _pos = _listConfigFiles.indexOf(_file);
    if (_pos != -1)
    {
        _listConfigFiles.removeAt(_pos);
        _listConfigFileNames.removeAt(_pos);
        _listMeshFiles.removeAt(_pos);
        _listMeshFileNames.removeAt(_pos);
    }
}

void MultizoneProjectData::removeMeshFile(QString _file)
{
    int _pos = _listMeshFiles.indexOf(_file);
    if (_pos != -1)
    {
        _listMeshFiles.insert(_pos,"NONE");
        _listMeshFileNames.insert(_pos,"NONE");
    }
}

void MultizoneProjectData::updatePair(QString _configPath, QString _meshPath)
{
    QString _configFileName = QFileInfo(_configPath).fileName();
    if (_listConfigFileNames.contains(_configFileName))
    {
        int _pos = _listConfigFileNames.indexOf(_configFileName);
        if (_pos != -1)
        {
            if (_pos> _listMeshFileNames.size())
                addMeshFile(_meshPath);
            else
            {
                _listMeshFiles.insert(_pos,_meshPath);
                _listMeshFileNames.insert(_pos,QFileInfo(_meshPath).fileName());
            }
        }
    }else
    {
        addConfigFile(_configPath);
        addMeshFile(_meshPath);
    }

}
