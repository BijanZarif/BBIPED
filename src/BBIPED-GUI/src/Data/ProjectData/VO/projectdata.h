#ifndef PROJECTDATA_H
#define PROJECTDATA_H

#include <QString>
#include "Data/Solver/VO/solvervo.h"


class ProjectData
{
public:

    // Constructor
    ProjectData();
    ~ProjectData();

    /************************************ Save ************************************/

    void saveProjectInformation(QString _fullPath);
    void saveProjectInformation(QString _fileName, QString _projectPath,SolverVO _solverVO);


    /************************************ Getters/Setters************************************/

    void setProjectPath(QString _path);
    QString getProjectPath();

    void setProjectFileName(QString _fileName);
    QString getProjectFileName();

    SolverVO getSolverDataInformation();
    void setSolverDataInformation(SolverVO _newSolverInfo);

    bool isEmpty();
    bool solverDataComplete();
    /************************************ Copy /Clear ************************************/
    void clear();
    void copy(ProjectData _newProjectOBJ);
    void updateProjectData(ProjectData _newProjectData);

private:
    QString _projectPath;
    QString _projectFileName;
    SolverVO _solverData;
};

#endif // PROJECTDATA_H
