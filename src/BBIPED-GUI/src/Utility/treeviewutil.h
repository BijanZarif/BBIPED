#ifndef TREEVIEWUTIL_H
#define TREEVIEWUTIL_H
#include <QFileSystemModel>
#include <QTreeView>

class TreeViewUtil
{
public:
    TreeViewUtil();
    void fillTreeView(QFileSystemModel * _model, QTreeView *_tree);
};

#endif // TREEVIEWUTIL_H
