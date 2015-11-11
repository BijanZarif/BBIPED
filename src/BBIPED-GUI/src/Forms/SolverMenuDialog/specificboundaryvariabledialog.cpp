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
#include "specificboundaryvariabledialog.h"
#include "ui_specificboundaryvariabledialog.h"
#include "WindowHelper/Dialog/SolverDialog/solverconfigmenuhelper.h"

#include "Defines/Tool/Solver/SolverDefines.h"
#include "Defines/Tool/Solver/MarkerDefines.h"
#include "Defines/General/GeneralDefines.h"
#include <QComboBox>
#include <QLineEdit>
#include "Forms/helpdialog.h"

#include "Utility/qtableutil.h"

/**
 * @brief Constructor
 *
 * @param parent
 */
SpecificBoundaryVariableDialog::SpecificBoundaryVariableDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SpecificBoundaryVariableDialog)
{
    ui->setupUi(this);
    _isLoadingTable = false;
    _signalMapper = new QSignalMapper(this);
    _signalMapperTxtEdit = new QSignalMapper(this);
    _userAcceptChanges = false;
}

/**
 * @brief Destructor
 *
 */
SpecificBoundaryVariableDialog::~SpecificBoundaryVariableDialog()
{
    delete ui;

}


/*************************************** LOAD FUNCTIONS ********************************/

/**
 * @brief Function that load the values
 *
 */
void SpecificBoundaryVariableDialog::loadValues()
{
    ui->boundaryTypeText->setText(_boundType);
    ui->boundaryNameText->setText(_boundName);
    loadTableHeader();
    loadUserTableForConfiguration(_boundType);
    ui->boundaryTable->resizeColumnsToContents();

}

/**
 * @brief Function that update the boundary values with the user ones
 *
 */
void SpecificBoundaryVariableDialog::updateWithUserValues()
{

    int _row = 0;
        for (int i = 0; i < _cValues.size();i++)
        {
            QString _valueToSet = _cValues.at(i);

            QComboBox *  _combo = qobject_cast<QComboBox *> ( ui->boundaryTable->cellWidget(_row,ColumnSelVal));
            if (_combo)
            {
                int _index = _combo->findText(_valueToSet);
                if (_index != -1)
                {
                    _combo->setCurrentIndex(_index);
                    ui->boundaryTable->item(_row,ColumnCheck)->setCheckState(Qt::Checked);
                }
            }else
            {
                QLineEdit *  _line = qobject_cast<QLineEdit *> ( ui->boundaryTable->cellWidget(_row,ColumnSelVal));
                if ((_line) && (!_valueToSet.isNull()) && (!_valueToSet.isEmpty()) )
                {
                    _line->setText(_valueToSet);
                    ui->boundaryTable->item(_row,ColumnCheck)->setCheckState(Qt::Checked);
                }

            }
            _row++;
        }
}

/**
 * @brief Function that saves the changes made by the user
 *
 * @param QHash<QString
 * @param _userValues
 */
void SpecificBoundaryVariableDialog::setStoredUserConfigValues(QHash<QString, QString> _userValues)
{
    _previousBoundConfig = _userValues;
}


/**
 * @brief Function that loads the correspondent table
 *
 * @param _fileName
 */
void SpecificBoundaryVariableDialog::loadUserTableForConfiguration(QString _fileName)
{
    _isLoadingTable = true;
    SolverMarkerMenu _element = _markerManager->getMarkerNodeByName(_fileName);
    addRow(_element);
    updateWithUserValues();
    addSignals();
    _isLoadingTable = false;
}

/**
 * @brief Function that loads the table header
 *
 */
void SpecificBoundaryVariableDialog::loadTableHeader()
{
    ui->boundaryTable->setColumnCount(4);
    ui->boundaryTable->setHorizontalHeaderLabels( QString("Changed;Parameter Name;Parameter Value;Unit").split(";"));
    ui->boundaryTable->horizontalHeader()->setVisible(true);

}

/*************************************** SIGNAL FUNCTIONS ********************************/


/**
 * @brief Function that adds the signals
 *
 */
