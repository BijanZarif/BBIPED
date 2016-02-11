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
#include "utilfiles.h"

#include <iostream>
#include <stdio.h>

#include "Utility/Xml/xmlutil.h"

using namespace std;

// DEFINE SECTION
#include "Defines/General/GeneralDefines.h"

/**
 * @brief
 *
 */
UtilFiles::UtilFiles()
{
}

/************************************ Project Functions ************************************/

/************************************ CREATE  ************************************/

/**** Project Data Structure CREATION */

/**
 * @brief Create the project Structure, file and paths in the corresponding path
 *          If everything goes Ok --> return true, otherwise --> False
 * @param _projectFilePath
 * @return bool
 */
bool UtilFiles::createProjectStructure(QString _projectFilePath)
{
    bool _createdProject = true;

    // 1. Get Base path and filename

    QFileInfo pathInfo( _projectFilePath );
    QString _baseFileName( pathInfo.fileName());
    QString _baseProjectName (pathInfo.baseName());
    QString _rootProjectPath ( pathInfo.absolutePath());

    QString _finalPath = QDir(_rootProjectPath).filePath(_baseProjectName);

    QDir _projectDir (_finalPath);
    if (!_projectDir.exists())
        _projectDir.mkpath(".");
    // Creating the project structure
    createProjectSubfolders(_finalPath);
    // Creating the associated file project
    _createdProject = createProjectFile(_baseFileName,_finalPath);

    return _createdProject;
}


/**
 * @brief Create the multizoen project Structure, file and paths in the corresponding path
 *          If everything goes Ok --> return true, otherwise --> False
 *
 * @param _projectFilePath
 * @return bool
 */
bool UtilFiles::createMultizoneProjectStructure(QString _projectFilePath)
{
    bool _createdProject = true;

    // 1. Get Base path and filename

    QFileInfo pathInfo( _projectFilePath );
    QString _baseFileName( pathInfo.fileName());
    QString _baseProjectName (pathInfo.baseName());
    QString _rootProjectPath ( pathInfo.absolutePath());

    QString _finalPath = QDir(_rootProjectPath).filePath(_baseProjectName);

    QDir _projectDir (_finalPath);
    if (!_projectDir.exists())
        _projectDir.mkpath(".");
    // Creating the project structure
    createProjectSubfolders(_finalPath);
    // Creating the associated file project
    _createdProject = createMultizoneProjectFile(_baseFileName,_finalPath);

    return _createdProject;
}

/**
 * @brief Create the project subfolders according with the implemented TOOLs in the corresponding path
 *
 * @param _basePath
 */
void UtilFiles::createProjectSubfolders(QString _basePath)
{
        QString _newPath = QDir(_basePath).filePath(getSubfolderName(Tool4Id));
        QDir _projectDir (_newPath);
        if (!_projectDir.exists())
            _projectDir.mkpath(".");
}

/**
 * @brief Create the project file (.prj) with the needed information. With the name and in the path
 *          passed as arguments. If everything is ok --> returns true, otherwise false
 *
 * @param _baseFileName
 * @param _finalPath
 * @return bool
 */
bool UtilFiles::createProjectFile(QString _baseFileName,QString _finalPath)
{
    bool _fileCreated = true;

    QString _suffix = QFileInfo(_baseFileName).suffix();
    if (_suffix.isEmpty())
        _baseFileName = _baseFileName + "."+ SOLVER_PROJECT_FILE_EXT;
    QString _finalFilePath = QDir(_finalPath).filePath(_baseFileName);
    XmlUtil _xmlUtil;
    _fileCreated= _xmlUtil.createInitialXmlProjectFile(_finalFilePath, _finalPath,_baseFileName);

    this->_finalAbsoluteFilePath = _finalFilePath;

    return _fileCreated;
}


/**
 * @brief Create the multizone project file (.mprj) with the needed information. With the name and in the path
 *          passed as arguments. If everything is ok --> returns true, otherwise false
 * @param _baseFileName
 * @param _finalPath
 * @return bool
 */
bool UtilFiles::createMultizoneProjectFile(QString _baseFileName,QString _finalPath)
{
    bool _fileCreated = true;

    QString _suffix = QFileInfo(_baseFileName).suffix();
    if (_suffix.isEmpty())
        _baseFileName = _baseFileName + "."+ MULTIZONE_PROJECT_FILE_EXT;
    QString _finalFilePath = QDir(_finalPath).filePath(_baseFileName);
    XmlUtil _xmlUtil;
    _fileCreated= _xmlUtil.createInitialXmlMultizoneProjectFile(_finalFilePath, _finalPath,_baseFileName);

    this->_finalAbsoluteFilePath = _finalFilePath;

    return _fileCreated;
}

