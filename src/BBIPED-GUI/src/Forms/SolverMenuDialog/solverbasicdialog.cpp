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
 *  Author: Dec 2016: Carmen Alonso Montes calonso@bcamath.org
***/
#include "solverbasicdialog.h"
#include "ui_solverbasicdialog.h"

#include <QFileSystemModel>
#include <QVariant>
#include <QMessageBox>
#include <QLineEdit>
#include <QMessageBox>


#include "Utility/Model/DomModel/dommodel.h"
#include "WindowHelper/Dialog/SolverDialog/solverconfigmenuhelper.h"
#include "Data/solversectiondao.h"

// DEFINES
#include "Defines/General/GeneralDefines.h"
#include "Defines/Tool/Solver/SolverDefines.h"
#include "Utility/showmessagehelper.h"

// IO WRITING
#include <stdio.h>
#include <iostream>

#include "clipdomaindialog.h"

#include "Forms/VirtualRotatingDialog/virtualrotatingdialog.h"

#include "Utility/qtableutil.h"


using namespace std;

/**
 * @brief Constructor
 *
 * @param parent
 */
SolverBasicDialog::SolverBasicDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SolverBasicDialog)
{
    ui->setupUi(this);
    intializeVariables();
    loadTableHeader();
    initializeHelpers();


}



/**
 * @brief Destructor
 *
 */
SolverBasicDialog::~SolverBasicDialog()
{
    delete ui;
}



/************************************ INITIALIZE ******************************/

void SolverBasicDialog::initializeHelpers()
{
    _searchHelper.registerUIElements(ui->solverMenuTree,ui->searchVariableNameLabelLineEdit,
                                     ui->resultsLabelToUpdate,ui->solverVariablesTable);
    _searchHelper.setSolverManager(_solverManager);
}

/**
 * @brief Initialize all the main internal variables
 *
 */
void SolverBasicDialog::intializeVariables()
{
    _solverManager = new SolverConfigManager();

    _isLoadingTable = false;
    _signalMapper = new QSignalMapper(this);
    _signalMapperTxtEdit = new QSignalMapper(this);
    _signalMapperCheckList = new QSignalMapper(this);
    _signalMapperNonEditText = new QSignalMapper(this);
    _resetedToDefault = false;
    _userHasModifiedValues = false;
    _is3DMesh = false;

}

/**
 * @brief Function to reset the dialog variables
 *
 */
void SolverBasicDialog::resetDialog()
{
    _resetedToDefault = false;
    _userHasModifiedValues = false;
     _isLoadingTable = false; 
    _searchHelper.setTotalFoundElements(0);


}

/************************************ Load ************************************/
/**
 * @brief Load the corresponding Tree menu according with the user selection (basic view or
 *          advanced view)
 *
 */
void SolverBasicDialog::loadMenu()
{
    DomModel * _model;
    if (!_solverManager->isInitialized())
        _solverManager->initializeManager();
    if (_showBasicView)
       _model = new DomModel(*_solverManager->getBasicMenuList(), this);
    else
       _model = new DomModel(*_solverManager->getAdvancedMenuList(), this);

    // Setting and showing the model tree

    ui->solverMenuTree->setModel(_model);
    ui->solverMenuTree->hideColumn(1);
    ui->solverMenuTree->hideColumn(2);
    ui->solverMenuTree->show();
    _searchHelper.setSolverManager(_solverManager);

}


/**
 * @brief Loads the common table header values
 *
 */
void SolverBasicDialog::loadTableHeader()
{
    ui->solverVariablesTable->setColumnCount(5);
    ui->solverVariablesTable->setHorizontalHeaderLabels( QString("Changed;Variable;Value;Unit;Help").split(";"));
    ui->solverVariablesTable->horizontalHeader()->setVisible(true);

}
void SolverBasicDialog::loadTableHeaderSpecific(QStringList _labelList)
{
    ui->solverVariablesTable->setColumnCount(5);
    ui->solverVariablesTable->setHorizontalHeaderLabels(_labelList);
    ui->solverVariablesTable->horizontalHeader()->setVisible(true);

}


/************************************ TREE ACTIONS *******************************/

/**
 * @brief Fills in the table according the selected value by the user
 *
 * @param index
 */
