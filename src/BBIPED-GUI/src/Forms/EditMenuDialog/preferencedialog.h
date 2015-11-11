#ifndef PREFERENCEDIALOG_H
#define PREFERENCEDIALOG_H

#include <QDialog>
#include <QString>
#include <QLineEdit>

//************************
#include "Defines/General/GeneralDefines.h"
#include "Data/PreferencesData/userpreferencesdata.h"



namespace Ui {
class PreferenceDialog;
}

class PreferenceDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit PreferenceDialog(QWidget *parent = 0);
    ~PreferenceDialog();

    /********************************** Initialize ********************************/
    void initialize();
    //void initialize(QString _path);
    void initialize(QLineEdit * _line, QString _path);
    void initialize(UserPreferencesData _userData);

    /********************************** Getter/Setter/Is ********************************/
    UserPreferencesData getUserPreferencesData();

    QString getCustomizedSolverEnginePath();
    QString getCustomizedMeshViewerSelectedPath();
    QString getCustomizedPostProcessorViewerSelectedPath();

    bool isCustomizedSolverSelected();
    bool isPreferencesAccepted();
    bool isCustomizedMeshViewerSelected();
    bool isCustomizedPostProcessorViewerSelectedPath();

    // Mesh & PostProcessors
    void setCustomizedMeshViewerPath(QString _path);
    void setCustomizedPostProcessorViewerSelectedPath(QString _path);

    // Template
    bool isTemplateConfigSelected();
    QString getTemplateConfigPath();
    void setTemplateConfigPath(QString _file);

private slots:

    /********************************** Tool Button/Button Actions********************************/
    void on_customizedSolverEngineToolButton_clicked();
    void on_preferenceButtonBox_accepted();
    void on_preferenceButtonBox_rejected();
    void on_meshViewerConfLineToolButton_clicked();
    void on_deleteMeshViewerConfButton_clicked();


    /***************************** POSTPROCESSOR ACTIONS **************************************/

    void on_postProcessorViewerConfLineToolButton_clicked();
    void on_deletePostProcessorViewerConfButton_clicked();
    void on_resetPreferencesToDefaultButton_clicked();
    void on_warningAppCheckBox_clicked(bool checked);
    void on_warningProjectCheckBox_clicked(bool checked);
    void on_templateConfToolButton_clicked();
    void on_deleteTemplateButton_clicked();

    void on_parallelPartitiontoolButton_clicked();

    void on_parallelEnginetoolButton_clicked();

    void on_clearSolverEngineLocationButton_clicked();

    void on_clearPartitionLocationButton_clicked();

    void on_clearParallelEngineLocationButton_clicked();

private:
    Ui::PreferenceDialog *ui;
    QString _customizedSolverEnginePath;
    QString _customizedMeshViewerPath;
    QString _customizedPostProcessorViewerPath;
    QString _customizedParallelEnginePath;
    QString _customizedPartitionEnginePath;

    bool _isCustomizedSolverSelected;
    bool _isDefaultSolverSelected;
    bool _userAcceptChanges;
    bool _isCustomizedParallelEngineSelected;
    bool _isCustomizedPartitionEngineSelected;
    // Mesh & PostProcessing variables
    bool _isCustomizedMeshViewerSelected;
    bool _isCustomizedPostProcessorViewerPath;
    // Warning Var
    bool _showWarningAppClose;
    bool _showWarningProjectClose;
    // Template Var
    bool _isTemplateConfigSelected;
    QString _templateConfigPath;



   // void enableCustomizedSolverMenu(bool _val);

    /******************************** RESET **************************************/
    void resetSolverEngineTab();
    void resetMeshViewerTab();
    void resetPostProcessorViewerTab();
    void resetTemplateTab();


    /******************************** INITIALIZE **************************************/
    void initializeVariables();
    void initializeWarningData(UserPreferencesData _userData);
    void initializePostProcessorViewerData(UserPreferencesData _userData);
    void initializeMeshViewerData(UserPreferencesData _userData);
    void initializeSolverData(UserPreferencesData _userData);
    void initializeTemplateConfigData(UserPreferencesData _userData);

    /******************************** GETTERs **************************************/
    void getSolverDataConfiguration(UserPreferencesData * _userData);
    void getMeshViewerConfiguration(UserPreferencesData * _userData);
    void getPostProcessorViewerConfiguration(UserPreferencesData * _userData);
    void getWarningConfiguration(UserPreferencesData * _userData);
    void getTemplateConfigPath(UserPreferencesData * _userData);

};

#endif // PREFERENCEDIALOG_H