void SpecificBoundaryVariableDialog::addSignals()
{
    bool _foundCombos = false;
    bool _foundLineEdit = true;

    if (ui->boundaryTable->rowCount() > 0)
    {
        for (int i = 0; i< ui->boundaryTable->rowCount();i++)
        {
                QWidget * _item;
                _item = ui->boundaryTable->cellWidget(i,ColumnSelVal);

                if (_item)
                {
                    QComboBox *_comboItem = qobject_cast<QComboBox *> (_item);
                    if (_comboItem)
                    {
                        connect(_comboItem, SIGNAL(currentIndexChanged(int)), _signalMapper, SLOT(map()),Qt::UniqueConnection);
                       _signalMapper->setMapping(_comboItem, QString("%1-%2").arg(i).arg(ColumnSelVal));
                        _foundCombos = true;
                    }
                    else
                    {
                      QLineEdit * _line = qobject_cast<QLineEdit *> (_item);
                        if (_line)
                        {
                            connect(_line,SIGNAL(editingFinished()),_signalMapperTxtEdit,SLOT(map()),Qt::UniqueConnection);
                            _signalMapperTxtEdit->setMapping(_line,QString("%1-%2").arg(i).arg(ColumnSelVal));
                            _foundLineEdit = true;
                        }
                    }
                }

        }
    }
   if (_foundCombos)
        connect(_signalMapper, SIGNAL(mapped(const QString &)),this, SLOT(changedComboValueByUser(const QString &)),Qt::UniqueConnection);
   if (_foundLineEdit)
        connect(_signalMapperTxtEdit, SIGNAL(mapped(const QString &)),this, SLOT(changedLineTextSlot(const QString &)),Qt::UniqueConnection);


}

/*************************************** SLOTS FUNCTIONS ********************************/


/**
 * @brief SLot that controls the change of text
 *
 * @param position
 */
void SpecificBoundaryVariableDialog::changedLineTextSlot(QString position)
{
   if (!_isLoadingTable)
    {
       QStringList coordinates = position.split("-");
       int row = coordinates[0].toInt();
       ui->boundaryTable->item(row,ColumnCheck)->setCheckState(Qt::Checked);
    }
}

/**
 * @brief Slot that controls the combo value changed by the user
 *
 * @param position
 */
void SpecificBoundaryVariableDialog::changedComboValueByUser(QString position)
{
    if (!_isLoadingTable)
    {
        QStringList coordinates = position.split("-");
        int row = coordinates[0].toInt();
        ui->boundaryTable->item(row,ColumnCheck)->setCheckState(Qt::Checked);
    }
}

/**
 * @brief Function that provides the functionality when the user accpets the changes
 *
 */
void SpecificBoundaryVariableDialog::on_boundaryDialogGroupBox_accepted()
{
    for (int i = 0; i < ui->boundaryTable->rowCount(); i++)
    {
        QString _result = getValueFromTable(i);
        if (!_result.isEmpty())
            _configuredBoundary.append(_result);
    }
   _userAcceptChanges = true;

}
/*************************************** TABLE FUNCTIONS ********************************/

/**
 * @brief Function that adds a row to the table
 *
 * @param _element
 */
void SpecificBoundaryVariableDialog::addRow(SolverMarkerMenu _element)
{
    QtableUtil _tableHelper;
    QStringList _parameterList = _element.getParametersList();
    QStringList _typeList = _element.getTypeList();
    if (_parameterList.count() != _typeList.count())
        return;

    for (int i = 0; i < _parameterList.count();i++)
    {
        int _lastRow = ui->boundaryTable->rowCount();
        ui->boundaryTable->insertRow(_lastRow);
        ui->boundaryTable->setItem(_lastRow,ColumnCheck,_tableHelper.addChangedColumn() );
        ui->boundaryTable->setItem(_lastRow,ColumnVarName,
                                   _tableHelper.addNonEditableText(_parameterList.at(i)));
        // In this case all the options are selectable. Otherwise this needs to be modified
        QList<bool> _selectable;
        for (int i = 0; i < _meshList.size();i++)
            _selectable.append(true);
        //if (QString::compare(_typeList.at(i),XML_MARKER_TYPE_NUMBER_VAL_ATT) == StrEqual)
        //{
        // Adding Default Values
        QString _defaultValue = _element.getDefaultValueList().at(i);
          ui->boundaryTable->setCellWidget(_lastRow,ColumnSelVal,_tableHelper.addEditText(_defaultValue));
        //}
        //else
        //    ui->boundaryTable->setCellWidget(_lastRow,ColumnSelVal,_tableHelper.addCombo(_meshList,_selectable));



        // Adding the Unit Column
        QString _unitElement = _element.getUnitList().at(i);
        if( (_unitElement.isEmpty()) || (QString::compare(_unitElement,"NONE") == StrEqual) )
            _unitElement = "";

        ui->boundaryTable->setItem(_lastRow,ColumnMarkerUnitMeasure,_tableHelper.addNonEditableText(_unitElement));
    }
}



