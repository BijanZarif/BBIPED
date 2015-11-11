#ifndef MESHINFOFILEWRITER_H
#define MESHINFOFILEWRITER_H
#include <QFile>
#include <QXmlStreamWriter>
#include "Data/Solver/MeshInfo/meshinfovo.h"

#include "Defines/General/MeshInfoDefines.h"

class MeshInfoFileWriter
{
public:
    MeshInfoFileWriter();
    void writeMeshFileWriter(MeshInfoVO * _meshData);

private:
    void saveBndValues(QXmlStreamWriter * _xmlWriter, QStringList _varList);

};

#endif // MESHINFOFILEWRITER_H