void SolverBasicDialog::on_solverMenuTree_clicked(const QModelIndex &index)
{
        QVariant _data = index.data(Qt::DisplayRole);
        _selectedSubMenu = _data.toString();
        _isLoadingTable = true;
        QString _parent = index.parent().data(Qt::DisplayRole).toString();

       resetTable();
       if (QString::compare(_selectedSubMenu,VIRTUAL_SUBSECTION_NAME) == StrEqual)
       {

           // Check if the mesh is already selected otherwise skip

            if ((!_configValues.contains("MESH_FILENAME")) || (_configValues.value("MESH_FILENAME").isEmpty()))
                QMessageBox::warning(this,QString("No mesh selected"),QString("You need to select a mesh prior to configure this section"));

            else
            {
               // Find the associated SolverVariableVO
               SolverSectionVO _sectionVO;
                _sectionVO=  _solverManager->getVariableListofSubsections(_selectedSubMenu);
                launchVirtualRotatingDialog(_sectionVO);
            }

       }else
       {           
           if (QString::compare(_selectedSubMenu,DOMAIN_CLIPPING_MENU_NAME) == StrEqual)
           {
               updateUserValuesSpecific();
               updateTableHeaderSpecific();

           }else
               fillTable(_parent,_selectedSubMenu);

           _isLoadingTable = false;
           ui->solverVariablesTable->resizeColumnsToContents();
           ui->solverVariablesTable->show();
           buildAllToolTips();

        // In case a search has been done
           _searchHelper.refreshSearchInTree(index);

       }
}



void SolverBasicDialog::launchVirtualRotatingDialog( SolverSectionVO _sectionVO)
{
    VirtualRotatingDialog _virtualDialog;
    _virtualDialog.setSelectedSubmenu(_selectedSubMenu);
    _virtualDialog.set2DMeshCase(!_is3DMesh);
    _virtualDialog.setSolverSection(_sectionVO);
    _virtualDialog.setUserDefinedVirtualMRFValues(_virtualUserValues);    
    _virtualDialog.initializeDialog();
    _virtualDialog.exec();

    if (_virtualDialog.hasUserAcceptModifiedValues())
    {
       _virtualUserValues = _virtualDialog.getUserDefinedVirtualMRFValues();
       _userHasModifiedValues = true;
    }

}


/**
 * @brief This fucntion fills the table widget according user selection
 *
 * @param _parentSelected
 * @param _childSelected
 */

void SolverBasicDialog::fillTable(QString  _parentSelected, QString _childSelected)
{
    SolverSectionVO _sectionVO;
    SolverConfigMenuHelper _uiHelper;
    bool _isPersonalizedViewSelected = ui->customizedViewRadioButton->isChecked();


    if (QString::compare(_parentSelected,"") != StrEqual)
    {
        if (_isPersonalizedViewSelected)
            _sectionVO =  _solverManager->getPersonalizedVariableListofSubsections(_childSelected);
        else
            _sectionVO =  _solverManager->getVariableListofSubsections(_childSelected);
        QList<SolverVariableVO> _list = _sectionVO.getSubsectionVarList();
        _uiHelper.fillMenuTable(_childSelected,_parentSelected, ui->solverVariablesTable,_list);

        updateUserValues(_sectionVO);
        addSignals();
    }

}

  void SolverBasicDialog::updateTableHeaderSpecific()
  {
        // Change Unit Column Name as Delete
        QStringList _newTableHeaders = QString("Changed;Variable;Value;Delete;Help").split(";");
        loadTableHeaderSpecific(_newTableHeaders);
        // Create first row with no-values and a delete icon
        int _lastRow = ui->solverVariablesTable->rowCount();

        // Add a delete icon in the rest of the table
        QtableUtil _tableHelper;
        for (int _row=0; _row < ui->solverVariablesTable->rowCount()-1;_row++)
            ui->solverVariablesTable->setCellWidget(_row, ColumnUnit,_tableHelper.addIcon(IconDelete));

        // Add extra row with no values to create a new clipping
        insertRowSpecificWithoutIcons(_lastRow);
  }

 void SolverBasicDialog::insertRowSpecific(int _row)
 {
     QtableUtil _tableHelper;
     ui->solverVariablesTable->insertRow(_row);
     ui->solverVariablesTable->setItem(_row,ColumnCheck,_tableHelper.addChangedColumn());
     ui->solverVariablesTable->setItem(_row,ColumnVarName,_tableHelper.addNonEditableText("Click here to add a new Box"));
     ui->solverVariablesTable->setItem(_row,ColumnSelVal,_tableHelper.addNonEditableText(""));
     ui->solverVariablesTable->setCellWidget(_row,ColumnUnit,_tableHelper.addIcon(IconDelete));
     ui->solverVariablesTable->setCellWidget(_row,ColumnHelp,_tableHelper.addIcon(IconHelp));

 }

 void SolverBasicDialog::insertRowSpecificWithoutIcons(int _row)
 {
     QtableUtil _tableHelper;
     ui->solverVariablesTable->insertRow(_row);
     ui->solverVariablesTable->setItem(_row,ColumnCheck,_tableHelper.addChangedColumn());
     ui->solverVariablesTable->setItem(_row,ColumnVarName,_tableHelper.addNonEditableText("Click here to add a new Box"));
     ui->solverVariablesTable->setItem(_row,ColumnSelVal,_tableHelper.addNonEditableText(""));
     ui->solverVariablesTable->setItem(_row,ColumnUnit,_tableHelper.addNonEditableText(""));
     ui->solverVariablesTable->setItem(_row,ColumnHelp,_tableHelper.addNonEditableText(""));
  }

