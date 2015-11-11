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
#include "salometemplatedialog.h"
#include "ui_salometemplatedialog.h"

#include "Forms/ToolsDialog/askfortemplatenamedialog.h"
#include "Defines/General/GeneralDefines.h"
#include "Forms/helpdialog.h"
#include "salometoolcreatemeshdialog.h"
#include <QFileInfo>
#include <QDir>
#include <QMessageBox>

/**
 * @brief
 *
 * @param parent
 */
SalomeTemplateDialog::SalomeTemplateDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SalomeTemplateDialog)
{
       ui->setupUi(this);
       registerHelpers();
       _signalMapperTxtEdit = new QSignalMapper(this);
       _isLoadingTable = true;
       _isUserDefinedTemplate = false;
       _needToSaveUserChanges = false;
       _meshViewerPath = "";
       _bbipedPath = "";
}

/**
 * @brief
 *
 */
SalomeTemplateDialog::~SalomeTemplateDialog()
{
    delete ui;
    _projectPath = "";
    _projectName = "";
    _meshViewerPath = "";
    _bbipedPath = "";

}

/**
 * @brief
 *
 */
void SalomeTemplateDialog::registerHelpers()
{
    _tableHelper.registerTable(ui->templateVarTable);
}

/**
 * @brief
 *
 */
void SalomeTemplateDialog::initializeDialog()
{
    if (!_templateManager.isInitialized())
        _templateManager.loadTemplate();
    loadTemplateMenu();
}


/**
 * @brief
 *
 * @return QString
 */
QString SalomeTemplateDialog::getProjectPath()
{
    return _projectPath;
}

/**
 * @brief
 *
 * @param _path
 */
void SalomeTemplateDialog::setProjectPath(QString _path)
{
    _projectPath = _path;
}

/**
 * @brief
 *
 * @return QString
 */
QString SalomeTemplateDialog::getProjectName()
{
    return _projectName ;
}

/**
 * @brief
 *
 * @param _name
 */
void SalomeTemplateDialog::setProjectName(QString _name)
{
    _projectName = _name;
}


/**
 * @brief
 *
 */
void SalomeTemplateDialog::loadTemplateMenu()
{
    refreshComboTemplates("");
}

/**
 * @brief
 *
 * @param _templatePosition
 */
void SalomeTemplateDialog::fillTable(int _templatePosition)
{
    _isLoadingTable = true;
    if ((_selectedTemplate.isEmpty()) || (_templatePosition == -1) )
        return;

    int _templateNo = getPosMenu();
    if (_templateNo == -1)
        return;
    SalomeTemplateData _templateData;
    int _sizePredefinedTemplates = _templateManager.getTemplateNameList().size();
    // Checking if it is a predefined template or an user defined one
    if ( (_templatePosition <= _sizePredefinedTemplates ) && (_templateNo < _templateManager.getTemplates().size()) )
        _templateData = _templateManager.getTemplates().at(_templateNo);
    if ( (_templatePosition > _sizePredefinedTemplates ) && (_templateNo < _templateManager.getUserTemplates().size()) )
        _templateData = _templateManager.getUserTemplates().at(_templateNo);

    QList<SalomeVariableData> _variableList = _templateData.getVariableList();

    for (int i = 0; i < _variableList.size();i++)
        addRow(_variableList.at(i));

    ui->templateVarTable->resizeColumnsToContents();
    ui->templateVarTable->show();
    _isLoadingTable = false;

}

/**
 * @brief
 *
 * @param _pos
 * @param _var
 */
