#ifndef PROJECTMENUTREEHELPER_H
#define PROJECTMENUTREEHELPER_H

#include <QFileInfo>
#include <QTextEdit>
#include <QTabWidget>
#include <QTreeView>

class ProjectMenuTreeHelper
{
public:
    ProjectMenuTreeHelper();
    void registerMenuTree(QTreeView * _tree);
    void openFileInTabFromClickedElement(QFileInfo _file, QTextEdit * _textEditor,
                                                                QTabWidget *  _tabWidget);
    void clearProjectTreeView();
    void fillProjectTreeView(QString _projectPath);
private:
    QTreeView * _registeredTree;

};

#endif // PROJECTMENUTREEHELPER_H
