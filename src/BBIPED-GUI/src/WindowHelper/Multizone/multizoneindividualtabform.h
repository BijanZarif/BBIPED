#ifndef MULTIZONEINDIVIDUALTABFORM_H
#define MULTIZONEINDIVIDUALTABFORM_H

#include <QWidget>
#include <mainwindow.h>
#include "Solver/solverconfigmanager.h"
#include <QHash>
#include <QList>
#include "Data/Solver/VirtualMRF/virtualmrfvo.h"
#include "WindowHelper/MainWindow/inputdatatabhelper.h"

namespace Ui {
class MultizoneIndividualTabForm;
}

class MultizoneIndividualTabForm : public QWidget
{
    Q_OBJECT

public:
    explicit MultizoneIndividualTabForm(QWidget *parent = 0);
    ~MultizoneIndividualTabForm();
    void setConfigFile(QString _filePath);
    QString getConfigFile();

    void setMeshFile(QString _filePath);
    QString getMeshFile();

    QHash<QString, QString> * getConfigValues();
    QList<VirtualMrfVO> * getVirtualValues();

    void setProjectPath(QString _path);
    void registerInitialStructures(MainWindow * _object, SolverConfigManager * _manager);
    bool isDataModified();
    void savedData();

private slots:
    void on_editConfigFileButton_clicked();

    void on_importMeshFileButton_clicked();

    void on_editBoundConfigButton_clicked();

    void on_saveButton_clicked();

    void on_closeButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::MultizoneIndividualTabForm *ui;
    QString _configFile;
    QString _meshFile;
    QString _projectPath;

    // REgistered Structures from parent
    MainWindow * _parentWindow;
    SolverConfigManager * _solverManager;

    // Registered Config Structures
    QHash<QString,QString>*  _configVal;
    QList<VirtualMrfVO>*  _virtualMRFList;
    InputDataTabHelper*  _regTabHelper;

    // Registered Mesh Structures
    QHash<QString,QString>* _bndValuesSolver;
    QHash<QString,QString>* _bndMeshVal;

    bool _needToSave;
    int _tabIndex;



    enum iconSelection{
        iconSaveSepia = 0,
        noIcon = -1
    };

    /******************************************************************************************/
    void closeTab();
    void removeAssociatedMeshValues();
    void getLoadedBoundaryValues();
    void updateBoundaryValues();
    void addSaveIconTab(int _option);





};

#endif // MULTIZONEINDIVIDUALTABFORM_H
