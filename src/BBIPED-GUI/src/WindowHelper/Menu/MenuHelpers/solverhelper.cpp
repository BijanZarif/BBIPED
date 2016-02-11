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
#include "solverhelper.h"


#include <QCoreApplication>
#include <QDir>
#include <QString>
#include <QFileDialog>


#include "Forms/SolverMenuDialog/templatedialog.h"
#include "Forms/SolverMenuDialog/filecomparisondialog.h"
/**
 * @brief
 *
 */
SolverHelper::SolverHelper()
{
}

/*****************************************TEMPLATE CONFIGURATION********************************************/

/**
 * @brief
 *
 * @param _solverConfigManager
 */
void SolverHelper::createConfigurationTemplateByUser(SolverConfigManager * _solverConfigManager)
{
    TemplateDialog _templateDialog;
    _templateDialog.setSolverConfigManager(_solverConfigManager);
    _templateDialog.exec();
}

/**
 * @brief
 *
 * @param _mainWindow
 * @param _solverConfigManager
 */
void SolverHelper::editConfigurationTemplateByUser(QMainWindow * _mainWindow,SolverConfigManager * _solverConfigManager)
{

    QString _workingPath = QDir(QCoreApplication::applicationDirPath()).filePath(".templates");
    if (!QDir(_workingPath).exists())
        QDir(QCoreApplication::applicationDirPath()).mkdir(".templates");

    QString _binPath = QFileDialog::getOpenFileName(_mainWindow, "Select the configuration template to Edit",
                                                            _workingPath,
                                                            "Template Config File Type(*.xml)");

    if (_binPath.isEmpty())
        return;
    TemplateDialog _templateDialog;
    _solverConfigManager->loadPersonalizedTemplate(_binPath);
    _templateDialog.setSolverConfigManager(_solverConfigManager);
    _templateDialog.loadTemplate();
    _templateDialog.exec();
}


/**
 * @brief
 *
 * @param _mainWindow
 * @param _solverConfigManager
 * @param _userPreferencesData
 */
void SolverHelper::loadConfigurationTemplateByUser(QMainWindow * _mainWindow, SolverConfigManager * _solverConfigManager,
                                                   UserPreferencesData * _userPreferencesData)
{
    QString _workingPath = QDir(QCoreApplication::applicationDirPath()).filePath(".templates");
    if (!QDir(_workingPath).exists())
        QDir(QCoreApplication::applicationDirPath()).mkdir(".templates");

    QString _binPath = QFileDialog::getOpenFileName(_mainWindow, "Select the configuration template to Load",
                                                            _workingPath,
                                                            "Template Config File Type(*.xml)");

    if (_binPath.isEmpty())
        return;

    _userPreferencesData->setTemplateConfigPath(_binPath);
    _solverConfigManager->loadPersonalizedTemplate(_binPath);
}


/*****************************************COMPARISON FILES*********************************************/
/**
 * @brief This function launchs the dialog for the comparison dialog
 *
 */
/**
 * @brief
 *
 * @param _isMultizoneProject
 * @param _multizoneProjectData
 * @param _projectData
 * @param _solverManager
 * @param _projectDetailsData
 * @param _userVirtualMRFDataList
 */
void SolverHelper::launchFileComparisonDialog(bool _isMultizoneProject,MultizoneProjectData _multizoneProjectData, ProjectData _projectData,
                                              SolverConfigManager _solverManager, ProjectDetails _projectDetailsData,
                                              QList<VirtualMrfVO> _userVirtualMRFDataList)
{
    FileComparisonDialog _fileComparison;
    QStringList _listFiles;
    if (_isMultizoneProject)
        _listFiles = _multizoneProjectData.getListConfigFiles();
    else
     _listFiles.append(_projectData.getProjectFileName());
    _fileComparison.setFilesToCompare(_listFiles);
    _fileComparison.setSectionMenuList(_solverManager.getAllSectionMenuList());
    _fileComparison.setProjectDetails(_projectDetailsData);
    _fileComparison.setVirtualMRFData(_userVirtualMRFDataList);
    _fileComparison.exec();
}
