#ifndef MESHINFOVO_H
#define MESHINFOVO_H
#include <QString>
#include <QStringList>
#include <QHash>

class MeshInfoVO
{
public:
    MeshInfoVO();
    bool is3DMesh();
    void set3DMesh(bool _val);

    QStringList getBoundaryNameList();
    QHash<QString,QString> getBoundaryNameListMap();
    void setBoundaryNameList(QStringList _list);
    void setBoundaryNameList( QHash<QString,QString> _map);


    QString getMeshFileName();
    void setMeshFileName(QString _filename);

    QString getMeshPath();
    void setMeshPath(QString _path);

    QString getMeshDetailsFileName();
    void setMeshDetailsFileName(QString _meshdetails);

    void clear();
    void saveMeshInfoFile();

private:
    bool _is3DMesh;
    QStringList _boundaryNameList;
    QString _meshFileName;
    QString _meshPath;
    QString _meshDetailsFileName;    

    void initializeVariables();

};

#endif // MESHINFOVO_H
