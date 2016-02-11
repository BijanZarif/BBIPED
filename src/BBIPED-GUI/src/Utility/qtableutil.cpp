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
#include "qtableutil.h"
#include <stdio.h>
#include <iostream>
#include <QPixmap>
#include "Defines/Tool/Solver/SolverDefines.h"
#include "Defines/General/GeneralDefines.h"
#include <QStandardItem>
#include <QStandardItemModel>


using namespace std;

/**
 * @brief
 *
 */
QtableUtil::QtableUtil()
{
    _isInitialized = false;
}

void QtableUtil::registerTable(QTableWidget * _table)
{
   _registeredTable = _table;
   _isInitialized = true;
}

/*void QtableUtil::disableRows(QStringList _indexDisableList)
{
    for (int i= 0; i < 1;i++) //_indexDisableList.size();i++)
    {
        QStringList _pairRowCol = _indexDisableList.at(i).split(",");
        if ( (! _pairRowCol.isEmpty()) && (_pairRowCol.size()==2))
        {
            int _row = _pairRowCol.at(0).toInt();
            int _col = _pairRowCol.at(1).toInt();

            //_registeredTable->item(0,2)->setFlags(Qt::NoItemFlags|Qt::ItemIsSelectable|Qt::ItemIsEnabled);

            QTableWidgetItem *item = new QTableWidgetItem();
            item = _registeredTable->item(_row,_col);
            if (item)
            {
                item->setFlags(item->flags() &= ~Qt::ItemIsEditable);
                _registeredTable->setItem(_row,_col,item);
            }
        }
    }
}
*/
// This function shall define which rows are considered to be active or not
/*Qt::ItemFlags QtableUtil::flags ( const QModelIndex & index ) const
{

    if( index.row() ==  ActiveRow ) return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;

    else return Qt::ItemIsEnabled | Qt::ItemIsSelectable;

}*/

/**
 * @brief Function that adds new rows into an existent table
 *
 * @param _nRows
 */
void QtableUtil::addRows(int _nRows)
{
    for (int i = 0; i < _nRows; i++)
        _registeredTable->insertRow(i);
}

/**
 * @brief Funciton that add new columns to an existent table
 *
 * @param _nColumns
 */
void QtableUtil::addColumns(int _nColumns)
{
    for (int i = 0; i < _nColumns; i++)
        _registeredTable->insertColumn(i);
}

/******************************************** ADD ELEMENTS TO TABLE ***********************/
/**
 * @brief Function that creates a help icon widget cell element to be added to the table
 *
 * @return QTableWidgetItem
 */
QTableWidgetItem * QtableUtil::addIcon(bool _isSelectable)
{
    QIcon icon(":/solverIcons/images/help.png");
    QTableWidgetItem *icon_item = new QTableWidgetItem;
    icon_item->setIcon(icon);

    if (_isSelectable)
        icon_item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    else
    {
        icon_item->setFlags(icon_item->flags() &= ~Qt::ItemIsEditable);
        icon_item->setFlags(icon_item->flags() &= ~Qt::ItemIsEnabled);
    }

    return icon_item;
}


QTableWidgetItem * QtableUtil::addIcon()
{
    QIcon icon(":/solverIcons/images/help.png");
    QTableWidgetItem *icon_item = new QTableWidgetItem;
    icon_item->setIcon(icon);
    icon_item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    return icon_item;
}



/**
 * @brief Fucntion to add a icon according the _idICon(0:Help, 1:Edit, 2:Import, 3:Delete,4:Add)
 *
 * @param _idIcon
 * @return QLabel
 */
QLabel * QtableUtil::addIcon(int _idIcon)
{
    QPixmap icon;

    switch(_idIcon)
    {
    case IconHelp:
        icon.load(":/solverIcons/images/help.png");
        break;
    case IconEdit:
        icon.load(":/solverIcons/images/script_edit.png");
        break;
    case IconImport:
         icon.load(":/new/toolbarIcons/images/document_import.png");
        break;
    case IconDelete:
         icon.load(":/new/toolbarIcons/images/cross.png");
        break;
    case IconAdd:
         icon.load(":/new/toolbarIcons/images/add.png");
        break;
    }

    QLabel * _labelWithIcon = new QLabel;
    _labelWithIcon->setPixmap(icon);
    _labelWithIcon->setAlignment(Qt::AlignCenter);
    return _labelWithIcon;

}
/**
 * @brief Function that creates a script edit icon widget cell element to be added to the table
 *
 * @return QTableWidgetItem
 */
