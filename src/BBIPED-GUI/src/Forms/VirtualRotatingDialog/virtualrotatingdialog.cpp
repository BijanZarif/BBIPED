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
#include "virtualrotatingdialog.h"
#include "ui_virtualrotatingdialog.h"

#include "Utility/qtableutil.h"
#include "Defines/General/GeneralDefines.h"
#include "virtualcurvedialog.h"

#include "Forms/helpdialog.h"


/**
 * @brief
 *
 * @param parent
 */
VirtualRotatingDialog::VirtualRotatingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VirtualRotatingDialog)
{
    ui->setupUi(this);
    _selectedSubmenu = "";
    _deletedByUser  = false;
    _userAcceptModifiedValues = false;
    _is2DMesh = true; // By default, we consider that is a 2D Mesh case
}


/***********************************************************************************************/
/**
 * @brief
 *
 */
void VirtualRotatingDialog::initializeDialog()
{

    if (!_resultUserVirtualMRFList.isEmpty())
    {
        fillTable();
        ui->numVirtualZonesSpinBox->blockSignals(true);
        ui->numVirtualZonesSpinBox->setValue(_resultUserVirtualMRFList.count());
        ui->numVirtualZonesSpinBox->blockSignals(false);
    }

}


/**
 * @brief
 *
 */
void VirtualRotatingDialog::fillTable()
{
    int _numRows = _resultUserVirtualMRFList.count();
    for (int i = 0; i < _numRows; i++)
    {
        VirtualMrfVO _virtualVO = _resultUserVirtualMRFList.at(i);
        addRow(i,_virtualVO);
    }

}


/**
 * @brief
 *
 */
void VirtualRotatingDialog::on_acceptButton_clicked()
{
    _userAcceptModifiedValues = true;
    updateVirtualInformation();
    close();
}

/**
 * @brief
 *
 */
void VirtualRotatingDialog::on_cancelButton_clicked()
{
    _userAcceptModifiedValues = false;
    close();
}

/**
 * @brief
 *
 */
void VirtualRotatingDialog::updateVirtualInformation()
{
    int _totalRows = ui->zoneInfoTable->rowCount();
    if ( _totalRows== 0)
        return;
    for (int i = 0; i <_totalRows; i++ )
        createVirtualMRFData(i);

}

/**
 * @brief
 *
 * @param arg1
 */
void VirtualRotatingDialog::on_numVirtualZonesSpinBox_valueChanged(int arg1)
{
    int _totalRows = ui->zoneInfoTable->rowCount();
    int _newZoneNo = arg1;
    int _cZoneNo = _totalRows;
    if (_newZoneNo > _cZoneNo) // We need to add new rows
    {
        int _noNewRows = _newZoneNo - _cZoneNo;
        for (int i = 0;i < _noNewRows;i++)
            addRow(_cZoneNo+i);

    }
    else{
        if (_newZoneNo < _cZoneNo) // We need to delete rows
        {            
            int _noDeletedRows = _cZoneNo -_newZoneNo;
            ui->numVirtualZonesSpinBox->blockSignals(true);
            for (int i = 0; i <= _noDeletedRows;i++)
                deleteRow(_totalRows - i);

            ui->numVirtualZonesSpinBox->blockSignals(false);
        }
    }

}

/**
 * @brief
 *
 * @param _rowPos
 */
