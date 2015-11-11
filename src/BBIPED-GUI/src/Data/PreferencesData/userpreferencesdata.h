#ifndef USERPREFERENCESDATA_H
#define USERPREFERENCESDATA_H

#include <QString>
#include <QStringList>

class UserPreferencesData
{
public:
    UserPreferencesData();

    bool isDefaultSolverEngineSelected();
    void setDefaultSolverEngineSelected(bool _val);

    bool isVirtualSolverEngineSelected();
    void setVirtualSolverEngineSelected(bool _val);

    bool isMultizoneSolverEngineSelected();
    void setMultizoneSolverEngineSelected(bool _val);

    QString getUserDefinedSolverPath();
    void setUserDefinedSolverPath(QString _path);

    QString getMeshViewerPath();
    void setMeshViewerPath(QString _path);

    QString getPostProcessorViewerPath();
    void setPostProcessorViewerPath(QString _path);

    QStringList getRecentProjectsList();
    void setRecentProjectsList(QStringList _list);

    void addProjectToList(QString _pathProject);

    bool showCloseWarning();
    void setShowCloseWarning(bool _val);

    bool showCloseProjectWarning();
    void setShowCloseProjectWarning(bool _val);

    QString getTemplateConfigPath();
    void setTemplateConfigPath(QString _file);

    void updateData(UserPreferencesData _newData);


    QString getDefaultParallelEnginePath();
    void setDefaultParallelEnginePath(QString _path);
    QString  getDefaultPartitionEnginePath();
    void setDefaultPartitionEnginePath(QString _path);
    QString getCustomizedParallelEnginePath();
    void setCustomizedParallelEnginePath(QString _path);
    QString getCustomizedPartitionEnginePath();
    void setCustomizedPartitionEnginePath(QString _path);
    bool isDefaultParallelEngineSelected();
    void setDefaultParallelEngineSelected(bool _val);
    bool isDefaultPartitionEngineSelected();
    bool setDefaultPartitionEngineSelected(bool _val);

private:
    bool _isDefaultSolverEngineSelected;
    bool _isVirtualSolverEngineSelected;
    bool _isMultizoneSolverEngineSelected;

    QString _meshViewerPath;
    QString _userDefinedSolverPath;
    QString _postProcessorViewerPath;
    QStringList _recentProjectsList;

    // Parallel Execution
    QString _defaultParallelEnginePath;
    QString  _defaultPartitionEnginePath;
    QString _customizedParallelEnginePath;
    QString _customizedPartitionEnginePath;
    bool _isDefaultParallelEngineSelected;
    bool _isDefaultPartitionEngineSelected;

    // Warning preferences
    bool _showCloseWarning;
    bool _showCloseProjectWarning;

    // Template preferences
    QString _templateConfigPath;


};

#endif // USERPREFERENCESDATA_H
