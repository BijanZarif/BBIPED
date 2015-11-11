#ifndef SOLVERUTIL_H
#define SOLVERUTIL_H

#include <QString>

#include "Data/ProjectData/VO/projectdata.h"
#include "Data/Solver/VO/solvervo.h"

class SolverUtil
{
public:

    SolverUtil();
    SolverVO uiImportSolverConfigFile(QString _filePath, ProjectData _projectVO);
    SolverVO uiImportSolverMeshFile(QString _meshFilePath, ProjectData _projectVO);
    SolverVO uiImportSolverMeshInfoFile(QString _meshFilePath, ProjectData _projectVO);

};

#endif // SOLVERUTIL_H