void VirtualRotatingDialog::addRow(int _rowPos)
{
    QtableUtil _tableHelper;
    ui->zoneInfoTable->insertRow(_rowPos);
    ui->zoneInfoTable->setItem(_rowPos,ColumnDone,_tableHelper.addChangedColumn());
    ui->zoneInfoTable->setCellWidget(_rowPos,ColumnZoneName,_tableHelper.addEditText("Name"));
    ui->zoneInfoTable->setCellWidget(_rowPos,ColumnOmega,_tableHelper.addEditText("0"));


    if (_is2DMesh)
    {    ui->zoneInfoTable->setCellWidget(_rowPos,ColumnAxisCenter,_tableHelper.addEditText("0,0"));
        ui->zoneInfoTable->setItem(_rowPos,ColumnAxisNormal,_tableHelper.addNonEditableText("0,0,1"));
    }
    else
    {
        ui->zoneInfoTable->setCellWidget(_rowPos,ColumnAxisCenter,_tableHelper.addEditText("0,0,0"));
        ui->zoneInfoTable->setCellWidget(_rowPos,ColumnAxisNormal,_tableHelper.addEditText("0,0,0"));
    }

    ui->zoneInfoTable->setCellWidget(_rowPos,ColumnConfigure,_tableHelper.addIcon(IconEdit));
    ui->zoneInfoTable->setCellWidget(_rowPos,ColumnDelete,_tableHelper.addIcon(IconDelete));
    ui->zoneInfoTable->setCellWidget(_rowPos,ColumnHelp,_tableHelper.addIcon(IconHelp));
}
/**
 * @brief
 *
 * @param _rowPos
 * @param _virtualVO
 */
void VirtualRotatingDialog::addRow(int _rowPos,VirtualMrfVO _virtualVO)
{
    QtableUtil _tableHelper;
    ui->zoneInfoTable->insertRow(_rowPos);
    ui->zoneInfoTable->setItem(_rowPos,ColumnDone,_tableHelper.addChangedColumn());
    ui->zoneInfoTable->setCellWidget(_rowPos,ColumnZoneName,_tableHelper.addEditText(_virtualVO.getZoneName()));
    ui->zoneInfoTable->setCellWidget(_rowPos,ColumnOmega,_tableHelper.addEditText(_virtualVO.getOmega()));
    if (_is2DMesh)
    {
        QStringList _centerValues = _virtualVO.getAxisCenter();
        if (_centerValues.size()>2)
            _centerValues.removeAt(2);

        ui->zoneInfoTable->setCellWidget(_rowPos,ColumnAxisCenter,_tableHelper.addEditText(_centerValues.join(",")));
        ui->zoneInfoTable->setItem(_rowPos,ColumnAxisNormal,_tableHelper.addNonEditableText("0,0,1"));
    }else
    {
        ui->zoneInfoTable->setCellWidget(_rowPos,ColumnAxisCenter,_tableHelper.addEditText(_virtualVO.getAxisCenter().join(",")));
        ui->zoneInfoTable->setCellWidget(_rowPos,ColumnAxisNormal,_tableHelper.addEditText(_virtualVO.getAxisNormal().join(",")));
    }

    ui->zoneInfoTable->setCellWidget(_rowPos,ColumnConfigure,_tableHelper.addIcon(IconEdit));
    ui->zoneInfoTable->setCellWidget(_rowPos,ColumnDelete,_tableHelper.addIcon(IconDelete));
    ui->zoneInfoTable->setCellWidget(_rowPos,ColumnHelp,_tableHelper.addIcon(IconHelp));
}

/**
 * @brief
 *
 * @param _rowPos
 */
void VirtualRotatingDialog::deleteRow(int _rowPos)
{
    // We need to delete the objects that corresponds
    if ((!_resultUserVirtualMRFList.isEmpty()) && (_rowPos < _resultUserVirtualMRFList.count()))
        _resultUserVirtualMRFList.removeAt(_rowPos);

    // Updating and removing the view
    ui->zoneInfoTable->removeRow(_rowPos);
    int _totalZones = ui->zoneInfoTable->rowCount();
    // Update the spinBox value accordingly
    ui->numVirtualZonesSpinBox->setValue(_totalZones);
}





/**
 * @brief
 *
 * @param row
 * @param column
 */
void VirtualRotatingDialog::on_zoneInfoTable_cellClicked(int row, int column)
{
    switch (column)
    {
    case ColumnConfigure:
        createVirtualMRFData(row); //We only create them at the moment of configuration
        launchCurveDialog(row);
        break;
    case ColumnDelete: // We need to remove the objects accordingly
        ui->numVirtualZonesSpinBox->blockSignals(true);
        deleteRow(row);
        ui->numVirtualZonesSpinBox->blockSignals(false);
        break;
    case ColumnHelp:
        buildAssociatedHelp(row);
        break;
    }
}


