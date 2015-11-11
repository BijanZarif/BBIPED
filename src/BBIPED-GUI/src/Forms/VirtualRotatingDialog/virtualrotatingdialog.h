#ifndef VIRTUALROTATINGDIALOG_H
#define VIRTUALROTATINGDIALOG_H

#include <QDialog>
#include <QList>
#include <QString>

#include "Data/Solver/VO/solvervariablevo.h"
#include "Data/Solver/VO/solversectionvo.h"
#include "Data/Solver/VirtualMRF/virtualmrfvo.h"

namespace Ui {
class VirtualRotatingDialog;
}

class VirtualRotatingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit VirtualRotatingDialog(QWidget *parent = 0);
    ~VirtualRotatingDialog();

    QList<SolverVariableVO> getSolverVariableInfoList();
    void setSolverVariableInfoList( QList<SolverVariableVO> _list);
    QList<QString> getPreviousZoneInfo();
    void setPreviousZoneInfo(QList<QString> _list);
    QList<QString> getUserDefinedZoneInfo();
    void setUserDefinedZoneInfo(QList<QString> _list);

    void setSelectedSubmenu(QString _selMenu);

    void setSolverSection(SolverSectionVO _element);

    QList<VirtualMrfVO> getUserDefinedVirtualMRFValues();
    void setUserDefinedVirtualMRFValues(QList<VirtualMrfVO> _list);

    bool hasUserAcceptModifiedValues();
    void initializeDialog();
    void fillTable();
    void updateTableWithUserValues();
    void set2DMeshCase(bool _val);


private slots:

    void on_acceptButton_clicked();

    void on_cancelButton_clicked();

    void on_numVirtualZonesSpinBox_valueChanged(int arg1);

    void on_zoneInfoTable_cellClicked(int row, int column);


private:
    Ui::VirtualRotatingDialog *ui;
    SolverSectionVO _solverSection;
    QList<SolverVariableVO> _solverVariableInfoList;
    QList<QString> _previousZoneInfo;
    QList<QString> _userDefinedZoneInfo;
    int _numZones;

    QList<VirtualMrfVO> _resultUserVirtualMRFList;
    bool _deletedByUser;
    bool _is2DMesh;

    enum ColumnTypes{
        ColumnDone=0,
        ColumnZoneName = 1,
        ColumnOmega = 2,
        ColumnAxisCenter =3,
        ColumnAxisNormal = 4,
        ColumnConfigure = 5,
        ColumnDelete =6,
        ColumnHelp=7
    };

    QString _selectedSubmenu;
    bool _userAcceptModifiedValues;


    void addRow(int _rowPos);
    void addRow(int _rowPos,VirtualMrfVO _virtualVO);
    void deleteRow(int _rowPos);
    void launchCurveDialog(int _row);
    void buildAssociatedHelp(int _row);
    void createVirtualMRFData(int _row);
    void updateVirtualInformation();




};

#endif // VIRTUALROTATINGDIALOG_H
