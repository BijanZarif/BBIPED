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
#include "filecomparisondialog.h"
#include "ui_filecomparisondialog.h"
#include <QHashIterator>
#include <QFileInfo>


#include "Utility/qtableutil.h"
#include "Parser/Solver/configurationfileparser.h"
#include "Defines/General/GeneralDefines.h"
#include "Data/solversectiondao.h"
#include "Utility/qtableutil.h"
#include "Solver/solverconfigmanager.h"

#define INITIAL_ROW 0
#define INITIAL_COL_VALUES 1

// PENDING: Comparar tambien los valores de VMRF
/**
 * @brief Constructor
 *
 * @param parent
 */
FileComparisonDialog::FileComparisonDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileComparisonDialog)
{
    ui->setupUi(this);
    _signalMapper = new QSignalMapper(this);
    //_isLoadingTable = false;
    _signalMapper = new QSignalMapper(this);
    _signalMapperTxtEdit = new QSignalMapper(this);
}

/**
 * @brief Destructor
 *
 */
FileComparisonDialog::~FileComparisonDialog()
{
    delete ui;
}



/************************************* BUTTON CLICK ****************************************/
/**
 * @brief This function allows to compare the files
 *
 */
void FileComparisonDialog::on_compareButton_clicked()
{
    //_isLoadingTable = true;
    ui->comparisonTable->blockSignals(true); // Block the signals while we add the values
    for (int i = 0; i < _filesToCompare.size();i++)
    {
            QString _fileSelected = _filesToCompare.at(i);
           loadFileValues(_fileSelected);
    }
    getDifferences();
    //getDifferencesAmongVirtualMRFData(); // TO BE DONE (FUTURE...)
    showValuesInTable();
    ui->compareButton->setEnabled(false);
    ui->comparisonTable->blockSignals(false);

    //_isLoadingTable = false;
}



/************************************* LOAD ****************************************/
/**
 * @brief This function loads the values of a file
 *
 * @param _fileName
 */
void FileComparisonDialog::loadFileValues(QString _fileName)
{
    ConfigurationFileParser _configParser;
   _configParser.setFilePath(_fileName);
    _configParser.parseFileProcess();
    QHash<QString,QString>  _userConfigVal = _configParser.getParsedValues();
    _loadedValues.append(_userConfigVal);
     // Adding the data of VirtualMRFData
    QHash<QString,QString> _virtualConfigVal;
    _virtualMRFData = _configParser.getParsedVirtualMRFData();

     for (int i = 0; i<_virtualMRFData.size();i++)
        {
            VirtualMrfVO _virtualVO = _virtualMRFData.at(i);
            QString _keyName = "VMRF_AXIS,"+QString::number(i);
            QString _value = _virtualVO.getZoneName() + COMMA_SEPARATOR + _virtualVO.getOmega() + COMMA_SEPARATOR +
                    _virtualVO.getAxisCenter().join(COMMA_SEPARATOR) + COMMA_SEPARATOR + _virtualVO.getAxisNormal().join(COMMA_SEPARATOR);
            _virtualConfigVal.insert(_keyName,_value);
            if (_virtualVO.getInnerCurvePoints().count() > 0)
            {
                QString _keyName_inner = "VMRF_INNER_CURVE,"+QString::number(i);
                QString _valueInnerCurve = _virtualVO.getZoneName() + COMMA_SEPARATOR + QString::number(_virtualVO.getInnerCurvePoints().count()) +
                        COMMA_SEPARATOR + _virtualVO.getInnerCurvePoints().join(COMMA_SEPARATOR);
                _virtualConfigVal.insert(_keyName_inner,_valueInnerCurve);
            }
            if (_virtualVO.getOuterCurvePoints().count() > 0)
            {
                QString _keyName_outer = "VMRF_OUTTER_CURVE,"+QString::number(i);
                QString _valueOuterCurve = _virtualVO.getZoneName() + COMMA_SEPARATOR + QString::number(_virtualVO.getOuterCurvePoints().count()) +
                        COMMA_SEPARATOR + _virtualVO.getOuterCurvePoints().join(COMMA_SEPARATOR);
                 _virtualConfigVal.insert(_keyName_outer,_valueOuterCurve);
            }
        }
    _virtualLoadedValues.append(_virtualConfigVal);

}


