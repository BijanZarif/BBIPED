/***
 *    This file is part of BBIPED GUI.
 *
 *   BBIPED GUI is free software: you can redistribute it and/or modify
 *   it under the terms of the Lesser GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   BBIPED GUI is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   Lesser GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with Foobar. If not, see <http://opensource.org/licenses/LGPL-3.0>.
 *
 *
 * * Author: Dec 2016: Carmen Alonso Montes calonso@bcamath.org
***/

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
