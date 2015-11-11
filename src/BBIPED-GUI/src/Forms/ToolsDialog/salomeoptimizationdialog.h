#ifndef SALOMEOPTIMIZATIONDIALOG_H
#define SALOMEOPTIMIZATIONDIALOG_H

#include <QDialog>
#include <QString>
#include <QList>
#include <QProcess>
#include <QTextEdit>
#include "Templates/salometemplatemanager.h"
#include "Utility/qtableutil.h"
#include "Data/SalomeTemplateData/salomevariabledata.h"
#include "Data/SalomeTemplateData/salometemplatedata.h"


namespace Ui {
class SalomeOptimizationDialog;
}

class SalomeOptimizationDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SalomeOptimizationDialog(QWidget *parent = 0);
    ~SalomeOptimizationDialog();
     void initializeDialog();

     void setTemplateData(QList<SalomeTemplateData> _template);
     void setProjectName(QString _name);
     void setProjectPath(QString _path);
     void setMeshViewerPath(QString _path);
signals:
    void outlogtext(QString ver);
    
private slots:

     void on_templateComboBox_currentIndexChanged(int index);

     void on_noIterationsLineEdit_editingFinished();

     void on_variablesListTable_cellChanged(int row, int column);

     //void on_buttonBox_accepted();

    void slotProcessError(QProcess::ProcessError error);
     void outputConsole();
     void errorConsole();
     void meshCreationFinished();

     void on_pushButton_clicked();

     void on_finishButton_clicked();

private:
    Ui::SalomeOptimizationDialog *ui;
    SalomeTemplateManager _templateManager;
    QString _selectedTemplate;

    QtableUtil _tableHelper;
    QtableUtil _tableSelVarHelper;

    bool _isLoadingTable;

    int _noIterations;

    enum{
        variableChangedCol = 0,
        variableNameCol = 1,
        variableValueCol = 2,
        variableOptimizeCol = 3
    };

    enum{
        selFirstIterCol = 2
    };

    enum{
        selVarName = 0,
        selActualVal = 1,
        initIterationCol = 2
    };

    // Variables for creating the Python file
    QList<SalomeTemplateData> _templateDataList;
    QString _projectName;
    QString _projectPath;
    QString _salomeMeshPath;

    QProcess * _meshCreationProcess;
    QTextEdit * _regEditor;

    void loadTemplateMenu();
    void refreshComboTemplates(QString _templateName);
    void registerHelpers();
    int getPosMenu();
    void addRow(SalomeVariableData _var);
    void fillTable(int _templatePosition);
    void resetColumns();

    void addColumns();
    void updateHeader();
    void addvarToSelectedTable(int _varRow);
    void removevarFromSelectedTable(int _varRow);
    int findRowByVarName(QString _name);
     QList< QList<SalomeVariableData> >  getVariablesList();
     void runPythonFile();


};

#endif // SALOMEOPTIMIZATIONDIALOG_H
