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
#include <QLineEdit>
#include "virtualcurvedialog.h"
#include "ui_virtualcurvedialog.h"

#include "Utility/qtableutil.h"

#include "Defines/General/GeneralDefines.h"
#include "Data/Solver/VO/solvervariablevo.h"
#include "Forms/helpdialog.h"

 const QString INNER_TABLE = "INNER_TABLE";
 const QString OUTER_TABLE = "OUTER_TABLE";
/**
 * @brief
 *
 * @param parent
 */
VirtualCurveDialog::VirtualCurveDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VirtualCurveDialog)
{
    ui->setupUi(this);
    _userAcceptChanges = false;
    _is3DCoordinates = false;
    _signalMapperTxtEdit = new QSignalMapper(this);
}

/**
 * @brief
 *
 */
VirtualCurveDialog::~VirtualCurveDialog()
{
    delete ui;
}


/**
 * @brief
 *
 */
void VirtualCurveDialog::initializeDialog()
{

    if (!_innerValues.isEmpty())
    {
        fillTable(ui->innerTable,_innerValues);
        if (!_is3DCoordinates)
          addSignals2D(Column_Y_Value,INNER_TABLE);
        // Updating the counters
        ui->innerCurveSpinBox->blockSignals(true);
        ui->innerCurveSpinBox->setValue(ui->innerTable->rowCount());
        ui->innerCurveSpinBox->blockSignals(false);        
    }

    if (!_outerValues.isEmpty())
    {
      fillTable(ui->outerTable,_outerValues);
      if (!_is3DCoordinates)
        addSignals2D(Column_Y_Value,OUTER_TABLE);
      // Updating the counters
      ui->outerCurvespinBox->blockSignals(true);
      ui->outerCurvespinBox->setValue(ui->outerTable->rowCount());
      ui->outerCurvespinBox->blockSignals(false);
    }

    if (_is3DCoordinates)
    {
        ui->selectedDimension->blockSignals(true);
        ui->selectedDimension->setCurrentIndex(1);
        ui->selectedDimension->blockSignals(false);

    }else
    {
        ui->innerCurveSpinBox->setEnabled(false);
        ui->outerCurvespinBox->setEnabled(false);
    }

}

/*************************************************************************************************/

/**
 * @brief
 *
 * @param _table
 * @param _listVal
 */
void VirtualCurveDialog::fillTable(QTableWidget * _table, QStringList _listVal)
{

    int _dimension = 2;
    int _totalRows = 0;
    _totalRows = _listVal.size()/_dimension;

    for (int i = 0; i < _totalRows; i++)
    {
        int _cPos=i*_dimension;
        addRow(i,_table,_listVal.at(_cPos),_listVal.at(_cPos+1)); // TODO --- Hay que pasarle los valores X e Y
    }
}

void VirtualCurveDialog::addSignals2D(int _column, QString _tableName)
{
    bool _foundLineEdit = false;
    QTableWidget * _table;
    if (QString::compare(_tableName,INNER_TABLE) == StrEqual)
        _table = ui->innerTable;
    if (QString::compare(_tableName,OUTER_TABLE) == StrEqual)
        _table = ui->outerTable;
    if (_table->rowCount() == 0)
        return;

    int _initialRow = 0;
    QWidget * _item;
    _item = _table->cellWidget(_initialRow,_column);
    QLineEdit * _lineEdit = qobject_cast<QLineEdit *> (_item);
    if (_lineEdit)
    {
        connect(_lineEdit,SIGNAL(editingFinished()),_signalMapperTxtEdit,SLOT(map()),Qt::UniqueConnection);
        _signalMapperTxtEdit->setMapping(_lineEdit,_tableName);
        _foundLineEdit = true;
    }

    if (_foundLineEdit)
        connect(_signalMapperTxtEdit, SIGNAL(mapped(const QString &)),this, SLOT(textChangedSlot2D(const QString &)),Qt::UniqueConnection);

}

void VirtualCurveDialog::textChangedSlot2D(QString _tableName)
{
    QTableWidget * _table;
    if (QString::compare(_tableName,INNER_TABLE) == StrEqual)
        _table = ui->innerTable;
    if (QString::compare(_tableName,OUTER_TABLE) == StrEqual)
        _table = ui->outerTable;
    int _totalRows = _table->rowCount();
    if (_totalRows != 2)
        return;
    int _rowToCopy = 0;
    QLineEdit * _firstElement = (QLineEdit *) _table->cellWidget(_rowToCopy,Column_Y_Value);
    QString _value = _firstElement->text();
    // Copy the value to the second row
    _table->item(_rowToCopy+1,Column_Y_Value)->setText(_value);
}

