#ifndef SUMMARYDIALOG_H
#define SUMMARYDIALOG_H

#include <QDialog>
#include <QHash>
#include <QString>
#include <QList>

#include "Data/Solver/VO/solversectionvo.h"
#include "Forms/helpdialog.h"

namespace Ui {
class SummaryDialog;
}

class SummaryDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SummaryDialog(QWidget *parent = 0);
    ~SummaryDialog();

    /************************************* Build **************************************/
    void buildSummaryTable();

    /************************************* Reset/Clear **************************************/
    void resetDialog();

    /************************************* Getter/Setters **************************************/
    void setValuesToShow(QHash<QString,QString> * _valuesMap);
    void setSectionMenuList(QList<SolverSectionVO>  _sectionList);
    bool userHasMadeChanges();

private slots:
    /************************************* CELL CLICKED **************************************/
    void on_summaryTable_cellClicked(int row, int column);

    /************************************* BUTTON CLICKED **************************************/
    void on_summaryButtonGroup_accepted();
    void on_summaryButtonGroup_rejected();

private:
    Ui::SummaryDialog *ui;
    HelpDialog * _helpDialog;
    QHash<QString,QString> * _valuesToShow;
    QList<SolverSectionVO>  _menuSectionList;
    QList<SolverVariableVO> _summaryVariables;
    bool _userHasMadeChanges;

    /************************************* Build **************************************/
    void buildTableHeader();
    void buildRows();

    /*************************************Insert/Update **************************************/
    void insertVariableInTable(SolverVariableVO _varVO,int _row);
    void updateNewValues();

    /************************************* Getter/Setter **************************************/
    int getRowTableByVarName(QString _varName);
    QString getVariableValue(int _row);

};

#endif // SUMMARYDIALOG_H
