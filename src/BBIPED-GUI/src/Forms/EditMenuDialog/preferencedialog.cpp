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
 *  Author: Dec 2016: Carmen Alonso Montes calonso@bcamath.org
***/
#include "preferencedialog.h"
#include "ui_preferencedialog.h"

#include <QFileDialog>
#include <QCoreApplication>

/**
 * @brief Constructor
 *
 * @param parent
 */
PreferenceDialog::PreferenceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreferenceDialog)
{
    ui->setupUi(this);
    initializeVariables();
}

/**
 * @brief Destructor
 *
 */
PreferenceDialog::~PreferenceDialog()
{
    delete ui;
}

/******************************************* INITIALIZE ***********************************************/

/**
 * @brief This function initializes all the variables needed by this dialog
 *
 */
void PreferenceDialog::initialize()
{
    _customizedSolverEnginePath = "";
    _customizedMeshViewerPath = "";
    _customizedPostProcessorViewerPath = "";
    _customizedParallelEnginePath = "";
    _customizedPartitionEnginePath = "";
}

/**
 * @brief Initialize with the user preferences
 *
 * @param _userData
 */
void PreferenceDialog::initialize(UserPreferencesData _userData)
{
    initializeSolverData(_userData);
    initializeMeshViewerData(_userData);
    initializePostProcessorViewerData(_userData);
    initializeWarningData(_userData);
    initializeTemplateConfigData(_userData);

}


/**
 * @brief Initialize the solver data
 *
 * @param _userData
 */
void PreferenceDialog::initializeSolverData(UserPreferencesData _userData)
{    

    if (!_userData.getUserDefinedSolverPath().isEmpty())
    {
        initialize(ui->customizedSolverEnginePath, _userData.getUserDefinedSolverPath());
        _isCustomizedSolverSelected = true;
    }
    if (!_userData.isDefaultParallelEngineSelected())
    {
        initialize(ui->parallelEngineFolderlineEdit, _userData.getCustomizedParallelEnginePath());
        _isCustomizedParallelEngineSelected = true;
    }
    if (!_userData.isDefaultPartitionEngineSelected())
    {
        initialize(ui->partitionFolderlineEdit, _userData.getCustomizedPartitionEnginePath());
        _isCustomizedPartitionEngineSelected = true;
    }

}

/**
 * @brief Initialize Mesh viewer data
 *
 * @param _userData
 */
void PreferenceDialog::initializeMeshViewerData(UserPreferencesData _userData)
{
    if (!_userData.getMeshViewerPath().isEmpty())
        setCustomizedMeshViewerPath(_userData.getMeshViewerPath());
}


/**
 * @brief Initialize post processor Viewer Data
 *
 * @param _userData
 */
void PreferenceDialog::initializePostProcessorViewerData(UserPreferencesData _userData)
{
    if (!_userData.getPostProcessorViewerPath().isEmpty())
        setCustomizedPostProcessorViewerSelectedPath( _userData.getPostProcessorViewerPath());
}

/**
 * @brief Establish the set up for the warnings according user selection
 *
 * @param _userData
 */
void PreferenceDialog::initializeWarningData(UserPreferencesData _userData)
{
     _showWarningAppClose = _userData.showCloseWarning();
     _showWarningProjectClose = _userData.showCloseProjectWarning();
     // Update the checkboxes
     if (_showWarningAppClose)
         ui->warningAppCheckBox->setChecked(Qt::Checked);
     if (_showWarningProjectClose)
         ui->warningProjectCheckBox->setChecked(Qt::Checked);
}


/**
 * @brief Initialize the template associated data according user selection
 *
 * @param _userData
 */
void PreferenceDialog::initializeTemplateConfigData(UserPreferencesData _userData)
{
    QString _templatePath = _userData.getTemplateConfigPath();
    if (!_templatePath.isEmpty())
    {
        _isTemplateConfigSelected = true;
        _templateConfigPath = _templatePath;
    }else
    {
        _isTemplateConfigSelected = false;
        _templateConfigPath = "";
    }
    ui->templateConfLineEdit->setText(_templateConfigPath);
}



