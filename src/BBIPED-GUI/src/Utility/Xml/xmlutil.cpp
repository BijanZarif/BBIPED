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
 *  *  Author: Dec 2016: Carmen Alonso Montes calonso@bcamath.org

***/
#include "xmlutil.h"

#include <QXmlStreamWriter>

// Defines
#include "Defines/General/GeneralDefines.h"
#include <QFileInfo>
/**
 * @brief
 *
 */
XmlUtil::XmlUtil()
{
}
/************************************************** CREATE ****************************************/
/**
 * @brief Function that creates the project XML file initial
 *
 * @param _filePath
 * @param _projectPath
 * @param _projectFileName
 * @return bool
 */
bool XmlUtil::createInitialXmlProjectFile(QString _filePath, QString _projectPath, QString _projectFileName)
{
    QFile _file(_filePath);
    bool _fileCreated = false;
    if (!_file.exists())
    {
         _fileCreated = _file.open(QIODevice::ReadWrite);
        if (_fileCreated)
        {
            QXmlStreamWriter xmlWriter(&_file);
            xmlWriter.setAutoFormatting(true);
            xmlWriter.writeStartDocument();

            xmlWriter.writeStartElement(PROJECT_ELEMENT);

            xmlWriter.writeStartElement(PROJECT_DATA_TAG);
                xmlWriter.writeAttribute(PROJECT_PATH_ATT,_projectPath);
                xmlWriter.writeAttribute(PROJECT_FILENAME_ATT,_projectFileName);
                xmlWriter.writeAttribute(PROJECT_FULLPATH_ATT,_filePath);
            xmlWriter.writeEndElement();


            // Adding Solver structure
            createSU2XmlElements(INITIAL_PATH, INITIAL_PATH,&xmlWriter);

            // Adding Project Details
            ProjectDetails _newProjectDetails;
            createProjectDetailsXmlElements(&xmlWriter,_newProjectDetails);
            xmlWriter.writeEndElement();

            // Closing the root and the document
            xmlWriter.writeEndElement();
            xmlWriter.writeEndDocument();
            _file.close();
        }
    }
    return _fileCreated;

}


/**
 * @brief Function that creates the project XML file initial for Multizone project
 *
 * @param _filePath
 * @param _projectPath
 * @param _projectFileName
 * @return bool
 */
bool XmlUtil::createInitialXmlMultizoneProjectFile(QString _filePath, QString _projectPath, QString _projectFileName)
{
    QFile _file(_filePath);
    bool _fileCreated = false;
    if (!_file.exists())
    {
         _fileCreated = _file.open(QIODevice::ReadWrite);
        if (_fileCreated)
        {
            QXmlStreamWriter xmlWriter(&_file);
            xmlWriter.setAutoFormatting(true);
            xmlWriter.writeStartDocument();

            xmlWriter.writeStartElement(PROJECT_ELEMENT);

            xmlWriter.writeStartElement(PROJECT_DATA_TAG);
                xmlWriter.writeAttribute(PROJECT_PATH_ATT,_projectPath);
                xmlWriter.writeAttribute(PROJECT_FILENAME_ATT,_projectFileName);
                xmlWriter.writeAttribute(PROJECT_FULLPATH_ATT,_filePath);
            xmlWriter.writeEndElement();

            xmlWriter.writeStartElement(SOLVER_LIST_ELEMENT);
            // Adding Solver structure
            createSU2XmlElements(INITIAL_PATH, INITIAL_PATH,&xmlWriter);

            // Adding Project Details
            ProjectDetails _newProjectDetails;
            createProjectDetailsXmlElements(&xmlWriter,_newProjectDetails);
            xmlWriter.writeEndElement();

            xmlWriter.writeEndElement();

            xmlWriter.writeEndDocument();
            _file.close();
        }
    }
    return _fileCreated;
}

/**
 * @brief Function that creates and updates if needed the XML project file
 *
 * @param _filePath
 * @param _projectPath
 * @param _projectFileName
 * @param _su2FilePath
 * @param _su2ConfigFilePath
 * @return bool
 */
