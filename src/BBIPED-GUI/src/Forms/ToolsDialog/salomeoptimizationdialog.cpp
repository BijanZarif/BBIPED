
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
#include "salomeoptimizationdialog.h"
#include "ui_salomeoptimizationdialog.h"

#include "Defines/General/GeneralDefines.h"
#include "Data/SalomeTemplateData/salomepythonfileutil.h"
#include <QLineEdit>
#include <QDir>

/**
 * @brief
 *
 * @param parent
 */
SalomeOptimizationDialog::SalomeOptimizationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SalomeOptimizationDialog)
{
    ui->setupUi(this);
    registerHelpers();
    _selectedTemplate = "";
    _isLoadingTable = false;
    _noIterations = 0;
    _regEditor = ui->consoleTextEdit;
}

/**
 * @brief
 *
 */
SalomeOptimizationDialog::~SalomeOptimizationDialog()
{
    delete ui;
}
/**
 * @brief
 *
 */
void SalomeOptimizationDialog::initializeDialog()
{
    if (!_templateManager.isInitialized())
        _templateManager.loadTemplate();
    loadTemplateMenu();
}

/**
 * @brief
 *
 */
void SalomeOptimizationDialog::loadTemplateMenu()
{
    refreshComboTemplates("");
}
/**
 * @brief
 *
 * @param _templateName
 */
void SalomeOptimizationDialog::refreshComboTemplates(QString _templateName)
{
    ui->templateComboBox->clear();
    QStringList _templateNameList;
    _templateNameList.append("");
    _templateNameList.append(_templateManager.getTemplateNameList());
    _templateNameList.append(_templateManager.getUserTemplateNameList());
   ui->templateComboBox->addItems(_templateNameList);
   _selectedTemplate = _templateName;
}


/**
 * @brief
 *
 * @param index
 */
void SalomeOptimizationDialog::on_templateComboBox_currentIndexChanged(int index)
{
    _selectedTemplate = ui->templateComboBox->itemText(index);
    /*if (isUserTemplateSelected(index))
    {
        _isUserDefinedTemplate = true;

    }else
    {
        _isUserDefinedTemplate = false;

    }*/
    if(_tableHelper.isInitialized())
        _tableHelper.resetTable();
    if (_tableSelVarHelper.isInitialized())
        _tableSelVarHelper.resetTable();

    fillTable(index);
    //addSignals();
}

/**
 * @brief
 *
 */
void SalomeOptimizationDialog::registerHelpers()
{
    _tableHelper.registerTable(ui->variablesListTable);
    _tableSelVarHelper.registerTable(ui->selectedVarListTable);
}

/**
 * @brief
 *
 * @param _templatePosition
 */
void SalomeOptimizationDialog::fillTable(int _templatePosition)
{
    _isLoadingTable = true;
    if ((_selectedTemplate.isEmpty()) || (_templatePosition == -1) )
        return;

    int _templateNo = getPosMenu();
    if (_templateNo == -1)
        return;
    SalomeTemplateData _templateData;
    int _sizePredefinedTemplates = _templateManager.getTemplateNameList().size();

    if ( (_templatePosition <= _sizePredefinedTemplates ) && (_templateNo < _templateManager.getTemplates().size()) )
        _templateData = _templateManager.getTemplates().at(_templateNo);
    if ( (_templatePosition > _sizePredefinedTemplates ) && (_templateNo < _templateManager.getUserTemplates().size()) )
        _templateData = _templateManager.getUserTemplates().at(_templateNo);

    QList<SalomeVariableData> _variableList = _templateData.getVariableList();

    for (int i = 0; i < _variableList.size();i++)
        addRow(_variableList.at(i));

    ui->variablesListTable->resizeColumnsToContents();
    ui->variablesListTable->show();
    _isLoadingTable = false;

}

/**
 * @brief
 *
 * @return int
 */
int SalomeOptimizationDialog::getPosMenu()
{
   int _result = -1;

   if (!_selectedTemplate.isEmpty())
   {
       QStringList _listTemplates = _templateManager.getTemplateNameList();
       if (_listTemplates.contains(_selectedTemplate,Qt::CaseSensitive))
            return _listTemplates.indexOf(_selectedTemplate,0);
       _listTemplates.clear();
       _listTemplates = _templateManager.getUserTemplateNameList();
       if (_listTemplates.contains(_selectedTemplate,Qt::CaseSensitive))
           return _listTemplates.indexOf(_selectedTemplate,0) ;
   }

   return _result;
}
/**
 * @brief
 *
 * @param _var
 */
