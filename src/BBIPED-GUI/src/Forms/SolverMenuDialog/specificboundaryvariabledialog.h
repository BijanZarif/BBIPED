#ifndef SPECIFICBOUNDARYVARIABLEDIALOG_H
#define SPECIFICBOUNDARYVARIABLEDIALOG_H

#include <QDialog>
#include <QString>
#include <QHash>
#include <QSignalMapper>

#include "Solver/markermenumanager.h"

namespace Ui {
class SpecificBoundaryVariableDialog;
}

class SpecificBoundaryVariableDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SpecificBoundaryVariableDialog(QWidget *parent = 0);
    ~SpecificBoundaryVariableDialog();

    /*************************************** Load Values **********************************/
    void loadValues();

    /*************************************** Getter/Setters **********************************/
    void setBoundaryType(QString _newBoundType);
    QString getBoundaryType();

    void setBoundaryName(QString _newBoundName);
    QString getBoundaryName();

    QStringList getConfiguredBoundary();    

    void setMarkerMenuManager(MarkerMenuManager * _manager);

    void setMeshVariables(QHash<QString,QString> _list);

    bool userAcceptChanges();

    void setStoredUserConfigValues(QHash<QString, QString> _userValues);

    void setHelpContent(QString _help);

    void setCurrentValues(QStringList _list);
    QStringList getCurrentValues();

private slots:
    /*************************************** Changed Value **********************************/
    void changedComboValueByUser(QString position);
    void changedLineTextSlot(QString position);

    /*************************************** BUTTON CLICK **********************************/
    void on_boundaryDialogGroupBox_accepted();
    void on_boundaryDialogGroupBox_rejected();
    void on_boundaryHelpButton_clicked();

private:
    Ui::SpecificBoundaryVariableDialog *ui;
    // Boundary and mesh Variables
    QString _boundType;
    QString _boundName;

    QStringList  _configuredBoundary;
    QString _helpContent;
    MarkerMenuManager *  _markerManager;
    QHash<QString,QString>  _meshVariables;
    QHash<QString,QString> _previousBoundConfig;
    QStringList _meshList;
    //Signal
    QSignalMapper * _signalMapper;
    QSignalMapper * _signalMapperTxtEdit;
    // Boolean
    bool _isLoadingTable;
    bool _userAcceptChanges;

    QStringList _cValues;

    /*************************************** Load/Add **********************************/
    void loadUserTableForConfiguration(QString _fileName);
    void loadTableHeader();

    void addRow(SolverMarkerMenu _element);
    void addSignals();
    /*************************************** Save/Update **********************************/
    void updateWithUserValues();//QString _boundValue);

    /*************************************** Getter/Setters **********************************/
    QString getValueFromTable(int _row);
};

#endif // SPECIFICBOUNDARYVARIABLEDIALOG_H
