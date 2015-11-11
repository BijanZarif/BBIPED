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
#include "toolshelper.h"

#include "Forms/ToolsDialog/salometemplatedialog.h"
#include "Forms/ToolsDialog/salomeoptimizationdialog.h"
#include "Utility/showmessagehelper.h"
#include <QCoreApplication>
#include <QFileInfo>


/**
 * @brief
 *
 */
ToolsHelper::ToolsHelper()
{
}

/********************** Automatic Salome Tools ********************/

/**
 * @brief
 *
 */
void ToolsHelper::launchSalomeWarning()
{
    ShowMessageHelper _showMessage;
     _showMessage.showWarningMessage("Problem with Salome Configuration",
                            "Salome is not properly configured, please go to Preferences and set up the path to the Salome binaries");
    return;
}


/**
 * @brief
 *
 * @param _textEdit
 * @param _projectData
 * @param _userPreferencesData
 */
void ToolsHelper::createAutomaticMesh(QTextEdit * _textEdit,ProjectData _projectData, UserPreferencesData _userPreferencesData)
{

    if (_userPreferencesData.getMeshViewerPath().isEmpty())
    {
        launchSalomeWarning();
        return;
    }
    SalomeTemplateDialog _salomeTemplateDialog;

    _salomeTemplateDialog.setMeshViewerPath(_userPreferencesData.getMeshViewerPath());
    _salomeTemplateDialog.setBBIPEDPath(QCoreApplication::applicationDirPath());
    _salomeTemplateDialog.registerConsole(_textEdit);
    _salomeTemplateDialog.setProjectPath(_projectData.getProjectPath());
    _salomeTemplateDialog.setProjectName(_projectData.getProjectFileName());
    _salomeTemplateDialog.initializeDialog();
    _salomeTemplateDialog.exec();
}


/**
 * @brief
 *
 * @param _projectData
 * @param _userPreferencesData
 */
void ToolsHelper::createAutomaticMeshWithOptimization(ProjectData _projectData, UserPreferencesData _userPreferencesData)
{

    if (_userPreferencesData.getMeshViewerPath().isEmpty())
    {
        launchSalomeWarning();
        return;
    }
    SalomeOptimizationDialog _salomeOptDialog;
    _salomeOptDialog.setMeshViewerPath(_userPreferencesData.getMeshViewerPath());
    _salomeOptDialog.setProjectPath(_projectData.getProjectPath());
    QFileInfo _fileInfo(_projectData.getProjectFileName());
    _salomeOptDialog.setProjectName(_fileInfo.baseName());
    _salomeOptDialog.initializeDialog();
    _salomeOptDialog.exec();
}