/**
 * @brief
 *
 * @param _row
 */
void VirtualRotatingDialog::createVirtualMRFData(int _row)
{
    int _totalRows = ui->zoneInfoTable->rowCount();
    int _totalObjects = _resultUserVirtualMRFList.count();
    bool _needToAdd = false;

    for (int i = 0; i < _totalRows; i++)
    {
        VirtualMrfVO _virtualInfo;
        if (i >= _totalObjects)
            _needToAdd = true;
        else
             _virtualInfo = _resultUserVirtualMRFList.at(i);

        if (i == _row)
        {
            _virtualInfo.setZoneName(((QLineEdit *) ui->zoneInfoTable->cellWidget(_row,ColumnZoneName))->text());
            _virtualInfo.setOmega(((QLineEdit *) ui->zoneInfoTable->cellWidget(_row,ColumnOmega))->text());

            QString _centerValue = ((QLineEdit *) ui->zoneInfoTable->cellWidget(_row,ColumnAxisCenter))->text();

            if (_is2DMesh)
            {
                QStringList _centerValueList = _centerValue.split(",");
                if (_centerValueList.size() > 2)
                    for (int i=_centerValueList.size(); i >= 2;i--)
                        _centerValueList.removeAt(i);
                _centerValueList.append("0");
                _centerValue = _centerValueList.join(",");
            }

            _virtualInfo.setAxisCenter(_centerValue.split(","));
            if (_is2DMesh)
                _virtualInfo.setAxisNormal(QString("0,0,1").split(","));
            else
                _virtualInfo.setAxisNormal(((QLineEdit *) ui->zoneInfoTable->cellWidget(_row,ColumnAxisNormal))->text().split(","));

        }
        if (_needToAdd)
            _resultUserVirtualMRFList.append(_virtualInfo);
        else
            _resultUserVirtualMRFList[i] = _virtualInfo;
    }

}


/**
 * @brief
 *
 * @param _row
 */
void VirtualRotatingDialog::buildAssociatedHelp(int _row)
{
    if (_selectedSubmenu.isEmpty())
        return;
    QList<SolverVariableVO> _infoVarVO = _solverSection.getSubsectionVarList();
    if (_infoVarVO.isEmpty())
        return;
    SolverVariableVO _variableVirtualZoneGeneralInfo =  _infoVarVO.at(0);
    QString _helpText = _variableVirtualZoneGeneralInfo.getHelpContents(HTML_FORMAT_ON);
    HelpDialog *  _helpDialog = new HelpDialog();
     _helpDialog->setHelpText(_helpText);
     _helpDialog->loadHelpText();
     _helpDialog->exec();
}


/**
 * @brief
 *
 * @param _row
 */