/************************************ TABLE ACTIONS ******************************/
/**
 * @brief This function help to manage the behaviour when clicking on the help column to launch the help dialog
 *
 * @param row
 * @param column
 */

void SolverBasicDialog::on_solverVariablesTable_cellClicked(int row, int column)
{

    if (column == ColumnHelp)
    {
        // In the last row of the Domain clipping, there is no help Icon, so no help shall be displayed
        if  ((QString::compare(_selectedSubMenu,DOMAIN_CLIPPING_MENU_NAME) == StrEqual) && (row == ( ui->solverVariablesTable->rowCount()-1)))
            return;
        // Normal help
        _helpDialog = new HelpDialog(this);
        SolverConfigMenuHelper _uiHelper;
        SolverSectionVO _sectionVO =  _solverManager->getVariableListofSubsections(_selectedSubMenu);

        QString _nodeName = ui->solverVariablesTable->item(row,ColumnVarName)->text();
        SolverSectionDAO _sectionDao;
        SolverVariableVO _var =_sectionDao.findVariableByValueName(_sectionVO,_nodeName);
        bool _isSelectable = true;
        if (!_var.isEmpty())
            _isSelectable = _var.isVariableSelectable();
        if (!_isSelectable)
            return;
        QString _helpText = _uiHelper.getAssociatedHelpText(_nodeName,_sectionVO.getSubsectionVarList(),HTML_FORMAT_ON);
        // Launch the help dialog
        _helpDialog->setHelpText(_helpText);
        _helpDialog->loadHelpText();
        _helpDialog->exec();
        ui->solverVariablesTable->clearSelection();

    }else
        if (QString::compare(_selectedSubMenu,DOMAIN_CLIPPING_MENU_NAME) == StrEqual)
            if (column == ColumnUnit)
            {
                // Delete the row
                int _lastRow = ui->solverVariablesTable->rowCount() -1; // The last row cannot be deleted
                if (row < _lastRow)
                {
                    QString _cValues = _configValues.value(DOMAIN_CLIPPING_VARIABLE_FILENAME);
                    QStringList _valuesList =_cValues.split(";");
                    _valuesList.removeAt(row);
                    _configValues.insert(DOMAIN_CLIPPING_VARIABLE_FILENAME,_valuesList.join(";"));
                    ui->solverVariablesTable->removeRow(row);
                    _userHasModifiedValues = true;
                }
            }else
               launchSpecificWindow(QString::number(row));

}


/**
 * @brief Function to build all the tooltips
 *
 */


void SolverBasicDialog::buildAllToolTips()
{
    SolverConfigMenuHelper _uiHelper;

        SolverSectionVO _sectionVO =  _solverManager->getVariableListofSubsections(_selectedSubMenu);
        int _totalRows = ui->solverVariablesTable->rowCount();
        for (int _row = 0; _row < _totalRows ; _row++)
        {
            QString _nodeName = ui->solverVariablesTable->item(_row,ColumnVarName)->text();
            QString _helpText = _uiHelper.getAssociatedHelpText(_nodeName,_sectionVO.getSubsectionVarList(),HTML_FORMAT_OFF);
            ui->solverVariablesTable->item(_row,ColumnVarName)->setToolTip(_helpText);
            ui->solverVariablesTable->item(_row,ColumnSelVal)->setToolTip(_helpText);
        }

}

/************************************ CHECKBOX ACTIONS ****************************/
/**
 * @brief Slot to control when the text is changed
 *
 * @param _position
 */

void SolverBasicDialog::textChangedSlot(QString _position)
{
    if (!_isLoadingTable)
    {
       int _row = _position.toInt();
       QLineEdit * _lineEdit = (QLineEdit *) ui->solverVariablesTable->cellWidget(_row,ColumnSelVal);
       QString _value = _lineEdit->text();
       // Check if the value is different to the default value
       if (!checkIsDefaultValue(_value,_row))
       {
           ui->solverVariablesTable->item(_row,ColumnCheck)->setCheckState(Qt::Checked);
           saveUserChanges(_value,_row);
       }else
       {
           ui->solverVariablesTable->item(_row,ColumnCheck)->setCheckState(Qt::Unchecked);
           resetToDefaultValue(_row);
       }

    }


}


