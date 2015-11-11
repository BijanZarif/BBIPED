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
#include "clipdomaindialog.h"
#include "ui_clipdomaindialog.h"
#include "Utility/qtableutil.h"

/**
 * @brief
 *
 * @param parent
 */
ClipDomainDialog::ClipDomainDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClipDomainDialog)
{
    ui->setupUi(this);
    _loadingTable = false;
}

/**
 * @brief
 *
 */
ClipDomainDialog::~ClipDomainDialog()
{
    delete ui;

}


/**
 * @brief
 *
 * @param _value
 */
void ClipDomainDialog::setUserValues(QString _value)
{
    _userValues = _value.split(",");
}

/**
 * @brief
 *
 * @return QString
 */
QString ClipDomainDialog::getValues()
{
    return (_userValues.join(","));
}

/**
 * @brief
 *
 * @param _variable
 */
void ClipDomainDialog::setSolverVariable(SolverVariableVO _variable)
{
    _solverVO = _variable;
}



/**
 * @brief
 *
 */
void ClipDomainDialog::initializeDialog()
{

    loadTableHeader();
    fillTable();
    _loadingTable = false;
}

/**
 * @brief
 *
 */
void ClipDomainDialog::loadTableHeader()
{
    ui->variablesTable->setColumnCount(2);
    ui->variablesTable->setHorizontalHeaderLabels( QString("Variable;Value").split(";"));
    ui->variablesTable->horizontalHeader()->setVisible(true);

}

/**
 * @brief
 *
 */
void ClipDomainDialog::fillTable()
{
    QtableUtil _tableHelper;
    _loadingTable = true;

    QStringList _listValueNames = _solverVO.getSubVariableNames();

    if (!_userValues.isEmpty())
        ui->domainTypeLineEdit->setText(_userValues.at(0));
    for (int i = 0; i < _listValueNames.size(); i++)
    {

        ui->variablesTable->insertRow(i);
        ui->variablesTable->setItem(i,ColumnVarName,_tableHelper.addNonEditableText(_listValueNames.at(i)));
        if ((!_userValues.isEmpty()) && (_listValueNames.size() ==(_userValues.size()-1)))
            ui->variablesTable->setCellWidget(i,ColumnValue,_tableHelper.addEditText(_userValues.at(i+1)));
        else
             ui->variablesTable->setCellWidget(i,ColumnValue,_tableHelper.addEditText(""));

    }
    _loadingTable = false;

}

/**
 * @brief
 *
 */
void ClipDomainDialog::on_doneButton_clicked()
{
    // Store the results of the user
    _userValues.clear();
    _userValues.append(ui->domainTypeLineEdit->text());
    int _lastRow = ui->variablesTable->rowCount();
    for (int i = 0; i < _lastRow; i++)
    {
        QLineEdit * _item = qobject_cast<QLineEdit *> (ui->variablesTable->cellWidget(i,ColumnValue));
        if (_item)
            _userValues.append(_item->text());
    }

    close();
}
