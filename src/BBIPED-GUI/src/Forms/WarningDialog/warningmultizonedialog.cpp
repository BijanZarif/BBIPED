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
#include "warningmultizonedialog.h"
#include "ui_warningmultizonedialog.h"

#include <QHashIterator>
#include <QStringList>

#include "Utility/qtableutil.h"
#include "Defines/General/GeneralDefines.h"
#include "Data/solversectiondao.h"
#include "Data/Solver/VO/solvervariablevo.h"

#define REPAIR_VARIABLE_NAME_COLUMN 0
#define REPAIR_OLD_VALUE_COLUMN 1
#define REPAIR_NEW_VALUE_COLUMN 2

/**
 * @brief Constructor
 *
 * @param parent
 */
WarningMultizoneDialog::WarningMultizoneDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WarningMultizoneDialog)
{
    ui->setupUi(this);
    _needToUpdate = true;
    _keepValues = false;
    _userAcceptChanges = false;
    _isLoadingTable = false;
    _repairTableFilled = false;
    _signalMapper = new QSignalMapper(this);
    SolverConfigManager _solverManager;
    _solverManager.initializeManager();
    _menuSectionList = _solverManager.getAllSectionMenuList();
}

/**
 * @brief Destructor
 *
 */
WarningMultizoneDialog::~WarningMultizoneDialog()
{
    delete ui;
}

/*************************************** Initialize **********************************/
/**
 * @brief Function to set the file names under analysis
 *
 * @param _configFile
 * @param _meshFile
 */
void WarningMultizoneDialog::initializeFileNames(QString _configFile, QString _meshFile)
{
    ui->configFileName->setText(_configFile);
    ui->meshFileName->setText(_meshFile);
    setEnableRepairSection(false);
}


/*************************************** RADIO BUTTON **********************************/

/**
 * @brief Function that controls when the user clicks the update radio button
 *
 */
void WarningMultizoneDialog::on_updateRadioButton_clicked()
{
    ui->updateRadioButton->setChecked(true);
    ui->keepRadioButton->setChecked(false);
    setEnableRepairSection(true);
    _isLoadingTable = true;
    if (!_repairTableFilled)
        fillRepairTable();
    _isLoadingTable = false;
    _needToUpdate = true;
    _keepValues = false;
}

/**
 * @brief Function that controsl when the user wants to keep the values
 *
 */
void WarningMultizoneDialog::on_keepRadioButton_clicked()
{
    ui->updateRadioButton->setChecked(false);
    ui->keepRadioButton->setChecked(true);
    _needToUpdate = false;
    _keepValues = true;
    setEnableRepairSection(false);
}

/******************************************** BUTTON BOX ***************************************/


/**
 * @brief Function that provides the actions when the user accepts the changes
 *
 */
void WarningMultizoneDialog::on_buttonBox_accepted()
{
    _userAcceptChanges = true;
    if (_needToUpdate)
        updateConfigurationValues();

    close();
}

/**
 * @brief Function that provides the actions when the user rejects the changes
 *
 */
void WarningMultizoneDialog::on_buttonBox_rejected()
{
    _userAcceptChanges = false;
    close();
}

/******************************** SLOT *******************************************************/

/**
 * @brief Slot that controls the change of values by the user in the combos
 *
 * @param _position
 */
void WarningMultizoneDialog::changedComboValueByUser(QString _position)
{

    if (!_isLoadingTable)
    {
        QStringList coordinates = _position.split("-");
        int row = coordinates[0].toInt();
        int col = coordinates[1].toInt();
        QComboBox* combo = (QComboBox*) ui->repairMeshValuesTable->cellWidget(row, col);
        QString _value = combo->currentText();
        QString _varName = ui->repairMeshValuesTable->item(row,REPAIR_VARIABLE_NAME_COLUMN)->text();

        SolverSectionDAO _dao;
        QString _keyName = _dao.findValueNameByVariableName(_menuSectionList,_varName);

        QString _oldValue = _configValues.value(_keyName);
        if (_oldValue.contains("("))
            _oldValue.remove("(");
        if (_oldValue.contains(")"))
            _oldValue.remove(")");
        QStringList _oldElements = _oldValue.split(COMMA_SEPARATOR);
        if (!_oldElements.isEmpty())
            _oldElements[0] = _value;
        else
            _oldElements.append(_value);
        QString _newValue = "("+_oldElements.join(COMMA_SEPARATOR) +")";

        _userRepairedValues.insert(_keyName,_newValue);
    }


}
/*************************************** ENABLE / DISABLE ***********************************/
/**
 * @brief Function that enables the sectioos for repairing the data
 *
 * @param _val
 */
void WarningMultizoneDialog::setEnableRepairSection(bool _val)
{
    ui->repairLabel->setEnabled(_val);
    ui->repairMeshValuesTable->setEnabled(_val);
}


/*************************************** Fill/Build/UPDATE **********************************************/


/**
 * @brief Function that fills the data in the table for repairing the values
 *
 */
void WarningMultizoneDialog::fillRepairTable()
{
    QHash<QString,QString> _oldMeshValues = getMarkerValues();
    QStringList _meshVariableName = getMeshVarNames();

    if (!_oldMeshValues.isEmpty())
    {
        QHashIterator<QString,QString> iter(_oldMeshValues);
        SolverSectionDAO _dao;
        while(iter.hasNext())
        {
            iter.next();
            SolverVariableVO _varVO = _dao.findVariableByFileName(_menuSectionList,iter.key());
            if ( (!_varVO.isEmpty()) && (!iter.value().contains("NONE")) )
               buildRow(_varVO.getVariableName(),iter.value(),_meshVariableName,_varVO.getVariableFileName());
        }
        connect(_signalMapper, SIGNAL(mapped(const QString &)),this, SLOT(changedComboValueByUser(const QString &)),Qt::UniqueConnection);
        _repairTableFilled = true;
    }
}


