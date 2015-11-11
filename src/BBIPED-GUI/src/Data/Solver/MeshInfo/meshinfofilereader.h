#ifndef MESHINFOFILEREADER_H
#define MESHINFOFILEREADER_H
#include "Defines/General/MeshInfoDefines.h"
#include "Data/Solver/MeshInfo/meshinfovo.h"
#include <QXmlStreamReader>


class MeshInfoFileReader
{
public:
    MeshInfoFileReader();
    void readMeshInfo(QString _filePath, MeshInfoVO * _meshData);
    bool checkMeshInfoExist(QString _projectPath, QString _meshDetailsFileName);

private:
    void get3DMeshData(QXmlStreamReader * _xmlReader, MeshInfoVO * _meshData);
    void getBndNameList(QXmlStreamReader * _xmlReader, MeshInfoVO * _meshData);
    void getMeshFileName(QXmlStreamReader * _xmlReader, MeshInfoVO * _meshData);
    void getMeshPath(QXmlStreamReader * _xmlReader, MeshInfoVO * _meshData);
    QString getBndName(QXmlStreamReader * _xmlReader);

};

#endif // MESHINFOFILEREADER_H
