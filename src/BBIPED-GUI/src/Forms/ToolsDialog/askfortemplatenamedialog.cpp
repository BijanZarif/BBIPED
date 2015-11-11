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
#include "askfortemplatenamedialog.h"
#include "ui_askfortemplatenamedialog.h"

/**
 * @brief
 *
 * @param parent
 */
AskForTemplateNameDialog::AskForTemplateNameDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AskForTemplateNameDialog)
{
    ui->setupUi(this);
    _templateName ="";
   // _templateFileName ="";
}

/**
 * @brief
 *
 */
AskForTemplateNameDialog::~AskForTemplateNameDialog()
{
    delete ui;
}

/**
 * @brief
 *
 * @return QString
 */
QString AskForTemplateNameDialog::getTemplateName()
{
    return _templateName;
}

/*QString AskForTemplateNameDialog::getTemplateFileName()
{
    return _templateFileName;
}*/

/**
 * @brief
 *
 */
void AskForTemplateNameDialog::on_buttonBox_accepted()
{
    _templateName = ui->templateNameLineEdit->text();
   // _templateFileName = ui->templateFileNameLineEdit->text();

    close();
}

/**
 * @brief
 *
 */
void AskForTemplateNameDialog::on_buttonBox_rejected()
{
    close();
}

/**
 * @brief
 *
 */
void AskForTemplateNameDialog::on_toolButton_clicked()
{
    //TODO
}