/**
 * @brief
 *
 */
void VirtualCurveDialog::on_acceptButton_clicked()
{

    _userAcceptChanges = true;

    // Update the values of INNER curve
    updateUserValues(UpdateInnerCurveVal);

    // Update the values of OUTER curve
    updateUserValues(UpdateOuterCurveVal);

    close();
}

/**
 * @brief
 *
 * @param _table
 * @param _row
 * @param _col
 * @return QString
 */
QString VirtualCurveDialog::addItemToTable(QTableWidget * _table,int _row, int _col)
{
    QString _value("");
    QLineEdit * _item = qobject_cast<QLineEdit *> ( _table->cellWidget(_row,_col));
    if (_item)
        _value = _item->text();
    else
    {
        QTableWidgetItem * _item = _table->item(_row,_col);
        if (_item)
            _value = _item->text();
    }
    return _value;
}

/**
 * @brief
 *
 * @param _option
 */
void VirtualCurveDialog::updateUserValues(int _option)
{
    int _totalRows = 0;
    QTableWidget * _table;
    if (_option == UpdateInnerCurveVal)
    {
        _innerValues.clear();
        _totalRows = ui->innerTable->rowCount();
        _table = ui->innerTable;
    }else{
        if (_option == UpdateOuterCurveVal)
        {
            _outerValues.clear();
            _totalRows = ui->outerTable->rowCount();
            _table = ui->outerTable;
        }
    }

    for (int i = 0; i < _totalRows; i++)
    {
        QString _valueX = addItemToTable(_table,i,Column_X_Value);
        QString _valueY = addItemToTable(_table,i,Column_Y_Value);

        if ( (!_valueX.isNull()) && (!_valueX.isEmpty()) && (!_valueY.isNull()) && (!_valueY.isEmpty()))
        {
            if (_option == UpdateInnerCurveVal)
            {
                _innerValues.append(_valueX);
                _innerValues.append(_valueY);
            }else
                if (_option == UpdateOuterCurveVal)
                {
                    _outerValues.append(_valueX);
                    _outerValues.append(_valueY);
                }
        }
    }

}



/**
 * @brief
 *
 */
void VirtualCurveDialog::on_cancelButton_clicked()
{
    _userAcceptChanges = false;
    _innerValues.clear();
    _outerValues.clear();
    close();
}

/**
 * @brief
 *
 * @param arg1
 */
void VirtualCurveDialog::on_innerCurveSpinBox_valueChanged(int arg1)
{
    rowAddRemoveSpinBox(arg1,ui->innerTable);
}

void VirtualCurveDialog::rowAddRemoveSpinBox(int _noZones, QTableWidget * _table)
{
    int _totalRows = _table->rowCount();
    int _newZoneNo = _noZones;
    int _cZoneNo = _totalRows;
    if (_newZoneNo > _cZoneNo) // We need to add new rows
    {
        int _noNewRows = _newZoneNo - _cZoneNo;
        for (int i = 0;i < _noNewRows;i++)
            addRow(_cZoneNo+i,_table,"","");
    }
    else{
        if (_newZoneNo < _cZoneNo) // We need to delete rows
        {
            int _noDeletedRows = _cZoneNo -_newZoneNo;
            for (int i = 0; i <= _noDeletedRows;i++)
                deleteRow(_totalRows - i,_table);
        }
    }

}

/**
 * @brief
 *
 * @param arg1
 */
void VirtualCurveDialog::on_outerCurvespinBox_valueChanged(int arg1)
{
    rowAddRemoveSpinBox(arg1,ui->outerTable);
}
/**
 * @brief
 *
 * @param _rowPos
 * @param _table
 * @param _valueX
 * @param _valueY
 * @param _valueZ
 */
void VirtualCurveDialog::addRow(int _rowPos,QTableWidget * _table,QString _valueX, QString _valueY)
{
    QtableUtil _tableHelper;
    _table->blockSignals(true);
    _table->insertRow(_rowPos);

    if (!_is3DCoordinates)
        _table->setItem(_rowPos,Column_X_Value,_tableHelper.addNonEditableText(_valueX));
    else
        _table->setCellWidget(_rowPos,Column_X_Value,_tableHelper.addEditText(_valueX));
    if ((!_is3DCoordinates) && (_rowPos==1))
        _table->setItem(_rowPos,Column_Y_Value,_tableHelper.addNonEditableText(_valueY));
    else
        _table->setCellWidget(_rowPos,Column_Y_Value,_tableHelper.addEditText(_valueY));
    _table->blockSignals(false);

 /*   if (_is3DCoordinates)
        _table->setCellWidget(_rowPos,Column_Z_Value,_tableHelper.addEditText(_valueZ));
    else
        _table->setItem(_rowPos,Column_Z_Value,_tableHelper.addNonEditableText(""));
        */
}


