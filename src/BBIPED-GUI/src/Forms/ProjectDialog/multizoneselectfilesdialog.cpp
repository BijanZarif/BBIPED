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
#include "multizoneselectfilesdialog.h"
#include "ui_multizoneselectfilesdialog.h"


#include <QFileDialog>
#include <QFileInfo>
#include <QDir>

#include "Utility/showmessagehelper.h"
#include "Defines/General/GeneralDefines.h"

MultizoneSelectFilesDialog::MultizoneSelectFilesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MultizoneSelectFilesDialog)
{
    ui->setupUi(this);
    _configFilePath = "";
    _meshFilePath = "";
    _projectPath = "";
    _fullConfigFilePath ="";
    _fullMeshFilePath = "";
    _hasSelectedFiles = false;
}

MultizoneSelectFilesDialog::~MultizoneSelectFilesDialog()
{
    delete ui;
}

void MultizoneSelectFilesDialog::on_importedConfigNameToolButton_clicked()
{
    if (!ui->newConfigNameLineEdit->text().isEmpty())
    {
        ShowMessageHelper _showMessage;
        bool _removeManualConfigLine = _showMessage.showWarningMessage("Warning! New configuration name has been previously entered!",
                                        "Warning message: You have entered the name for creating a new configuration file. If you import an existent one, the previous one will be lost. Do you want to continue?");
        if (_removeManualConfigLine)
            ui->newConfigNameLineEdit->text().clear();
        else
            return;
    }

    // Launch dialog for getting the files to import
    _fullConfigFilePath = QFileDialog::getOpenFileName(this, "Select the Configuration File to import",
                                                            QCoreApplication::applicationDirPath(),
                                                            "BBIPED Configuration Files (*.cfg)");
    if (_fullConfigFilePath.isEmpty()) return;
    QFileInfo _file(_fullConfigFilePath);
    ui->importedConfigNameLineEdit->setText(_file.fileName());
}

void MultizoneSelectFilesDialog::on_meshFileToolButton_clicked()
{
    // Launch dialog for getting the files to import
    _fullMeshFilePath = QFileDialog::getOpenFileName(this, "Select the Mesh File to import",
                                                            QCoreApplication::applicationDirPath(),
                                                            "Mesh Files (*.su2)");
    if (_fullMeshFilePath.isEmpty()) return;
    QFileInfo _file(_fullMeshFilePath);
    ui->meshFileLineEdit->setText(_file.fileName());
}

void MultizoneSelectFilesDialog::on_acceptButton_clicked()
{
    if (!ui->newConfigNameLineEdit->text().isEmpty())
    {
        QString _configFileName = ui->newConfigNameLineEdit->text();
        QFileInfo _fileInfo(_configFileName);
        if (_fileInfo.suffix().isEmpty())
            _configFileName = _configFileName + ".cfg";
        _configFilePath = QDir(_projectPath).filePath(QDir(TOOL4).filePath(_configFileName));
    }
    if (!ui->importedConfigNameLineEdit->text().isEmpty())
        _configFilePath = _fullConfigFilePath;
    if (!ui->meshFileLineEdit->text().isEmpty())
        _meshFilePath = _fullMeshFilePath;

    _hasSelectedFiles = true;
    close();
}

void MultizoneSelectFilesDialog::on_cancelButton_clicked()
{
    _hasSelectedFiles = false;
    close();

}

/**************************************************************************/
QString MultizoneSelectFilesDialog::getConfigFilePath()
{
    return _configFilePath;
}

QString MultizoneSelectFilesDialog::getMeshFilePath()
{
    return _meshFilePath;
}
bool MultizoneSelectFilesDialog::hasSelectedFiles()
{
    return _hasSelectedFiles;
}
void MultizoneSelectFilesDialog::setProjectPath(QString _path)
{
    _projectPath = _path;
}