/**
 * @brief This function checks if the value stored in a row is a default value, returning a true, otherwise, returns false
 *
 * @param _cValue
 * @param _row
 * @return bool
 */

bool SolverBasicDialog::checkIsDefaultValue(QString _cValue, int _row)
{
    bool _result = true;
    SolverSectionDAO _dao;
    QString _cVariable = ui->solverVariablesTable->item(_row,ColumnVarName)->text();
    SolverSectionVO _sectionVO =  _solverManager->getVariableListofSubsections(_selectedSubMenu);
    QString _cVariableFileName = _dao.findValueNameByVariableName(_sectionVO,_cVariable);
    QList<SolverSectionVO> _list ;
    _list.append(_sectionVO);
    _result =_dao.isDefaultValueByFileName(_list,_cVariableFileName,_cValue);


    return _result;
}

/**
 * @brief Slot to control the change of the combo value by the user
 *
 * @param position
 */

void SolverBasicDialog::changedComboValueByUser(QString position)
{

    if (!_isLoadingTable)
    {
        QStringList coordinates = position.split("-");
        int row = coordinates[0].toInt();
        int col = coordinates[1].toInt();
        QComboBox* combo = (QComboBox*) ui->solverVariablesTable->cellWidget(row, col);
        ui->solverVariablesTable->item(row,ColumnCheck)->setCheckState(Qt::Checked);
        QString _value = combo->currentText();
        _value = getAssociatedFileNameValue(_value,row);
        saveUserChanges(_value,row);
    }


}


/**
 * @brief This function handles the selection of the checkitems to be saved
 *
 * @param position
 */

void SolverBasicDialog::handleItem(QString position)
{
    if (!_isLoadingTable)
    {
        int row = position.toInt();

         QListWidget * _checkList = qobject_cast<QListWidget *> (ui->solverVariablesTable->cellWidget(row,ColumnSelVal));
         if (!_checkList)
             return;
         // Recovering the previous value of the user
         QString _rowVariableName = ui->solverVariablesTable->item(row,ColumnVarName)->text();
         QStringList _valueList;
         for (int i = 0; i < _checkList->count();i++)
         {
             QListWidgetItem * _item = _checkList->item(i);
             if (_item->checkState() == Qt::Checked)
                 _valueList.append(_item->text());
         }
         QString _value = _valueList.join(",");
         ui->solverVariablesTable->item(row,ColumnCheck)->setCheckState(Qt::Checked);
         saveUserChanges(_value,row);
    }

}



void SolverBasicDialog::launchSpecificWindow(QString position)
{
    if (!_isLoadingTable)
    {
        QString _variableName;
        int row = position.toInt();
        bool _isNewVariable = false;
        if (row == ui->solverVariablesTable->rowCount() -1)
            _isNewVariable = true;
        if (QString::compare(_selectedSubMenu,DOMAIN_CLIPPING_MENU_NAME) == StrEqual)
            _variableName=DOMAIN_CLIPPING_VARIABLE_NAME;


        // Recovering the previous value of the user
        SolverSectionDAO _solverDAO;
        SolverSectionVO _sectionVO =  _solverManager->getVariableListofSubsections(_selectedSubMenu);
        QString _keyName = _solverDAO.findValueNameByVariableName(_sectionVO,_variableName);
        SolverVariableVO _variable = _solverDAO.findVariableByFileName(_sectionVO.getSubsectionVarList(),_keyName);
        QTableWidgetItem * _item = ui->solverVariablesTable->item(row,ColumnSelVal);
        if (!_item)
             return;
        QString _value = _item->text();
        if ((_value.isEmpty()) || (_value.isNull()))
                _value = _variable.getDefaultValue();

        // Launching the Dialog
        if (QString::compare(_selectedSubMenu,DOMAIN_CLIPPING_MENU_NAME) == StrEqual)
        {
            ClipDomainDialog _clipDialog;
            _clipDialog.setUserValues(_value);
            _clipDialog.setSolverVariable(_variable);
            _clipDialog.initializeDialog();
            _clipDialog.exec();
            _value = _clipDialog.getValues();
        }
        else
            return;


        // Update the cell
        QtableUtil _tableHelper;
        ui->solverVariablesTable->item(row,ColumnVarName)->setText(_variable.getVariableName());
        ui->solverVariablesTable->item(row,ColumnSelVal)->setText(_value);
        ui->solverVariablesTable->item(row,ColumnCheck)->setCheckState(Qt::Checked);
        ui->solverVariablesTable->setCellWidget(row,ColumnUnit,_tableHelper.addIcon(IconDelete));
        ui->solverVariablesTable->setCellWidget(row,ColumnHelp,_tableHelper.addIcon(IconHelp));


         // Add a new row
        if (_isNewVariable)
            insertRowSpecificWithoutIcons(ui->solverVariablesTable->rowCount());
         // Save the changes
        saveUserChangesSpecificWindow(_value,row);

    }
}