bool XmlUtil::createXmlProjectFile(QString _filePath, QString _projectPath, QString _projectFileName, QString _su2FilePath,
                                   QString _su2ConfigFilePath,ProjectDetails _projectDetails)
{
    QFile _file(_filePath);
    bool _fileCreated = false;

    if (_file.exists())
        _file.remove();

    _fileCreated = _file.open(QIODevice::ReadWrite);
    if (_fileCreated)
    {
        QXmlStreamWriter xmlWriter(&_file);
        xmlWriter.setAutoFormatting(true);
        xmlWriter.writeStartDocument();

        xmlWriter.writeStartElement(PROJECT_ELEMENT);

        xmlWriter.writeStartElement(PROJECT_DATA_TAG);
            xmlWriter.writeAttribute(PROJECT_PATH_ATT,_projectPath);
            xmlWriter.writeAttribute(PROJECT_FILENAME_ATT,_projectFileName);
            xmlWriter.writeAttribute(PROJECT_FULLPATH_ATT,_filePath);
        xmlWriter.writeEndElement();

        // Adding SU2 structure
        createSU2XmlElements(_su2FilePath, _su2ConfigFilePath,&xmlWriter);

        // Add the Project Details
        createProjectDetailsXmlElements(&xmlWriter,_projectDetails);
        // Ending the root and the document
        xmlWriter.writeEndElement();
        xmlWriter.writeEndDocument();
        _file.close();
    }
  //  }
    return _fileCreated;

}


/**
 * @brief Function that creates the project details element from the xml
 *
 * @param _xmlWriter
 * @param _projectDetails
 */
void XmlUtil::createProjectDetailsXmlElements(QXmlStreamWriter * _xmlWriter, ProjectDetails _projectDetails)
{
    _xmlWriter->writeStartElement(PROJECT_DETAIL_TAG);
        _xmlWriter->writeAttribute(PROJECT_DETAIL_TITLE_TAG,_projectDetails.getProjectTile());
        _xmlWriter->writeAttribute(PROJECT_DETAIL_AUTHOR_TAG,_projectDetails.getAuthor());
        _xmlWriter->writeAttribute(PROJECT_DETAIL_STARTINGDATE_TAG,_projectDetails.getStartingDate());
        _xmlWriter->writeAttribute(PROJECT_DETAIL_OTHERDETAILS_TAG,_projectDetails.getOtherDetails());
    _xmlWriter->writeEndElement();
}

/**
 * @brief Function that creates the multizone project details element from the xml
 *
 * @param _filePath
 * @param _multizoneProjectData
 * @return bool
 */
bool XmlUtil::createXmlMultizoneProjectFile(QString _filePath, MultizoneProjectData _multizoneProjectData, ProjectDetails _projectDetails)
{
    QFile _file(_filePath);
    bool _fileCreated = false;

    if (_file.exists())
        _file.remove();

    _fileCreated = _file.open(QIODevice::ReadWrite);
    if (_fileCreated)
    {
        QXmlStreamWriter xmlWriter(&_file);
        xmlWriter.setAutoFormatting(true);
        xmlWriter.writeStartDocument();

        xmlWriter.writeStartElement(PROJECT_ELEMENT);

        xmlWriter.writeStartElement(PROJECT_DATA_TAG);
            xmlWriter.writeAttribute(PROJECT_PATH_ATT,_multizoneProjectData.getProjectPath());
            QString _projectNameWithExt = _multizoneProjectData.getProjectName() + "." + MULTIZONE_PROJECT_FILE_EXT;
            xmlWriter.writeAttribute(PROJECT_FILENAME_ATT,_projectNameWithExt);
            xmlWriter.writeAttribute(PROJECT_FULLPATH_ATT,_filePath);
        xmlWriter.writeEndElement();

        // Adding SU2 structure
         xmlWriter.writeStartElement(SOLVER_LIST_ELEMENT);
         QStringList _configFiles =  _multizoneProjectData.getListConfigFiles();
         QStringList _meshFiles = _multizoneProjectData.getListMeshFiles();
         int _totalsize =_configFiles.size();
         if (_configFiles.size() != _meshFiles.size())
             if (_configFiles.size() > _meshFiles.size())
                 _totalsize = _meshFiles.size();

         for (int i = 0; i < _totalsize; i++)
             createSU2XmlElements(_meshFiles.at(i), _configFiles.at(i),&xmlWriter);
         xmlWriter.writeEndElement();
        // Adding the project details
         createProjectDetailsXmlElements(&xmlWriter,_projectDetails);

        // Closing the root and the document
        xmlWriter.writeEndElement();
        xmlWriter.writeEndDocument();
        _file.close();
    }
  //  }
    return _fileCreated;

}
/**
 * @brief Function that creates the correspondent Solver elements to be included in the XML project file
 *
 * @param _su2FilePath
 * @param _su2ConfigFilePath
 * @param _xmlWriter
 */
