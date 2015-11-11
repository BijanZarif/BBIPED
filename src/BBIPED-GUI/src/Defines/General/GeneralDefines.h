#ifndef GENERALDEFINES_H
#define  GENERALDEFINES_H

#include <QString>


enum StrValues{
    StrEqual  = 0
};

const QString SOLVER_CONFIG_FILE_EXT = "cfg";
const QString SOLVER_PROJECT_FILE_EXT = "prj";
const QString SOLVER_DIR_NAME = "Solver";
const QString MULTIZONE_PROJECT_FILE_EXT= "mprj";
const QString MESH_CONFIG_SUFFIX ="meshcfg";

enum TreeViewColumnNo{
    TreeColSize = 1,
    TreeColType = 2,
    TreeColDate = 3
};

const QString SPACE_TAB = " ";
const QString YES_VALUE = "yes";
const QString NO_VALUE = "no";

//-----------------------------------------------
// MENU IDs
enum MenuIds{
    Tool1Id = 1,
    Tool2Id = 2,
    Tool3Id = 3,
    Tool4Id = 4
};


#define NUM_SUBFOLDERS  1
const QString TOOL4 = "Solver";

// ----------------------------------------------
// PROJECT DEFINES

const QString PROJECT_ELEMENT = "project";
const QString PROJECT_DATA_TAG = "project-data";
const QString PROJECT_PATH_ATT = "project-path";
const QString PROJECT_FILENAME_ATT = "project-filename";
const QString PROJECT_FULLPATH_ATT = "FullPath";

const QString PATH_ATT = "path";
const QString INITIAL_PATH = "";
// DEFINE SU2 TAGS
const QString SU2_ELEMENT = "solver";
const QString SU2_FILE_ELEMENT = "solverfilename";
const QString SU2_CONFIG_FILE_ELEMENT =  "solverconfigfilepath";

// PROJECT DETAILS TAGS
const QString PROJECT_DETAIL_TAG = "project-details";
const QString PROJECT_DETAIL_TITLE_TAG = "title";
const QString PROJECT_DETAIL_AUTHOR_TAG = "author";
const QString PROJECT_DETAIL_STARTINGDATE_TAG ="starting-date";
const QString PROJECT_DETAIL_OTHERDETAILS_TAG = "other-details";
//-----------------------------------------------
// MULTIZONE DEFINES
const QString SOLVER_LIST_ELEMENT=  "solvers";

//-----------------------------------------------
// Running commands
// SOLVER
const QString SOLVER_DEFAULT_FOLDER="bin";
const QString SOLVER_RUN_COMMAND = "BBIPED-Serial";
//const QString SOLVER_VIRTUAL_RUN_COMMAND = "SU2_CFD_VMRF";
//const QString SOLVER_MULTIZONE_RUN_COMMAND = "SU2_CFD_MULTIZONE";
const QString SOLVER_DEFAULT_ENGINE  = "BBIPED-Serial";

// Parallel solver - BBIPED
const QString PARALLEL_BBIPED_RUN_COMMAND = "BBIPED-Parallel";
const QString PARALLEL_BBIPED_PARTITION_COMMAND = "BBIPED-Partition";
const QString MPI_EXEC_COMMAND = "mpiexec -np";

enum RunningCommands{
    DefaultSolverOption = 0,
    VirtualSolverOption = 1,
    MultizoneSolverOption = 2,
    CustomizedSolverOption = 3
};

//--------------------------------------------------
// PARSER options

enum ParserOptions{
    ParserSolverConfigFile = 0,
    ParserSolverMeshFile = 1

};

// WRITING OPTIONS
const QString COMMA_SEPARATOR=  ",";
const QString SEMICOLON_SEPARATOR = ";";

// FORMAT OPTIONS
#define HTML_FORMAT_ON true
#define HTML_FORMAT_OFF false

//-------------------------------------
// ICON OPTIONS


enum IconOptions{
    IconHelp = 0,
    IconEdit = 1,
    IconImport = 2,
    IconDelete = 3,
    IconAdd = 4
};

//-------------------------------------
// Files Preferences Path
const QString PREFERENCE_FILE_NAME = ".bbiped_preferences";

enum MultizoneOptions{
    OptionMzConfigPath = 0,
    OptionMzMeshPath = 1
};

//___________________________
const QString IS_3D_MESH_TAG="IS_3D_MESH";

#endif // GENERALDEFINES_H

