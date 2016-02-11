
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
#include "templatedialog.h"
#include "ui_templatedialog.h"

#include "Utility/Model/DomModel/dommodel.h"
#include "WindowHelper/Dialog/SolverDialog/solverconfigmenuhelper.h"
#include "Defines/General/GeneralDefines.h"
#include "Utility/qtableutil.h"
#include "Defines/Tool/Solver/SolverDefines.h"
#include "Forms/helpdialog.h"

#include <QHashIterator>
#include <QFileDialog>

#include "Templates/templatemanager.h"

/**
 * @brief Constructor
 *
 * @param parent
 */
TemplateDialog::TemplateDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TemplateDialog)
{
    ui->setupUi(this);
    initializeTableHeaders();
    initializeVariables();
    initializeHelpers();
}

/**
 * @brief Destructor
 *
 */
TemplateDialog::~TemplateDialog()
{
    delete ui;
}
/****************************************** LOAD ********************************************/
/**
 * @brief Load all teh values of the template
 *
 */
void TemplateDialog::loadTemplate()
{
    QList<SolverSectionVO> _list = _solverManager->getAllPersonalizedSectionMenuList();
    for (int i = 0; i <_list.size(); i++)
        addVariable(_list.at(i));
    ui->selectedVariablesTableWidget->resizeColumnsToContents();
    _userHasMadeChanges = false;

}


/**
 * @brief Adds a variable to the table
 *
 * @param _section
 */
void TemplateDialog::addVariable(SolverSectionVO _section)
{
    QString _subsectionName = _section.getSubsectionName();
    QList<SolverVariableVO> _varList = _section.getSubsectionVarList();
    int _row = ui->selectedVariablesTableWidget->rowCount();
    for (int i = 0; i < _varList.size();i++)
    {
        SolverVariableVO _variableVO = _varList.at(i);
        QString _varName = _variableVO.getVariableName();
        _selectedVariables.insert(_varName,_subsectionName);
        addVariableToTable(_varName,_subsectionName);
        _row++;
    }
}

/**
 * @brief Function that sets the Solver Config manager to set the Menu List to be used
 *
 * @param _solverManagerData
 */
void TemplateDialog::setSolverConfigManager(SolverConfigManager * _solverManagerData)
{
    _solverManager = _solverManagerData;

    if (!_solverManager->isInitialized())
        _solverManager->initializeManager();

    setMenuList(_solverManager->getAllMenuList());
    _searcherHelper.setSolverManager(_solverManager);
}

/**
 * @brief This function sets the menu list
 *
 * @param _menu
 */
void TemplateDialog::setMenuList(QDomDocument * _menu)
{
    _variablesMenuToShow = _menu;
    loadMenu();
}


/**
 * @brief This function loads teh menu list
 *
 */
void TemplateDialog::loadMenu()
{
    DomModel * _model;
   _model = new DomModel((* _variablesMenuToShow), this);

    // Setting and showing the model tree
    ui->variablesToSelectTree->setModel(_model);
    ui->variablesToSelectTree->hideColumn(1);
    ui->variablesToSelectTree->hideColumn(2);
    ui->variablesToSelectTree->show();
}



/****************** INITIALIZE ************************************************************/
/**
 * @brief This function initialize the variables
 *
 */
void TemplateDialog::initializeVariables()
{
    _selectedMenu ="";
    _isLoadingTable = false;
    _userHasMadeChanges = false;
}

/**
 * @brief This function initialize the helpers
 *
 */
void TemplateDialog::initializeHelpers()
{
    _searcherHelper.registerUIElements(ui->variablesToSelectTree,ui->searchLineEdit,ui->resultFoundNoLabel,
                                       ui->variablesToSelectTableWidget);

}

/**
 * @brief This function initialize the table headers
 *
 */