/*void VirtualCurveDialog::enableThirdDimension(QTableWidget * _table)
{
    QtableUtil _tableHelper;
    for (int _row=0; _row < _table->rowCount();_row++)
        _table->setCellWidget(_row,Column_Z_Value,_tableHelper.addEditText(""));
}

void VirtualCurveDialog::disableThirdDimension(QTableWidget * _table)
{
    QtableUtil _tableHelper;
    for (int _row=0; _row < _table->rowCount();_row++)
    {
        _table->removeCellWidget(_row,Column_Z_Value);
        _table->setItem(_row,Column_Z_Value,_tableHelper.addNonEditableText(""));
    }
}
*/
 /*void VirtualCurveDialog::updateUserDataInTable()
 {

     if (_is3DCoordinates) // Before it was 2D
     {
         if (ui->innerTable->rowCount()>0)
            enableThirdDimension(ui->innerTable);
         if (ui->outerTable->rowCount()>0)
             enableThirdDimension(ui->outerTable);
     }else
     {
         if (ui->innerTable->rowCount()>0)
            disableThirdDimension(ui->innerTable);
         if (ui->outerTable->rowCount()>0)
            disableThirdDimension(ui->outerTable);
     }

 }*/

/**
 * @brief
 *
 * @param _rowPos
 * @param _table
 */
void VirtualCurveDialog::deleteRow(int _rowPos,QTableWidget * _table)
{
    _table->removeRow(_rowPos);
}

/**
 * @brief
 *
 * @param arg1
 */
void VirtualCurveDialog::on_selectedDimension_currentIndexChanged(const QString &arg1)
{
    if (QString::compare(arg1,"3D") == StrEqual)
        _is3DCoordinates = true;
    else
        _is3DCoordinates = false;
    //updateUserDataInTable();
}

/**
 * @brief
 *
 */
void VirtualCurveDialog::on_innerButtonHelp_clicked()
{
    generateHelpDialog(1);

}

/**
 * @brief
 *
 */
void VirtualCurveDialog::on_outerButtonHelp_clicked()
{
   generateHelpDialog(2);
}

/**
 * @brief
 *
 * @param _helpPos
 */
void VirtualCurveDialog::generateHelpDialog(int _helpPos)
{
    QList<SolverVariableVO> _infoVarVO = _solverSection.getSubsectionVarList();
    if (_infoVarVO.isEmpty())
        return;
    if (_helpPos >= _infoVarVO.size())
        return;
    SolverVariableVO _varGeneralInfo =  _infoVarVO.at(_helpPos);
    QString _helpText = _varGeneralInfo.getHelpContents(HTML_FORMAT_ON);
    HelpDialog *  _helpDialog = new HelpDialog();
     _helpDialog->setHelpText(_helpText);
     _helpDialog->loadHelpText();
     _helpDialog->exec();
}
/**
 * @brief
 *
 * @param _var
 */
void VirtualCurveDialog::setSolverSection(SolverSectionVO _var)
{
    _solverSection = _var;
}


/**
 * @brief
 *
 * @param _val
 */
void VirtualCurveDialog::set3DCoordinates(bool _val)
{
    _is3DCoordinates = _val;
}
/**
 * @brief
 *
 * @return bool
 */
bool VirtualCurveDialog::is3DCoordinates()
{
    return _is3DCoordinates;
}

/**
 * @brief
 *
 * @param _list
 */
void VirtualCurveDialog::setInnerValues(QStringList _list)
{
    _innerValues = _list;
}

/**
 * @brief
 *
 * @return QStringList
 */
QStringList VirtualCurveDialog::getInnerValues()
{
    return _innerValues;
}

/**
 * @brief
 *
 * @param _list
 */
void VirtualCurveDialog::setOuterValues(QStringList _list)
{
    _outerValues = _list;
}

/**
 * @brief
 *
 * @return QStringList
 */
QStringList VirtualCurveDialog::getOuterValues()
{
    return _outerValues;
}
/**
 * @brief
 *
 * @return bool
 */
bool  VirtualCurveDialog::hasUserModifiedValues()
{
    return _userAcceptChanges;
}





