
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
#include "helpdialog.h"
#include "ui_helpdialog.h"

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
HelpDialog::HelpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HelpDialog)
{
        ui->setupUi(this);

}

/**
 * @brief Destructor
 *
 */
/**
 * @brief
 *
 */
HelpDialog::~HelpDialog()
{
    delete ui;
}


/**
 * @brief Set the help text
 *
 * @param _text
 */
/**
 * @brief
 *
 * @param _text
 */
void HelpDialog::setHelpText(QString _text)
{
    _helpText = _text;
}

/**
 * @brief Load the help text
 *
 */
/**
 * @brief
 *
 */
void HelpDialog::loadHelpText()
{
   // ui->helpAreaText->setText(_helpText);
    ui->helpAreaText->insertHtml(_helpText);
}

/**
 * @brief Action on click
 *
 */
/**
 * @brief
 *
 */
void HelpDialog::on_okHelpDialog_clicked()
{
    this->close();
}