/************************************ OPEN  ************************************/

/**
 * @brief Obtains the data stored in the file project (.prj) returning the data into the Project data object
 *
 * @param _projectFilePath
 * @return ProjectData
 */
ProjectData UtilFiles::openProjectStructure(QString _projectFilePath)
{

    XmlUtil _xmlUtil;
    ProjectData _openedProject = _xmlUtil.readXmlProjectFile(_projectFilePath);

    return _openedProject;
}

/**
 * @brief
 *
 * @param _projectFilePath
 * @return ProjectDetails
 */
ProjectDetails UtilFiles::openProjectDetails(QString _projectFilePath)
{
    XmlUtil _xmlUtil;
    ProjectDetails _projectDetails = _xmlUtil.getXmlProjectDetails(_projectFilePath);

    return _projectDetails;
}


/**
 * @brief Obtains the data stored in the multizone file project (.mprj) returning the data into the Multizone Project data object
 *
 * @param _projectFilePath
 * @return MultizoneProjectData
 */
MultizoneProjectData UtilFiles::openMultizoneProjectStructure(QString _projectFilePath)
{

    XmlUtil _xmlUtil;
    MultizoneProjectData _openedProject = _xmlUtil.readXmlMultizoneProjectFile(_projectFilePath);

    return _openedProject;
}

/************************************ UPDATE  ************************************/
/**
 * @brief Update the project Data object values into the correspondent file and save it to disk
 *
 * @param _projectDataToSave
 */
void UtilFiles::updateProjectDataToFile(ProjectData _projectDataToSave,ProjectDetails _projectDetails)
{
    XmlUtil _xmlUtil;
    QString _fullPath = QDir( _projectDataToSave.getProjectPath()).filePath(_projectDataToSave.getProjectFileName());
    _xmlUtil.createXmlProjectFile(_fullPath, _projectDataToSave.getProjectPath(),
                                  _projectDataToSave.getProjectFileName(),
                                  _projectDataToSave.getSolverDataInformation().getSolverFilePath(),
                                  _projectDataToSave.getSolverDataInformation().getSolverConfigFilePath(),
                                  _projectDetails);


}

/**
 * @brief Update the multizone project Data object values into the correspondent file and save it to disk
 *
 * @param _multizoneProjectDataToSave
 */
void UtilFiles::updateMultizoneProjectDataToFile(MultizoneProjectData _multizoneProjectDataToSave,ProjectDetails _projectDetails)
{
    XmlUtil _xmlUtil;
    QString _fileNameWithExt = _multizoneProjectDataToSave.getProjectName();// + "."+ MULTIZONE_PROJECT_FILE_EXT;
    QString _fileExt = QFileInfo(_multizoneProjectDataToSave.getProjectName()).completeSuffix();
    if(  (_fileExt.isEmpty()) || (QString::compare(_fileExt,MULTIZONE_PROJECT_FILE_EXT) != StrEqual) )
    {
        // We add the file Extension
        _fileNameWithExt = _fileNameWithExt + "."+ MULTIZONE_PROJECT_FILE_EXT;
    }


    QString _fullPath = QDir(_multizoneProjectDataToSave.getProjectPath()).filePath(_fileNameWithExt);
    _xmlUtil.createXmlMultizoneProjectFile(_fullPath, _multizoneProjectDataToSave,_projectDetails);

}
/**
 * @brief Returns a ProjectData object with the updated path values
 *
 * @param _oldProjectData
 * @param _newProjectData
 * @return ProjectData
 */