/**
 * @brief This function returns teh associated value name of a variable in the correspondent row
 *
 * @param _val
 * @param _itemRow
 * @return QString
 */


QString SolverBasicDialog::getAssociatedFileNameValue(QString _val, int _itemRow)
{
    QString _result;
    SolverSectionDAO _solverDAO;
    QString _variableName =  ui->solverVariablesTable->item(_itemRow,ColumnVarName)->text();
    SolverSectionVO _sectionVO =  _solverManager->getVariableListofSubsections(_selectedSubMenu);
    QString _keyName = _solverDAO.findValueNameByVariableName(_sectionVO,_variableName);
    SolverVariableVO _variable = _solverDAO.findVariableByFileName(_sectionVO.getSubsectionVarList(),_keyName);

    if (_variable.getComboValueNamesList().contains(_val))
    {
        QStringList _valList = _variable.getComboValuesList();
        int _pos = _variable.getComboValueNamesList().indexOf(_val);
        _result = _valList.at(_pos);
    }else
        _result = _val; // This is when the combo allows user entry values

   return _result;
}

/**
 * @brief Slot to control when the user check/uncheck the rotating frame menu element
 *
 * @param arg1
 */

/*void SolverBasicDialog::on_rotatingFrameCheckBox_stateChanged(int arg1)
{

    resetTable();
    int _newState = arg1;

    // If the new State is equeal to the previous one
    if( (_newState == Qt::Checked) )
    {
        bool _isChecked = ui->rotatingFrameCheckBox->isChecked();
        _solverManager->buildRotatingMenu(_showBasicView);
        loadMenu();
        _showRotatingFrameElements = true;

    }
    if ((_newState == Qt::Unchecked))
    {
        _solverManager->removeRotatingMenu(_showBasicView);
        loadMenu();
        _showRotatingFrameElements = false;
    }

 }
*/
/**
 * @brief Slot to control when the user check/uncheck the unsteady menu element
 *
 * @param arg1
 */

/*void SolverBasicDialog::on_unsteadySimCheckBox_stateChanged(int arg1)
{

    resetTable();
    int _newState = arg1;

    // If the new State is equeal to the previous one
    if( (_newState == Qt::Checked) )
    {
         _solverManager->buildUnsteadyMenu(_showBasicView);
        loadMenu();
       // _showUnsteadyElements = true;

    }
    if ((_newState == Qt::Unchecked))
    {
        _solverManager->removeUnsteadyMenu(_showBasicView);
        loadMenu();
       // _showUnsteadyElements = false;
    }

 }*/

/************************************ BUTTON ACTIONS ******************************/

/**
 * @brief Function that controls the functionality when the user click the summary button
 *
 */

void SolverBasicDialog::on_summarySolverButton_clicked()
{

    resetTable();
    _summaryDialog = new SummaryDialog(this);
    _summaryDialog->setValuesToShow(&_configValues);
    _summaryDialog->setSectionMenuList(_solverManager->getAllSectionMenuList());
    _summaryDialog->buildSummaryTable();
    _summaryDialog->exec();
    if (_summaryDialog->userHasMadeChanges())
    {
        _userHasModifiedValues = true;
        _summaryDialog->resetDialog();
    }

}


/**
 * @brief Function that controls the functionality when the user click the accept button
 *
 */

void SolverBasicDialog::on_solverGroupButton_accepted()
{
    close();
}

/**
 * @brief Function that controls the functionality when the user click the cancel button
 *
 */

void SolverBasicDialog::on_solverGroupButton_rejected()
{

    _configValues.clear();
    _resetedToDefault = false;
    _userHasModifiedValues = false;
    close();
}


/**
 * @brief Function that controls the functionality when the user click the reset to default button
 *
 */

