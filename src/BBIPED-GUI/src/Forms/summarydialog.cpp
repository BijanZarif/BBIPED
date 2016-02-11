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
#include "summarydialog.h"
#include "ui_summarydialog.h"

#include "Data/solversectiondao.h"
#include "WindowHelper/Dialog/SolverDialog/solverconfigmenuhelper.h"
#include "Defines/General/GeneralDefines.h"
#include <QHashIterator>
#include <QLineEdit>
#include <QComboBox>




/**
 * @brief Constructor
 *
 * @param parent
 */
/**
 * @brief
 *
 * @param parent
 */
SummaryDialog::SummaryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SummaryDialog)
{
    ui->setupUi(this);
    _userHasMadeChanges = false;

}
/**
 * @brief Destructor
 *
 */
/**
 * @brief
 *
 */
SummaryDialog::~SummaryDialog()
{
   delete ui;
}

/**
 * @brief Function to reset the dialog
 *
 */
/**
 * @brief
 *
 */
void SummaryDialog::resetDialog()
{
        _userHasMadeChanges = false;
}


/**
 * @brief Function that stores the values from the user to be shown in the dialog
 *
 * @param QHash<QString
 * @param _valuesMap
 */
/**
 * @brief
 *
 * @param QHash<QString
 * @param _valuesMap
 */
void SummaryDialog::setValuesToShow(QHash<QString,QString> * _valuesMap)
{
    _valuesToShow  = _valuesMap;
}

/**
 * @brief Function to set up the menu elements
 *
 * @param _sectionList
 */
/**
 * @brief
 *
 * @param _sectionList
 */
void SummaryDialog::setSectionMenuList(QList<SolverSectionVO>  _sectionList)
{
    _menuSectionList = _sectionList;
}



/**
 * @brief Function that builds up the summary table
 *
 */
/**
 * @brief
 *
 */
void SummaryDialog::buildSummaryTable()
{
    buildTableHeader();
    buildRows();
    ui->summaryTable->resizeColumnsToContents();
}

/**
 * @brief Function that builds up the header of the table
 *
 */
/**
 * @brief
 *
 */
void SummaryDialog::buildTableHeader()
{
    ui->summaryTable->setColumnCount(4);
    ui->summaryTable->setHorizontalHeaderLabels( QString("Changed;Variable;Value;Help").split(";"));
    ui->summaryTable->horizontalHeader()->setVisible(true);
}

/**
 * @brief  Function that builds up the rows of the table
 *
 */
/**
 * @brief
 *
 */
void SummaryDialog::buildRows()
{
    SolverSectionDAO _dao;
    SolverConfigMenuHelper _uiHelper;
    int _row = 0;
    if( (!_valuesToShow->isEmpty()) && (!_menuSectionList.isEmpty()))
    {
        QHashIterator<QString,QString> iter(*_valuesToShow);
        while(iter.hasNext())
        {
            iter.next();
            QString _keyValue = iter.key();
            SolverVariableVO _varVO = _dao.findVariableByFileName(_menuSectionList,_keyValue);
            if (!_varVO.isEmpty())
            {
                insertVariableInTable(_varVO,_row);
                _row++;
                _uiHelper.updateVariableValueinTable(_varVO,ui->summaryTable, _valuesToShow->value(_keyValue));
                _summaryVariables.append(_varVO);
            }
        }
    }
}


/**
 * @brief  Function that inserts each variabel with the correspondent value in the table
 *
 * @param _varVO
 * @param _row
 */
/**
 * @brief
 *
 * @param _varVO
 * @param _row
 */
void SummaryDialog::insertVariableInTable(SolverVariableVO _varVO,int _row)
{
        SolverConfigMenuHelper _uiHelper;
        _uiHelper.addVariableToTable(_varVO,ui->summaryTable,_row);
}


/**
 * @brief  Function that loads the help text in a new window
 *
 * @param row
 * @param column
 */
/**
 * @brief
 *
 * @param row
 * @param column
 */
void SummaryDialog::on_summaryTable_cellClicked(int row, int column)
{
    if (column == 3)
    {
        _helpDialog = new HelpDialog();
        SolverVariableVO _variable = _summaryVariables.at(row);

        _helpDialog->setHelpText(_variable.getHelpContents(true)); // TO BE REVIEWED
        _helpDialog->loadHelpText();
        _helpDialog->exec();
      }
}


/**
 * @brief Function that launchs the actions when the user clicks accept button
 *
 */
/**
 * @brief
 *
 */
void SummaryDialog::on_summaryButtonGroup_accepted()
{

    updateNewValues();
}

/**
 * @brief Function that launchs the actions when the user clicks cancel button
 *
 */
/**
 * @brief
 *
 */
void SummaryDialog::on_summaryButtonGroup_rejected()
{
    close();
}


/**
 * @brief Function that update the values changed by the user
 *
 */
/**
 * @brief
 *
 */
void SummaryDialog::updateNewValues()
{
    QHashIterator<QString,QString> iter(*_valuesToShow);
    while(iter.hasNext())
    {
        iter.next();
        QString _keyValue = iter.key();
        SolverSectionDAO _dao;
        SolverVariableVO _varVO = _dao.findVariableByFileName(_menuSectionList,_keyValue);
        QString _varName = _varVO.getVariableName();
        if( (!_varName.isEmpty()) && (!_varName.isNull()))
        {
            QString _actualValue = getVariableValue(getRowTableByVarName(_varName));
            if (QString::compare(_actualValue,_valuesToShow->value(_keyValue)) != StrEqual)
            {
                _valuesToShow->insert(_keyValue,_actualValue);
                _userHasMadeChanges = true;
            }
        }


    }
}
 /**
  * @brief Function that obatins the row of a specific variable name
  *
  * @param _varName
  * @return int
  */
 /**
  * @brief
  *
  * @param _varName
  * @return int
  */
 int SummaryDialog::getRowTableByVarName(QString _varName)
 {
     for (int i = 0;i < ui->summaryTable->rowCount();i++)
     {
         QString _cVarName = ui->summaryTable->item(i,1)->text();
         if (QString::compare(_cVarName,_varName) == StrEqual)
         {
             return i;
         }
     }
     return -1;
 }

 /**
  * @brief Function that gets the value of a specific variable in a row
  *
  * @param _row
  * @return QString
  */
 /**
  * @brief
  *
  * @param _row
  * @return QString
  */
 QString SummaryDialog::getVariableValue(int _row)
 {
     QLineEdit * _lineEdit = qobject_cast<QLineEdit * >(ui->summaryTable->cellWidget(_row,2));
     if (_lineEdit)
     {
         QString _text = _lineEdit->text();

       return _text;
     }

     QComboBox* combo =  qobject_cast<QComboBox * > (ui->summaryTable->cellWidget(_row, 2));
     if (combo)
     {
         QString _text = combo->currentText();
        return _text;
     }
     return QString();
 }


 /**
  * @brief Function that returns true if the user has made changes
  *
  * @return bool
  */
 /**
  * @brief
  *
  * @return bool
  */
 bool SummaryDialog::userHasMadeChanges()
 {
     return _userHasMadeChanges;
 }
