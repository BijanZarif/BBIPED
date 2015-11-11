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
#include "warningclosingdialog.h"
#include "ui_warningclosingdialog.h"

/**
 * @brief CONSTRUCTOR
 *
 * @param parent
 */
WarningClosingDialog::WarningClosingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WarningClosingDialog)
{
    ui->setupUi(this);
    _needToBeClosed = false;
    _showWindow = true;
}

/**
 * @brief DESTRUCTOR
 *
 */
WarningClosingDialog::~WarningClosingDialog()
{
    delete ui;
}
/********************************************* BUTTON CLICK *******************************/
/**
 * @brief Function that controls when the user accepts
 *
 */
void WarningClosingDialog::on_buttonBox_accepted()
{
    _needToBeClosed = true;
    close();
}

/**
 * @brief  Function that controls when the user rejects
 *
 */
void WarningClosingDialog::on_buttonBox_rejected()
{
    close();
}


/**
 * @brief  Function that controls when the user decide not to show the message dialog again
 *
 * @param checked
 */
void WarningClosingDialog::on_donotShowMessageCheckBox_clicked(bool checked)
{
    _showWindow = !checked;
}

/********************************************* Getter/Setter/Is *******************************/

/**
 * @brief Returns true if the application needs to be closed
 *
 * @return bool
 */
bool  WarningClosingDialog::needsToCloseApplication()
{
    return _needToBeClosed;
}

/**
 * @brief Establish the message title of the window
 *
 * @param _newTitle
 */
void WarningClosingDialog::setTitleMessage(QString _newTitle)
{
    ui->titleMessage->setText(_newTitle);
}


/**
 * @brief Returns true if the window shall be shown again in the future
 *
 * @return bool
 */
bool WarningClosingDialog::getShowAgainValue()
{
    return _showWindow;
}