void SolverBasicDialog::on_resetDefaultButton_clicked()
{
    ShowMessageHelper _showHelper;
    bool _doReset = _showHelper.showMessageReset();
   if (_doReset)
   {
        resetAllConfigurationValues();
        resetTable();
        if (!ui->customizedViewRadioButton->isChecked())
          loadMenu();

        _resetedToDefault = true;
        _userHasModifiedValues = true;
   }

}
/************************************ Save/UPDATE/Change **********************/

/**
 * @brief Function to save the user changes made in the session
 *
 * @param _value
 * @param _itemRow
 */

void SolverBasicDialog::saveUserChanges(QString _value,int _itemRow)
{
    SolverSectionDAO _solverDAO;
    SolverSectionVO _sectionVO;

    QString _variableName =  ui->solverVariablesTable->item(_itemRow,ColumnVarName)->text();
    QString _keyName;

    _sectionVO =  _solverManager->getVariableListofSubsections(_selectedSubMenu);
    _keyName = _solverDAO.findValueNameByVariableName(_sectionVO,_variableName);
    _configValues.insert(_keyName,_value);

    _userHasModifiedValues = true;
 }

void SolverBasicDialog::saveUserChangesSpecificWindow(QString _value,int _itemRow)
{
    if ((_value.isEmpty()) || (_value.isNull()))
        return;
    SolverSectionDAO _solverDAO;
    SolverSectionVO _sectionVO;

    QString _variableName =  ui->solverVariablesTable->item(_itemRow,ColumnVarName)->text();
    QString _keyName;

    _sectionVO =  _solverManager->getVariableListofSubsections(_selectedSubMenu);
    _keyName = _solverDAO.findValueNameByVariableName(_sectionVO,_variableName);

    if (QString::compare(_selectedSubMenu,DOMAIN_CLIPPING_MENU_NAME) == StrEqual)
    {
        QString _oldValue = _configValues.value(_keyName);
        QStringList _oldValueList;
        if ((!_oldValue.isNull()) && (!_oldValue.isEmpty()) )
            _oldValueList = _configValues.value(_keyName).split(";");
        if ( (!_oldValue.isEmpty()) && (_itemRow < _oldValueList.size()-1))
            _oldValueList.replace(_itemRow,_value);
        else
            _oldValueList.append(_value);
        _configValues.insert(_keyName,_oldValueList.join(";"));
        _userHasModifiedValues = true;
    }

}


void SolverBasicDialog::resetToDefaultValue(int _itemRow)
{
    SolverSectionDAO _solverDAO;
    SolverSectionVO _sectionVO;
    QString _keyName;

    _sectionVO =  _solverManager->getVariableListofSubsections(_selectedSubMenu);
    _keyName = _solverDAO.findValueNameByVariableName(_sectionVO,
                ui->solverVariablesTable->item(_itemRow,ColumnVarName)->text());
    _configValues.remove(_keyName);

    _userHasModifiedValues = true;
 }



/**
 * @brief Function to update with the user values
 *
 * @param _solverSecvo
 */

void SolverBasicDialog::updateUserValues(SolverSectionVO _solverSecvo)
{

    SolverConfigMenuHelper _uiHelper;
    _uiHelper.updateTableWithUserValues(_solverSecvo,ui->solverVariablesTable, _configValues);
}

void SolverBasicDialog::updateUserValuesSpecific()
{
    if (QString::compare(_selectedSubMenu,DOMAIN_CLIPPING_MENU_NAME) == StrEqual)
    {
        QString _aux = _configValues.value(DOMAIN_CLIPPING_VARIABLE_FILENAME);
        if (_aux.isEmpty())
            return;
        QStringList _userVal =_configValues.value(DOMAIN_CLIPPING_VARIABLE_FILENAME).split(";");
        if ((_userVal.isEmpty()) || (_userVal.size() == 0))
            insertRowSpecific(ui->solverVariablesTable->rowCount());
        else
        {
            int _noValues = _userVal.size();
            for (int _row=0; _row <_noValues;_row++)
            {
                insertRowSpecific(_row);
                ui->solverVariablesTable->item(_row,ColumnVarName)->setText(DOMAIN_CLIPPING_VARIABLE_NAME);
                ui->solverVariablesTable->item(_row,ColumnSelVal)->setText(_userVal.at(_row));
                ui->solverVariablesTable->item(_row,ColumnCheck)->setCheckState(Qt::Checked);
            }
        }

    }
}



/************************************ Reset ************************************/

/**
 * @brief Function to reset all
 *
 */

void SolverBasicDialog::resetAllConfigurationValues()
{
    _solverManager->initializeManager();
    _configValues.clear();
    _virtualUserValues.clear();

}
/**
 * @brief Function to reset the table
 *
 */