void SalomeOptimizationDialog::addRow(SalomeVariableData _var)
{
   // QtableUtil _tableHelper;
    int _lastPos = ui->variablesListTable->rowCount();
    ui->variablesListTable->insertRow(_lastPos);
    ui->variablesListTable->setItem(_lastPos,variableChangedCol,_tableHelper.addChangedColumn());
    ui->variablesListTable->setItem(_lastPos,variableNameCol,_tableHelper.addNonEditableText(_var.getName()));
    ui->variablesListTable->setItem(_lastPos,variableValueCol,_tableHelper.addNonEditableText(_var.getCurrentValue()));
    ui->variablesListTable->setItem(_lastPos,variableOptimizeCol,_tableHelper.addSelectableCheckColumn());

    // Updating the values of the checkbox
    if (QString::compare(_var.getDefaultValue(),_var.getCurrentValue()) != StrEqual)
        ui->variablesListTable->item(_lastPos,variableChangedCol)->setCheckState(Qt::Checked);
    if (QString::compare(_var.getOptimizedValue(),"YES") == StrEqual)
    {
        ui->variablesListTable->item(_lastPos,variableOptimizeCol)->setCheckState(Qt::Checked);
        ui->variablesListTable->item(_lastPos,variableChangedCol)->setCheckState(Qt::Checked);
    }
}

/**
 * @brief
 *
 */
void SalomeOptimizationDialog::on_noIterationsLineEdit_editingFinished()
{
    resetColumns();
    _noIterations = ui->noIterationsLineEdit->text().toInt();
    addColumns();
    updateHeader();
}

/**
 * @brief
 *
 */
void SalomeOptimizationDialog::resetColumns()
{
    int _noCol = ui->selectedVarListTable->columnCount();
    if (_noCol > 2)
        for (int i = _noCol; i >= 2; i--)
            ui->selectedVarListTable->removeColumn(i);
}

/**
 * @brief
 *
 */
void SalomeOptimizationDialog::addColumns()
{
    int _lastCol = ui->selectedVarListTable->columnCount();    
    for (int i = _lastCol; i < (_lastCol + _noIterations) ; i++)
        ui->selectedVarListTable->insertColumn(i);

}

/**
 * @brief
 *
 */
void SalomeOptimizationDialog:: updateHeader()
{
    QString _headerTitle="Variable Name;Actual Value";
    for (int i = 1; i <=_noIterations;i++ )
        _headerTitle = _headerTitle + ";Iteration "+QString::number(i);
    ui->selectedVarListTable->setHorizontalHeaderLabels(_headerTitle.split(";"));
}

/**
 * @brief
 *
 * @param row
 * @param column
 */
void SalomeOptimizationDialog::on_variablesListTable_cellChanged(int row, int column)
{
    if ( (!_isLoadingTable) && (column == variableOptimizeCol) )
    {
        if (ui->variablesListTable->item(row,variableOptimizeCol)->checkState() == Qt::Checked)
            addvarToSelectedTable(row);
        else
            removevarFromSelectedTable(row);
    }
}

/**
 * @brief
 *
 * @param _varRow
 */
void SalomeOptimizationDialog::addvarToSelectedTable(int _varRow)
{
    int _lastRow = ui->selectedVarListTable->rowCount();
    QString _varName = ui->variablesListTable->item(_varRow,variableNameCol)->text();
    QString _varValue = ui->variablesListTable->item(_varRow,variableValueCol)->text();
    ui->selectedVarListTable->insertRow(_lastRow);
    ui->selectedVarListTable->setItem(_lastRow,selVarName,_tableSelVarHelper.addNonEditableText(_varName));
    ui->selectedVarListTable->setItem(_lastRow,selActualVal,_tableSelVarHelper.addNonEditableText(_varValue));

    for (int i = 0; i < _noIterations;i++)
        ui->selectedVarListTable->setCellWidget(_lastRow,(initIterationCol+i), _tableSelVarHelper.addEditText(""));

}
/**
 * @brief
 *
 * @param _varRow
 */
void SalomeOptimizationDialog::removevarFromSelectedTable(int _varRow)
{
    QString _varName = ui->variablesListTable->item(_varRow,variableNameCol)->text();
    if (_varName.isEmpty())
        return;
    int _posToRemove = findRowByVarName(_varName);
    if ( (_posToRemove == -1) || (_posToRemove >= ui->selectedVarListTable->rowCount()) )
        return;
    ui->selectedVarListTable->removeRow(_posToRemove);
}

/**
 * @brief
 *
 * @param _name
 * @return int
 */
int SalomeOptimizationDialog::findRowByVarName(QString _name)
{
    int _pos = -1;
    int _noRows = ui->selectedVarListTable->rowCount();
    for (int i = 0; i < _noRows;i++)
    {
        QString _cName= ui->selectedVarListTable->item(i,selVarName)->text();
        if (QString::compare(_cName,_name) == StrEqual)
            return i;
    }

    return _pos;
}


//void SalomeOptimizationDialog::on_buttonBox_accepted()
/**
 * @brief
 *
 */
void SalomeOptimizationDialog::on_pushButton_clicked()
{
    // TODO

    QList< QList<SalomeVariableData> > _userListVarOpt =  getVariablesList();

    int _posSelTemplate = getPosMenu();
    SalomeTemplateData _origTemplate;
    int _selectedTemplateIndex = ui->templateComboBox->currentIndex();
 // Nota: mirar en el caso de user defined templates (seguramente anhadir getPos()
   if (_selectedTemplateIndex > _templateManager.getTemplates().size())
        _origTemplate = _templateManager.getUserTemplates().at(_posSelTemplate);
   else
        _origTemplate = _templateManager.getTemplates().at(_posSelTemplate);

    SalomePythonFileUtil _pythonUtil;
    _pythonUtil.setFileName(_projectName);
    _pythonUtil.setPathProject(_projectPath);
    _pythonUtil.setTemplateData(_origTemplate);
    _pythonUtil.setVariableList(_userListVarOpt);

    int _iterations = ui->noIterationsLineEdit->text().toInt();
    _pythonUtil.buildPythonFileForOptimization(_iterations);

     // Running the python file
    runPythonFile();

}

