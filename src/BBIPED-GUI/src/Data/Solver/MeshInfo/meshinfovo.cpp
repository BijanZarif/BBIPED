#include "meshinfovo.h"

#include "Data/Solver/MeshInfo/meshinfofilewriter.h"

MeshInfoVO::MeshInfoVO()
{
    initializeVariables();
}

void MeshInfoVO::clear()
{
    initializeVariables();
}
void  MeshInfoVO::saveMeshInfoFile()
{
   MeshInfoFileWriter _writer;
   _writer.writeMeshFileWriter(this);
}


void MeshInfoVO::initializeVariables()
{
    _is3DMesh = false;
    _meshFileName = "";
    _meshDetailsFileName = "";
    _meshPath = "";
}

/*********************************************************************************************************/

bool MeshInfoVO::is3DMesh()
{
    return _is3DMesh;
}

void MeshInfoVO::set3DMesh(bool _val)
{
    _is3DMesh  = _val;
}

QStringList MeshInfoVO::getBoundaryNameList()
{
    return _boundaryNameList;
}
QHash<QString,QString> MeshInfoVO::getBoundaryNameListMap()
{
    QHash<QString,QString> _result;
    for (int i = 0; i < _boundaryNameList.size();i++)
        _result.insert(QString::number(i),_boundaryNameList.at(i));
    return _result;
}

void MeshInfoVO::setBoundaryNameList(QStringList _list)
{
    _boundaryNameList = _list;
}
void MeshInfoVO::setBoundaryNameList( QHash<QString,QString> _map)
{
    QHash<QString, QString>::iterator it;
    _boundaryNameList.clear();
    for (it = _map.begin(); it != _map.end(); ++it)
        _boundaryNameList.append(it.value());
}

QString MeshInfoVO::getMeshFileName()
{
    return _meshFileName ;
}

void MeshInfoVO::setMeshFileName(QString _filename)
{
    _meshFileName = _filename;
}

QString MeshInfoVO::getMeshPath()
{
    return _meshPath;
}

void MeshInfoVO::setMeshPath(QString _path)
{
    _meshPath = _path;
}

QString MeshInfoVO::getMeshDetailsFileName()
{
    return _meshDetailsFileName;
}

void MeshInfoVO::setMeshDetailsFileName(QString _meshdetails)
{
    _meshDetailsFileName = _meshdetails;
}