QTableWidgetItem * QtableUtil::addEditIcon()
{
    QIcon icon(":/solverIcons/images/script_edit.png");
    QTableWidgetItem *icon_item = new QTableWidgetItem;
    icon_item->setIcon(icon);
    icon_item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    return icon_item;
}

/**
 * @brief Function that creates check widget cell to the table
 *
 * @return QTableWidgetItem
 */
QTableWidgetItem * QtableUtil::addChangedColumn()
{
    QTableWidgetItem * _checkStatusItem = new QTableWidgetItem("");
    _checkStatusItem->setFlags(_checkStatusItem->flags() &= ~(Qt::ItemIsEditable | Qt::ItemIsUserCheckable));
    _checkStatusItem->setCheckState(Qt::Unchecked);
    return _checkStatusItem;
}

QTableWidgetItem * QtableUtil::addChangedColumn(bool _isSelectable)
{
    QTableWidgetItem * _checkStatusItem = new QTableWidgetItem("");
    if (!_isSelectable)
    {
        _checkStatusItem->setFlags(_checkStatusItem->flags() &= ~(Qt::ItemIsEditable | Qt::ItemIsUserCheckable| Qt::ItemIsEnabled));
        _checkStatusItem->setCheckState(Qt::Unchecked);
        return _checkStatusItem;
    }else
        return addChangedColumn();
}

QTableWidgetItem * QtableUtil::addSelectableCheckColumn()
{
    QTableWidgetItem * _checkStatusItem = new QTableWidgetItem("");
    //_checkStatusItem->setFlags(_checkStatusItem->flags() &= ~(Qt::ItemIsEditable | Qt::ItemIsUserCheckable));
    _checkStatusItem->setCheckState(Qt::Unchecked);
    return _checkStatusItem;
}

/**
 * @brief Function that creates variable widget cell to the table
 *
 * @param _variableName
 * @return QTableWidgetItem
 */
QTableWidgetItem * QtableUtil::addVariable(QString _variableName, bool _isSelectable)
{
    QTableWidgetItem * _varTextItem = new QTableWidgetItem();
    _varTextItem->setFlags(_varTextItem->flags() &= ~Qt::ItemIsEditable);
    if (!_isSelectable)
        _varTextItem->setFlags(_varTextItem->flags() &= ~Qt::ItemIsEnabled);
    _varTextItem->setText(_variableName);
    return _varTextItem;

}

QTableWidgetItem * QtableUtil::addVariable(QString _variableName)
{
    QTableWidgetItem * _varTextItem = new QTableWidgetItem();
    _varTextItem->setFlags(_varTextItem->flags() &= ~Qt::ItemIsEditable);
    _varTextItem->setText(_variableName);
    return _varTextItem;
}


/**
 * @brief Function that creates combo widget cell to the table
 *
 * @param _list
 * @return QComboBox
 */


QComboBox * QtableUtil::addCombo(QStringList _list, QList<bool> _selectableList, bool _isSelectable)
{
    QComboBox* _listCombo = new QComboBox();
    if (!_isSelectable)
    {
        _listCombo->setEditable(false);
        _listCombo->setEnabled(false);
        return _listCombo;
    }
    return( addCombo(_list,_selectableList ));
}