/************************************* TABLE ****************************************/
/**
 * @brief This function updates the Header table
 *
 */
void FileComparisonDialog::updateTableHeader()
{
    int _lastRow = ui->comparisonTable->rowCount();
    ui->comparisonTable->insertRow(_lastRow);
    int _noFileCols = _filesToCompare.size();
    if (_noFileCols >= 2)
    {
        int _initColPos = ui->comparisonTable->columnCount();
        int _noColToInsert =  _noFileCols -_initColPos +1;
        QtableUtil _tableUtil;

        for (int i = 0; i < _noColToInsert; i++)
            ui->comparisonTable->insertColumn(_initColPos+i);
        for (int j = 0; j < _filesToCompare.size();j++)
        {
            QString _fileName = QFileInfo(_filesToCompare.at(j)).fileName();
            ui->comparisonTable->setItem(_lastRow,(_initColPos + j -1),_tableUtil.addNonEditableText(_fileName));
        }
        // Setting the style of the header Rows and columns
        ui->comparisonTable->setSpan(INITIAL_ROW,INITIAL_COL_VALUES,1,_noFileCols);
        ui->comparisonTable->item(INITIAL_ROW,INITIAL_COL_VALUES)->setTextAlignment(Qt::AlignCenter);
        ui->comparisonTable->setSpan(INITIAL_ROW,INITIAL_COL_VALUES-1,2,1);
        ui->comparisonTable->item(INITIAL_ROW,INITIAL_COL_VALUES-1)->setTextAlignment(Qt::AlignCenter);

    }


}

/**
 * @brief This function insert the Header titles
 *
 */
void FileComparisonDialog::insertHeaderTitles()
{
    QtableUtil _tableUtil;
    ui->comparisonTable->insertRow(0);
    ui->comparisonTable->insertColumn(0);
    ui->comparisonTable->insertColumn(1);
    ui->comparisonTable->setItem(0,0,_tableUtil.addNonEditableText("Variable"));
    ui->comparisonTable->setItem(0,1,_tableUtil.addNonEditableText("Values"));
}

/**
 * @brief This function reset the table values
 *
 */
void FileComparisonDialog::resetTable()
{
    if (ui->comparisonTable->rowCount() > 0)
        for (int i = (ui->comparisonTable->rowCount()-1); i >= 0; i--)
            ui->comparisonTable->removeRow(i);
    if (ui->comparisonTable->columnCount() > 2)
        for (int i = (ui->comparisonTable->columnCount()-1); i >= 0; i--)
            ui->comparisonTable->removeColumn(i);

}

/**
 * @brief This function build the individual rows according with the data
 *
 * @param _keyval
 * @param _value
 */
void FileComparisonDialog::buildRow(QString _keyval, QString _value)
{
    QStringList _valueList = _value.split(SEMICOLON_SEPARATOR);
    SolverSectionDAO _dao;
    SolverVariableVO _varVO = _dao.findVariableByFileName(_menuSectionList,_keyval);
    if (!_varVO.isEmpty())
        fillRowWithData(_varVO,_valueList);
}

/**
 * @brief This function fill the row with the data
 *
 * @param _varVO
 * @param _valueList
 */
void FileComparisonDialog::fillRowWithData(SolverVariableVO _varVO,QStringList _valueList)
{
    QtableUtil _tableUtil;
    int _lastRow = ui->comparisonTable->rowCount();
    ui->comparisonTable->insertRow(_lastRow);
    ui->comparisonTable->setItem(_lastRow,0,_tableUtil.addNonEditableText(_varVO.getVariableName()));

    for (int i = 0; i < _valueList.size();i++)
    {
        int _column =  INITIAL_COL_VALUES +i;
        QString _fileValue = _valueList.at(i);
        fillUserValueInCell( _varVO,  _fileValue, _lastRow,  _column);
    }

}

