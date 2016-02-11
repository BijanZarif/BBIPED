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
#include "meshingtoolconfigurationwarningdialog.h"
#include "ui_meshingtoolconfigurationwarningdialog.h"

#include "WindowHelper/Menu/MenuHelpers/edithelper.h"

MeshingToolConfigurationWarningDialog::MeshingToolConfigurationWarningDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MeshingToolConfigurationWarningDialog)
{
    ui->setupUi(this);
}

MeshingToolConfigurationWarningDialog::~MeshingToolConfigurationWarningDialog()
{
    delete ui;
}

void MeshingToolConfigurationWarningDialog::on_warningMeshingButtonBox_accepted()
{
    EditHelper _editHelper;
    _editHelper.launchPreferencesDialog(_userPreferencesData,_solverManager);
    close();
}

void MeshingToolConfigurationWarningDialog::on_warningMeshingButtonBox_rejected()
{
    close();
}
void MeshingToolConfigurationWarningDialog::initializeStructures(UserPreferencesData * _userData,SolverConfigManager * _manager )
{
    _userPreferencesData = _userData;
    _solverManager = _manager;
}
