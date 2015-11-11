#ifndef TOOLSHELPER_H
#define TOOLSHELPER_H

#include "Data/PreferencesData/userpreferencesdata.h"
#include "Data/ProjectData/VO/projectdata.h"
#include <QTextEdit>

class ToolsHelper
{
public:
    ToolsHelper();

    /********************** Automatic Salome Tools ********************/
    void createAutomaticMesh(QTextEdit * _textEdit,ProjectData _projectData, UserPreferencesData _userPreferencesData);
    void createAutomaticMeshWithOptimization(ProjectData _projectData, UserPreferencesData _userPreferencesData);

private:
    void launchSalomeWarning();
};

#endif // TOOLSHELPER_H
