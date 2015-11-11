#ifndef MULTIZONESUMMARYDIALOG_H
#define MULTIZONESUMMARYDIALOG_H

#include <QDialog>
#include <QString>
#include <QStringList>
#include <QHash>
#include <QList>
#include "Data/Solver/VirtualMRF/virtualmrfvo.h"
#include "Solver/solverconfigmanager.h"

namespace Ui {
class MultizoneSummaryDialog;
}

class MultizoneSummaryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MultizoneSummaryDialog(QWidget *parent = 0);
    ~MultizoneSummaryDialog();
    void setConfigFileList(QStringList _list);
    void setProjectPath(QString _path);
    void setSolverManager(SolverConfigManager * _manager);

private slots:
    void on_okButton_clicked();

    void on_selectConfigFilesTable_cellClicked(int row, int column);

private:
    Ui::MultizoneSummaryDialog *ui;
    QString _projectPath;
    QStringList _configFileList;
    int _selectedRow;
    SolverConfigManager *_solverManager;

    enum configTableColumns{
        checkBoxColumn = 0,
        configFileNameCol =1
    };
    enum variablesTableColumsn{
        variableNameCol = 0,
        variableValueCol =1,
        variableUnitCol = 2

    };

    void fillConfigTable();
    void addConfigData(QString _file);
    void fillDataTable(QHash<QString,QString> * _mapValues, QList<VirtualMrfVO> * _virtualList);
    void insertRow(int _pos, QString _key, QString _value, QString _unit);
    void removeDefaultValues(QHash<QString,QString> * _configVal);
    void resetTable();


};

#endif // MULTIZONESUMMARYDIALOG_H