void SalomeTemplateDialog::addRow(SalomeVariableData _var)
{
   // QtableUtil _tableHelper;
    int _lastPos = ui->templateVarTable->rowCount();
    ui->templateVarTable->insertRow(_lastPos);
    ui->templateVarTable->setItem(_lastPos,variableChangedCol,_tableHelper.addChangedColumn());
    ui->templateVarTable->setItem(_lastPos,variableNameCol,_tableHelper.addNonEditableText(_var.getName()));
    ui->templateVarTable->setCellWidget(_lastPos,variableValueCol,_tableHelper.addEditText(_var.getCurrentValue()));
    //ui->templateVarTable->setItem(_lastPos,variableOptimizeCol,_tableHelper.addSelectableCheckColumn());

    // Updating the values of the checkbox
    if (QString::compare(_var.getDefaultValue(),_var.getCurrentValue()) != StrEqual)
        ui->templateVarTable->item(_lastPos,variableChangedCol)->setCheckState(Qt::Checked);
    /*if (QString::compare(_var.getOptimizedValue(),"YES") == StrEqual)
    {
        ui->templateVarTable->item(_lastPos,variableOptimizeCol)->setCheckState(Qt::Checked);
        ui->templateVarTable->item(_lastPos,variableChangedCol)->setCheckState(Qt::Checked);
    }*/
}

/**
 * @brief
 *
 * @return int
 */
int SalomeTemplateDialog::getPosMenu()
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
 */
void SalomeTemplateDialog::on_helpButton_clicked()
{
    HelpDialog _helpDialog;
    _helpDialog.setHelpText("TODO Help");
    _helpDialog.exec();
}


/**
 * @brief
 *
 */
void SalomeTemplateDialog::on_deleteTemplateButton_clicked()
{
    _templateManager.deleteUserDefinedTempalte(_selectedTemplate);
    refreshComboTemplates("");
    if(_tableHelper.isInitialized())
        _tableHelper.resetTable();
}

/**
 * @brief
 *
 */
void SalomeTemplateDialog::on_saveButton_clicked()
{
    if (_selectedTemplate.isEmpty())
        return;
    saveData();

}


/**
 * @brief
 *
 * @param _template
 */
void SalomeTemplateDialog::updateTemplateValues(SalomeTemplateData * _template)
{
    QList<SalomeVariableData> _varList = _template->getVariableList();
    int _totalRows = ui->templateVarTable->rowCount();
    for (int i = 0 ; i < _totalRows;i++)
    {
        SalomeVariableData _varData = _varList.at(i);
        if (valueChangedByUser(i,_varData.getCurrentValue()))
        {
            QString _value = ( ( QLineEdit *)  ( ui->templateVarTable->cellWidget(i,variableValueCol) ) )->text();

            _varData.setCurrentValue(_value);
            /*if (ui->templateVarTable->item(i,variableOptimizeCol)->checkState() == Qt::Checked)
                _varData.setOptimizedValue("YES");
            else
                _varData.setOptimizedValue("NO");
                */
            _varList[i] = _varData;
        }
    }
    _template->setVariableList(_varList);
}
/**
 * @brief
 *
 * @param _rowPos
 * @return bool
 */
bool SalomeTemplateDialog::valueChangedByUser(int _rowPos, QString _oldValue)
{

    QString _cData;

    QLineEdit * _lineValue = qobject_cast<QLineEdit * > (
                ui->templateVarTable->cellWidget(_rowPos,variableValueCol));
    if (_lineValue)
        _cData = _lineValue->text();


    if (ui->templateVarTable->item(_rowPos,variableChangedCol)->checkState() == Qt::Checked)
        return true;
    else
    {
        if (QString::compare(_oldValue,_cData) != StrEqual)
            return true;
        else
            return false;
    }
}

/**
 * @brief
 *
 */
void SalomeTemplateDialog::on_saveAsButton_clicked()
{
    if (_selectedTemplate.isEmpty())
        return;

    AskForTemplateNameDialog _askForTemplateNameDialog;
    _askForTemplateNameDialog.exec();
    QString _templateName = _askForTemplateNameDialog.getTemplateName();
    if (_templateName.isEmpty())
        return;
//    QString _templateFileName = _askForTemplateNameDialog.getTemplateFileName();
    QString _templateFileName = _templateManager.getTemplateFileNameByTemplateName(_selectedTemplate);
    SalomeTemplateData _templateToSave  = createTemplateFromUserValues(_templateName,_templateFileName);

    QList<SalomeTemplateData> _templateList;

    _templateList = _templateManager.getUserTemplates();
    _templateList.push_back(_templateToSave);
    _templateManager.updateUserTemplateValues(_templateList);
    ui->templateSelComboBox->clear();
    refreshComboTemplates(_templateName);
    _needToSaveUserChanges = false;

}

