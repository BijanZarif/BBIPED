#ifndef QTABLEUTIL_H
#define QTABLEUTIL_H
#include <QTableWidget>
#include <QComboBox>
#include <QDomDocument>
#include <QList>
#include <QString>
#include <QStringList>
#include <QLineEdit>
#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>
#include <QList>

class QtableUtil
{
public:
    QtableUtil();

    void registerTable(QTableWidget * _table);
    bool isInitialized();

   // void disableRows(QStringList _indexDisableList);


    /******************************** ADD *************************************/

    QTableWidgetItem * addChangedColumn();
    QTableWidgetItem * addChangedColumn(bool _isSelectable);
    QTableWidgetItem * addVariable(QString _variableName);
    QTableWidgetItem * addVariable(QString _variableName, bool _isSelectable);
    QComboBox * addCombo(QStringList _list, QList<bool> _selectableList);
    QComboBox * addCombo(QStringList _list, QList<bool> _selectableList, bool _isSelectable);
    QTableWidgetItem * addEditText2(QString _value);
    QLineEdit * addEditText(QString _value);
    QLineEdit * addEditText(QString _value, bool _isSelectable);
    QTableWidgetItem * addNonEditableText(QString _value);
    QTableWidgetItem * addNonEditableText(QString _value,bool _isSelectable);
    QTableWidgetItem * addSelectableCheckColumn();

    QTableWidgetItem * addIcon(bool _isSelectable);
    QTableWidgetItem * addIcon();
    QLabel * addIcon(int _idIcon);
    QTableWidgetItem * addEditIcon();
    void addDoubleValidator(QLineEdit * _editTxt);
    QListWidget * addCheckBox(QStringList _list,QString _defaultValue);
    QListWidget * addCheckBox(QStringList _list,QString _defaultValue,bool _isSelectable);

    /******************************** REMOVE *************************************/

    void resetTable();

private:
    QTableWidget * _registeredTable;
    bool _isInitialized;

    void addRows(int _nRows);
    void addColumns( int _nColumns);
    /******************************** Copy *************************************/
   /* void copyTo(QTableWidget * _tableToCopy);
    QComboBox* getCopyOfComboItem(QComboBox * _toBeCopied);
    QTableWidgetItem * getCopyOfVariableItem(QTableWidgetItem * _toBeCopied);
    QTableWidgetItem * getCopyOfCheckStatusItem(QTableWidgetItem * _toBeCopied);*/
};

#endif // QTABLEUTIL_H