/**
 * @brief This function fill the User Values in the cell
 *
 * @param _varVO
 * @param _userValue
 * @param _row
 * @param _col
 */
void FileComparisonDialog::fillUserValueInCell(SolverVariableVO _varVO, QString _userValue,int _row, int _col)
{
    QtableUtil _tableUtil;
    if ( (_varVO.getVariableFileName().contains("MARKER")) || (_varVO.getVariableFileName().contains("BC_"))|| (_varVO.getComboValuesList().isEmpty()) )
    {
        ui->comparisonTable->setCellWidget(_row,_col,_tableUtil.addEditText(_userValue));
        ui->comparisonTable->setItem(_row,_col,_tableUtil.addEditText2("") );
    }else{
        QComboBox * _combo = _tableUtil.addCombo(_varVO.getComboValuesList(),_varVO.getSelectableComboValues());
        int _indexVal = _combo->findText(_userValue);
        if (_indexVal != -1)
            _combo->setCurrentIndex(_indexVal);
        else
        {
            _combo->insertItem(0,_userValue);
            _combo->setCurrentIndex(0);
        }
        ui->comparisonTable->setCellWidget(_row,_col,_combo);
        ui->comparisonTable->setItem(_row,_col,_tableUtil.addEditText2("") );
    }
}

/**
 * @brief This function shows those values that are different in the table
 *
 */
void FileComparisonDialog::showValuesInTable()
{

    if (!_differentValues.isEmpty())
    {
        QHashIterator<QString,QString> iter(_differentValues);
        while(iter.hasNext())
        {
            iter.next();
            QString _keyValue = iter.key();
            QString _value = iter.value();
            buildRow(_keyValue,_value);
        }
        addSignals();
        ui->comparisonTable->resizeColumnsToContents();
    }
}



/************************************* Getter/Setter ****************************************/

/**
 * @brief Set the section Menu List
 *
 * @param _list
 */
void FileComparisonDialog::setSectionMenuList(QList<SolverSectionVO> _list)
{
    _menuSectionList = _list;
}

/**
 * @brief Set the project details
 *
 * @param _details
 */
void FileComparisonDialog::setProjectDetails(ProjectDetails _details)
{
    _projectDetails = _details;
}

void FileComparisonDialog::setVirtualMRFData(QList<VirtualMrfVO> _list)
{
    _virtualMRFData = _list;
}

/**
 * @brief Set the list of files to be compared
 *
 * @param _list
 */
void FileComparisonDialog::setFilesToCompare(QStringList _list)
{
    _filesToCompare = _list;
    resetTable();
    insertHeaderTitles();
    updateTableHeader();
    ui->comparisonTable->resizeColumnsToContents();
}

/**
 * @brief Get the differences among the files
 *
 */
void FileComparisonDialog::getDifferences()
{
    if ( (!_loadedValues.isEmpty()) && (_loadedValues.size() > 1) )
    {
        QList< QHash<QString,QString> > _loadedValueSetAux = _loadedValues;
        QStringList _removedMapPos;

        bool _stop = areAllEmpty(_loadedValueSetAux);
        int _posMapToCompare =  getIdSmallVariableSet(_loadedValueSetAux,_removedMapPos);
        while(!_stop)
        {
            if (_posMapToCompare != -1)
            {
                QHash<QString,QString> _mapToCompare = _loadedValueSetAux.at(_posMapToCompare);
                QHashIterator<QString,QString> iter(_mapToCompare);
                while(iter.hasNext())
                {
                    iter.next();
                    QString _keyValue = iter.key();
                    if (isDifferent(_keyValue))
                    {
                        QString _comparedValues = getComparedValuesList(_keyValue);
                        _differentValues.insert(_keyValue,_comparedValues);
                    }
                    // Remove the key value
                    for (int i = 0; i < _loadedValueSetAux.size();i++)
                       if (!_loadedValueSetAux.at(i).isEmpty())
                       {
                           QHash<QString,QString> _map = _loadedValueSetAux.at(i);
                           _map.remove(_keyValue);
                           _loadedValueSetAux[i] = _map;
                       }

                }
                // Fill with the other Values

                if (!areAllEmpty(_loadedValueSetAux))
                {
                    if (_posMapToCompare != -1)
                    {
                        _removedMapPos.append(QString::number(_posMapToCompare));
                        _posMapToCompare = getIdSmallVariableSet(_loadedValueSetAux,_removedMapPos);
                        _stop = false;
                    }
                    else
                        _stop = true;
                }
                else
                    _stop = true;
            }
        }

    }
}

