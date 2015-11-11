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
***/
#include "solverconfigmanager.h"
#include <stdio.h>
#include <iostream>
#include <QFile>

#include "Solver/solverconfigreader.h"
#include "Solver/solverconfigwriter.h"

#include "WindowHelper/Dialog/SolverDialog/solverconfigmenuhelper.h"
#include "WindowHelper/Dialog/SolverDialog/solvermarkermenuhelper.h"
#include "Data/solversectiondao.h"

// DEFINES
#include "Defines/Tool/Solver/SolverDefines.h"
#include "Defines/General/GeneralDefines.h"


using namespace std;

/**
 * @brief Constructor
 *
 */
SolverConfigManager::SolverConfigManager()
{
    _isInitialized = false;
    _isPersonalizedViewInitialized = false;
}
SolverConfigManager::~SolverConfigManager()
{

}

/* ****************************** INITIALIZER *****************************************************/
/**
 * @brief Function that initialize the Solver Manager loading the Config template
 *
 */
void SolverConfigManager::initializeManager()
{
    loadConfigTemplate();
}


/* ****************************** LOAD *****************************************************/

/**
 * @brief Function that loads the config template from an XML file
 *
 */
void SolverConfigManager::loadConfigTemplate()
{
    // Loading the file from the resource
    QFile * _templateConfigFile = new QFile(CONFIG_TEMPLATE_PATH);
    _configDomTree = new QDomDocument();

    if( !_templateConfigFile->open( QIODevice::ReadOnly | QIODevice::Text ) )
    {
        qDebug( "Failed to open file for reading." );
    }
    else
    {
        if( !_configDomTree->setContent( _templateConfigFile ) )
        {
            qDebug( "Failed to parse the file into a DOM tree." );
            _templateConfigFile->close();
        }
          _templateConfigFile->close();
          buildBasicMenuList();
          buildAdvancedMenuList();
          buildMenuVariablesList();
          buildBoundaryMenuList();
          buildCompleteMenuList();
          _isInitialized = true;
    }
    _templateConfigFile->close();

}

/**
 * @brief This function loads the config template from a specific file path (for the case
 * of loading a template)
 *
 * @param _tree
 * @param _path
 * @return bool
 */
bool SolverConfigManager::loadConfigTemplate(QDomDocument * _tree, QString _path)
{
    bool _isLoaded = false;
   // _tree = new QDomDocument();
    QFile * _templateFile = new QFile (_path);
    bool _fileOpened =  _templateFile->open(QIODevice::ReadOnly | QIODevice::Text ) ;
    if (!_fileOpened)
    {   _templateFile->close();
        return _isLoaded;
    }
    // Parsing the file into a Dom Tree
    bool _isSetContent = _tree->setContent(_templateFile);
    if (!_isSetContent)
    {   _templateFile->close();
        return _isLoaded;
    }
    _templateFile->close();
    buildPersonalizedMenuList();
    buildPersonalizedMenuVariablesList();
    _isLoaded = true;
    return _isLoaded;
}


/**
 * @brief This fucntion loads a personalized user defined template
 *
 * @param _path
 */
void SolverConfigManager::loadPersonalizedTemplate(QString _path)
{
    _personalizedDomTree = new QDomDocument();
    _isPersonalizedViewInitialized = loadConfigTemplate(_personalizedDomTree,_path);
}

/* ****************************** BUILD *****************************************************/



/**
 * @brief Function to build all the elements of the basic menu
 *
 */
void SolverConfigManager::buildBasicMenuList()
{
    _basicMenuList = new QDomDocument("basicMenuList");
    buildMainMenu(true, _configDomTree,_basicMenuList);

}


/**
 * @brief This function builds the complete menu list regardless the basic or advanced view from the XMl file
 *
 */
void SolverConfigManager::buildCompleteMenuList()
{
    _completeMenuList = new QDomDocument("completeMenuList");
    buildCompleteMenuList(false,_configDomTree,_completeMenuList);
}

/**
 * @brief This function builds a personalized menu list
 *
 */
void SolverConfigManager::buildPersonalizedMenuList(){
    _personalizedTemplateMenuList = new QDomDocument("personalizedMenuList");
    buildCompleteMenuList(false,_personalizedDomTree,_personalizedTemplateMenuList);
}