/**
 * @brief This function initializes all the variables needed by this dialog in case other solver engine
 *  has been selected (a customized solver engine)
 *
 * @param _path
 */
void PreferenceDialog::initialize(QLineEdit * _line, QString _path)
{
    _customizedSolverEnginePath = _path;
    // Changing the values of the window
   // _isCustomizedSolverSelected = true;

    if (!_customizedSolverEnginePath.isEmpty())
        _line->setText(_customizedSolverEnginePath);

}

/**
 * @brief This function initialize all the variables of this dialog
 *
 */
void PreferenceDialog::initializeVariables()
{
    _userAcceptChanges = false;
    _isCustomizedMeshViewerSelected = false;
    _isCustomizedPostProcessorViewerPath = false;

    _isTemplateConfigSelected = false;
    _templateConfigPath = "";

    // Parallel Engine parameters
    _isCustomizedParallelEngineSelected = false;
    _isCustomizedPartitionEngineSelected = false;
    initialize();
}

/******************************************* ENABLE/DISABLE/CHECK/UNCHECK ***********************************************/

/**
 * @brief This function enables/disables the Customized Solver line edit
 *
 * @param _val
 */
/*void PreferenceDialog::enableCustomizedSolverMenu(bool _val)
{
   ui->customizedSolverLabel->setEnabled(_val);
    ui->customizedSolverEnginePath->setEnabled(_val);
    ui->customizedSolverEngineToolButton->setEnabled(_val);
}
*/

/******************************************* BUTTON CLICK ***********************************************/

/****** PREFERENCE TAB ***************/
/**
 * @brief This function loads the dialog for choosing the path of the customized solver engine
 *
 */
void PreferenceDialog::on_customizedSolverEngineToolButton_clicked()
{
    QString _binPath = QFileDialog::getOpenFileName(this, tr("Choose the Solver Engine Executable file"),
                                                            QCoreApplication::applicationDirPath(),//QDir::currentPath(),
                                                            tr("All files(*)"));

    if (!_binPath.isNull())
    {
        _customizedSolverEnginePath = _binPath;
        _isCustomizedSolverSelected = true;
        ui->customizedSolverEnginePath->setText(_customizedSolverEnginePath);
    }
}
/**
 * @brief If the user accepts the change in the dialog
 *
 */
void PreferenceDialog::on_preferenceButtonBox_accepted()
{
    _userAcceptChanges = true;
    close();
}

/**
 * @brief Action when the user rejects the changes
 *
 */
void PreferenceDialog::on_preferenceButtonBox_rejected()
{
    _userAcceptChanges = false;
    close();
}

/****** MESH VIEWER TAB ***************/

/**
 * @brief Function that defines the dialog launch for selecting the path for the mesh viewer tool
 *
 */
void PreferenceDialog::on_meshViewerConfLineToolButton_clicked()
{
    QString _binPath = QFileDialog::getOpenFileName(this, tr("Choose the Mesh Tool Engine Executable file"),
                                                            QCoreApplication::applicationDirPath(),//QDir::currentPath(),
                                                            tr("All files(*)"));

    if (!_binPath.isNull())
        setCustomizedMeshViewerPath(_binPath);

}

/**
 * @brief Function that deletes the already selected path for the mesh viewer tool
 *
 */
void PreferenceDialog::on_deleteMeshViewerConfButton_clicked()
{
    ui->meshViewerConfLineEdit->clear();
    _isCustomizedMeshViewerSelected = false;
}

/****** TEMPLATE TAB ***************/

/**
 * @brief Function that controls the behaviour of the Template tab when deleting the selected tempalte
 *
 */
void PreferenceDialog::on_deleteTemplateButton_clicked()
{
    ui->templateConfLineEdit->clear();
    _isTemplateConfigSelected = false;
    _templateConfigPath = "";
}
/**
 * @brief Select the path of the template file
 *
 */