/**
 * @brief
 *
 * @param _templateName
 */
void SalomeTemplateDialog::refreshComboTemplates(QString _templateName)
{
    ui->templateSelComboBox->clear();
    QStringList _templateNameList;
    _templateNameList.append("");
    _templateNameList.append(_templateManager.getTemplateNameList());
    _templateNameList.append(_templateManager.getUserTemplateNameList());
   ui->templateSelComboBox->addItems(_templateNameList);
   _selectedTemplate = _templateName;
}

/**
 * @brief
 *
 * @param _templateName
 * @param _templateFileName
 * @return SalomeTemplateData
 */
SalomeTemplateData SalomeTemplateDialog::createTemplateFromUserValues(QString _templateName,QString _templateFileName)
{
    SalomeTemplateData _templateResult;
    _templateResult.setTemplateName(_templateName);
    _templateResult.setTemplateFileName(_templateFileName);
    QList<SalomeVariableData> _variableList;
    int _varSize = ui->templateVarTable->rowCount();

    //-----------------------------------
    // The filename variables are the same that those coming from the original template the user modified
    int _posOriginalTemplate = getPosMenu();
    int _indexPos = ui->templateSelComboBox->currentIndex();

    SalomeTemplateData _origTemplate;
    if (_indexPos > _templateManager.getTemplates().size())
        _origTemplate = _templateManager.getUserTemplates().at(_posOriginalTemplate);
    else
        _origTemplate = _templateManager.getTemplates().at(_posOriginalTemplate);
    //-----------------------------------

    for (int i = 0; i < _varSize;i++)
    {
        QString _varName = ui->templateVarTable->item(i,variableNameCol)->text();
        QString _cvalue = ((QLineEdit *) ui->templateVarTable->cellWidget(i,variableValueCol) )->text();
        SalomeVariableData _origVariableData = _origTemplate.getVariableList().at(i);
        QString _isOptimized = "NO";
       // if (ui->templateVarTable->item(i,variableOptimizeCol)->checkState() == Qt::Checked)
       //     _isOptimized = "YES";
        SalomeVariableData _varData;
        _varData.setName(_varName);
        _varData.setFileName(_origVariableData.getFileName());
        _varData.setDefaultValue(_origVariableData.getDefaultValue());
        _varData.setCurrentValue(_cvalue);
        _varData.setOptimizedValue(_isOptimized);
        _variableList.push_back(_varData);
    }
    _templateResult.setVariableList(_variableList);

    return _templateResult;
}


/**
 * @brief
 *
 */
void SalomeTemplateDialog::on_cancelButton_clicked()
{
    close();
}

/**
 * @brief
 *
 */
void SalomeTemplateDialog::on_createMeshButton_clicked()
{
    if (_selectedTemplate.isEmpty())
        return;
     // Check if the changes need to be saved
    if (_needToSaveUserChanges)
        saveData();
    int _posTemplate = -1;
    if (_isUserDefinedTemplate)
        _posTemplate = _templateManager.getUserTemplateNameList().indexOf(_selectedTemplate);
    else
        _posTemplate = _templateManager.getTemplateNameList().indexOf(_selectedTemplate);
    SalomeTemplateData _template;
    if (_posTemplate != -1)
    {
       QList<SalomeTemplateData> _list;
        if (_isUserDefinedTemplate)
            _list = _templateManager.getUserTemplates();
       else
            _list = _templateManager.getTemplates();
         _template = _list.at(_posTemplate);
    }
    if (_template.isEmpty())
        return;


    // Creating the Dialog for Meshing names and executing the phyton file
    SalomeToolCreateMeshDialog _createMeshDialog;
    _createMeshDialog.setSalomeMeshViewerPath(_meshViewerPath);
    _createMeshDialog.setBBIPEDPath(_bbipedPath);

    _createMeshDialog.setProjectPath(_projectPath);
    //_createMeshDialog.registerTextEditor(_regEditor);

    QFileInfo _fileInfo(_projectName);
    _createMeshDialog.setProjectName(_fileInfo.baseName());
    _createMeshDialog.setTemplateData(_template);
    close();
    _createMeshDialog.exec();


}