void TemplateDialog::initializeTableHeaders()
{
    // Setting up the columns for the variables to be selected
    ui->variablesToSelectTableWidget->setColumnCount(ColNoVarToSelect);
    ui->variablesToSelectTableWidget->setHorizontalHeaderLabels(QString("Selected;Name of the variable;Help").split(";"));
    ui->variablesToSelectTableWidget->horizontalHeader()->setVisible(true);
    ui->variablesToSelectTableWidget->resizeColumnsToContents();
    ui->variablesToSelectTableWidget->show();

    // Setting up the columns for the selected variables
    ui->selectedVariablesTableWidget->setColumnCount(ColNoVarSelected);
    ui->selectedVariablesTableWidget->setHorizontalHeaderLabels(QString("Section;Name of the variable;Help;Remove").split(";"));
    ui->selectedVariablesTableWidget->horizontalHeader()->setVisible(true);
    ui->selectedVariablesTableWidget->resizeColumnsToContents();
    ui->selectedVariablesTableWidget->show();

}

/**
 * @brief This function controls when a menu item is clicked on the tree to show teh associated variables
 *
 * @param index
 */
void TemplateDialog::on_variablesToSelectTree_clicked(const QModelIndex &index)
{
       QVariant _data = index.data(Qt::DisplayRole);
       QString _parent = index.parent().data(Qt::DisplayRole).toString();
       _selectedMenu = _data.toString();

       // Filling the table
       _isLoadingTable = true;
       resetTable(ui->variablesToSelectTableWidget);
       fillTableVarToSelect(_parent,_selectedMenu);
       _isLoadingTable = false;
       ui->variablesToSelectTableWidget->resizeColumnsToContents();
       ui->variablesToSelectTableWidget->show();
       _searcherHelper.refreshSearchInTree(index);
}

/**
 * @brief This function loads the table for a specific menu item
 *
 * @param _parentSelected
 * @param _childSelected
 */
void TemplateDialog::fillTableVarToSelect(QString  _parentSelected, QString _childSelected)
{
    if (QString::compare(_parentSelected,"") != StrEqual)
    {
        SolverSectionVO _sectionVO;
        _sectionVO =  _solverManager->getVariableListofSubsections(_childSelected);
        fillMenuTable(_childSelected,_sectionVO.getSubsectionVarList());
        updateSelectedVariableCheckBox(_sectionVO);
    }

}

/**
 * @brief This funtion updates the values of a selected variable
 *
 * @param _sectionVO
 */
void TemplateDialog::updateSelectedVariableCheckBox(SolverSectionVO _sectionVO)
{
    if (!_selectedVariables.isEmpty())
    {
       QList<SolverVariableVO> _list = _sectionVO.getSubsectionVarList();
       for (int i = 0; i < _list.size();i++)
       {
           SolverVariableVO _variable = _list.at(i);
           QString _varName = _variable.getVariableName();
           if (_selectedVariables.contains(_varName))
               ui->variablesToSelectTableWidget->item(i,ColumnCheck)->setCheckState(Qt::Checked);
       }
    }
}

/**
 * @brief This function resets the table
 *
 * @param _table
 */
void TemplateDialog::resetTable(QTableWidget * _table)
{
    if (_table->rowCount() > 0)
    {
        //  Delete all rows
        int _totalRows = _table->rowCount();
        for (int i = _totalRows; i >= 0; i--)
            _table->removeRow(i);
    }
}

/**
 * @brief This function fills the menu table
 *
 * @param _parentName
 * @param _list
 */
void TemplateDialog::fillMenuTable(QString _parentName, QList<SolverVariableVO> _list)
{
    if ( (_parentName.isEmpty()) || (QString::compare(_parentName,"") == StrEqual) )
        return;

    for (int i= 0; i < _list.count();i++)
    {
        SolverVariableVO _vo = _list.at(i);
        addVariableToTable(_vo,i);
     }

}


/**
 * @brief Function that adds the variable elements to each row in the table
 *
 * @param _variable
 * @param _row
 */
void TemplateDialog::addVariableToTable(SolverVariableVO _variable, int _row )
{
    QtableUtil _tableHelper;
    ui->variablesToSelectTableWidget->insertRow(_row);
    ui->variablesToSelectTableWidget->setItem(_row,ColumnVarName,_tableHelper.addVariable(_variable.getVariableName()));
    ui->variablesToSelectTableWidget->setItem(_row, ColumnTemplateHelp, _tableHelper.addIcon());
    ui->variablesToSelectTableWidget->setItem(_row,ColumnCheck,_tableHelper.addSelectableCheckColumn());
}

