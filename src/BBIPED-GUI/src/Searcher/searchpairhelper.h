#ifndef SEARCHPAIRHELPER_H
#define SEARCHPAIRHELPER_H

#include <QTreeView>
#include <QList>
#include <QLineEdit>
#include <QLabel>
#include <QTableWidget>
#include "searchedpair.h"
#include "../Solver/solverconfigmanager.h"
class SearchPairHelper
{
public:
    SearchPairHelper();
    /********************************* REGISTER ***********************************************************/
    void registerTreeView(QTreeView *_newTree);
    void registerUIElements(QTreeView * _newTree, QLineEdit * _lineEdit, QLabel * _label,
                            QTableWidget * _newTable);

    /********************************* Getter/Setter ***********************************************************/
    int getTotalFoundElements();
    void setTotalFoundElements(int _val);
    void setSolverManager(SolverConfigManager * _manager);

    /********************************* Refresh ***********************************************************/
    void refreshSearchInTree(const QModelIndex &index);
    void refreshHighlight();

    /********************************* Find/Search ***********************************************************/
    QList<int> findParentChildinSearchList(int _parent, int _child);
    void searchItems();

    /********************************* Clear ***********************************************************/
    void clearSearchStatus();

private:
    QTreeView * _tree;
    QLineEdit * _searchLineEdit;
    QLabel * _nFoundItemsLabel;
    QTableWidget * _table;
    SolverConfigManager * _solverManager;
    QList<SearchedPair>  _searchedTermList;
    int _totalFoundElements;

    void expandParentChild(int _idParent);
    void searchTreeBySearchTerm(QString _searchTerm);
    void hihghlightRowTree(int _row,int _col,QModelIndex _modelIdx);

};

#endif // SEARCHPAIRHELPER_H
