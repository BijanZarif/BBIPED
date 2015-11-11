#ifndef WARNINGMULTIZONEDIALOG_H
#define WARNINGMULTIZONEDIALOG_H

#include <QDialog>
#include <QString>
#include <QHash>
#include <QStringList>
#include <QComboBox>
#include <QSignalMapper>
#include "Data/Solver/VO/solversectionvo.h"
#include <QList>
#include "Solver/solverconfigmanager.h"
namespace Ui {
class WarningMultizoneDialog;
}

class WarningMultizoneDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit WarningMultizoneDialog(QWidget *parent = 0);
    ~WarningMultizoneDialog();
    void initializeFileNames(QString _configFile, QString _meshFile);
    void setConfigValues(QHash<QString, QString> _map);
    QHash<QString, QString>  getConfigValues();
    void setMeshValueNames(QHash<QString, QString> _map);
    bool isNeedToUpdate();
    bool keepValues();
    bool userAcceptChanges();
    //void setMenuSectionList(QList<SolverSectionVO> _list);
    
private slots:
    void on_updateRadioButton_clicked();

    void on_keepRadioButton_clicked();

    void on_buttonBox_accepted();
    void changedComboValueByUser(QString _position);
    void on_buttonBox_rejected();

private:
    Ui::WarningMultizoneDialog *ui;
    bool _needToUpdate;
    bool _keepValues;
    bool _userAcceptChanges;
    bool _repairTableFilled;
    QHash<QString,QString> _configValues;
    QHash<QString,QString> _meshValueNames;
    QHash<QString,QString> _userRepairedValues;
    QSignalMapper * _signalMapper;
    bool _isLoadingTable;
    QList<SolverSectionVO>  _menuSectionList;

    void buildRow(QString _markerFileName,QString cValue,QStringList _meshVarNameList,QString _markerKeyName);
    QStringList getMeshVarNames();
    QHash<QString,QString> getMarkerValues();
    void setEnableRepairSection(bool _val);
    void fillRepairTable();

    void addSignalToCombo(QComboBox * _combo, int _idRow, int _idCol);
    void updateConfigurationValues();
    QStringList buildList(QHash<QString,QString> _map);
    QString removeBrakets(QString _text);


};

#endif // WARNINGMULTIZONEDIALOG_H
