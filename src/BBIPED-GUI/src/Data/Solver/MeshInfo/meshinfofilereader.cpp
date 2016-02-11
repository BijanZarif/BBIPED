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

#include "meshinfofilereader.h"

#include <QFile>
#include <QDir>
#include "Defines/General/GeneralDefines.h"

MeshInfoFileReader::MeshInfoFileReader()
{
}

bool MeshInfoFileReader::checkMeshInfoExist(QString _projectPath, QString _meshDetailsFileName)
{
    QString _filePath = QDir(_projectPath).filePath(QString(_meshDetailsFileName + "." + MESH_CONFIG_SUFFIX));
    QFile _file(_filePath);
     return _file.exists();
}

void MeshInfoFileReader::readMeshInfo(QString _filePath, MeshInfoVO * _meshData)
{
    QFile _file(_filePath);
    bool _fileExist = true;

    _fileExist = _file.open(QIODevice::ReadOnly | QIODevice::Text);

    if (_fileExist)
    {
        QXmlStreamReader _xmlReader(&_file);
        get3DMeshData(&_xmlReader,_meshData);
        getBndNameList(&_xmlReader,_meshData);
        getMeshFileName(&_xmlReader,_meshData);
        getMeshPath(&_xmlReader,_meshData);
        _meshData->setMeshDetailsFileName(_filePath);
    }
    _file.close();
}

/*bool MeshInfoFileReader::get3DMeshInfo(QString _filePath)
{
    QFile _file(_filePath);
    bool _fileExist = true;

    _fileExist = _file.open(QIODevice::ReadOnly | QIODevice::Text);

    if (_fileExist)
    {
        QXmlStreamReader _xmlReader(&_file);
        get3DMeshData(&_xmlReader,_meshData);
    }
    _file.close();

}*/


void MeshInfoFileReader::get3DMeshData(QXmlStreamReader * _xmlReader, MeshInfoVO * _meshData)
{
    bool _finish = false;
    bool _is3DToken = false;
    while ( (!_xmlReader->atEnd()) && (!_xmlReader->hasError()) && (!_finish))
    {
        QXmlStreamReader::TokenType token = _xmlReader->readNext();
        QString _name = _xmlReader->name().toString();
        if ( (!_is3DToken) && (token == QXmlStreamReader::StartElement) &&
            (QString::compare(_xmlReader->name().toString(), XML_MESH_3D_NATURE_NODE ) == StrEqual))
            _is3DToken = true;
        if ((_is3DToken) && (token != QXmlStreamReader::EndElement))
        {
           QString _is3DMesh = _xmlReader->text().toString();
           if (!_is3DMesh.isEmpty())
           {
               if (QString::compare(_is3DMesh,"YES") == StrEqual)
                   _meshData->set3DMesh(true);
               else
                   _meshData->set3DMesh(false);
           }
        }
        if ((_is3DToken) && (token == QXmlStreamReader::EndElement)&&
                (QString::compare(_xmlReader->name().toString(), XML_MESH_3D_NATURE_NODE ) == StrEqual))
            _finish = true;
    }
}

void MeshInfoFileReader::getBndNameList(QXmlStreamReader * _xmlReader, MeshInfoVO * _meshData)
{
    bool _finish = false;
    bool _isBndList = false;

    QStringList _bndList;
    while ( (!_xmlReader->atEnd()) && (!_xmlReader->hasError()) && (!_finish))
    {
        QXmlStreamReader::TokenType token = _xmlReader->readNext();
        QString _tokenName2 = _xmlReader->name().toString();
        if ( (!_isBndList) &&  (token == QXmlStreamReader::StartElement) &&
             (QString::compare(_xmlReader->name().toString(), XML_MESH_BND_LIST ) == StrEqual))
            _isBndList = true;

        if (_isBndList)
        {
            QString _tokenName = _xmlReader->name().toString();
            if ( (token == QXmlStreamReader::StartElement) &&
                (QString::compare(_xmlReader->name().toString(), XML_MESH_BND_NAME ) == StrEqual))
            {
                QString _val = getBndName(_xmlReader);
                _bndList.append(_val);
            }

            if ( (token == QXmlStreamReader::EndElement) &&
                (QString::compare(_xmlReader->name().toString(), XML_MESH_BND_LIST ) == StrEqual))
                _finish = true;
        }
    }
    _meshData->setBoundaryNameList(_bndList);
}


QString MeshInfoFileReader::getBndName(QXmlStreamReader * _xmlReader)
{
    bool _finish = false;
    QString _tokenVal = "";

    while ( (!_xmlReader->atEnd()) && (!_xmlReader->hasError()) && (!_finish))
    {
        QXmlStreamReader::TokenType token = _xmlReader->readNext();
        QString _token = _xmlReader->name().toString();

        if ((token != QXmlStreamReader::EndElement))
            _tokenVal = _xmlReader->text().toString();
        if ((token == QXmlStreamReader::EndElement) &&
                (QString::compare(_xmlReader->name().toString(), XML_MESH_BND_NAME ) == StrEqual))
            _finish= true;

    }
    return _tokenVal;
}

void MeshInfoFileReader::getMeshFileName(QXmlStreamReader * _xmlReader, MeshInfoVO * _meshData)
{
    bool _finish = false;
    bool _isMeshToken = false;
    while ( (!_xmlReader->atEnd()) && (!_xmlReader->hasError()) && (!_finish))
    {
        QXmlStreamReader::TokenType token = _xmlReader->readNext();
        if ( (!_isMeshToken) && (token == QXmlStreamReader::StartElement) &&
            (QString::compare(_xmlReader->name().toString(), XML_MESH_FILE_NAME ) == StrEqual))
                _isMeshToken = true;
        if ((_isMeshToken) && (token != QXmlStreamReader::EndElement) && (!_xmlReader->text().toString().isEmpty()))
        {
            QString _val = _xmlReader->text().toString();
           _meshData->setMeshFileName(_val);
        }
        if ((_isMeshToken) && (token == QXmlStreamReader::EndElement) &&
                (QString::compare(_xmlReader->name().toString(), XML_MESH_FILE_NAME ) == StrEqual))
            _finish = true;

    }

}
void MeshInfoFileReader::getMeshPath(QXmlStreamReader * _xmlReader, MeshInfoVO * _meshData)
{
    bool _finish = false;
    bool _isMeshToken = false;
    while ( (!_xmlReader->atEnd()) && (!_xmlReader->hasError()) && (!_finish))
    {
        QXmlStreamReader::TokenType token = _xmlReader->readNext();
        if ( (!_isMeshToken) && (token == QXmlStreamReader::StartElement) &&
            (QString::compare(_xmlReader->name().toString(), XML_MESH_PATH ) == StrEqual))
                _isMeshToken = true;
        if ((_isMeshToken) && (token != QXmlStreamReader::EndElement) && (!_xmlReader->text().toString().isEmpty()))
        {
            QString _val = _xmlReader->text().toString();
           _meshData->setMeshPath(_val);
        }
        if ((_isMeshToken) && (token == QXmlStreamReader::EndElement) &&
                (QString::compare(_xmlReader->name().toString(), XML_MESH_PATH ) == StrEqual))
            _finish = true;

    }

}
