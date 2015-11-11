#ifndef MULTIZONEPROJECTDATA_H
#define MULTIZONEPROJECTDATA_H

#include <QString>
#include <QStringList>

class MultizoneProjectData
{
public:
    MultizoneProjectData();

    void setProjectPath(QString _path);
    QString getProjectPath();

    void setProjectName(QString _name);
    QString getProjectName();

    void setListConfigFiles(QStringList _list);
    QStringList getListConfigFiles();

    void setListMeshFiles(QStringList _list);
    QStringList getListMeshFiles();

    QStringList getListConfigFileNames();
    QStringList getListMeshFileNames();

    bool isEmpty();
    void updateMultizoneProjectData(MultizoneProjectData _newMultiData);

    void addConfigFile(QString _file);
    void addMeshFile(QString _file);

    void removeConfigFile(QString _file);
    void removeMeshFile(QString _file);

    void updatePair(QString _configPath, QString _meshPath);

private:
    QString _projectPath;
    QString _projectName;
    QStringList _listConfigFiles;
    QStringList _listMeshFiles;
    QStringList _listConfigFileNames;
    QStringList _listMeshFileNames;

    void buildListofFileNames(QStringList _list,int _option);

};

#endif // MULTIZONEPROJECTDATA_H
