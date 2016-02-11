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
#include "salometoolcreatemeshdialog.h"
#include "ui_salometoolcreatemeshdialog.h"
#include "Data/SalomeTemplateData/salomepythonfileutil.h"
#include <QDebug>
#include <QDir>
#include <QMessageBox>
#include "Forms/ToolsDialog/salomeautomaticmeshprogressdialog.h"
#include <QFile>

/**
 * @brief
 *
 * @param parent
 */
SalomeToolCreateMeshDialog::SalomeToolCreateMeshDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SalomeToolCreateMeshDialog)
{
    ui->setupUi(this);
    _regEditor = ui->consoleTextEdit;
}

/**
 * @brief
 *
 */
SalomeToolCreateMeshDialog::~SalomeToolCreateMeshDialog()
{
    delete ui;
}
/**
 * @brief
 *
 * @param _template
 */
void SalomeToolCreateMeshDialog::setTemplateData(SalomeTemplateData _template)
{
    _templateData = _template;
}
/**
 * @brief
 *
 * @param _name
 */
void SalomeToolCreateMeshDialog::setProjectName(QString _name)
{
    _projectName = _name;
}

/**
 * @brief
 *
 * @param _path
 */
void SalomeToolCreateMeshDialog::setProjectPath(QString _path)
{
    _projectPath = _path;
}



//************************************* REVISARRR
/**
 * @brief
 *
 */
void SalomeToolCreateMeshDialog::runPythonFile()
{
    QFileInfo _salomeFilePath(_salomeMeshPath);
    QString _salomeToolPath = _salomeFilePath.absolutePath();
    if (_salomeToolPath.isEmpty())
        return;

    _regEditor->append("Starting Python JobManager Execution ....");
    _meshCreationProcess = new QProcess(this);
    _meshCreationProcess->setWorkingDirectory(QDir(_projectPath).filePath("Python"));
    QString _meshCommand("") ;
    _meshCommand = _salomeToolPath + "/runSession runSalome -t -u" + _projectName +".py";
    connect(_meshCreationProcess, SIGNAL(error(QProcess::ProcessError)),
            this, SLOT(slotProcessError(QProcess::ProcessError)));
    connect(_meshCreationProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(outputConsole()));
    connect(_meshCreationProcess, SIGNAL(readyReadStandardError()), this, SLOT(errorConsole()));
    connect(_meshCreationProcess, SIGNAL(finished(int)), this, SLOT(meshCreationFinished()));

    _regEditor->append("[START MESH CREATION] --> Running " + _meshCommand);
    _meshCreationProcess->start(_meshCommand);

    // Updating the Text of the Create Mesh Button
    ui->createMeshButton->setDisabled(true);
}

/**
 * @brief
 *
 * @param error
 */
void SalomeToolCreateMeshDialog::slotProcessError(QProcess::ProcessError error)
{

    switch (error) {
        case QProcess::FailedToStart :
        _regEditor->append("FAILED to start the application: Check the executable configuration in the Preferences Menu");
            break;
    case QProcess::Crashed:
       _regEditor->append( "STOP Running simulation" );
        break;

       case QProcess::Timedout:
       _regEditor->append("TimedOut process: Contact Support");
        break;
    case QProcess::WriteError:
       _regEditor->append("Write Error: Check the permissions of the folder");
        break;
    case QProcess::ReadError:
       _regEditor->append( "Read Error: Check the permissions of the files ");
        break;
    case QProcess::UnknownError:
      _regEditor->append("Unknown Error: Contact Support ");
        break;

    }

 //stopSolverProcess();
}

/**
 * @brief
 *
 */
void SalomeToolCreateMeshDialog::outputConsole()
{
    QString _output =_meshCreationProcess->readAllStandardOutput() ;
    _output = "[OUTPUT] --> " + _output;
    emit outlogtext(_output);
    _regEditor->append(_output);
}
/**
 * @brief
 *
 */
void SalomeToolCreateMeshDialog::errorConsole()
{
    QString _output =_meshCreationProcess->readAllStandardError();
     //emit outlogtext(_output);
    _output = "[ERROR] -->" + _output;
    _regEditor->append(_output);
}
/**
 * @brief
 *
 */
void SalomeToolCreateMeshDialog::meshCreationFinished()
{
    ui->finishButton->setEnabled(true);
    if (_meshCreationProcess->exitStatus() < 0)
     {
         _regEditor->append("[ERROR] --> Mesh cannot be created. Check your files");
         _regEditor->append(_meshCreationProcess->errorString());
     }else
    {
        _regEditor->append("[OUTPUT] --> Mesh created. Check your folder");
        // Change the name of the file to the user selection
        QString _userFileNameSelection = ui->meshFileNameLineEdit->text();
        if (!_userFileNameSelection.isEmpty())
        {
            QString _pythonPath=QDir(_projectPath).filePath("Python");
            QString _finalOldFile = QDir(_pythonPath).filePath("AutomaticMesh.su2");
            QString _finalNewFile = QDir(_pythonPath).filePath(_userFileNameSelection+".su2");
            QFile::rename(_finalOldFile,_finalNewFile);
        }

    }
}


/**
 * @brief
 *
 * @param _path
 */
void SalomeToolCreateMeshDialog::setSalomeMeshViewerPath(QString _path)
{
    _salomeMeshPath = _path;
}

/**
 * @brief
 *
 * @param _path
 */
void SalomeToolCreateMeshDialog::setBBIPEDPath(QString _path)
{
    _bbipedPath = _path;
}


/**
 * @brief
 *
 */
void SalomeToolCreateMeshDialog::on_createMeshButton_clicked()
{
    if (_templateData.isEmpty())
        return;
    SalomePythonFileUtil _pythonUtil;
    _pythonUtil.setFileName(_projectName);
    _pythonUtil.setPathProject(_projectPath);
    _pythonUtil.setTemplateData(_templateData);
    _pythonUtil.buildPythonFile();

     // Running the python file
    runPythonFile();



}

/**
 * @brief
 *
 */
void SalomeToolCreateMeshDialog::on_finishButton_clicked()
{
    close();
}