/**
 * @brief This function builds the menu to be shown in the tree
 *
 * @param _basicView
 * @param _fullTree
 * @param _menuTree
 */
void SolverConfigManager::buildCompleteMenuList(bool _basicView,QDomDocument * _fullTree, QDomDocument * _menuTree )
{
    SolverConfigMenuHelper _uiHelper;
    buildMainMenu(_basicView,_fullTree,_menuTree);
 }


/**
 * @brief Function to build all the elements main menu
 *
 * @param _basicView
 * @param _fullTree
 * @param _menuTree
 */
void SolverConfigManager::buildMainMenu(bool _basicView,QDomDocument * _fullTree, QDomDocument * _menuTree )
{
     SolverConfigMenuHelper _solverUIHelper;
    _solverUIHelper.addMenuElements(_basicView,_fullTree,_menuTree);
    _solverUIHelper.removeMenuElementByName(MARKER_TITLE,_menuTree);
    _solverUIHelper.removeSubsectionElementByName(IO_TITLE,MESH_TITLE,_menuTree);

}


/**
 * @brief Function to build the Boundary menu
 *
 */
void SolverConfigManager::buildBoundaryMenuList()
{
    SolverMarkerMenuHelper _markerMenuHelper;
    _boundaryMenuList = new QDomDocument("boundaryView");
    _markerMenuHelper.addMarkerMenu(_configDomTree,_boundaryMenuList);

}

/**
 * @brief Function to build all the elements of the advanced menu
 *
 */
void SolverConfigManager::buildAdvancedMenuList()
{

    _advancedMenuList = new QDomDocument("advancedView");
    buildMainMenu(false, _configDomTree,_advancedMenuList);
}

/**
 * @brief Function that builds the menu elements
 *
 */
void SolverConfigManager::buildMenuVariablesList()
{
    SolverConfigReader _reader;
    _sectionMenuList = _reader.getAllSectionVariableMenus(_configDomTree);


}


/**
 * @brief This function builds the personalized variable list
 *
 */
void SolverConfigManager::buildPersonalizedMenuVariablesList()
{
    SolverConfigReader _reader;
    _personalizedMenuList = _reader.getAllSectionVariableMenus(_personalizedDomTree);
}





/* ****************************** UPDATE/SAVE *****************************************************/

/**
 * @brief Functions that save the user configuration values into the file
 *
 * @param _file
 */
void SolverConfigManager::saveConfigFile(QString _file,QHash<QString,QString> _userConfigValues,
                                         ProjectDetails _projectDetails,QList<VirtualMrfVO> _virtualMRFList)
{
    SolverConfigWriter _solverWriter;
    SolverSectionDAO _sectionDAO;
    QList<SolverSectionVO> _sectionList = getAllSectionMenuList();

    _solverWriter.setUserValues(_userConfigValues);
    _solverWriter.setOptionalVariableList(_sectionDAO.getOptionalVariables(_sectionList));
    _solverWriter.setVirtualMRFUserValues(_virtualMRFList);
    _solverWriter.writeToConfigurationFile(_file,_configDomTree,_projectDetails);
}

/**
 * @brief Function that uptades teh values of the DOM tree according to the Map values
 *
 * @param QHash<QString
 * @param _solverHash
 */
void SolverConfigManager::updateDOMTree(QHash<QString, QString> _solverHash)
{
    SolverConfigReader _solverReader;
    _solverReader.updateAllDomTree(_solverHash,_configDomTree);

}

/**
 * @brief This function allows to update the visibility of the variables according the rules and the user selection
 *
 * @param _listVariables
 * @param _visibility
 */
void SolverConfigManager::updateVisibility(QStringList _listVariables, bool _visibility)
{
   if ((!_listVariables.isEmpty()) && (!_sectionMenuList.isEmpty()))
    {

        for(int i = 0; i < _sectionMenuList.size();i++)
        {
            SolverSectionVO _section = _sectionMenuList.at(i);
            QList<SolverVariableVO> _varList = _section.getSubsectionVarList();
            QList<SolverVariableVO> _newVarList;
            if (!_varList.isEmpty())
                for (int j= 0; j<_varList.size();j++)
                {
                    SolverVariableVO _var = _varList.at(j);
                    if (_listVariables.contains(_var.getVariableFileName()))
                        _var.setVisibility(_visibility);
                    _newVarList.append(_var);
                }
            _sectionMenuList[i].setSubsectionVarList(_newVarList);
        }
    }
}

