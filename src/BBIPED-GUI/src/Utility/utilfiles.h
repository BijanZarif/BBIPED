#ifndef UTILFILES_H
#define UTILFILES_H

#include <QString>
//#include <QDir>
#include <QFileDialog>

#include "Data/ProjectData/VO/projectdata.h"
#include "Data/ProjectData/VO/multizoneprojectdata.h"
#include "../Data/ProjectData/VO/projectdetails.h"



class UtilFiles
{
public:

    // Constructor
    UtilFiles();

    /************************************ Project Functions ************************************/
    MultizoneProjectData openMultizoneProjectStructure(QString _projectFilePath);

    /**** Project Data Structure CREATION */

    bool createProjectStructure(QString _projectFilePath);
    bool createProjectFile(QString _baseFileName,QString _finalPath);
    void createProjectSubfolders(QString _basePath);
    bool createMultizoneProjectFile(QString _baseFileName,QString _finalPath);
    bool createMultizoneProjectStructure(QString _projectFilePath);


    /**** Project Data Structure OPEN */
    ProjectData openProjectStructure(QString _projectFilePath);
    ProjectDetails openProjectDetails(QString _projectFilePath);


    /**** Project Data Structure UPDATE */
     void updateProjectDataToFile(ProjectData _projectDataToSave,ProjectDetails _projectDetails);
     ProjectData updateToolPaths(ProjectData _oldProjectData,ProjectData _newProjectData);


    /************************************ COPY/Import ************************************/
     void copyAllToolData(ProjectData _oldProjectData,ProjectData _newProjectData);
     void copyAllFiles(QDir _oldPath, QDir _newPath, QStringList _listFiles);
     void copyFileToNewName(QString _filetoCopy, QString _newName);

     QString importFile(QString _filePath,  ProjectData _projectVO, QString _toolFolder);
     QString importFile(QString _destinationPath, QString _originPath, QString _toolFolder);
     bool createLink(QString _filePath,  ProjectData _projectVO, QString _toolFolder);


    /************************************ Getters/Setters/Has/Is  ******************/

    QString getSubfolderName(int _indexTool);
    QString getFinalPath();
    bool hasFileExtension(QString _path, QString _extension);
    void updateMultizoneProjectDataToFile(MultizoneProjectData _multizoneProjectDataToSave,ProjectDetails _projectDetails);
    MultizoneProjectData updateMultizoneToolPaths(MultizoneProjectData _oldMultizoneProjectData,MultizoneProjectData _newMultizoneProjectData);

    void copyAllToolData(MultizoneProjectData _oldProjectData,MultizoneProjectData _newProjectData);



private:
    QString _finalAbsoluteFilePath;

};

#endif // UTILFILES_H