void SolverBasicDialog::resetTable()
{
    if (ui->solverVariablesTable->rowCount() > 0)
    {
        //  Delete all rows
        int _totalRows = ui->solverVariablesTable->rowCount();
        for (int i = _totalRows; i >= 0; i--)
            ui->solverVariablesTable->removeRow(i);
    }
    loadTableHeader();
    //_specificWindowIndexList.clear();
}
/************************************ GETTERS/SETTERs/Is/Check ***********************/


/**
 * @brief Function to return the map with the config values
 *
 * @return QHash<QString, QString>
 */
QHash<QString, QString> SolverBasicDialog::getConfigurationValues()
{
    return _configValues;
}
/**
 * @brief Function to return the map with the user input
 *
 * @return QHash<QString, QString>
 */
QHash<QString, QString> SolverBasicDialog::getUserInput()
{
    return _configValues;
}



/**
 * @brief Check if the dialog is reseted to default
 *
 * @return bool
 */
bool SolverBasicDialog::isReseted()
{
    return _resetedToDefault;
}

/**
 * @brief Function to set the new values
 *
 * @param QHash<QString,QString>
 * @param _values
 */
void SolverBasicDialog::setConfigurationValues(QHash<QString, QString> _values)
{
    _configValues = _values;
}

/**
 * @brief Enables the personalized view
 *
 */
void SolverBasicDialog::enablePersonalizedView()
{
    ui->customizedViewRadioButton->setEnabled(true);
}

/**
 * @brief Function to enable basic view
 *
 * @param _value
 */
void SolverBasicDialog::setBasicView(bool _value)
{
     _showBasicView = _value;
}


/**
 * @brief Functio to set the config manager
 *
 * @param _solverManagerData
 */
void SolverBasicDialog::setData(SolverConfigManager * _solverManagerData)
{
    this->_solverManager = _solverManagerData;
}



/************************************Others **********************************/

void SolverBasicDialog::addSignalsSpecificWindows(QList<int> _rowIndexList)
{
 /*   int _totalRows = ui->solverVariablesTable->rowCount();
    bool _foundSpecificWindowNeeds = false;

    for (int _cIndex=0;_cIndex<_rowIndexList.size();_cIndex++)
    {
        int _cRow = _rowIndexList.at(_cIndex);

        if (_cRow < _totalRows)
        {
            QLineEdit *_lineEdit = qobject_cast<QLineEdit *> (ui->solverVariablesTable->cellWidget(_cRow,ColumnSelVal));
            if (_lineEdit)
            {
                connect(_lineEdit,SIGNAL(textEdited(QString)),_signalMapperNonEditText,SLOT(map()),Qt::UniqueConnection);
                _signalMapperNonEditText->setMapping(_lineEdit,QString("%1").arg(_cRow));
                _foundSpecificWindowNeeds = true;
            }
        }
    }

    if (_foundSpecificWindowNeeds)
        connect(_signalMapperNonEditText, SIGNAL(mapped(const QString &)),this, SLOT(launchSpecificWindowSlot(const QString &)),Qt::UniqueConnection);*/

}


/**
 * @brief Function to add the signals
 *
 */
void SolverBasicDialog::addSignals()
{

    bool _foundCombos = false;
    bool _foundLineEdit = false;
    bool _foundCheckList = false;

    if (ui->solverVariablesTable->rowCount() > 0)
    {
        for (int i = 0; i< ui->solverVariablesTable->rowCount();i++)
        {
                QWidget * _item;
                _item = ui->solverVariablesTable->cellWidget(i,ColumnSelVal);

                if (_item)
                {
                    QComboBox *_comboItem = qobject_cast<QComboBox *> (_item);
                    if (_comboItem)
                    {
                        if (_comboItem->isEditable())
                        {
                            connect(_comboItem,SIGNAL(editTextChanged(QString)),_signalMapper,SLOT(map()),Qt::UniqueConnection);
                            _signalMapper->setMapping(_comboItem,QString("%1-%2").arg(i).arg(ColumnSelVal));
                        }
                        connect(_comboItem, SIGNAL(currentIndexChanged(int)), _signalMapper, SLOT(map()),Qt::UniqueConnection);
                       _signalMapper->setMapping(_comboItem, QString("%1-%2").arg(i).arg(ColumnSelVal));

                        _foundCombos = true;
                    }else
                    {
                        QLineEdit * _lineEdit = qobject_cast<QLineEdit *> (_item);
                        if (_lineEdit)
                        {
                            connect(_lineEdit,SIGNAL(editingFinished()),_signalMapperTxtEdit,SLOT(map()),Qt::UniqueConnection);
                            _signalMapperTxtEdit->setMapping(_lineEdit,QString("%1").arg(i));
                            _foundLineEdit = true;
                        }
                        else
                        {
                            QListWidget * _checkList = qobject_cast<QListWidget *> (_item);
                            if (_checkList)
                            {
                                connect(_checkList,SIGNAL(itemClicked(QListWidgetItem*)),_signalMapperCheckList,SLOT(map()),Qt::UniqueConnection);
                               _signalMapperCheckList->setMapping(_checkList,QString("%1").arg(i));
                               _foundCheckList = true;
                            }
                        }
                    }
                }
        }
    }
    if (_foundCombos)
        connect(_signalMapper, SIGNAL(mapped(const QString &)),this, SLOT(changedComboValueByUser(const QString &)),Qt::UniqueConnection);
    if (_foundLineEdit)
        connect(_signalMapperTxtEdit, SIGNAL(mapped(const QString &)),this, SLOT(textChangedSlot(const QString &)),Qt::UniqueConnection);
    if (_foundCheckList)
        connect(_signalMapperCheckList, SIGNAL(mapped(const QString &)),this, SLOT(handleItem(const QString &)),Qt::UniqueConnection);
}






