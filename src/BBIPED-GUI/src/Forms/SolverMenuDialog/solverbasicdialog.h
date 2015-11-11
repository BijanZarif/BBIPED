#ifndef SOLVERBASICDIALOG_H
#define SOLVERBASICDIALOG_H

#include <QDialog>
#include <QDomDocument>
#include <QModelIndex>
#include <QSignalMapper>
#include <QTableWidgetItem>
#include <QKeyEvent>
#include <QListWidget>
#include <QListWidgetItem>


#include "Solver/solverconfigmanager.h"
#include "Forms/helpdialog.h"
#include "Forms/summarydialog.h"
#include "Searcher/searchedpair.h"
#include "Rule/ruleengine.h"
#include "Searcher/searchpairhelper.h"

#include "Data/Solver/VirtualMRF/virtualmrfvo.h"

namespace Ui {
class SolverBasicDialog;
}

class SolverBasicDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SolverBasicDialog(QWidget *parent = 0);
    ~SolverBasicDialog();

    /************************************ Load ************************************/
     void loadMenu();
//     void loadCheckBoxesStatus();

    /************************************ Reset ************************************/
    bool isReseted();

    /************************************ GETTERS/SETTERs/Is ***********************/
    void setData(SolverConfigManager * _solverManagerData);
    void setBasicView(bool _value);
    void setConfigurationValues(QHash<QString, QString> _values);    
    void enablePersonalizedView();

    QHash<QString, QString> getConfigurationValues();
    bool hasUserMadeChanges();
    void resetDialog();

    void setVirtualMRFData(QList<VirtualMrfVO> _list);
    QList<VirtualMrfVO> getVirtualMRFData();
    void set3DMeshType(bool _val);

private slots:
    /************************************ TREE ACTIONS *******************************/
    void on_solverMenuTree_clicked(const QModelIndex &index);

    /************************************ TABLE ACTIONS ******************************/
    void on_solverVariablesTable_cellClicked(int row, int column);
    void textChangedSlot(QString _position);

    /************************************ BUTTON ACTIONS ******************************/
    void on_resetDefaultButton_clicked();
    void changedComboValueByUser(QString position);
    void on_summarySolverButton_clicked();
    void on_solverGroupButton_accepted();
    void on_solverGroupButton_rejected();
    void on_searchButton_clicked();
    void on_clearSearchButton_clicked();

    void on_basicViewRadioButton_clicked();
    void on_expertViewRadioButton_clicked();
    void on_customizedViewRadioButton_clicked();
    void on_searchVariableNameLabelLineEdit_returnPressed();
    void keyPressEvent(QKeyEvent *evt);

    void handleItem(QString position);

private:
    // Dialog Variables
    Ui::SolverBasicDialog *ui;
    HelpDialog * _helpDialog;
    SummaryDialog *  _summaryDialog;

    // Solver Variables
    SolverConfigManager * _solverManager;
    QHash<QString, QString> _configValues;
    QString _selectedSubMenu;

    // SignalMapper
    QSignalMapper *  _signalMapper;
    QSignalMapper * _signalMapperTxtEdit;
    QSignalMapper * _signalMapperCheckList;
    QSignalMapper * _signalMapperNonEditText;

    // Boolean
    bool _isLoadingTable;
    bool _resetedToDefault;
    bool _showBasicView;

    bool _userHasModifiedValues;
    bool _isVirtualSubMenuSelected;

    // Search Variables
    SearchPairHelper _searchHelper;


    // Rule Variables
    RuleEngine _ruleEngine;


   // QList<int> _specificWindowIndexList;

    // VIRTUAL MRF Variables
    QList<VirtualMrfVO> _virtualUserValues;
    bool _is3DMesh;

    /************************************ INITIALIZE ******************************/
    void intializeVariables();
    void initializeHelpers();


    /************************************ LOAD ************************************/
    void loadTableHeader();
    void fillTable(QString  _parentSelected, QString _childSelected);
    void buildAllToolTips();

    /************************************ RESET **********************************/
    void resetTable();
    void resetAllConfigurationValues();

    /************************************ Save/UPDATE/Change **********************/
    void updateUserValues(SolverSectionVO _solverSecvo);
    void saveUserChanges(QString _value,int _itemRow);
    void changeStatusChecked();

    /************************************ Get/Check ******************************/
    QHash<QString, QString> getUserInput();

    /************************************Find/Search/Signal(Others **********************************/
    void addSignals();
    void addSignalsSpecificWindows(QList<int> _rowIndexList);

    void expandParentChild(int _idParent);



    void updateRadioButtonCheckStatus(bool _val);
    bool checkIsDefaultValue(QString _cValue, int _row);
    void resetToDefaultValue(int _itemRow);
    QString getAssociatedFileNameValue(QString _val, int _itemRow);
    void setEnableCheckBoxes(bool _value);
    void updatePersonalizedMenu();

    void launchSpecificWindow(QString position);
    void launchVirtualRotatingDialog(SolverSectionVO _sectionVO);
    void loadTableHeaderSpecific(QStringList _labelList);
    void updateTableHeaderSpecific();
    void insertRowSpecific(int _row);
    void saveUserChangesSpecificWindow(QString _value,int _itemRow);
    void updateUserValuesSpecific();

    void insertRowSpecificWithoutIcons(int _row);


};

#endif // SOLVERBASICDIALOG_H