/**
 * @brief Function that sest the values of the boundary values
 *
 * @param QHash<QString
 * @param _list
 */
void SpecificBoundaryVariableDialog::setMeshVariables(QHash<QString,QString> _list)
{
    _meshVariables = _list;
    if (!_meshVariables.isEmpty())
    {
        QHashIterator<QString,QString> _iter(_meshVariables);
        while(_iter.hasNext())
        {
            _iter.next();
            QString _value = _iter.value();
            _meshList.append(_value);
        }

    }

}



/**
 * @brief Function that gets the values from the table
 *
 * @param _row
 * @return QString
 */
QString SpecificBoundaryVariableDialog::getValueFromTable(int _row)
{
    QComboBox * _combo = qobject_cast<QComboBox *> (ui->boundaryTable->cellWidget(_row,ColumnSelVal));
    if (_combo)
         return (_combo->currentText());
    else{
        QLineEdit * _line = qobject_cast<QLineEdit *>  (ui->boundaryTable->cellWidget(_row,ColumnSelVal));
        if (_line)
            return (_line->text());
    }
    return QString("");
}

/************************** GETTER/SETTER *****************************************/
/**
 * @brief Set the boundary type
 *
 * @param _newBoundType
 */
void SpecificBoundaryVariableDialog::setBoundaryType(QString _newBoundType)
{
    _boundType = _newBoundType;
}

/**
 * @brief Returns the boundary type
 *
 * @return QString
 */
QString SpecificBoundaryVariableDialog::getBoundaryType()
{
    return _boundType;

}

/**
 * @brief Set the boundary name
 *
 * @param _newBoundName
 */
void SpecificBoundaryVariableDialog::setBoundaryName(QString _newBoundName)
{
    _boundName = _newBoundName;
}

/**
 * @brief Returns the boundary name
 *
 * @return QString
 */
QString SpecificBoundaryVariableDialog::getBoundaryName()
{
    return _boundName;
}

/**
 * @brief Returns the configured Boundary list
 *
 * @return QStringList
 */
QStringList  SpecificBoundaryVariableDialog::getConfiguredBoundary()
{
    return _configuredBoundary;
}

/**
 * @brief set the marker menu manager
 *
 * @param _manager
 */
void SpecificBoundaryVariableDialog::setMarkerMenuManager(MarkerMenuManager * _manager)
{
    _markerManager = _manager;
}

/**
 * @brief Function that controls when the user press the cancel button
 *
 */
void SpecificBoundaryVariableDialog::on_boundaryDialogGroupBox_rejected()
{
    _userAcceptChanges = false;
}
/**
 * @brief Function that controls when the user press the accept button
 *
 * @return bool
 */
bool SpecificBoundaryVariableDialog::userAcceptChanges()
{
    return _userAcceptChanges;
}


/**
 * @brief Function that controls when the user press the help button
 *
 */
void SpecificBoundaryVariableDialog::on_boundaryHelpButton_clicked()
{
    HelpDialog *  _helpDialog = new HelpDialog();
     _helpDialog->setHelpText(_helpContent);
     _helpDialog->loadHelpText();
     _helpDialog->exec();
     delete _helpDialog;

}

/**
 * @brief Function to set the help content
 *
 * @param _help
 */
void  SpecificBoundaryVariableDialog::setHelpContent(QString _help)
{
    _helpContent = _help;
}

void SpecificBoundaryVariableDialog::setCurrentValues(QStringList _list)
{
    _cValues = _list;
}

QStringList SpecificBoundaryVariableDialog::getCurrentValues()
{
    return _cValues;
}
