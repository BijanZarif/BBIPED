#ifndef MULTIZONEDEFINES_H
#define MULTIZONEDEFINES_H

// Column table in the Multizone creation project

enum MultizoneColumProjectCreation{
    ColumnZoneNo = 0,
    ColumnFileName = 1,
    ColumnAddFile = 2,
    ColumnImportFile = 3,
    ColumnDeleteFile = 4
};

// Default file names and extensions
const QString DEFAULT_CONFIG_FILE_NAME_PREFIX = "default_config";
const QString DEFAULT_CONFIG_FILE_NAME_SUFFIX = ".cfg";
const QString DEFAULT_MESH_FILE_NAME_PREFIX = "default_mesh";
const QString DEFAULT_MESH_FILE_NAME_SUFFIX = ".su2";


#endif // MULTIZONEDEFINES_H
