#ifndef TEMPLATEDIALOG_H
#define TEMPLATEDIALOG_H

#include <QDialog>
#include <QString>
#include <QDomDocument>
#include <QModelIndex>
#include <QHash>
#include <QTableWidget>
#include "Solver/solverconfigmanager.h"
#include "Data/Solver/VO/solvervariablevo.h"
#include "Data/Solver/VO/solversectionvo.h"
#include "Searcher/searchpairhelper.h"
#include "Searcher/searchedpair.h"
#include <QKeyEvent>

namespace Ui {
class TemplateDialog;
}

class TemplateDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit TemplateDialog(QWidget *parent = 0);
    ~TemplateDialog();
    void setSolverConfigManager(SolverConfigManager * _solverManagerData);
    void loadTemplate();

private slots:
    void on_variablesToSelectTree_clicked(const QModelIndex &index);

    void on_variablesToSelectTableWidget_cellClicked(int row, int column);

    void on_variablesToSelectTableWidget_cellChanged(int row, int column);

    void on_selectedVariablesTableWidget_cellClicked(int row, int column);

    void on_searchButton_clicked();

    void on_clearSearchButton_clicked();

    void on_buttonBox_accepted();

    void on_searchLineEdit_returnPressed();
    void keyPressEvent(QKeyEvent *evt);
private:
    Ui::TemplateDialog *ui;
    QDomDocument * _variablesMenuToShow;
    QString _selectedMenu;
    bool _isLoadingTable;
    SolverConfigManager * _solverManager;
    QHash<QString,QString> _selectedVariables;
    SearchPairHelper _searcherHelper;
    bool _userHasMadeChanges;

    // CONSTANTS
    enum ColumnNo{
        ColNoVarToSelect = 3,
        ColNoVarSelected = 4
    };
    enum ColumnSelectedVar{
        SelectedColSectionName = 0,
        SelectedColVariableName = 1,
        SelectedColHelp = 2,
        SelectedColDelete = 3
    };

  //  static const QString COL_HEADER_VAR_TO_SELECT ="Selected;Name of the variable;Help";
  //  static const QString COL_HEADER_VAR_SELECTED ="No;Section;Name of the variable;Help";

    void initializeVariables();
    void initializeTableHeaders();
    void initializeHelpers();

    void loadMenu();
    void setMenuList(QDomDocument * _menu);
    void resetTable(QTableWidget * _table);
    void fillTableVarToSelect(QString  _parentSelected, QString _childSelected);
    void updateSelectedVariableCheckBox(SolverSectionVO _sectionVO);
    void addVariableToTable(SolverVariableVO _variable, int _row );
    void fillMenuTable(QString _parentName, QList<SolverVariableVO> _list);
    void addVariableToTable(QString _name, QString _subsectionName);
    void removeVariableFromTable(QString _name);
    void refreshSelectVarTable(bool _addVar, QString _variableName);
    void loadHelp( QString _helpText);
    void disableVariableInTableToSelect(QString _varName);
    void addVariable(SolverSectionVO _section);


};

#endif // TEMPLATEDIALOG_H
