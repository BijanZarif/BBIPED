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
#include "helpmanager.h"
#include <QDir>
#include <QProcess>
#include <QStringList>
#include <QDesktopServices>
#include <QUrl>
#include <QCoreApplication>
/**
 * @brief
 *
 */
HelpManager::HelpManager()
{
}
/**
 * @brief This function opens the help URK
 *
 * @param _file
 * @param _folder
 */
void HelpManager::openHelp(QString _file, QString _folder)
{
    QString _currentPath = QCoreApplication::applicationDirPath();//QDir::currentPath();
    QString _docPath = QDir(QDir(QDir(_currentPath).filePath("Documentation")).filePath(_folder)).filePath(_file);
    QUrl  _url  = QUrl("file://"+_docPath);
    QDesktopServices::openUrl(_url);
}

/**
 * @brief This function opens the Qt Assistant with the correspondent help files
 *
 */
void HelpManager::openQtAssistant()
{
    QString _path = QDir(QCoreApplication::applicationDirPath()).filePath("Documentation");
               // QDir::currentPath()).filePath("Documentation");
    QProcess * _helpProcess = new QProcess;
    _helpProcess->setWorkingDirectory(_path);

    QStringList args;
    args << QLatin1String("-collectionFile") << QLatin1String("bbiped.qhc") << QLatin1String("-enableRemoteControl");
    _helpProcess->start(QLatin1String("assistant"), args);
    if (!_helpProcess->waitForStarted())
        return;
}