/*
void FileComparisonDialog::getDifferencesAmongVirtualMRFData()
{
   if (_virtualLoadedValues.isEmpty())
        return;

    if (_filesToCompare.size() < 2)
        return;

    QHash<QString,QString> _mapToCompare = _virtualLoadedValues.at(0);
    QList< QHash<QString,QString> > _virtualLoadedValues_tmp = _virtualLoadedValues;
    QList<int> _indexEqualElements;
    int _totalFilesToCompare = _filesToCompare.size();
    int _cFileIndex = 1; // Position of the file to be compared to
    // Comparing all the values of one _map to the others ...
   // for (int i = _cFileIndex; i < _totalFilesToCompare;i++)
   // {
        QHashIterator<QString,QString> iter(_mapToCompare);
        while(iter.hasNext())
        {
           // bool _isEqual = true;
            iter.next();
            QString _keyNameFull = iter.key();
            QString _keyName = _keyNameFull.split(",").at(0);
            QString _valueToCompare = iter.value();
            QStringList _valueToCompareList = _valueToCompare.split(",");

            QHash<QString,QString> _mapToBeComparedTo = _virtualLoadedValues_tmp.at(1);
            QHashIterator<QString,QString> iterToCompare(_mapToBeComparedTo);
            while(iterToCompare.hasNext())
            {
                iterToCompare.next();

            }

        }
    //}

}*/

/*QList< QList<int> > FileComparisonDialog::getIndexOfEqualElements(QList< QHash<QString,QString>> _listOfMaps, QString _keyname)
{
   QList< QList<int> > _result;
   int _totalElements = _listOfMaps.size();

   for (int i = 0; i < (_totalElements-1);i++)
   {
     QHash<QString,QString> _map1 = _listOfMaps.at(i);
     QHashIterator<QString,QString> _iter1(_map1);
     QList<int> _indexMap1;

     while(_iter1.hasNext())
     {
         _iter1.hasNext();
         QString _fullKeyName = _iter1.key();
         if (_fullKeyName.contains(_keyname))
         {
            QString _value1 = _iter1.value();

            for (int j = (i+1); j < _totalElements; j++ )
            {
                 QHash<QString,QString> _map2 = _listOfMaps.at(j);
                 QHashIterator<QString,QString> _iter2(_map2);
                 QStringList _equalVals;
                 while(_iter2.hasNext())
                 {
                     _iter2.next();
                     QString _fullKeyName2 = _iter2.key();
                     if (_fullKeyName.contains(_keyname))
                     {
                         QString _value2 = _iter2.value();
                         if (QString::compare(_value1,_value2) == StrEqual)
                         {
                            _equalVals.append(_value2);
                         }

                     }
                 }
            }

         }

     }
   }
  return _result;

}*/

/**
 * @brief Get the list of values for a specific keyvalue
 *
 * @param _keyValue
 * @return QString
 */
QString FileComparisonDialog::getComparedValuesList(QString _keyValue)
{
    QString _result;
    QStringList _resultList;
    for (int i = 0; i < _loadedValues.size(); i++)
    {
        if (!_loadedValues.at(i).isEmpty())
            _resultList.append(_loadedValues.at(i).value(_keyValue));
        else
            _resultList.append("");
    }
    _result = _resultList.join(SEMICOLON_SEPARATOR);

    return _result;
}

