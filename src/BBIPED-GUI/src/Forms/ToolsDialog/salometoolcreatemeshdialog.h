#ifndef SALOMETOOLCREATEMESHDIALOG_H
#define SALOMETOOLCREATEMESHDIALOG_H

#include <QDialog>
#include <QString>
#include <QProcess>
#include <QTextEdit>
#include "Data/SalomeTemplateData/salometemplatedata.h"

namespace Ui {
class SalomeToolCreateMeshDialog;
}

class SalomeToolCreateMeshDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SalomeToolCreateMeshDialog(QWidget *parent = 0);
    ~SalomeToolCreateMeshDialog();

    void setTemplateData(SalomeTemplateData _template);
    void setProjectName(QString _name);
    void setProjectPath(QString _path);

    void setSalomeMeshViewerPath(QString _path);
    void setBBIPEDPath(QString _path);
signals:
    void outlogtext(QString ver);

private slots:
    void slotProcessError(QProcess::ProcessError error);
    void outputConsole();
    void errorConsole();
    void meshCreationFinished();

    void on_createMeshButton_clicked();

    void on_finishButton_clicked();

private:
    Ui::SalomeToolCreateMeshDialog *ui;
    SalomeTemplateData _templateData;
    QString _projectName;
    QString _projectPath;
    QString _salomeMeshPath;
    QString _bbipedPath;


    QProcess * _meshCreationProcess;
    QTextEdit * _regEditor;


    void runPythonFile();
    void closeSalomeTool();


};

#endif // SALOMETOOLCREATEMESHDIALOG_H