/** This function launches the help menu
 * @brief
 *
 * @param row
 * @param column
 */
void TemplateDialog::on_variablesToSelectTableWidget_cellClicked(int row, int column)
{
    if(column == ColumnTemplateHelp)
    {
        SolverConfigMenuHelper _uiHelper;
        SolverSectionVO _sectionVO =  _solverManager->getVariableListofSubsections(_selectedMenu);
        QString _nodeName = ui->variablesToSelectTableWidget->item(row,ColumnVarName)->text();
        QString _helpText = _uiHelper.getAssociatedHelpText(_nodeName,_sectionVO.getSubsectionVarList(),HTML_FORMAT_ON);
        loadHelp(_helpText);
        ui->variablesToSelectTableWidget->clearSelection();
    }
}


/**
 * @brief This function loads the help dialog
 *
 * @param _helpText
 */
void TemplateDialog::loadHelp( QString _helpText)
{
    HelpDialog * _helpDialog = new HelpDialog(this);
    _helpDialog->setHelpText(_helpText);
    _helpDialog->loadHelpText();
    _helpDialog->exec();
}

/**
 * @brief Slot function that controls those variables that have been selected
 *
 * @param row
 * @param column
 */
void TemplateDialog::on_variablesToSelectTableWidget_cellChanged(int row, int column)
{
    if ((!_isLoadingTable) && (column == ColumnCheck) )
    {
       QTableWidgetItem *item = ui->variablesToSelectTableWidget->item(row,ColumnCheck);
       QString _varName = ui->variablesToSelectTableWidget->item(row,ColumnVarName)->text();
       if (item->checkState() == Qt::Checked)
           _selectedVariables.insert(_varName,_selectedMenu);
       else
           if (item->checkState() == Qt::Unchecked)
                 _selectedVariables.remove(_varName);
       refreshSelectVarTable(item->checkState(), _varName);
    }

}

/**
 * @brief This function refresh the table of selected variables
 *
 * @param _addVar
 * @param _variableName
 */
void TemplateDialog::refreshSelectVarTable(bool _addVar, QString _variableName)
{
    if (_addVar)
        addVariableToTable(_variableName,_selectedMenu);
    else
        removeVariableFromTable(_variableName);
    ui->selectedVariablesTableWidget->resizeColumnsToContents();
    ui->selectedVariablesTableWidget->show();
}


/**
 * @brief This function unchecks teh variable status in the selection table
 *
 * @param _varName
 */
void TemplateDialog::disableVariableInTableToSelect(QString _varName)
{
    int _totalRows = ui->variablesToSelectTableWidget->rowCount();
    for (int i = 0; i <_totalRows ;i++)
    {
        QString _nameTable = ui->variablesToSelectTableWidget->item(i,ColumnVarName)->text();
        if (QString::compare(_varName, _nameTable) == StrEqual)
            ui->variablesToSelectTableWidget->item(i,ColumnCheck)->setCheckState(Qt::Unchecked);

    }

}

/**
 * @brief This function adds to the selected var table, the variable of the selection table
 *
 * @param _name
 * @param _subsectionName
 */
void TemplateDialog::addVariableToTable(QString _name, QString _subsectionName)
{
    int _row = ui->selectedVariablesTableWidget->rowCount();
    ui->selectedVariablesTableWidget->insertRow(_row);

    QtableUtil _tableHelper;
    ui->selectedVariablesTableWidget->setItem(_row,SelectedColVariableName,_tableHelper.addVariable(_name));
    ui->selectedVariablesTableWidget->setItem(_row,SelectedColSectionName,_tableHelper.addVariable(_subsectionName));
    ui->selectedVariablesTableWidget->setItem(_row,SelectedColHelp, _tableHelper.addIcon());
    ui->selectedVariablesTableWidget->setCellWidget(_row,SelectedColDelete, _tableHelper.addIcon(IconDelete));
    _userHasMadeChanges = true;
}