/**
 * @brief
 *
 */
void SalomeOptimizationDialog::runPythonFile()
{
    QFileInfo _salomeFilePath(_salomeMeshPath);
    QString _salomeToolPath = _salomeFilePath.absolutePath();
    if (_salomeToolPath.isEmpty())
        return;
    ui->pushButton->setEnabled(false);
    _regEditor->append("Starting Python JobManager Execution ....");
    _meshCreationProcess = new QProcess(this);
    _meshCreationProcess->setWorkingDirectory(QDir(_projectPath).filePath("Python"));
  QString _meshCommand("") ;
    _meshCommand = _salomeToolPath + "/runSession runSalome -t -u" + _projectName +"_opt.py";
    connect(_meshCreationProcess, SIGNAL(error(QProcess::ProcessError)),
            this, SLOT(slotProcessError(QProcess::ProcessError)));
    connect(_meshCreationProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(outputConsole()));
    connect(_meshCreationProcess, SIGNAL(readyReadStandardError()), this, SLOT(errorConsole()));
    connect(_meshCreationProcess, SIGNAL(finished(int)), this, SLOT(meshCreationFinished()));

    _regEditor->append("[START MESH CREATION] --> Running " + _meshCommand);
    _meshCreationProcess->start(_meshCommand);

    // Updating the Text of the Create Mesh Button


}

/**
 * @brief
 *
 * @return QList<QList<SalomeVariableData> >
 */
QList< QList<SalomeVariableData> > SalomeOptimizationDialog::getVariablesList()
{
   int _noRows = ui->selectedVarListTable->rowCount();
   int _noCols = ui->selectedVarListTable->columnCount();
   int _selectedTemplateIndex = ui->templateComboBox->currentIndex();
   int _posSelTemplate = getPosMenu();
   SalomeTemplateData _origTemplate;
// Nota: mirar en el caso de user defined templates (seguramente anhadir getPos()
  if (_selectedTemplateIndex > _templateManager.getTemplates().size())
       _origTemplate = _templateManager.getUserTemplates().at(_posSelTemplate);
  else
       _origTemplate = _templateManager.getTemplates().at(_posSelTemplate);

   // We skip the first 2 columns
   // We build a List per each Iteration (Column)
   QList< QList<SalomeVariableData> > _listOfVariables;
    for (int _cColumn = 2; _cColumn < _noCols ; _cColumn++)
    {
       QList<SalomeVariableData> _listVariablesIteration;
       for (int _cRow = 0; _cRow < _noRows; _cRow++)
       {
           QString _variableName = ui->selectedVarListTable->item(_cRow,0)->text();
           QString _value = ((QLineEdit *)ui->selectedVarListTable->cellWidget(_cRow,_cColumn))->text();
           SalomeVariableData _variable = _origTemplate.findByName(_variableName);
           _variable.setCurrentValue(_value);
           _listVariablesIteration.push_back(_variable);
       }
       _listOfVariables.push_back(_listVariablesIteration);
    }

    return _listOfVariables;
}


/**
 * @brief
 *
 * @param _template
 */
void SalomeOptimizationDialog::setTemplateData(QList<SalomeTemplateData> _template)
{
    _templateDataList = _template;
}
/**
 * @brief
 *
 * @param _name
 */
void SalomeOptimizationDialog::setProjectName(QString _name)
{
    _projectName = _name;
}

/**
 * @brief
 *
 * @param _path
 */
void SalomeOptimizationDialog::setProjectPath(QString _path)
{
    _projectPath = _path;
}
/**
 * @brief
 *
 * @param _path
 */
void SalomeOptimizationDialog::setMeshViewerPath(QString _path)
{
    _salomeMeshPath = _path;
}


/**
 * @brief
 *
 * @param error
 */
void SalomeOptimizationDialog::slotProcessError(QProcess::ProcessError error)
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
}

/**
 * @brief
 *
 */
void SalomeOptimizationDialog::outputConsole()
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
void SalomeOptimizationDialog::errorConsole()
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
void SalomeOptimizationDialog::meshCreationFinished()
{
    ui->finishButton->setEnabled(true);
    if (_meshCreationProcess->exitStatus() < 0)
     {
         _regEditor->append("[ERROR] --> Mesh cannot be created. Check your files");
         _regEditor->append(_meshCreationProcess->errorString());
     }else
        _regEditor->append("[OUTPUT] --> Mesh created. Check your folder");

}


/**
 * @brief
 *
 */
void SalomeOptimizationDialog::on_finishButton_clicked()
{
    close();
}
