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
#include "solverutil.h"
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <stdio.h>
#include <iostream>

#include "Defines/General/GeneralDefines.h"
#include "Utility/utilfiles.h"

/**
 * @brief Constructor
 *
 */
SolverUtil::SolverUtil()
{
}


/************************************** Import ************************************************/


/**
 * @brief Function to import an existent solver configuration file
 *
 * @param _filePath
 * @param _projectVO
 * @return SolverVO
 */
SolverVO SolverUtil::uiImportSolverConfigFile(QString _filePath,  ProjectData _projectVO)
{

    SolverVO _solverVO;
    UtilFiles _fileHelper;
    QString _destFilePath = _fileHelper.importFile(_filePath,_projectVO,TOOL4);

    _solverVO.setSolverConfigFilePath(_destFilePath);

    return _solverVO;
}

/**
 * @brief Function to import an existent mesh  file
 *
 * @param _meshFilePath
 * @param _projectVO
 * @return SolverVO
 */
SolverVO SolverUtil::uiImportSolverMeshFile(QString _meshFilePath, ProjectData _projectVO)
{

    UtilFiles _fileHelper;
    SolverVO _solverVO;
    bool _ok = _fileHelper.createLink(_meshFilePath,_projectVO,TOOL4);
    QString _destFilePath = QDir(QDir(_projectVO.getProjectPath()).filePath(TOOL4)).filePath(QFileInfo(_meshFilePath).fileName());
    _solverVO.setSolverFilePath(_destFilePath);
    return _solverVO;
}
SolverVO SolverUtil::uiImportSolverMeshInfoFile(QString _meshFilePath, ProjectData _projectVO)
{

    UtilFiles _fileHelper;
    SolverVO _solverVO;
    QString _destFilePath = _fileHelper.importFile(_meshFilePath,_projectVO,TOOL4);
    _solverVO.setSolverFilePath(_destFilePath);
    return _solverVO;
}
