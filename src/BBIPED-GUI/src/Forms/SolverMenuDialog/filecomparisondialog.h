#ifndef FILECOMPARISONDIALOG_H
#define FILECOMPARISONDIALOG_H

#include <QDialog>
#include <QStringList>
#include <QCheckBox>
#include <QSignalMapper>
#include <QList>
#include <QHash>
#include <QString>
#include <QComboBox>
#include <QWidget>
#include <QLineEdit>
#include <QSignalMapper>

#include "Data/Solver/VO/solversectionvo.h"
#include "Data/ProjectData/VO/projectdetails.h"
#include "Data/Solver/VirtualMRF/virtualmrfvo.h"

namespace Ui {
class FileComparisonDialog;
}

class FileComparisonDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit FileComparisonDialog(QWidget *parent = 0);
    ~FileComparisonDialog();

    void setFilesToCompare(QStringList _list);
    void setSectionMenuList(QList<SolverSectionVO> _list);
    void setProjectDetails(ProjectDetails _details);
    void setVirtualMRFData(QList<VirtualMrfVO> _list);

private slots:
    void on_compareButton_clicked();
    void changedComboValueByUser(QString position);
    void textChangedSlot(QString _position);
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::FileComparisonDialog *ui;
    QStringList _filesToCompare;
    QSignalMapper * _signalMapper;
    QList<bool> _statusSelection;
    QList< QHash<QString,QString> > _loadedValues;
    QList< QHash<QString,QString> > _virtualLoadedValues;
    QHash<QString,QString> _differentValues;
    QList<SolverSectionVO>  _menuSectionList;
   // bool _isLoadingTable;
    QSignalMapper * _signalMapperTxtEdit;
    ProjectDetails _projectDetails;
    QList<VirtualMrfVO> _virtualMRFData;

    void updateTableHeader();
    void resetTable();
    void insertHeaderTitles();
    QCheckBox * createCheckBoxElement(QString _name);
    void loadFileValues(QString _fileName);
    void getDifferences();
    void showValuesInTable();
    void buildRow(QString _keyval, QString _value);
    void fillRowWithData(SolverVariableVO _varVO,QStringList _valueList);

    QString getComparedValuesList(QString _keyValue);
    bool isDifferent(QString _keyValue);
    bool areAllEmpty(QList< QHash<QString,QString> > _list);
    int getIdSmallVariableSet(QList< QHash<QString,QString> > _list, QStringList _removedPositions);
    void hideColumnsView();
    void fillUserValueInCell(SolverVariableVO _varVO, QString _userValue,int _row, int _col);
    void addSignals();
    void addSignalToCombo(QComboBox * _combo, int _idRow, int _idCol);
    void addSignalToLinEdit(QLineEdit * _lineEdit,int _idRow, int _idCol );
    void saveUserChanges(QString _value, int _row, int _col);

};

#endif // FILECOMPARISONDIALOG_H