void PreferenceDialog::on_templateConfToolButton_clicked()
{
    QString _workingPath = QDir(QCoreApplication::applicationDirPath()).filePath(".templates");
    if (!QDir(_workingPath).exists())
        QDir(QCoreApplication::applicationDirPath()).mkdir(".templates");

    QString _binPath = QFileDialog::getOpenFileName(this, tr("Choose the Template for the Physics Configuration"),
                                                            _workingPath,
                                                            tr("Template Config File Type(*.xml)"));

    if (!_binPath.isNull())
           setTemplateConfigPath(_binPath);

}
/****** WARNING TAB ***************/
/**
 * @brief Set the option of showing (true value) or not (false value) the warning application close window
 *
 * @param checked
 */
void PreferenceDialog::on_warningAppCheckBox_clicked(bool checked)
{
    _showWarningAppClose = checked;
}

/**
 * @brief Set the option of showing (true value) or not (false value) the warning project close window
 *
 * @param checked
 */
void PreferenceDialog::on_warningProjectCheckBox_clicked(bool checked)
{
    _showWarningProjectClose = checked;
}

/****** PostProcessor Viewer TAB ***************/

/**
 * @brief Launchs the dialog to get the path of the postprocessor viewer
 *
 */
void PreferenceDialog::on_postProcessorViewerConfLineToolButton_clicked()
{

    QString _binPath = QFileDialog::getOpenFileName(this, tr("Choose the PostProcessor Engine Executable file"),
                                                            QCoreApplication::applicationDirPath(),//QDir::currentPath(),
                                                            tr("All files(*)"));

    if (!_binPath.isNull())
            setCustomizedPostProcessorViewerSelectedPath(_binPath);

}

/**
 * @brief Restore the postprocessor viewer tab to its defaults
 *
 */
void PreferenceDialog::on_deletePostProcessorViewerConfButton_clicked()
{
    ui->postProcessorViewerConfLineEdit->clear();
    _isCustomizedPostProcessorViewerPath = false;
}

/**
 * @brief Reset all the tabs to their default values
 *
 */
void PreferenceDialog::on_resetPreferencesToDefaultButton_clicked()
{
    resetSolverEngineTab();
    resetMeshViewerTab();
    resetPostProcessorViewerTab();
    resetTemplateTab();
}



/******************************************* GETTER/SETTER/IS ***********************************************/


/**
 * @brief Returns the path of the customized solver engine
 *
 * @return QString
 */
QString PreferenceDialog::getCustomizedSolverEnginePath()
{
    return _customizedSolverEnginePath;
}

/**
 * @brief Returns the path of the customized mesh viewer
 *
 * @return QString
 */
QString PreferenceDialog::getCustomizedMeshViewerSelectedPath()
{
    return _customizedMeshViewerPath;
}

/**
 * @brief Returns true if the customized solver engine is selected
 *
 * @return bool
 */
bool PreferenceDialog::isCustomizedSolverSelected()
{
    return _isCustomizedSolverSelected;
}



/**
 * @brief Returns true if a customized mesh viewer has been selected by the suer
 *
 * @return bool
 */
bool PreferenceDialog::isCustomizedMeshViewerSelected()
{
    return _isCustomizedMeshViewerSelected;
}

/**
 * @brief Set the user path of the mesh viewer tool
 *
 * @param _path
 */
void PreferenceDialog::setCustomizedMeshViewerPath(QString _path)
{
    if ( (!_path.isNull()) && (!_path.isEmpty()) )
    {
        _customizedMeshViewerPath = _path;
        _isCustomizedMeshViewerSelected = true;
        ui->meshViewerConfLineEdit->setText(_path);
    }
}



/**
 * @brief  Returns true if the user has accepted the changes in the dialog
 *
 * @return bool
 */
bool PreferenceDialog::isPreferencesAccepted()
{
    return _userAcceptChanges;
}


/***************************** POSTPROCESSOR ACTIONS **************************************/

/**
 * @brief Returns the path of the customized postprocessor viewer
 *
 * @return QString
 */
QString PreferenceDialog::getCustomizedPostProcessorViewerSelectedPath()
{
    return _customizedPostProcessorViewerPath;
}

/**
 * @brief Returns true if the user has selected a specific post processor viewer
 *
 * @return bool
 */