/**
 * @brief Function that builds the Rows with the correspondent values
 *
 * @param _markerFileName
 * @param _cValue
 * @param _meshVarNameList
 */
void WarningMultizoneDialog::buildRow(QString _markerFileName,QString _cValue,QStringList _meshVarNameList,QString _markerKeyName)
{
    QtableUtil _tableHelper;
    int _lastRow = ui->repairMeshValuesTable->rowCount();
    ui->repairMeshValuesTable->insertRow(_lastRow);
    ui->repairMeshValuesTable->setItem(_lastRow,REPAIR_VARIABLE_NAME_COLUMN,_tableHelper.addNonEditableText(_markerFileName));
    ui->repairMeshValuesTable->setItem(_lastRow,REPAIR_OLD_VALUE_COLUMN,_tableHelper.addNonEditableText(_cValue));
    QList<bool> _selectable;
    for (int i = 0; i< _meshVarNameList.size();i++)
        _selectable.append(true);
    QComboBox * _combo = _tableHelper.addCombo(_meshVarNameList,_selectable);
    ui->repairMeshValuesTable->setCellWidget(_lastRow,REPAIR_NEW_VALUE_COLUMN,_combo);
    addSignalToCombo(_combo,_lastRow,REPAIR_NEW_VALUE_COLUMN);

    _userRepairedValues.insert(_markerKeyName,"NONE");
}

/**
 * @brief Function that update the configuration values
 *
 */
void WarningMultizoneDialog::updateConfigurationValues()
{
    QHashIterator<QString,QString> _iter(_userRepairedValues);
    while(_iter.hasNext())
    {
        _iter.next();
        QString _keyName = _iter.key();
        QString _value = removeBrakets(_iter.value());

       _configValues.insert(_keyName,_value);
    }
   // _configValues.insert("MESH_FILENAME",ui->meshFileName->text());
}

/**
 * @brief This function builds a QStringList with all the values of the map
 *
 * @param QHash<QString
 * @param _map
 * @return QStringList
 */
QStringList WarningMultizoneDialog::buildList(QHash<QString,QString> _map)
{
    QStringList _list;
    if (!_map.isEmpty())
    {
        QHashIterator<QString,QString> _iter(_map);
        while(_iter.hasNext())
        {
            _iter.next();
            _list.append(_iter.value());
        }

    }
    return _list;

}



/**
 * @brief This function remove Brakets fo a text
 *
 * @param _text
 * @return QString
 */
QString WarningMultizoneDialog::removeBrakets(QString _text)
{
    if (_text.contains("("))
        _text.remove("(");
    if (_text.contains(")"))
        _text.remove(")");
    return _text;
}

/***************************************ADD SIGNALS **********************************************/

/**
 * @brief Function that adds the signal to the combos
 *
 * @param _combo
 * @param _idRow
 * @param _idCol
 */
void WarningMultizoneDialog::addSignalToCombo(QComboBox * _combo, int _idRow, int _idCol)
{
        connect(_combo, SIGNAL(currentIndexChanged(int)), _signalMapper, SLOT(map()),Qt::UniqueConnection);
       _signalMapper->setMapping(_combo, QString("%1-%2").arg(_idRow).arg(_idCol));
}




/******************************************** GETTER/SETTER ***************************************/
/**
 * @brief Function to set the configuration values
 *
 * @param QHash<QString
 * @param _map
 */
void WarningMultizoneDialog::setConfigValues(QHash<QString, QString> _map)
{
    _configValues = _map;
}

/**
 * @brief Function to set the mesh values
 *
 * @param QHash<QString
 * @param _map
 */
void WarningMultizoneDialog::setMeshValueNames(QHash<QString, QString> _map)
{
    _meshValueNames = _map;
}
/**
 * @brief Function to get the configuration values
 *
 * @return QHash<QString, QString>
 */
QHash<QString, QString> WarningMultizoneDialog::getConfigValues()
{
    return _configValues ;
}


/**
 * @brief Function that returns if it is needed to uptdate the values
 *
 * @return bool
 */
bool  WarningMultizoneDialog::isNeedToUpdate()
{
    return _needToUpdate;
}

/**
 * @brief Function that returns if it is needed to keep the values
 *
 * @return bool
 */
bool  WarningMultizoneDialog::keepValues()
{
    return _keepValues;
}

/**
 * @brief Function that returns if the user accept the changes
 *
 * @return bool
 */
bool  WarningMultizoneDialog::userAcceptChanges()
{
    return _userAcceptChanges;
}

/**
 * @brief Function that get the mesh var names
 *
 * @return QStringList
 */
QStringList WarningMultizoneDialog::getMeshVarNames()
{
    QStringList _list;
    QHashIterator<QString,QString> iter(_meshValueNames);
    _list.append("NONE");
    while(iter.hasNext())
    {
        iter.next();
        _list.append(iter.value());
    }

    return _list;
}




/**
 * @brief Function that get the mesh values
 *
 * @return QHash<QString, QString>
 */
QHash<QString,QString> WarningMultizoneDialog::getMarkerValues()
{
    QHash<QString,QString> _markerSet;

    QHashIterator<QString,QString> iter(_configValues);
    while(iter.hasNext())
    {
        iter.next();
        QString _key = iter.key();
        if( (_key.contains("MARKER_")) || (_key.contains("BC_")) )
           _markerSet.insert(_key,iter.value());
    }
    return _markerSet;
}