QComboBox * QtableUtil::addCombo(QStringList _list, QList<bool> _selectableList)
{
    QComboBox* _listCombo = new QComboBox();

    if (_list.contains(XML_CUSTOMISED_TAG))
    {
        _listCombo->setEditable(true);
        _listCombo->InsertAtBottom;
    }

    _listCombo->addItems(_list);


    // Changing the selectability
    // SOL: http://stackoverflow.com/questions/7632645/how-to-set-non-selectable-default-text-on-qcombobox
    // SOL color: http://stackoverflow.com/questions/3070450/qt-how-do-i-change-the-text-color-of-one-item-of-a-qcombobox-c
    QStandardItemModel* model =  qobject_cast<QStandardItemModel*>(_listCombo->model());

    for (int i=0; i < _selectableList.size(); i++)
    {
        bool _itemIsSelectable = _selectableList.at(i);
        if (!_itemIsSelectable)
        {
            QModelIndex firstIndex = model->index(i, _listCombo->modelColumn(), _listCombo->rootModelIndex());
            QStandardItem* _item = model->itemFromIndex(firstIndex);
            _item->setSelectable(false);
            _listCombo->setItemData(i, Qt::gray, Qt::TextColorRole);
        }
      }

    return _listCombo;
}


QListWidget * QtableUtil::addCheckBox(QStringList _list, QString _defaultValue, bool _isSelectable)
{
    QListWidget* _listCombo = new QListWidget();
    if (!_isSelectable)
    {
        _listCombo->setEnabled(false);
        return _listCombo;
    }
    return( addCheckBox(_list,_defaultValue));

}


QListWidget * QtableUtil::addCheckBox(QStringList _list, QString _defaultValue)
{
    QListWidget* _listCombo = new QListWidget();
    QStringList _defaultValueList;
    if (!_defaultValue.isEmpty())
        _defaultValueList=_defaultValue.split(COMMA_SEPARATOR);

    for (int i = 0; i < _list.size();i++)
    {
        QString _itemName = _list.at(i);
        QListWidgetItem * _itemToAdd = new QListWidgetItem(_itemName,_listCombo);
        _itemToAdd->setFlags(_itemToAdd->flags() | Qt::ItemIsUserCheckable); // set checkable flag
        if (_defaultValue.contains(_itemName))
            _itemToAdd->setCheckState(Qt::Checked); // AND initialize check state
        else
            _itemToAdd->setCheckState(Qt::Unchecked); // AND initialize check state

    }

    //_listCombo->addItems(_list);
    _listCombo->setSelectionMode(QAbstractItemView::MultiSelection );
    return _listCombo;
}


/**
 * @brief Function that creates edit default text cell to the table
 *
 * @param _value
 * @return QTableWidgetItem
 */
QTableWidgetItem * QtableUtil::addEditText2(QString _value)
{
     QTableWidgetItem * _defaultTxt = new QTableWidgetItem(_value);
     return _defaultTxt;
}

/**
 * @brief  Function that creates edit default text cell to the table
 *
 * @param _value
 * @return QLineEdit
 */
QLineEdit * QtableUtil::addEditText(QString _value)
{
     QLineEdit * _defaultTxt = new QLineEdit(_value);

     // Setting the validator
     bool isDouble;
     double _doubleVal = _value.toDouble(&isDouble);
     /*if (isDouble)
     {
         addDoubleValidator(_defaultTxt);
     }*/

     return _defaultTxt;
}
QLineEdit * QtableUtil::addEditText(QString _value, bool _isSelectable)
{
     QLineEdit * _defaultTxt = new QLineEdit(_value);

     if (!_isSelectable)
     {
         _defaultTxt->setEnabled(false);
         return _defaultTxt;

     }
     else
        return (addEditText(_value));
}

/**
 * @brief This function adds a validator to control the user input in double type fields
 *
 * @param _editTxt
 */
void QtableUtil::addDoubleValidator(QLineEdit * _editTxt)
{
    QDoubleValidator * _validator = new QDoubleValidator(_editTxt);
    _validator->setNotation(QDoubleValidator::ScientificNotation);
    _editTxt->setValidator(_validator);
}


/**
 * @brief Function that creates non edit default text cell to the table
 *
 * @param _value
 * @return QTableWidgetItem
 */
QTableWidgetItem * QtableUtil::addNonEditableText(QString _value)
{
    QTableWidgetItem * _defaultTxt = new QTableWidgetItem(_value);
    _defaultTxt->setFlags(_defaultTxt->flags() &= ~Qt::ItemIsEditable);
    return _defaultTxt;

}