/**
 * @brief
 *
 * @param index
 */
void SalomeTemplateDialog::on_templateSelComboBox_currentIndexChanged(int index)
{
    _selectedTemplate = ui->templateSelComboBox->itemText(index);
    if (isUserTemplateSelected(index))
    {
        _isUserDefinedTemplate = true;
        ui->deleteTemplateButton->setEnabled(true);
    }else
    {
        _isUserDefinedTemplate = false;
        ui->deleteTemplateButton->setEnabled(false);
    }
    if(_tableHelper.isInitialized())
        _tableHelper.resetTable();
    fillTable(index);
    addSignals();
}

/**
 * @brief
 *
 * @param _pos
 * @return bool
 */
bool SalomeTemplateDialog::isUserTemplateSelected(int _pos)
{
    if (_pos > _templateManager.getTemplateNameList().size())
       return true;
    return false;
}


/**
 * @brief
 *
 */
void SalomeTemplateDialog::addSignals()
{
    bool _foundLineEdit = false;

    int _tableSize = ui->templateVarTable->rowCount();
    if (_tableSize <= 0)
        return;
    for (int i = 0; i < _tableSize; i++)
    {
        QLineEdit * _lineEdit =  qobject_cast<QLineEdit *> (ui->templateVarTable->cellWidget(i,variableValueCol));

        if (_lineEdit)
        {
            connect(_lineEdit,SIGNAL(editingFinished()),_signalMapperTxtEdit,SLOT(map()),Qt::UniqueConnection);
            _signalMapperTxtEdit->setMapping(_lineEdit,QString("%1").arg(i));
            _foundLineEdit = true;
        }


    }
    if (_foundLineEdit)
        connect(_signalMapperTxtEdit, SIGNAL(mapped(const QString &)),this, SLOT(textChangedSlot(const QString &)),Qt::UniqueConnection);


}

/**
 * @brief
 *
 * @param _position
 */
void SalomeTemplateDialog::textChangedSlot(QString _position)
{
    if (!_isLoadingTable)
    {
       int _row = _position.toInt();
       ui->templateVarTable->item(_row,variableChangedCol)->setCheckState(Qt::Checked);
        _needToSaveUserChanges = true;
    }


}
/**
 * @brief
 *
 * @param _position
 */
void SalomeTemplateDialog::changedComboSlot(QString _position)
{

    if (!_isLoadingTable)
    {
       int _row = _position.toInt();
       ui->templateVarTable->item(_row,variableChangedCol)->setCheckState(Qt::Checked);
       _needToSaveUserChanges = true;

    }

}

/*
 * @brief
 *
 * @param row
 * @param column
 void SalomeTemplateDialog::on_templateVarTable_cellChanged(int row, int column)
{
    if ( (!_isLoadingTable) && (column == variableOptimizeCol) )
    {
      ui->templateVarTable->item(row,variableChangedCol)->setCheckState(Qt::Checked);
    }
}*/

/**
 * @brief
 *
 */
void SalomeTemplateDialog::on_dumpMeshButton_clicked()
{
    // TODO
}

/**
 * @brief
 *
 */
void SalomeTemplateDialog::on_dumpGeometryButton_clicked()
{
    // TODO

}

