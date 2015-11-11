#ifndef MESHDIALOG_H
#define MESHDIALOG_H

#include <QDialog>
#include <QHash>
#include <QString>
#include <QModelIndex>
#include <QSignalMapper>
#include <QComboBox>
#include <QList>


#include "Solver/solverconfigmanager.h"
#include "Solver/markermenumanager.h"
#include "Data/Solver/usermeshvo.h"

namespace Ui {
class MeshDialog;
}

class MeshDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit MeshDialog(QWidget *parent = 0);
    ~MeshDialog();

    void loadMenu();

    /****************** GETTER/SETTERS ***************************/
    void setMeshValues( QHash<QString,QString> _val);

    QHash<QString,QString> getMeshValues();
    QHash<QString,QString> getUserConfiguredMeshValues();
    void setUserConfiguredMeshValues(QHash<QString,QString> _newUserValues);

    void setSolverConfigManager(SolverConfigManager * _manager);
    bool needToSave();
    void setIsNSProblem(bool _val);
    void setIsMultizoneProject(bool _val);
    QHash<QString,QString> getUserBndVariableValues();
    bool hasUserAcceptChanges();

private slots:
    void on_meshButtonBox_accepted();
    void on_meshTreeView_clicked(const QModelIndex &index);
    void changedComboValueByUser(QString position);
    void on_meshButtonBox_rejected();
    void on_meshTableWidget_cellClicked(int row, int column);

private:
    Ui::MeshDialog *ui;
    // Other Variables
    QHash<QString,QString> _meshValues;
    QHash<QString,QString> _configUserMeshValues;
    QHash<QString,QString> _previousUserMeshValues;
    QHash<QString,QString>_storedSpecificBoundValues;
    SolverConfigManager * _solverManager;
    QSignalMapper * _signalMapper;
    QSignalMapper *_signalMapperCheckList;
    QString _selectedSubMenu;
    bool _isLoadingTable;
    MarkerMenuManager * _markerManager;
    bool _needToSave;
    bool _isNSProblem;
    bool _isMultizoneProject;
    bool _userAcceptChanges;

    QHash<QString,QString> _userBndVariableValues;
    QStringList _associatedMeshVariables;
    // List to manage the values of the Variables
    QList<UserMeshVO> _userMeshVariablesList;

    enum ColumnNames{
        ChangedColumn=0,
        BndNameColumn = 1,
        BndTypeColumn = 2,
        HelpIconColumn = 3,
        CurrentValueColumn = 4,
        EditIconColum = 5
    };


    /******************** Initialize FUNCTIONS ******************************/

    void launchSpecificBoundaryConfigWindow(int row,QString _value, QComboBox * _combo,
                                            bool _isEdit);

    /******************** LOAD/FILL FUNCTIONS ******************************/
    void loadTableHeader();
    void fillTable(QString _parent);
    QString buildHelpContent(int row);
    void buildAllToolTips();

    /******************** RESET/REMOVE FUNCTIONS ******************************/
    void resetTable();

    /******************** UPDATE/SAVE FUNCTIONS ******************************/
    void saveUserChanges(QString _value,int _itemRow);
    void updateComboValue(int _rowToUpdate,QString _selVarName);
     void restoreValues(int _prevRow);
    /******************** SIGNALS/Launch FUNCTIONS ******************************/
    void addSignals();

    /******************** GET/SET FUNCTIONS ******************************/
    int getBoundaryRow(QString _boundaryName);
    bool needToOpenSpecificConfigWindow(QString _varName);
    bool isSpecialBoundary(QString _keyVariable);
    void showValuesInValueCol(int row, QString _value);



    void initializeMeshValues();
    void updateTable(QString _userVarName,QStringList _bndNames,QStringList _bndValues);
    void updateTableWithUserValues();

    QStringList splitBndNameAndValues(QStringList _list);
    bool  checkIfAllBoundaryNames(QStringList _list);
    void updateTableWithYesNoValues(QStringList _bndNames,QStringList _bndValues);
    void createMeshVariables(QString _menu, QList<SolverVariableVO> _varList);
    int findMeshVariableToUpdate(QString _nameToFind,QString _menu);
    int findMeshVariableToUpdateByBndValue(QString _bndToFind,QString _menu);
    QString getBndTypeName();
    int findMeshVariableToUpdateByFileNameVar(QString _nameToFind,QString _menu);
    void buildUserBndValueMap();
    QStringList removeEmptyStrings(QStringList _list);
    QList<SolverVariableVO>  removeNonVisibleVariables(QList<SolverVariableVO> _list);
    bool isAllowedTheAssociatedMeshVariables(QString _variableName);
    void removeBoundaryNamesInAssociatedVariables();

    void initBndNamesInAssociatedMeshVar();


};

#endif // MESHDIALOG_H