bool PreferenceDialog::isCustomizedPostProcessorViewerSelectedPath()
{
       return _isCustomizedPostProcessorViewerPath;
}

/**
 * @brief Sets the user selected path of the post-processor viewer
 *
 * @param _path
 */
void PreferenceDialog::setCustomizedPostProcessorViewerSelectedPath(QString _path)
{
    if ( (!_path.isNull()) && (!_path.isEmpty()) )
    {
        _customizedPostProcessorViewerPath = _path;
        _isCustomizedPostProcessorViewerPath = true;
        ui->postProcessorViewerConfLineEdit->setText(_path);
    }
}



/**
 * @brief Reset the solver engine Tab to its defaults
 *
 */
void PreferenceDialog::resetSolverEngineTab()
{
    _isDefaultSolverSelected = true;
    _isCustomizedParallelEngineSelected = false;
    _isCustomizedPartitionEngineSelected = false;

    ui->customizedSolverEnginePath->clear();
    ui->parallelEngineFolderlineEdit->clear();
    ui->partitionFolderlineEdit->clear();

    _customizedSolverEnginePath = "";
    _customizedParallelEnginePath = "";
    _customizedPartitionEnginePath = "";
}

/**
 * @brief Reset the mesh viewer tab to its defaults
 *
 */
void PreferenceDialog::resetMeshViewerTab()
{
    _isCustomizedMeshViewerSelected = false;
    setCustomizedMeshViewerPath("");
    ui->meshViewerConfLineEdit->clear();
}

/**
 * @brief Reset the postprocessor viewer tab to its defaults
 *
 */
void PreferenceDialog::resetPostProcessorViewerTab()
{
    _isCustomizedPostProcessorViewerPath = false;
    setCustomizedPostProcessorViewerSelectedPath("");
    ui->postProcessorViewerConfLineEdit->clear();
}

/**
 * @brief Reset the tempalte tab to its defaults
 *
 */
void PreferenceDialog::resetTemplateTab()
{
    _isTemplateConfigSelected = false;
    setTemplateConfigPath("");
    ui->templateConfLineEdit->clear();
}


/**
 * @brief Returns the User Preferencecs data with the new values
 *
 * @return UserPreferencesData
 */
UserPreferencesData PreferenceDialog::getUserPreferencesData()
{
    UserPreferencesData _newUserData;

    // Getting the data from the Solver Engine
    getSolverDataConfiguration(&_newUserData);

    // Getting the data from the Mesh Viewer
    getMeshViewerConfiguration(&_newUserData);

    // Getting the data from the PostProcessor Viewer
    getPostProcessorViewerConfiguration(&_newUserData);

    // Getting the data from the Warning Conf
    getWarningConfiguration(&_newUserData);

    // Getting the data from the Template Conf
    getTemplateConfigPath(&_newUserData);

    return _newUserData;

}

/**
 * @brief Returns the SolverData Configuration storing it in the user data
 *
 * @param _userData
 */
void PreferenceDialog::getSolverDataConfiguration(UserPreferencesData * _userData)
{
    if (_isCustomizedSolverSelected)
    {
        _userData->setUserDefinedSolverPath(ui->customizedSolverEnginePath->text());
        _userData->setDefaultSolverEngineSelected(false);
    }
    else
     _userData->setDefaultSolverEngineSelected(true);

    if (_isCustomizedParallelEngineSelected)
    {
        _userData->setCustomizedParallelEnginePath(ui->parallelEngineFolderlineEdit->text());
        _userData->setDefaultParallelEngineSelected(!_isCustomizedParallelEngineSelected);
    }
    else
         _userData->setDefaultParallelEngineSelected(true);
    if (_isCustomizedPartitionEngineSelected)
    {
        _userData->setCustomizedPartitionEnginePath(ui->partitionFolderlineEdit->text());
        _userData->setDefaultPartitionEngineSelected(!_isCustomizedPartitionEngineSelected);
    }else
         _userData->setDefaultPartitionEngineSelected(true);

}

/**
 * @brief Returns the Mesh Data Configuration storing it in the user data
 *
 * @param _userData
 */