/**
 * @brief
 *
 */
void SalomeTemplateDialog::on_reset2DefaultButton_clicked()
{

    QMessageBox _messageBox;
    int _reset = 0;
    int _posSelTemplate = getPosMenu();
    if (_posSelTemplate < 0)
        return;
    if (isUserTemplateSelected(_posSelTemplate)) // REVISAR ¡¡¡¡
    {
        _messageBox.setText("User defined templates cannot be reseted to default.");
        _messageBox.exec();
        return;
    }else{
        _messageBox.setText("You are about to remove all your configuration to the default values");
        _messageBox.setInformativeText(" Are you sure?");
        _messageBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
        _reset = _messageBox.exec();
    }
    if (_reset == QMessageBox::Yes)
        resetValuesToDefault();

}

/**
 * @brief
 *
 */
void SalomeTemplateDialog::resetValuesToDefault()
{
    // Reset the values
    int _posSelTemplate = getPosMenu();
    if (_posSelTemplate < 0)
        return;
    SalomeTemplateData _templateToReset;
    _templateToReset = _templateManager.getTemplates().at(_posSelTemplate);
    QList<SalomeVariableData> _varList = _templateToReset.getVariableList();
    int _totalRows = ui->templateVarTable->rowCount();
    for (int i = 0 ; i < _totalRows;i++)
    {
        SalomeVariableData _varData = _varList.at(i);
        if (valueChangedByUser(i,_varData.getDefaultValue()))
        {
            QLineEdit * _lineValue = qobject_cast<QLineEdit * > ( ui->templateVarTable->cellWidget(i,variableValueCol));

            if (_lineValue)
                _lineValue->setText(_varData.getDefaultValue());

            ui->templateVarTable->item(i,variableChangedCol)->setCheckState(Qt::Unchecked);
            _needToSaveUserChanges = true;
        }
    }
    restoreTemplateDefaults(_posSelTemplate);

}

/**
 * @brief
 *
 * @param _tempId
 */
void SalomeTemplateDialog::restoreTemplateDefaults(int _tempId)
{
    // At the moment, only restores for the provided templates not the user defined
    _templateManager.restoreToDefaultTemplate(_tempId);

}

/**
 * @brief
 *
 */
void SalomeTemplateDialog::saveData()
{
    int _posTemplateList = getPosMenu();
    int _cPos = ui->templateSelComboBox->currentIndex();
    bool _isUserTemplateModified = false;
    if (_cPos > _templateManager.getTemplateNameList().size())
        _isUserTemplateModified = true;
    SalomeTemplateData _templateToSave;
    QList<SalomeTemplateData> _templateList;
    if (_isUserTemplateModified)
    {
        _templateToSave = _templateManager.getUserTemplates().at(_posTemplateList);
        updateTemplateValues(&_templateToSave);
        _templateList = _templateManager.getUserTemplates();
        _templateList[_posTemplateList]   = _templateToSave;
        _templateManager.updateUserTemplateValues(_templateList);
        _needToSaveUserChanges = false;
    }
    else
    {
        _templateToSave = _templateManager.getTemplates().at(_posTemplateList);
        updateTemplateValues(&_templateToSave);
        _templateList = _templateManager.getTemplates();
        _templateList[_posTemplateList]  = _templateToSave;
        _templateManager.updateTemplateValues(_templateList);
        _needToSaveUserChanges = false;
    }

}

/**
 * @brief
 *
 * @param _editor
 */
void SalomeTemplateDialog::registerConsole(QTextEdit * _editor)
{
    _regEditor = _editor;
}

/**
 * @brief
 *
 * @param _salomePath
 */
void SalomeTemplateDialog::setMeshViewerPath(QString _salomePath)
{
    _meshViewerPath = _salomePath;
}

/**
 * @brief
 *
 * @param _path
 */
void SalomeTemplateDialog::setBBIPEDPath(QString _path)
{
    _bbipedPath = _path;
}