/* ****************************** GETTER/SETTER/IS*****************************************************/

/**
 * @brief Function that checks if the config manager is initialized
 *
 * @return bool
 */
bool SolverConfigManager::isInitialized()
{
    return _isInitialized;
}

/**
 * @brief Returns true if a personalized view is available
 *
 * @return bool
 */
bool SolverConfigManager::isPersonalizedMenuListAvailable()
{
    return _isPersonalizedViewInitialized;
}
/**
 * @brief Function that sets a new DOM tree
 *
 * @param _newDom
 */
void SolverConfigManager::setConfigDomTree(QDomDocument * _newDom)
{
    this->_configDomTree = _newDom;
}

/**
 * @brief Function that gets the current DOM Tree
 *
 * @return QDomDocument
 */
QDomDocument * SolverConfigManager::getConfigDomTree()
{
    return this->_configDomTree;
}

/**
 * @brief Returns the complete menu list from the Xml document
 *
 * @return QDomDocument
 */
QDomDocument * SolverConfigManager::getAllMenuList()
{
    return _completeMenuList;
}

/**
 * @brief Function that returns the Basic Menu List elements
 *
 * @return QDomDocument
 */
QDomDocument *  SolverConfigManager::getBasicMenuList()
{
    return _basicMenuList;
}

/**
 * @brief Function that returns the Advanced Menu List elements
 *
 * @return QDomDocument
 */
QDomDocument *  SolverConfigManager::getAdvancedMenuList()
{
    return _advancedMenuList;
}


/**
 * @brief Function that returns the Boundary Menu List elements
 *
 * @return QDomDocument
 */
QDomDocument *  SolverConfigManager::getBoundaryMenuList()
{
    return _boundaryMenuList;
}
QDomDocument *  SolverConfigManager::getBoundaryMenuListWithoutMultizoneChildren()
{
    SolverConfigMenuHelper _solverUIHelper;
   _solverUIHelper.removeSubsectionElementByName(MARKER_TITLE,"Boundary Multizone",_boundaryMenuList);
   return _boundaryMenuList;
}

/**
 * @brief Returns the personalized template menu list
 *
 * @return QDomDocument
 */
QDomDocument *  SolverConfigManager::getPersonalizedTemplateMenuList()
{
    return _personalizedTemplateMenuList;
}

/**
 * @brief Function that gets the list of variables from a Subsection Element of the menu
 *
 * @param _subsection
 * @return SolverSectionVO
 */
SolverSectionVO SolverConfigManager::getVariableListofSubsections(QString _subsection)
{
    SolverSectionDAO _sectionDAO;
    SolverSectionVO _foundList = _sectionDAO.findSubSection(_subsection,_sectionMenuList);
    return _foundList;
}

/**
 * @brief Returns
 *
 * @param _subsection
 * @return SolverSectionVO
 */
SolverSectionVO SolverConfigManager::getPersonalizedVariableListofSubsections(QString _subsection)
{
    SolverSectionDAO _sectionDAO;
    SolverSectionVO _foundList = _sectionDAO.findSubSection(_subsection,_personalizedMenuList);
    return _foundList;
}


/**
 * @brief Function that gets all the sections to be shown in the menu list
 *
 * @return QList<SolverSectionVO>
 */
QList<SolverSectionVO> SolverConfigManager::getAllSectionMenuList()
{
    return _sectionMenuList;
}

/**
 * @brief
 *
 * @return QList<SolverSectionVO>
 */
QList<SolverSectionVO> SolverConfigManager::getAllPersonalizedSectionMenuList()
{
    return _personalizedMenuList;
}


/* ****************************** RESET *****************************************************/

/**
 * @brief Reset all the variables associated to the Personalized Menu
 *
 */
void  SolverConfigManager::resetPersonalizedMenu()
{
    _isPersonalizedViewInitialized = false;
    _personalizedDomTree = new QDomDocument();
    _personalizedTemplateMenuList = new QDomDocument();
    _personalizedMenuList.clear();
}

