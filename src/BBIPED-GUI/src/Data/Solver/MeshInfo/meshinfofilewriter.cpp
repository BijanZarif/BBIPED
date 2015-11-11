#include "meshinfofilewriter.h"


MeshInfoFileWriter::MeshInfoFileWriter()
{
}

void MeshInfoFileWriter::writeMeshFileWriter(MeshInfoVO * _meshData)
{
    QString _filePath = _meshData->getMeshDetailsFileName();
    QFile _file(_filePath);

    if (_file.exists())
        _file.remove();

    bool _fileCreated = _file.open(QIODevice::ReadWrite | QIODevice::Text);
    if (_fileCreated)
    {
        QXmlStreamWriter xmlWriter(&_file);
        xmlWriter.setAutoFormatting(true);
        xmlWriter.writeStartDocument();

        xmlWriter.writeStartElement(XML_MESH_ROOT);

        xmlWriter.writeStartElement(XML_MESH_3D_NATURE_NODE);
        if (_meshData->is3DMesh())
            xmlWriter.writeCharacters("YES");
        else
            xmlWriter.writeCharacters("NO");
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement(XML_MESH_BND_LIST);
        saveBndValues(&xmlWriter,_meshData->getBoundaryNameList());
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement(XML_MESH_FILE_NAME);
        xmlWriter.writeCharacters(_meshData->getMeshFileName());
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement(XML_MESH_PATH);
        xmlWriter.writeCharacters(_meshData->getMeshPath());
        xmlWriter.writeEndElement();

        xmlWriter.writeEndDocument();

    }
    _file.close();
}

void MeshInfoFileWriter::saveBndValues(QXmlStreamWriter * _xmlWriter, QStringList _varList)
{
    if (_varList.isEmpty())
        return;
    for (int i = 0; i < _varList.size();i++)
    {
        _xmlWriter->writeStartElement(XML_MESH_BND_NAME);
        _xmlWriter->writeCharacters(_varList.at(i));
        _xmlWriter->writeEndElement();
    }
}