/**
 * @brief This function remove a variaable from the selected var table
 *
 * @param _name
 */
void TemplateDialog::removeVariableFromTable(QString _name)
{
    for (int _row = 0; _row < ui->selectedVariablesTableWidget->rowCount();_row++)
    {
        QString _varName = ui->selectedVariablesTableWidget->item(_row,SelectedColVariableName)->text();
        if (QString::compare(_varName,_name) == StrEqual)
            ui->selectedVariablesTableWidget->removeRow(_row);
    }

}

/**
 * @brief This function controls the behaviour over clicking in the cells of the selected var table
 *
 * @param row
 * @param column
 */
void TemplateDialog::on_selectedVariablesTableWidget_cellClicked(int row, int column)
{
    if (column == SelectedColHelp)
    {
        SolverConfigMenuHelper _uiHelper;
        QString _menu = ui->selectedVariablesTableWidget->item(row,SelectedColSectionName)->text();
        SolverSectionVO _sectionVO =  _solverManager->getVariableListofSubsections(_menu);
        QString _nodeName = ui->selectedVariablesTableWidget->item(row,SelectedColVariableName)->text();
        QString _helpText = _uiHelper.getAssociatedHelpText(_nodeName,_sectionVO.getSubsectionVarList(),HTML_FORMAT_ON);
        loadHelp(_helpText);
        ui->selectedVariablesTableWidget->clearSelection();
    }else{
        if (column == SelectedColDelete)
        {
           QString _varName = ui->selectedVariablesTableWidget->item(row,SelectedColVariableName)->text();
           QString _sectionName = ui->selectedVariablesTableWidget->item(row,SelectedColSectionName)->text();
           removeVariableFromTable(_varName);
           _selectedVariables.remove(_varName);
           if (QString::compare(_selectedMenu, _sectionName) == StrEqual)
               disableVariableInTableToSelect(_varName);
        }
    }

}

/**
 * @brief This function controls the search functionality
 *
 */
void TemplateDialog::on_searchButton_clicked()
{
    resetTable(ui->variablesToSelectTableWidget);
    _searcherHelper.searchItems();
}

/**
 * @brief This function controls the clear search functionality
 *
 */
void TemplateDialog::on_clearSearchButton_clicked()
{
    resetTable(ui->variablesToSelectTableWidget);
    _searcherHelper.clearSearchStatus();
}

/**
 * @brief This function controls the acceptance by the user
 *
 */
void TemplateDialog::on_buttonBox_accepted()
{
    if (!_userHasMadeChanges)
    {
        close();
        return;
    }
    QString _workingPath = QDir(QCoreApplication::applicationDirPath()).filePath(".templates");
                                //QDir::currentPath()).filePath(".templates");
    if (!QDir(_workingPath).exists())
        QDir(QCoreApplication::applicationDirPath()).mkdir(".templates");

                    //QDir::currentPath()).mkdir(".templates");

    QString _binPath = QFileDialog::getSaveFileName(this, tr("Select a name for saving your template"),
                                                            _workingPath,
                                                            tr("Template Config File Type(*.xml)"));

    if (!_binPath.isNull())
    {
        TemplateManager _templateManager;
        _templateManager.setSectionTreeList(_solverManager->getAllSectionMenuList());
        _templateManager.saveTemplate(_binPath,_selectedVariables);
    }
    else
        return;
}

/**
 * @brief This function controls the search functionality on key pressed
 *
 */
void TemplateDialog::on_searchLineEdit_returnPressed()
{
    ui->searchButton->setFocus();
     resetTable(ui->variablesToSelectTableWidget);
    _searcherHelper.searchItems();
}
/**
 * @brief This function controls the  events of the key pressed
 *
 * @param evt
 */
void TemplateDialog::keyPressEvent(QKeyEvent *evt)
{
    if(evt->key() == Qt::Key_Enter || evt->key() == Qt::Key_Return)
        return;
    QDialog::keyPressEvent(evt);
}