void XmlUtil::createSU2XmlElements(QString _su2FilePath, QString _su2ConfigFilePath, QXmlStreamWriter * _xmlWriter)
{
    _xmlWriter->writeStartElement(SU2_ELEMENT);
        _xmlWriter->writeStartElement(SU2_FILE_ELEMENT);
        _xmlWriter->writeAttribute(PATH_ATT,_su2FilePath);
        _xmlWriter->writeEndElement();

        _xmlWriter->writeStartElement(SU2_CONFIG_FILE_ELEMENT);
        _xmlWriter->writeAttribute(PATH_ATT,_su2ConfigFilePath);
        _xmlWriter->writeEndElement();

    _xmlWriter->writeEndElement();
}


/************************************************** READ ****************************************/

/**
 * @brief Function to read the XML Project file
 *
 * @param _projectFilePath
 * @return ProjectData
 */
ProjectData XmlUtil::readXmlProjectFile( QString _projectFilePath)
{
    QFile _file(_projectFilePath);
    bool _fileRead = true;
    ProjectData _projectData;
    SolverVO _solverData;

    _fileRead = _file.open(QIODevice::ReadOnly | QIODevice::Text);

    if (_fileRead)
    {
        QXmlStreamReader xmlReader(&_file);
        _projectData = getInitialProjectInformation(&xmlReader);
       //TODO: Read the rest of the Xml File and load it into the GUI

        // SU2 reading
        _solverData = getSU2Information(&xmlReader);
        _projectData.setSolverDataInformation(_solverData);
    }
    _file.close();
    return _projectData;
}

/**
 * @brief Function to read the XML Multizone Project file
 *
 * @param _projectFilePath
 * @return MultizoneProjectData
 */
MultizoneProjectData XmlUtil::readXmlMultizoneProjectFile( QString _projectFilePath)
{
    QFile _file(_projectFilePath);
    bool _fileRead = true;
    MultizoneProjectData _multizoneProjectData;

    _fileRead = _file.open(QIODevice::ReadOnly | QIODevice::Text);

    if (_fileRead)
    {
        QXmlStreamReader xmlReader(&_file);
        _multizoneProjectData = getInitialMultizoneProjectInformation(&xmlReader);
        QStringList _configFileList;
        QStringList _meshFileList;
        // Solver List information
        getFileListInformation(&xmlReader,&_configFileList,&_meshFileList);
        _multizoneProjectData.setListConfigFiles(_configFileList);
        _multizoneProjectData.setListMeshFiles(_meshFileList);
    }
    _file.close();
    return _multizoneProjectData;
}


/************************************************** Getter/Setter ****************************************/

/**
 * @brief Function to read the XML Project Details file
 *
 * @param _projectFilePath
 * @return ProjectDetails
 */
ProjectDetails XmlUtil::getXmlProjectDetails(QString _projectFilePath)
{
    QFile _file(_projectFilePath);
    bool _fileRead = true;
    ProjectDetails _projectDetails;

    _fileRead = _file.open(QIODevice::ReadOnly | QIODevice::Text);

    if (_fileRead)
    {
        QXmlStreamReader xmlReader(&_file);
        _projectDetails = getProjectDetailsInformation(&xmlReader);
   }
    _file.close();
    return _projectDetails;
}