QTableWidgetItem * QtableUtil::addNonEditableText(QString _value, bool _isSelectable)
{
    QTableWidgetItem * _defaultTxt = new QTableWidgetItem(_value);
    _defaultTxt->setFlags(_defaultTxt->flags() &= ~Qt::ItemIsEditable);
    if (!_isSelectable)
         _defaultTxt->setFlags(_defaultTxt->flags() &= ~Qt::ItemIsEnabled);
    return _defaultTxt;

}

void QtableUtil::resetTable()
{
    if (_registeredTable->rowCount() > 0)
    {
        //  Delete all rows
        int _totalRows = _registeredTable->rowCount();
        for (int i = _totalRows; i >= 0; i--)
            _registeredTable->removeRow(i);
    }
}

bool QtableUtil::isInitialized()
{
    return _isInitialized;
}


/**
 * @brief Function to deep copy the contents of a table to other
 *
 * @param _tableToFill
 * @param _tableToCopy
 */
/*
void QtableUtil::copyTo(QTableWidget *_tableToCopy)
{
    //_tableToCopy->setVisible(true);
    int _totalRows = _tableToCopy->rowCount();
    int _totalColumn = _tableToCopy->columnCount();

    // Checking the size of both tables
    if (_registeredTable->rowCount() < _totalRows)
        addRows( _totalRows - _registeredTable->rowCount());
    if (_registeredTable->columnCount() < _totalColumn)
        addColumns(_totalColumn - _registeredTable->columnCount());

    // We copy each row in the other table
    for (int i = 0; i < _totalRows ; i++)
    {
        // We copy each widget of the column into the other table
        for (int j = 0; j < _totalColumn ; j++)
        {
            if (j == ColumnCheck)
            {
                QTableWidgetItem * _item = getCopyOfCheckStatusItem(_tableToCopy->item(i,j));
                _registeredTable->setItem(i,j,_item);
            }
            if (j == ColumnVarName)
            {
                QTableWidgetItem * _item = getCopyOfVariableItem(_tableToCopy->item(i,j));
                _registeredTable->setItem(i,j,_item);
            }
            if (j == ColumnSelVal)
            {
                QTableWidgetItem * _item = _tableToCopy->item(i,j);

                if (_item != NULL)
                {
                    _registeredTable->setItem(i,j,getCopyOfVariableItem(_tableToCopy->item(i,j)));
                }
                else
                {
                    QComboBox * _item = getCopyOfComboItem( (QComboBox * )_tableToCopy->cellWidget(i,j));
                     _registeredTable->setCellWidget(i,j,_item);
                }

         }
          if (j == ColumnHelp)
            {
                QIcon icon(":/solverIcons/images/help.png");
                  QTableWidgetItem *icon_item = new QTableWidgetItem;
                  icon_item->setIcon(icon);
                  icon_item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
                  _registeredTable->setItem(i, j, icon_item);
            }
        }


    }


}
*/

/**
 * @brief Function to copy to the status of a item to a new item
 *
 * @param _toBeCopied
 * @return QTableWidgetItem
 */
/*QTableWidgetItem * QtableUtil::getCopyOfCheckStatusItem(QTableWidgetItem * _toBeCopied)
{
    QTableWidgetItem * _checkStatusItem = new QTableWidgetItem("");
    _checkStatusItem->data(Qt::CheckStateRole);
    _checkStatusItem->setCheckState(_toBeCopied->checkState());

    return _checkStatusItem;
}
*/
/**
 * @brief Function that deep copies the text of a item to a new one
 *
 * @param _toBeCopied
 * @return QTableWidgetItem
 */
/*QTableWidgetItem * QtableUtil::getCopyOfVariableItem(QTableWidgetItem * _toBeCopied)
{
    QTableWidgetItem * _varTextItem = new QTableWidgetItem();
    _varTextItem->setText(_toBeCopied->text());
    return _varTextItem;
}*/

/**
 * @brief Function that deep copies the contents of a combo in a new one
 *
 * @param _toBeCopied
 * @return QComboBox
 */
/*QComboBox* QtableUtil::getCopyOfComboItem(QComboBox * _toBeCopied)
{
  QComboBox* _listCombo = new QComboBox();
  QStringList _list ;
  for (int i = 0; i < _toBeCopied->count(); i++)
      _list << _toBeCopied->itemText(i);

  _listCombo->addItems(_list);


  return _listCombo;
}
*/
