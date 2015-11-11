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
#include "searchpairhelper.h"
#include "../Data/Solver/VO/solversectionvo.h"
#include "../Data/Solver/VO/solvervariablevo.h"

/**
 * @brief Constructor
 *
 */
SearchPairHelper::SearchPairHelper()
{
    _totalFoundElements = 0;

}
/********************************* Register *******************************************/
/**
 * @brief This function allows the register of the UI elements
 *
 * @param _newTree
 * @param _lineEdit
 * @param _label
 * @param _newTable
 */
void SearchPairHelper::registerUIElements(QTreeView * _newTree, QLineEdit * _lineEdit, QLabel * _label,
                                          QTableWidget * _newTable)
{
    _tree= _newTree;
    _searchLineEdit = _lineEdit;
    _nFoundItemsLabel = _label;
    _table = _newTable;
}


/**
 * @brief This function registers the TreeView
 *
 * @param _newTree
 */
void SearchPairHelper::registerTreeView(QTreeView * _newTree)
{
    _tree = _newTree;
}
/********************************************* Getter/Setter ************************************/
/**
 * @brief Returns the total number of found elements
 *
 * @return int
 */
int SearchPairHelper::getTotalFoundElements()
{
    return _totalFoundElements;
}

/**
 * @brief Sets the total number of found elements
 *
 * @param _val
 */
void SearchPairHelper::setTotalFoundElements(int _val)
{
    _totalFoundElements = _val;
}

/**
 * @brief Sets the solver Manager
 *
 * @param _manager
 */
void SearchPairHelper::setSolverManager(SolverConfigManager * _manager)
{
    _solverManager = _manager;
}

/********************************************* Refresh/highlight ************************************/

/**
 * @brief This function allows to refresh the highligt according the search
 *
 */
void SearchPairHelper::refreshHighlight()
{
    if (!_searchedTermList.isEmpty())
    {
        for (int i = 0; i < _searchedTermList.count();i++)
        {
            SearchedPair _pair = _searchedTermList.at(i);

            hihghlightRowTree(_pair.getIdParent(),0,QModelIndex());
            expandParentChild(_pair.getIdParent());
            QModelIndex _idxParent = _tree->model()->index(_pair.getIdParent(),0,QModelIndex());
            hihghlightRowTree(_pair.getIdChild(),0,_idxParent);

        }
    }
}
/**
 * @brief This function is able to highlight those rows in the tree that corresponds with the search
 *
 * @param _row
 * @param _col
 * @param _modelIdx
 */

void SearchPairHelper::hihghlightRowTree(int _row,int _col,QModelIndex _modelIdx)
{
    QModelIndex _idx = _tree->model()->index(_row,_col,_modelIdx);
    _tree->selectionModel()->setCurrentIndex(_idx, QItemSelectionModel::Select| QItemSelectionModel::Rows);
}

/** This function resfresh the search in the tree
 * @brief
 *
 * @param index
 */
void SearchPairHelper::refreshSearchInTree(const QModelIndex &index)
{
    if (!_searchedTermList.isEmpty())
   {
       int _parentRow = index.parent().row();
       int _childRow = index.row();
       QList<int> _searchedTableRows = findParentChildinSearchList(_parentRow,_childRow);
       if (!_searchedTableRows.isEmpty())
       {
           for (int i = 0; i < _searchedTableRows.count();i++)
           {
               int _pos = _searchedTableRows.at(i);
               _table->selectRow(_pos);
               _table->setFocus();
           }

       }
       refreshHighlight();
   }
}
/********************************************* Expand ************************************/

/**
 * @brief This function allows to expand the parent children
 *
 * @param _idParent
 */

void SearchPairHelper::expandParentChild(int _idParent)
{
    QModelIndex _idx = _tree->model()->index(_idParent,0, QModelIndex());
    _tree->setExpanded(_idx,true);
}

/********************************************* Search ************************************/