ProjectData  UtilFiles::updateToolPaths(ProjectData _oldProjectData,ProjectData _newProjectData)
{
    ProjectData  _updatedProjectData;

    if (_oldProjectData.isEmpty())
        return _updatedProjectData;

    _updatedProjectData.setProjectPath(_newProjectData.getProjectPath());
    _updatedProjectData.setProjectFileName(_newProjectData.getProjectFileName());

    QString _projectPath = _newProjectData.getProjectPath();

    // Solver updated
    QString _tool4Path = QDir(_projectPath).filePath(TOOL4);
    //Setting the mesh file name
    QString _oldMeshFileName;
    QString _meshFileName;
    if (!_oldProjectData.getSolverDataInformation().isMeshFileEmpty())
    {
        _oldMeshFileName  = QFileInfo(_oldProjectData.getSolverDataInformation().getSolverFilePath()).fileName();
       _meshFileName = QDir(_tool4Path).filePath(_oldMeshFileName);
    }

    QString _oldConfigFileName;
    QString _configFileName;
    if (!_oldProjectData.getSolverDataInformation().isConfigFileEmpty())
    {

       _oldConfigFileName = QFileInfo(_oldProjectData.getSolverDataInformation().getSolverConfigFilePath()).fileName();
      _configFileName = QDir(_tool4Path).filePath(_oldConfigFileName);
    }
    // Setting up the new values in the solver data
    SolverVO _newSolverVO;
    _newSolverVO.setSolverFilePath(_meshFileName);
    _newSolverVO.setSolverConfigFilePath(_configFileName);
    _updatedProjectData.setSolverDataInformation(_newSolverVO);

    return _updatedProjectData;
}

/**
 * @brief Update teh Multizone Tool paths according with the new Project Data
 *
 * @param _oldMultizoneProjectData
 * @param _newMultizoneProjectData
 * @return MultizoneProjectData
 */
MultizoneProjectData UtilFiles::updateMultizoneToolPaths(MultizoneProjectData _oldMultizoneProjectData,MultizoneProjectData _newMultizoneProjectData)
{
    MultizoneProjectData  _updatedMultizoneProjectData;

    _updatedMultizoneProjectData.setProjectPath(_newMultizoneProjectData.getProjectPath());
    _updatedMultizoneProjectData.setProjectName(_newMultizoneProjectData.getProjectName());

    QString _projectPath = _newMultizoneProjectData.getProjectPath();

    // Solver updated
    QString _tool4Path = QDir(_projectPath).filePath(TOOL4);
    QStringList _listConfigNames = _oldMultizoneProjectData.getListConfigFileNames();
    QStringList _newConfigList;
    if (!_listConfigNames.isEmpty())
        for (int i = 0; i < _listConfigNames.size();i++)
        {
            QString _fileName = _listConfigNames.at(i);
            _newConfigList.append(QDir(_tool4Path).filePath(_fileName));
        }
    QStringList _listMeshNames = _oldMultizoneProjectData.getListMeshFileNames();
    QStringList _newMeshList;
    if (!_listMeshNames.isEmpty())
        for (int i = 0; i < _listMeshNames.size();i++)
        {
            QString _fileMeshName = _listMeshNames.at(i);
            _newMeshList.append(QDir(_tool4Path).filePath(_fileMeshName));
        }

    _updatedMultizoneProjectData.setListConfigFiles(_newConfigList);
    _updatedMultizoneProjectData.setListMeshFiles(_newMeshList);

    return _updatedMultizoneProjectData;
}

/************************************ COPY Functions ************************************/
/**
 * @brief Copy all the files from one project to other, regarding the content of the tool folders
 *
 * @param _oldProjectData
 * @param _newProjectData
 */
void UtilFiles::copyAllToolData(ProjectData _oldProjectData,ProjectData _newProjectData)
{
    QDir _oldPath(_oldProjectData.getProjectPath());
    QDir _newPath (_newProjectData.getProjectPath());

    if (_oldPath.exists())
    {
        QDir _oldTool4Path (_oldPath.filePath(TOOL4));
        QStringList _listFiles;

        if (_oldTool4Path.exists())
        {
            _listFiles = _oldTool4Path.entryList();
            QDir _newTool4Path(_newPath.filePath(TOOL4));
            copyAllFiles(_oldTool4Path,_newTool4Path,_listFiles);
        }

    }

}

/**
 * @briefCopy all the files from one project to other, regarding the content of the tool folders for Multizone Project
 *
 * @param _oldProjectData
 * @param _newProjectData
 */
void UtilFiles::copyAllToolData(MultizoneProjectData _oldProjectData,MultizoneProjectData _newProjectData)
{
    QDir _oldPath(_oldProjectData.getProjectPath());
    QDir _newPath (_newProjectData.getProjectPath());

    if (_oldPath.exists())
    {
        QDir _oldTool4Path (_oldPath.filePath(TOOL4));
        QStringList _listFiles;

        if (_oldTool4Path.exists())
        {
            _listFiles = _oldTool4Path.entryList();
            QDir _newTool4Path(_newPath.filePath(TOOL4));
            copyAllFiles(_oldTool4Path,_newTool4Path,_listFiles);
        }

    }

}
/**
 * @brief Copy all the files from one project to other
 *
 * @param _oldPath
 * @param _newPath
 * @param _listFiles
 */
