#ifndef XMLUTIL_H
#define XMLUTIL_H

#include <QFile>
#include <QString>
#include <QStringList>
#include <QXmlStreamReader>

#include "Data/ProjectData/VO/projectdata.h"
#include "Data/Solver/VO/solvervo.h"
#include "Data/ProjectData/VO/multizoneprojectdata.h"
#include "Data/ProjectData/VO/projectdetails.h"

class XmlUtil
{
public:

   // Constructor

    XmlUtil();

    /************************************ Create  ************************************/

    bool createInitialXmlProjectFile(QString _filePath, QString _projectPath, QString _projectFileName);
    bool createInitialXmlMultizoneProjectFile(QString _filePath, QString _projectPath, QString _projectFileName);

    void createSU2XmlElements(QString _su2FilePath, QString _su2ConfigFilePath, QXmlStreamWriter * _xmlWriter);
    bool createXmlProjectFile(QString _filePath, QString _projectPath, QString _projectFileName,
                              QString _su2FilePath, QString _su2ConfigFilePath, ProjectDetails _projectDetails);

    /************************************ READ   ************************************/

    ProjectData readXmlProjectFile( QString _projectFilePath);
    ProjectDetails getXmlProjectDetails(QString _projectFilePath);

    /************************************ Getters/Setters/Has/Is  ******************/
    ProjectData getInitialProjectInformation(QXmlStreamReader *xmlReader);
    SolverVO getSU2Information(QXmlStreamReader *xmlReader);

    MultizoneProjectData getInitialMultizoneProjectInformation(QXmlStreamReader *xmlReader);
    MultizoneProjectData readXmlMultizoneProjectFile( QString _projectFilePath);
    void getFileListInformation(QXmlStreamReader *xmlReader,QStringList * _configFileList,
                                         QStringList * _meshFileList);

    bool createXmlMultizoneProjectFile(QString _filePath, MultizoneProjectData _multizoneProjectData,ProjectDetails _projectDetails);

private:
    void createProjectDetailsXmlElements(QXmlStreamWriter * _xmlWriter, ProjectDetails _projectDetails);
    ProjectDetails getProjectDetailsInformation(QXmlStreamReader *xmlReader);

};

#endif // XMLUTIL_H