/**
 * @brief This fucntion allows to search in the tree model by a specific search term
 *
 * @param _searchTerm
 */

void SearchPairHelper::searchTreeBySearchTerm(QString _searchTerm)
{
    if (!_searchedTermList.isEmpty())
        _searchedTermList.clear();
    int _totalParents = _tree->model()->rowCount(QModelIndex());

    for (int _idParent = 0; _idParent < _totalParents; _idParent++)
    {
        QModelIndex _idxParent = _tree->model()->index(_idParent,0,QModelIndex());
        // Getting the Child
        int _noChildren = _tree->model()->rowCount(_idxParent);
        for (int _idChild = 0; _idChild < _noChildren; _idChild++)
        {
            QModelIndex _idxChildren = _tree->model()->index(_idChild,0,_idxParent);
            QVariant _dataChild = _idxChildren.data(Qt::DisplayRole);
            QString _childName = _dataChild.toString();
            SolverSectionVO _sectionVO =  _solverManager->getVariableListofSubsections(_childName);
            QList<SolverVariableVO> _list = _sectionVO.getSubsectionVarList();
            QList<int> _resultChildren;
            for (int i = 0; i< _list.count();i++)
            {
                SolverVariableVO _solver = _list.at(i);
                QString _nameVariable = _solver.getVariableName().toLower();
                if (_nameVariable.contains(_searchTerm.toLower()))
                {
                    _resultChildren.append(i);
                    _totalFoundElements ++;
                }
            }

            if (!_resultChildren.isEmpty())
            {
                SearchedPair _pairFound;
                _pairFound.setIdParent(_idParent);
                _pairFound.setIdChild(_idChild);
                _pairFound.setChildrenList(_resultChildren);
                _searchedTermList.append(_pairFound);
            }

        }

        }
}


/**
 * @brief This function is able to find the children (variable elemnts) that correspond to a specific search
 *  of a child + parent (the unique element formed by the combination of both)
 *
 * @param _parent
 * @param _child
 * @return QList<int>
 */
QList<int> SearchPairHelper::findParentChildinSearchList(int _parent, int _child)
{
    QList<int> _result;
    for (int i = 0; i < _searchedTermList.count();i++)
    {
        SearchedPair _pair = _searchedTermList.at(i);
        if (_pair.contains(_parent, _child))
            _result = _pair.getChildrenList();
    }
    return _result;
}


/**
 * @brief This function searchs the items
 *
 */
void SearchPairHelper::searchItems()
{
    // Reseting the values just in case the search is not cleared
    if (!_searchedTermList.isEmpty())
        _searchedTermList.clear();
    if (_totalFoundElements > 0)
        _totalFoundElements = 0;
    _tree->clearSelection();
    _table->clearSelection();
    //resetTable(); // HACERLO antes de llamar a search Items

    QString _searchTerm = _searchLineEdit->text();
    if (!_searchTerm.isEmpty())
    {
        searchTreeBySearchTerm(_searchTerm);
        if (!_searchedTermList.isEmpty())
        {
            _tree->clearSelection();
            for (int i = 0; i < _searchedTermList.count();i++)
            {
                SearchedPair _pair = _searchedTermList.at(i);

                hihghlightRowTree(_pair.getIdParent(),0,QModelIndex());
                expandParentChild(_pair.getIdParent());
                QModelIndex _idxParent = _tree->model()->index(_pair.getIdParent(),0,QModelIndex());
                hihghlightRowTree(_pair.getIdChild(),0,_idxParent);
            }
        }
    }
   _nFoundItemsLabel->setText(QString::number(_totalFoundElements));
    _tree->setFocus();
}

/********************************************* Clear ************************************/


/**
 * @brief It clear the status to default
 *
 */
void SearchPairHelper::clearSearchStatus()
{
    _searchLineEdit->clear();
    _tree->clearSelection();
    _table->clearSelection();
   _searchedTermList.clear();
   _totalFoundElements = 0;
   _nFoundItemsLabel->setText("0");
}