/**
 * @brief Function to get the XML Project Details file
 *
 * @param xmlReader
 * @return ProjectDetails
 */
ProjectDetails XmlUtil::getProjectDetailsInformation(QXmlStreamReader *xmlReader)
{
  ProjectDetails _projectDetails;
  bool endInformation = false;
   while ( (!xmlReader->atEnd()) && (!xmlReader->hasError()) && (!endInformation))
   {
       QXmlStreamReader::TokenType token = xmlReader->readNext();
       if (token == QXmlStreamReader::StartDocument) continue;
       if (token == QXmlStreamReader::StartElement)
       {
           if (QString::compare(xmlReader->name().toString(), PROJECT_DETAIL_TAG) == StrEqual){
               QXmlStreamAttributes _elementAtt = xmlReader->attributes();
               if (_elementAtt.hasAttribute(PROJECT_DETAIL_TITLE_TAG))
                  _projectDetails.setProjectTile(_elementAtt.value(PROJECT_DETAIL_TITLE_TAG).toString());
               if (_elementAtt.hasAttribute(PROJECT_DETAIL_AUTHOR_TAG))
                   _projectDetails.setAuthor(_elementAtt.value(PROJECT_DETAIL_AUTHOR_TAG).toString());
               if (_elementAtt.hasAttribute(PROJECT_DETAIL_STARTINGDATE_TAG))
                   _projectDetails.setStartingDate(_elementAtt.value(PROJECT_DETAIL_STARTINGDATE_TAG).toString());
               if (_elementAtt.hasAttribute(PROJECT_DETAIL_OTHERDETAILS_TAG))
                   _projectDetails.setOtherDetails(_elementAtt.value(PROJECT_DETAIL_OTHERDETAILS_TAG).toString());

               return _projectDetails;
       }
      }

   }
   return _projectDetails;
}

