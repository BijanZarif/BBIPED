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
#include "projectmenutreehelper.h"

#include <QString>
#include <QFile>
#include <QFileSystemModel>

#include "Defines/General/GeneralDefines.h"
#include "Forms/EditMenuDialog/projectsettingsdialog.h"


/**
 * @brief
 *
 */
ProjectMenuTreeHelper::ProjectMenuTreeHelper()
{
}
void ProjectMenuTreeHelper::registerMenuTree(QTreeView * _tree)
{
    _registeredTree = _tree;
}

/**
 * @brief Function that controls the opening of a text file into a new tab element from user double click
 *
 * @param _file
 * @param _textEditor
 * @param _tabWidget
 */
void ProjectMenuTreeHelper::openFileInTabFromClickedElement(QFileInfo _file, QTextEdit * _textEditor,
                                                            QTabWidget * _tabWidget)
{
    // If it is a file, we will open it in a new window or in the correspondent tab
    if((!_file.isDir()) && (_file.isFile()))
    {
        if (QString::compare(_file.suffix(),SOLVER_CONFIG_FILE_EXT) == StrEqual)
            {
                QFile _fileToRead(_file.absoluteFilePath());

               if(_fileToRead.open(_fileToRead.Text | _fileToRead.ReadOnly))
               {
                   QString text = _fileToRead.readAll();
                   _textEditor->setText(text);
               }
               else
                  _textEditor->setText("Error Opening File");

               _textEditor->setVisible(true);
               _textEditor->setReadOnly(true);
               _textEditor->setTextInteractionFlags(_textEditor  -> textInteractionFlags() | Qt::TextSelectableByKeyboard);
               QIcon _icon(":/new/toolbarIcons/images/page_white.png");

               // Check if it is a reload
               QString _fileName(_file.fileName());
               int _tabToReload = -1;
               int _totalTabs= _tabWidget->count();
               for (int i=3; ((i<_totalTabs) && (_tabToReload == -1)) ;i++)
               {
                    QString _tabTitle(_tabWidget->tabText(i));
                    if (QString::compare(_fileName,_tabTitle) == StrEqual)
                        _tabToReload = i;
               }
               if (_tabToReload != -1)
                   _tabWidget->removeTab(_tabToReload);

               _tabWidget->addTab(_textEditor, _icon,_file.fileName());
               _tabWidget->setTabsClosable(true);

            }

        }

}



/**
 * @briefClear or reset the project tree view
 *
 */
void ProjectMenuTreeHelper::clearProjectTreeView()
{
    if (!_registeredTree)
        return;
    _registeredTree->reset();
    _registeredTree->setEnabled(false);

}



/**
 * @brief Initialize and update the tree view
 *
 * @param _projectPath
 */
void ProjectMenuTreeHelper::fillProjectTreeView(QString _projectPath)
{
    if (!_registeredTree)
        return;
    QFileSystemModel *_model = new QFileSystemModel();
    _model->setRootPath(_projectPath);
    _registeredTree->setEnabled(true);
    _registeredTree->setModel(_model);
    _registeredTree->setAnimated(false);
    _registeredTree->setIndentation(20);
    _registeredTree->setSortingEnabled(true);
    _registeredTree->hideColumn(TreeColDate);
    _registeredTree->hideColumn(TreeColSize);
    _registeredTree->hideColumn(TreeColType);
    _registeredTree->setRootIndex(_model->index(_projectPath));
    _registeredTree->setWindowTitle(QObject::tr("Project View"));
    _registeredTree->show();

}
