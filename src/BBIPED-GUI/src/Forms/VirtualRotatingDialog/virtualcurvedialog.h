#ifndef VIRTUALCURVEDIALOG_H
#define VIRTUALCURVEDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QString>
#include <QStringList>
#include <QSignalMapper>
#include "Data/Solver/VO/solversectionvo.h"

namespace Ui {
class VirtualCurveDialog;
}

class VirtualCurveDialog : public QDialog
{
    Q_OBJECT

public:
    explicit VirtualCurveDialog(QWidget *parent = 0);
    ~VirtualCurveDialog();

    void setInnerValues(QStringList _list);
    QStringList getInnerValues();
    void setOuterValues(QStringList _list);
    QStringList getOuterValues();
    bool hasUserModifiedValues();

    void initializeDialog();
    void set3DCoordinates(bool _val);
    bool is3DCoordinates();
    void setSolverSection(SolverSectionVO _var);


private slots:
    void on_acceptButton_clicked();

    void on_cancelButton_clicked();

    void on_innerCurveSpinBox_valueChanged(int arg1);

    void on_outerCurvespinBox_valueChanged(int arg1);

    void on_selectedDimension_currentIndexChanged(const QString &arg1);

    void on_innerButtonHelp_clicked();

    void on_outerButtonHelp_clicked();

    void textChangedSlot2D(QString _tableName);
    //void textChangedSlot3D(QString _tableName);

private:
    Ui::VirtualCurveDialog *ui;
    QStringList _innerValues;
    QStringList _outerValues;
    bool _userAcceptChanges;
    bool _is3DCoordinates;
    SolverSectionVO _solverSection;
    QSignalMapper * _signalMapperTxtEdit;

    enum ColumnValues{
        Column_X_Value=0,
        Column_Y_Value = 1,
        Column_Z_Value = 2
    };


    enum UpdateOption{
        UpdateInnerCurveVal=0,
        UpdateOuterCurveVal = 1
    };

    void deleteRow(int _rowPos,QTableWidget * _table);
    void addRow(int _rowPos,QTableWidget * _table,QString _valueX, QString _valueY);

    void updateUserValues(int _option);
    void fillTable(QTableWidget * _table, QStringList _listVal);
    QString addItemToTable(QTableWidget * _table,int _row, int _col);
    void generateHelpDialog(int _helpPos);

 //   void enableThirdDimension(QTableWidget * _table);
 //   void disableThirdDimension(QTableWidget * _table);
 //   void updateUserDataInTable();

    void rowAddRemoveSpinBox(int _noZones, QTableWidget * _table);
    void addSignals2D(int _column, QString _tableName);


};

#endif // VIRTUALCURVEDIALOG_H