/**
   * @brief Function to get the all the information of the project
   *
   * @param xmlReader
   * @return ProjectData
   */
  ProjectData XmlUtil::getInitialProjectInformation(QXmlStreamReader *xmlReader)
 {
     ProjectData _projectData;
     bool endInformation = false;

     while ( (!xmlReader->atEnd()) && (!xmlReader->hasError()) && (!endInformation))
     {
         QXmlStreamReader::TokenType token = xmlReader->readNext();
         if (token == QXmlStreamReader::StartDocument) continue;
         if (token == QXmlStreamReader::StartElement)
         {
             if (QString::compare(xmlReader->name().toString(), PROJECT_DATA_TAG) == StrEqual)
             {
                QXmlStreamAttributes _elementAtt = xmlReader->attributes();
                if (_elementAtt.hasAttribute(PROJECT_PATH_ATT))
                   _projectData.setProjectPath(_elementAtt.value(PROJECT_PATH_ATT).toString());

                if (_elementAtt.hasAttribute(PROJECT_FILENAME_ATT))
                    _projectData.setProjectFileName(_elementAtt.value(PROJECT_FILENAME_ATT).toString());

                return _projectData;
             }
         }

     }
     return _projectData;
 }

 /**
  * @brief  Function to get the all the information of the multizone project
  *
  * @param xmlReader
  * @return MultizoneProjectData
  */
 MultizoneProjectData XmlUtil::getInitialMultizoneProjectInformation(QXmlStreamReader *xmlReader)
 {
     MultizoneProjectData _projectData;
     bool endInformation = false;

     while ( (!xmlReader->atEnd()) && (!xmlReader->hasError()) && (!endInformation))
     {
         QXmlStreamReader::TokenType token = xmlReader->readNext();
         if (token == QXmlStreamReader::StartDocument) continue;
         if (token == QXmlStreamReader::StartElement)
         {
             if (QString::compare(xmlReader->name().toString(), PROJECT_DATA_TAG) == StrEqual)
             {
                QXmlStreamAttributes _elementAtt = xmlReader->attributes();
                if (_elementAtt.hasAttribute(PROJECT_PATH_ATT))
                {
                   _projectData.setProjectPath(_elementAtt.value(PROJECT_PATH_ATT).toString());

                }
                if (_elementAtt.hasAttribute(PROJECT_FILENAME_ATT))
                {
                    QString _fileNameBase = QFileInfo(_elementAtt.value(PROJECT_FILENAME_ATT).toString()).baseName();
                    _projectData.setProjectName(_fileNameBase);
                }
                return _projectData;
             }
         }

     }
     return _projectData;
 }


 /**
  * @brief  Function to get the all the information of config and mesh lists
  *
  * @param xmlReader
  * @param _configFileList
  * @param _meshFileList
  */
 void XmlUtil::getFileListInformation(QXmlStreamReader *xmlReader,QStringList * _configFileList,
                                      QStringList * _meshFileList)
 {
     bool _foundSolverTag = false;
     while ( (!xmlReader->atEnd()) && (!xmlReader->hasError()) )
     {
         QXmlStreamReader::TokenType token = xmlReader->readNext();
         if (token == QXmlStreamReader::StartDocument) continue;
         if (token == QXmlStreamReader::StartElement)
         {
             if (QString::compare(xmlReader->name().toString(), SU2_ELEMENT) == StrEqual)
             {
                 _foundSolverTag = true;
                 continue;
             }
             if (_foundSolverTag)
             {
                 if (QString::compare(xmlReader->name().toString(), SU2_FILE_ELEMENT) == StrEqual)
                 {
                         QXmlStreamAttributes _elementAtt = xmlReader->attributes();
                         if (_elementAtt.hasAttribute("path"))
                             _meshFileList->append(_elementAtt.value("path").toString());
                  }

                 if (QString::compare(xmlReader->name().toString(), SU2_CONFIG_FILE_ELEMENT) == StrEqual)
                 {
                         QXmlStreamAttributes _elementAtt = xmlReader->attributes();
                         if (_elementAtt.hasAttribute("path"))
                          _configFileList->append(_elementAtt.value("path").toString());
                  }
             }
         }

     }
 }

 /**
  * @brief Function to get the Solver information
  *
  * @param xmlReader
  * @return SolverVO
  */
 SolverVO XmlUtil::getSU2Information(QXmlStreamReader *xmlReader)
 {
     SolverVO _solverObj;
     bool _foundSu2Tag = false;
     int _su2TagsProcessed = 0;
     bool endInformation = false;

     while ( (!xmlReader->atEnd()) && (!xmlReader->hasError()) && (!endInformation))
     {
         QXmlStreamReader::TokenType token = xmlReader->readNext();
         if (token == QXmlStreamReader::StartDocument) continue;
         if (token == QXmlStreamReader::StartElement)
         {
             if (QString::compare(xmlReader->name().toString(), SU2_ELEMENT) == StrEqual)
             {
                 _foundSu2Tag = true;
                 continue;
             }
             if (_foundSu2Tag)
             {
                 if (QString::compare(xmlReader->name().toString(), SU2_FILE_ELEMENT) == StrEqual)
                 {
                         QXmlStreamAttributes _elementAtt = xmlReader->attributes();
                         if (_elementAtt.hasAttribute("path"))
                         {
                            _solverObj.setSolverFilePath(_elementAtt.value("path").toString());
                            _su2TagsProcessed++;
                         }
                  }

                 if (QString::compare(xmlReader->name().toString(), SU2_CONFIG_FILE_ELEMENT) == StrEqual)
                 {
                         QXmlStreamAttributes _elementAtt = xmlReader->attributes();
                         if (_elementAtt.hasAttribute("path"))
                         {
                            _solverObj.setSolverConfigFilePath(_elementAtt.value("path").toString());
                            _su2TagsProcessed++;
                         }
                  }
             }
         }
         else
           if ((_foundSu2Tag) && (token == QXmlStreamReader::EndElement) && (_su2TagsProcessed == 2))
              endInformation = true;
     }
     return _solverObj;
 }