void UtilFiles::copyAllFiles(QDir _oldPath, QDir _newPath, QStringList _listFiles)
{
    for (int i=0;i<_listFiles.count();i++)
    {
        QString _oldFilePath = _oldPath.filePath(_listFiles.at(i));
        QString _newFilePath = _newPath.filePath(_listFiles.at(i));
        QFile::copy(_oldFilePath,_newFilePath);
    }
}

/**
 * @brief Create a copy of a file with a different name
 *
 * @param _filetoCopy
 * @param _newName
 */
void UtilFiles::copyFileToNewName(QString _filetoCopy, QString _newName)
{
    QFile _file(_filetoCopy);
    if (_file.exists())
    {
        QFileInfo _fileInfo(_filetoCopy);
        QString _filePath = _fileInfo.absolutePath();
        QString _newFileName = QDir(_filePath).filePath(_newName);
        QFile::copy(_filetoCopy,_newFileName);
    }
}

/************************************ Getters/Setters/Has/Is Functions ******************/

/**
 * @brief Get the correspondent QString name of the tools subfolders
 *
 * @param _indexTool
 * @return QString
 */
QString UtilFiles::getSubfolderName(int _indexTool)
{
    QString _subfolderName = "";
    switch(_indexTool)
    {
         case 4:
            _subfolderName = TOOL4;
            break;
       }
    return _subfolderName;
}


/**
 * @brief Gets the final Path of the whole project
 *
 * @return QString
 */
QString UtilFiles::getFinalPath()
{
    return _finalAbsoluteFilePath;
}



/**
 * @brief Checks if the file (absolute path + filename) contains the _extension, returning true, otherwise false
 *
 * @param _path
 * @param _extension
 * @return bool
 */
bool UtilFiles::hasFileExtension(QString _path, QString _extension)
{
    QFileInfo _fileInfo = QFileInfo(_path);
    QString _fileExtension = _fileInfo.completeSuffix();
    if (QString::compare(_fileExtension, _extension) != StrEqual)
    {
        return false;
    }else
    {
        return true;
    }
}

/************************************ IMPORT  ************************************/

/**
 * @brief Functionality to import a file and update the correspondent values in the project data
 * and copying this file into the project folder
 *
 * @param _filePath
 * @param _projectVO
 * @param _toolFolder
 * @return QString
 */
QString UtilFiles::importFile(QString _filePath,  ProjectData _projectVO, QString _toolFolder)
{
    QFileInfo _origPathInfo(_filePath);
    QString _destPath = QDir(_projectVO.getProjectPath()).filePath(_toolFolder);
    QString _fileName = _origPathInfo.fileName();
    QString _destFilePath = QDir(_destPath).filePath(_fileName);

    QFile _destConfigFile(_destFilePath);
     if( (_destConfigFile.exists()) && (QString::compare(_destFilePath,_filePath) != StrEqual))
        _destConfigFile.remove();
    QFile::copy(_filePath,_destFilePath);

    return _destFilePath;
}

bool UtilFiles::createLink(QString _filePath, ProjectData _projectVO, QString _toolFolder)
{
    QFileInfo _origPathInfo(_filePath);
    QString _destPath = QDir(_projectVO.getProjectPath()).filePath(_toolFolder);
    QString _fileName = _origPathInfo.fileName();
    QString _destFilePath = QDir(_destPath).filePath(_fileName);

    QFile _destConfigFile(_destFilePath);
     if( (_destConfigFile.exists()) && (QString::compare(_destFilePath,_filePath) != StrEqual))
        _destConfigFile.remove();
    bool _success = QFile::link(_filePath,_destFilePath);
    return _success;
}

/**
 * @brief Functionality to import a file
 *
 * @param _destinationPath
 * @param _originPath
 * @param _toolFolder
 * @return QString
 */
QString UtilFiles::importFile(QString _destinationPath, QString _originPath, QString _toolFolder)
{
    QFileInfo _origPathInfo(_originPath);
    QString _destPath = QDir(_destinationPath).filePath(_toolFolder);
    QString _fileName = _origPathInfo.fileName();
    QString _destFilePath = QDir(_destPath).filePath(_fileName);

    QFile _destConfigFile(_destFilePath);
     if( (_destConfigFile.exists()) && (QString::compare(_destFilePath,_originPath) != StrEqual))
        _destConfigFile.remove();
    QFile::copy(_originPath,_destFilePath);

    return _destFilePath;
}