/**
 * @brief Check if for a keyvalue are different values
 *
 * @param _keyValue
 * @return bool
 */
bool FileComparisonDialog::isDifferent(QString _keyValue)
{
    bool _isDifferent = false;
    if ( (!_loadedValues.isEmpty()) && (_loadedValues.size() > 1) )
    {
       QString _prevVal = _loadedValues.at(0).value(_keyValue);
       for (int i = 1; i < _loadedValues.size(); i++)
       {
         if (_loadedValues.at(i).isEmpty())
             return true;
         if (QString::compare(_prevVal,_loadedValues.at(i).value(_keyValue)) != StrEqual)
               return true;
       }

    }
    return _isDifferent;
}

/**
 * @brief Check if the lists are all Empty
 *
 * @param QList<QHash<QString
 * @param _list
 * @return bool
 */
bool FileComparisonDialog::areAllEmpty(QList< QHash<QString,QString> > _list)
{
    bool _allEmpty = true;
    if (!_list.isEmpty())
    {
        for (int i = 0; i < _list.size();i++)
            if (!_list.at(i).isEmpty() )
                return false;
    }

    return _allEmpty;
}

/**
 * @brief Get the id of the list with less values
 *
 * @param QList<QHash<QString
 * @param _list
 * @return int
 */
int FileComparisonDialog::getIdSmallVariableSet(QList< QHash<QString,QString> > _list,
                                                QStringList _removedPositions)
{
    int _id = -1;

    if (!_list.isEmpty())
    {
        _id = 0;
        int _sizePrevList = _list.at(0).size();
        for (int i = 0; i < _list.size();i++)
        {
            QString _cPos = QString::number(i);
            if (_sizePrevList > _list.at(i).size())
            {
                if  (!_removedPositions.isEmpty())
                {
                    if (!_removedPositions.contains(_cPos))
                    {
                        _sizePrevList = _list.at(i).size();
                        _id = i;
                    }
                }
                else
                {
                    _sizePrevList = _list.at(i).size();
                    _id = i;
                }

            }
        }
    }
    return _id;
}


/****************************************************** SIGNAL *****************************************/
/**
 * @brief Add signals to the combos and the line edits to control user input
 *
 */
void FileComparisonDialog::addSignals()
{

    bool _foundCombos = false;
    bool _foundLineEdit = false;
    int _tableSize = ui->comparisonTable->rowCount();

    if ( _tableSize > 0)
    {
        for (int i = 0; i< _tableSize;i++)
        {
            for (int j = 0; j < _filesToCompare.size(); j++)
            {
                QWidget * _item;
                int _cPosition = INITIAL_COL_VALUES +j;
                _item = ui->comparisonTable->cellWidget(i,_cPosition);
                if (_item)
                {
                    QComboBox *_comboItem = qobject_cast<QComboBox *> (_item);
                    if (_comboItem)
                    {
                        addSignalToCombo(_comboItem,i, _cPosition);
                        _foundCombos = true;
                    }else
                    {
                        QLineEdit * _lineEdit = qobject_cast<QLineEdit *> (_item);
                        if (_lineEdit)
                        {
                            addSignalToLinEdit(_lineEdit,i,_cPosition);
                            _foundLineEdit = true;
                        }
                    }
                }
            }
        }
    }
    if (_foundCombos)
        connect(_signalMapper, SIGNAL(mapped(const QString &)),this, SLOT(changedComboValueByUser(const QString &)),Qt::UniqueConnection);
    if (_foundLineEdit)
        connect(_signalMapperTxtEdit, SIGNAL(mapped(const QString &)),this, SLOT(textChangedSlot(const QString &)),Qt::UniqueConnection);


}


/**
 * @brief This function add the signal to a combo
 *
 * @param _combo
 * @param _idRow
 * @param _idCol
 */