void PreferenceDialog::getMeshViewerConfiguration(UserPreferencesData * _userData)
{
    if (_isCustomizedMeshViewerSelected)
        _userData->setMeshViewerPath(_customizedMeshViewerPath);
    else
        _userData->setMeshViewerPath("");

}

/**
 * @brief Returns the PostProcessor Data Configuration storing it in the user data
 *
 * @param _userData
 */
void PreferenceDialog::getPostProcessorViewerConfiguration(UserPreferencesData * _userData)
{
    if (_isCustomizedPostProcessorViewerPath)
        _userData->setPostProcessorViewerPath(_customizedPostProcessorViewerPath);
    else
        _userData->setPostProcessorViewerPath("");
}

/**
 * @brief Returns the Warning Data Configuration storing it in the user data
 *
 * @param _userData
 */
void PreferenceDialog::getWarningConfiguration(UserPreferencesData * _userData)
{
    _userData->setShowCloseWarning(_showWarningAppClose);
    _userData->setShowCloseProjectWarning(_showWarningProjectClose);

}

/**
 * @brief Returns the Template Data Configuration storing it in the user data
 *
 * @param _userData
 */
void PreferenceDialog::getTemplateConfigPath(UserPreferencesData * _userData)
{
    _userData->setTemplateConfigPath(_templateConfigPath);

}


/**
 * @brief Returns true if a template has been selected by the user
 *
 * @return bool
 */
bool PreferenceDialog::isTemplateConfigSelected()
{
    return _isTemplateConfigSelected;
}

/**
 * @brief Returns the path of the user selected template file
 *
 * @return QString
 */
QString PreferenceDialog::getTemplateConfigPath()
{
    return _templateConfigPath;
}

/**
 * @brief Stores the path of the user selected template file
 *
 * @param _file
 */
void PreferenceDialog::setTemplateConfigPath(QString _file)
{
    _templateConfigPath = _file;

    if (!_templateConfigPath.isEmpty())
    {
        _isTemplateConfigSelected = true;
         ui->templateConfLineEdit->setText(_templateConfigPath);
    }
    else
    {
        _isTemplateConfigSelected = false;
        ui->templateConfLineEdit->clear();
    }
}


/***************** PARALLEL ENGINE OPTIONS ************************************/

void PreferenceDialog::on_parallelPartitiontoolButton_clicked()
{
    QString _binPath = QFileDialog::getOpenFileName(this, tr("Choose the Partition Solver Engine Executable file"),
                                                            QCoreApplication::applicationDirPath(),
                                                            tr("All files(*)"));

    if (!_binPath.isNull())
    {
        _customizedPartitionEnginePath = _binPath;
        _isCustomizedPartitionEngineSelected = true;
        ui->partitionFolderlineEdit->setText(_customizedPartitionEnginePath);
    }
}

void PreferenceDialog::on_parallelEnginetoolButton_clicked()
{
    QString _binPath = QFileDialog::getOpenFileName(this, tr("Choose the Parallel Solver Engine Executable file"),
                                                            QCoreApplication::applicationDirPath(),
                                                            tr("All files(*)"));

    if (!_binPath.isNull())
    {
        _customizedParallelEnginePath = _binPath;
        _isCustomizedParallelEngineSelected = true;
        ui->parallelEngineFolderlineEdit->setText(_customizedParallelEnginePath);
    }
}

void PreferenceDialog::on_clearSolverEngineLocationButton_clicked()
{
    ui->customizedSolverEnginePath->clear();
    _isCustomizedSolverSelected = false;
    _customizedSolverEnginePath = "";
}

void PreferenceDialog::on_clearPartitionLocationButton_clicked()
{
    ui->partitionFolderlineEdit->clear();
    _isCustomizedPartitionEngineSelected = false;
    _customizedPartitionEnginePath = "";
}

void PreferenceDialog::on_clearParallelEngineLocationButton_clicked()
{
    ui->parallelEngineFolderlineEdit->clear();
    _isCustomizedParallelEngineSelected = false;
    _customizedParallelEnginePath = "";
}