void VirtualRotatingDialog::launchCurveDialog(int _row)
{
    VirtualMrfVO _virtualVO;
    _virtualVO = _resultUserVirtualMRFList.at(_row);


    VirtualCurveDialog  _virtualCurveDialog;
    _virtualCurveDialog.setSolverSection(_solverSection);

    if (!_virtualVO.isEmpty())
    {
        QStringList _listInnerPoints = _virtualVO.getInnerCurvePoints();
        QStringList _listOuterPoints = _virtualVO.getOuterCurvePoints();
        QString _point1;
        QString _point2;

        if (_is2DMesh)
        {
             _point1 = "-1,0";
             _point2 = "1,0";
        }else
        {
            // By default in 3D, we need a minimum of 2 points
            _point1 = "0,0";
            _point2 = "0,0";
        }
        if (_listInnerPoints.size() == 0)
        {
            _listInnerPoints.append(_point1.split(","));
            _listInnerPoints.append(_point2.split(","));
        }
        if (_listOuterPoints.size() == 0)
        {
            _listOuterPoints.append(_point1.split(","));
            _listOuterPoints.append(_point2.split(","));
        }
        if ((_is2DMesh) && (_listInnerPoints.size() > 4 ))
            for (int i=_listInnerPoints.size();i>3;i--)
                _listInnerPoints.removeAt(i);

        if (_is2DMesh)
        { // we need to be sure that the points follow the same structre 1,x; -1,y
            _listInnerPoints.replace(0, QString("-1"));
            _listInnerPoints.replace(2, QString("1"));
        }


        if ((_is2DMesh) && (_listOuterPoints.size() > 4 ))
            for (int i=_listOuterPoints.size();i>3;i--)
                _listOuterPoints.removeAt(i);


        if (_is2DMesh)
        {// we need to be sure that the points follow the same structre 1,x; -1,y
            _listOuterPoints.replace(0, QString("-1"));
            _listOuterPoints.replace(2, QString("1"));
        }

        _virtualCurveDialog.setInnerValues(_listInnerPoints);
        _virtualCurveDialog.setOuterValues(_listOuterPoints);
        _virtualCurveDialog.set3DCoordinates(!_is2DMesh);
        _virtualCurveDialog.initializeDialog();
    }
    _virtualCurveDialog.exec();

    // If it is accepted the changes we can mark is as done
   if (_virtualCurveDialog.hasUserModifiedValues())
    {
        ui->zoneInfoTable->item(_row,ColumnDone)->setCheckState(Qt::Checked);
        // Save the results of the user
        _virtualVO.setInnerCurvePoints(_virtualCurveDialog.getInnerValues());
        _virtualVO.setOuterCurvePoints(_virtualCurveDialog.getOuterValues());
        _virtualVO.setCoordinatesIn3D(_virtualCurveDialog.is3DCoordinates());
        _resultUserVirtualMRFList.replace(_row,_virtualVO);

    }


}


void VirtualRotatingDialog::set2DMeshCase(bool _val)
{
    _is2DMesh = _val;
}


/**
 * @brief
 *
 */
VirtualRotatingDialog::~VirtualRotatingDialog()
{
    delete ui;
}

/**
 * @brief
 *
 * @return bool
 */
bool VirtualRotatingDialog::hasUserAcceptModifiedValues()
{
    return _userAcceptModifiedValues;
}

/**
 * @brief
 *
 * @return QList<VirtualMrfVO>
 */
QList<VirtualMrfVO> VirtualRotatingDialog::getUserDefinedVirtualMRFValues()
{
    return _resultUserVirtualMRFList;
}

/**
 * @brief
 *
 * @param _element
 */
void VirtualRotatingDialog::setSolverSection(SolverSectionVO _element)
{
    _solverSection =_element;
}

/**
 * @brief
 *
 * @param _selMenu
 */
void VirtualRotatingDialog::setSelectedSubmenu(QString _selMenu)
{
    _selectedSubmenu = _selMenu;
}


/**
 * @brief
 *
 * @return QList<SolverVariableVO>
 */
QList<SolverVariableVO> VirtualRotatingDialog::getSolverVariableInfoList()
{
    return _solverVariableInfoList;
}

/**
 * @brief
 *
 * @param _list
 */
void VirtualRotatingDialog::setSolverVariableInfoList( QList<SolverVariableVO> _list)
{
    _solverVariableInfoList = _list;
}

/**
 * @brief
 *
 * @return QList<QString>
 */
QList<QString> VirtualRotatingDialog::getPreviousZoneInfo()
{
    return _previousZoneInfo;
}

/**
 * @brief
 *
 * @param _list
 */
void VirtualRotatingDialog::setPreviousZoneInfo(QList<QString> _list)
{
    _previousZoneInfo = _list;
}

/**
 * @brief
 *
 * @return QList<QString>
 */
QList<QString> VirtualRotatingDialog::getUserDefinedZoneInfo()
{
    return _userDefinedZoneInfo;
}

/**
 * @brief
 *
 * @param _list
 */
void VirtualRotatingDialog::setUserDefinedZoneInfo(QList<QString> _list)
{
    _userDefinedZoneInfo = _list;
}
/**
 * @brief
 *
 * @param _list
 */
void VirtualRotatingDialog::setUserDefinedVirtualMRFValues(QList<VirtualMrfVO> _list)
{
    _resultUserVirtualMRFList = _list;
}


