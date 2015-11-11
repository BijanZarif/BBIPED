#ifndef SOLVERCONFIGMANAGER_H
#define SOLVERCONFIGMANAGER_H
#include <QDomDocument>
#include <QString>
#include <QHash>
#include <QList>
#include "Data/Solver/VO/solversectionvo.h"
#include "../Data/ProjectData/VO/projectdetails.h"
#include "Data/Solver/VirtualMRF/virtualmrfvo.h"

class SolverConfigManager
{
public:
    SolverConfigManager();
    ~SolverConfigManager();

    /********************************* Initialize *************************************/
    void initializeManager();
    void loadPersonalizedTemplate(QString _path);

    /********************************* Update/Save *************************************/
    void saveConfigFile(QString _file,QHash<QString,QString> _userConfigValues, ProjectDetails _projectDetails,
                        QList<VirtualMrfVO> _virtualMRFList);
    void updateDOMTree(QHash<QString, QString> _solverHash);
    void updateVisibility(QStringList _listVariables,bool _visibility);

    /********************************* Build *************************************/
    void buildMenuVariablesList();

    /********************************* Remove/Delete *************************************/

    void resetPersonalizedMenu();

    /********************************* Getter/Setter/Is *************************************/
    bool isInitialized();
    bool isPersonalizedMenuListAvailable();
    void setConfigDomTree(QDomDocument * _newDom);
    QDomDocument * getConfigDomTree();
    QDomDocument * getAllMenuList();
    QDomDocument * getBasicMenuList();
    QDomDocument * getAdvancedMenuList();
    QDomDocument * getBoundaryMenuList();
    QDomDocument *  getBoundaryMenuListWithoutMultizoneChildren();
    QDomDocument * getPersonalizedTemplateMenuList();
    SolverSectionVO getVariableListofSubsections(QString _subsection);
    QList<SolverSectionVO> getAllSectionMenuList();
    QList<SolverSectionVO> getAllPersonalizedSectionMenuList();
    SolverSectionVO getPersonalizedVariableListofSubsections(QString _subsection);


private:
    QDomDocument * _configDomTree; // Complete Template Tree
    QDomDocument * _personalizedDomTree;
    QDomDocument * _completeMenuList;
    QDomDocument * _basicMenuList;
    QDomDocument * _advancedMenuList;
    QDomDocument * _boundaryMenuList;
    QDomDocument * _personalizedTemplateMenuList;

    QList<SolverSectionVO> _sectionMenuList;
    QList<SolverSectionVO> _personalizedMenuList;

    SolverSectionVO _markerMenu;
    bool _isInitialized;
    bool _isPersonalizedViewInitialized;

    /********************************* Build/Load *************************************/

    void loadConfigTemplate();
    bool loadConfigTemplate(QDomDocument * _tree, QString _path);

    void buildBasicMenuList();
    void buildAdvancedMenuList();
    void buildBoundaryMenuList();
    void buildMainMenu(bool _basicView,QDomDocument * _fullTree, QDomDocument * _menuTree );
    void buildPersonalizedMenuVariablesList();


    void buildCompleteMenuList();
    void buildCompleteMenuList(bool _basicView,QDomDocument * _fullTree, QDomDocument * _menuTree );
    void buildPersonalizedMenuList();
};

#endif // SOLVERCONFIGMANAGER_H
