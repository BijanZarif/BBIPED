#ifndef SALOMETEMPLATEDIALOG_H
#define SALOMETEMPLATEDIALOG_H

#include <QDialog>
#include <QString>
#include <QSignalMapper>
#include <QTextEdit>
#include "Templates/salometemplatemanager.h"
#include "Utility/qtableutil.h"

namespace Ui {
class SalomeTemplateDialog;
}

class SalomeTemplateDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SalomeTemplateDialog(QWidget *parent = 0);
    ~SalomeTemplateDialog();
    void initializeDialog();

    QString getProjectPath();
    void setProjectPath(QString _path);
    QString getProjectName();
    void setProjectName(QString _name);
    void registerConsole(QTextEdit * _editor);

    void setMeshViewerPath(QString _salomePath);
    void setBBIPEDPath(QString _path);
    
private slots:
    void on_helpButton_clicked();

    void on_deleteTemplateButton_clicked();

    void on_saveButton_clicked();

    void on_saveAsButton_clicked();

    void on_cancelButton_clicked();

    void on_createMeshButton_clicked();

    void on_templateSelComboBox_currentIndexChanged(int index);

    void textChangedSlot(QString _position);

    void changedComboSlot(QString _position);

    //void on_templateVarTable_cellChanged(int row, int column);

    void on_dumpMeshButton_clicked();

    void on_dumpGeometryButton_clicked();

    void on_reset2DefaultButton_clicked();

private:
    Ui::SalomeTemplateDialog *ui;
    SalomeTemplateManager _templateManager;
    QTextEdit * _regEditor;

    QString _projectPath;
    QString _projectName;

    QString _meshViewerPath;
    QString _bbipedPath;

    QString _selectedTemplate;
    QtableUtil _tableHelper;
    bool _isLoadingTable;
    bool _isUserDefinedTemplate;
    bool _needToSaveUserChanges;

    QSignalMapper * _signalMapperTxtEdit;


    enum{
        variableChangedCol = 0,
        variableNameCol = 1,
        variableValueCol = 2
    };

    void registerHelpers();
    void loadTemplateMenu();
    void fillTable(int _templatePosition);
    int getPosMenu();
    void addRow(SalomeVariableData _var);

    void updateTemplateValues(SalomeTemplateData * _template);
    bool valueChangedByUser(int _rowPos, QString _oldValue);

    SalomeTemplateData createTemplateFromUserValues(QString _templateName,QString _templateFileName);
    void refreshComboTemplates(QString _templateName);
    void addSignals();

    bool isUserTemplateSelected(int _pos);

    void resetValuesToDefault();
    void restoreTemplateDefaults(int _tempId);
    void saveData();


};

#endif // SALOMETEMPLATEDIALOG_H