void FileComparisonDialog::addSignalToCombo(QComboBox * _combo, int _idRow, int _idCol)
{
      if (_combo->isEditable())
        {
            connect(_combo,SIGNAL(editTextChanged(QString)),_signalMapper,SLOT(map()),Qt::UniqueConnection);
            _signalMapper->setMapping(_combo,QString("%1-%2").arg(_idRow).arg(_idCol));
        }
        connect(_combo, SIGNAL(currentIndexChanged(int)), _signalMapper, SLOT(map()),Qt::UniqueConnection);
       _signalMapper->setMapping(_combo, QString("%1-%2").arg(_idRow).arg(_idCol));

}
/**
 * @brief This function add the signal to a line Edit
 *
 * @param _lineEdit
 * @param _idRow
 * @param _idCol
 */
void FileComparisonDialog::addSignalToLinEdit(QLineEdit * _lineEdit,int _idRow, int _idCol )
{
        connect(_lineEdit,SIGNAL(editingFinished()),_signalMapperTxtEdit,SLOT(map()),Qt::UniqueConnection);
        _signalMapperTxtEdit->setMapping(_lineEdit,QString("%1-%2").arg(_idRow).arg(_idCol));
}

/**
 * @brief Slot: Controls if the user change the value in a combo
 *
 * @param _position
 */
void FileComparisonDialog::changedComboValueByUser(QString _position)
{

//    if (!_isLoadingTable)
//    {

        QStringList coordinates = _position.split("-");
        int row = coordinates[0].toInt();
        int col = coordinates[1].toInt();
        QComboBox* combo = (QComboBox*) ui->comparisonTable->cellWidget(row, col);
        QString _value = combo->currentText();
        saveUserChanges(_value,row,col);
  //  }


}

/**
 * @brief Slot: Controls if the user change the value of a line Edit
 *
 * @param _position
 */
void FileComparisonDialog::textChangedSlot(QString _position)
{
//   if (!_isLoadingTable)
//    {
       QStringList coordinates = _position.split("-");
       int row = coordinates[0].toInt();
       int col = coordinates[1].toInt();
       QLineEdit * _lineEdit = (QLineEdit *) ui->comparisonTable->cellWidget(row,col);
       QString _value = _lineEdit->text();
       saveUserChanges(_value,row,col);
//    }
}


/**
 * @brief This function saves the changes made by the user
 *
 * @param _value
 * @param _row
 * @param _col
 */
void FileComparisonDialog::saveUserChanges(QString _value, int _row, int _col)
{
    if (!_differentValues.isEmpty())
    {
        SolverSectionDAO _dao;
        QString _varName = ui->comparisonTable->item(_row,_col)->text();
        QString _keyValue = _dao.findValueNameByVariableName(_menuSectionList,_varName);
        if (_differentValues.contains(_keyValue))
        {
            QString _oldValues = _differentValues.value(_keyValue);
            QStringList _oldValList = _oldValues.split(SEMICOLON_SEPARATOR);
            if (_col < _oldValList.size())
                _oldValList[_col] = _value;
            _differentValues.insert(_keyValue,_oldValList.join(SEMICOLON_SEPARATOR));
        }

    }
}

/**
 * @brief Action that controls when the user accept the changes
 *
 */
void FileComparisonDialog::on_buttonBox_accepted()
{
    SolverConfigManager _solverManager;
    _solverManager.initializeManager();

    for (int i = 0; i < _filesToCompare.size();i++)
    {
         QHash<QString,QString> _mapDiff;
         QHashIterator<QString,QString> iter(_differentValues);
         while(iter.hasNext())
         {
             iter.next();
             QString _keyVal = iter.key();
             QStringList _valueList = iter.value().split(SEMICOLON_SEPARATOR);
             QString _val;
             if (i < _valueList.size())
                 _val = _valueList.at(i);
             _mapDiff.insert(_keyVal,_val);
         }
         _solverManager.saveConfigFile(_filesToCompare.at(i),_mapDiff, _projectDetails,_virtualMRFData);
    }
    close();
}

/**
 * @brief Action that controls when the user reject the changes
 *
 */
void FileComparisonDialog::on_buttonBox_rejected()
{
    close();
}