/**
 * @brief Function that returns if the user has made changes
 *
 * @return bool
 */
bool SolverBasicDialog::hasUserMadeChanges()
{
    return _userHasModifiedValues;
}

/**
 * @brief  This method provides the functionality of the search button
 *
 */
void SolverBasicDialog::on_searchButton_clicked()
{
    resetTable();
   _searchHelper.searchItems();
}



/**
 * @brief This function resets all the views to their status before the search
 *
 */
void SolverBasicDialog::on_clearSearchButton_clicked()
{
    resetTable();
    _searchHelper.clearSearchStatus();
}


/**
 * @brief Function that restore the elements for the BASIC VIEW
 *
 */
void SolverBasicDialog::on_basicViewRadioButton_clicked()
{
    resetTable();
    updateRadioButtonCheckStatus(true);
    _showBasicView = true;
    loadMenu();

}

/**
 * @brief Function that updates the radio button status
 *
 * @param _val
 */
void SolverBasicDialog::updateRadioButtonCheckStatus(bool _val)
{
    ui->basicViewRadioButton->setChecked(_val);
    ui->expertViewRadioButton->setChecked(!_val);
    ui->customizedViewRadioButton->setChecked(false);
}

/**
 * @brief Function that restore the elements for the PERSONALIZED VIEW
 *
 */
void SolverBasicDialog::updatePersonalizedMenu()
{
    DomModel * _model;
   _model = new DomModel(*_solverManager->getPersonalizedTemplateMenuList(), this);
    // Setting and showing the model tree

    ui->solverMenuTree->setModel(_model);
    ui->solverMenuTree->hideColumn(1);
    ui->solverMenuTree->hideColumn(2);
    ui->solverMenuTree->show();
    _searchHelper.setSolverManager(_solverManager);
}

/**
 * @brief Function that restore the elements for the ADVANCED VIEW
 *
 */
void SolverBasicDialog::on_expertViewRadioButton_clicked()
{
    resetTable();
    updateRadioButtonCheckStatus(false);
    _showBasicView = false;
    loadMenu();

}

/**
 * @brief Function that restore the elements for the CUSTOMIZED VIEW
 *
 */
void SolverBasicDialog::on_customizedViewRadioButton_clicked()
{
    resetTable();
    bool _val= true;
    ui->basicViewRadioButton->setChecked(!_val);
    ui->expertViewRadioButton->setChecked(!_val);
    ui->customizedViewRadioButton->setChecked(_val);
    updatePersonalizedMenu();
}

/**
 * @brief This function controls the return to search function
 *
 */
void SolverBasicDialog::on_searchVariableNameLabelLineEdit_returnPressed()
{
    ui->searchButton->setFocus();
    resetTable();
    _searchHelper.searchItems();
}

/**
 * @brief This function controls the press event,
 *
 * @param evt
 */
void SolverBasicDialog::keyPressEvent(QKeyEvent *evt)
{
    if(evt->key() == Qt::Key_Enter || evt->key() == Qt::Key_Return)
        return;
    QDialog::keyPressEvent(evt);
}

void SolverBasicDialog::setVirtualMRFData(QList<VirtualMrfVO> _list)
{
    _virtualUserValues =_list;
}

QList<VirtualMrfVO> SolverBasicDialog::getVirtualMRFData()
{
    return _virtualUserValues;
}
void SolverBasicDialog::set3DMeshType(bool _val)
{
    _is3DMesh = _val;
}
