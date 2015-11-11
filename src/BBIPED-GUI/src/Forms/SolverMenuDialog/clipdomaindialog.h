#ifndef CLIPDOMAINDIALOG_H
#define CLIPDOMAINDIALOG_H

#include <QDialog>
#include <QString>
#include <QStringList>

#include "Data/Solver/VO/solvervariablevo.h"

namespace Ui {
class ClipDomainDialog;
}

class ClipDomainDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ClipDomainDialog(QWidget *parent = 0);
    ~ClipDomainDialog();

    void setUserValues(QString _value);
    QString getValues();

    void setSolverVariable(SolverVariableVO _variable);

    void initializeDialog();


private slots:
    void on_doneButton_clicked();

private:
    Ui::ClipDomainDialog *ui;
    QStringList _userValues;
    SolverVariableVO _solverVO;
    bool _loadingTable;

    enum ColumnClipDialog
    {
        ColumnVarName = 0,
        ColumnValue = 1
    };

    void loadTableHeader();
    void fillTable();

};

#endif // CLIPDOMAINDIALOG_H
