#ifndef SOLVERCONFIGMENUHELPER_H
#define SOLVERCONFIGMENUHELPER_H
#include <QDomDocument>
#include <QTableWidget>
#include <QList>
#include <QComboBox>
#include <QString>
#include <QStringList>
#include <QDomElement>
#include <QDomDocument>
#include <QLineEdit>
#include <QHash>

#include "Data/Solver/VO/solvervariablevo.h"
#include "Data/Solver/VO/solversectionvo.h"

class SolverConfigMenuHelper
{
public:
    // Constructor
    SolverConfigMenuHelper();

    /************************************ CREATE Functions ************************************/
    void createSectionElement(QDomElement _sectionElement, bool _basicView, QDomDocument *  _menuList);
    void createSectionElement(QDomElement _sectionElement, QDomDocument * _menuList);
    void createSubSectionElements(QDomElement _element, QDomElement * _root, bool _basicView,
                                  QDomDocument * _menuList);
    void createNotVisibleSectionElement(QDomElement _element, QDomDocument * _tree);
    /************************************ UPDATE/ADD Functions ************************************/
    void updateTableWithUserValues(SolverSectionVO _solverSecvo, QTableWidget * _table,
                                   QHash<QString,QString> _newMapValues);
    void updateVariableValueinTable( SolverVariableVO _solverVO, QTableWidget* _table,
                                                             QString _newValue);
    /************************************ ADD Functions ************************************/
    void addMenuElements(bool _basicView, QDomDocument * _configDomTree, QDomDocument * _menuToFill);
    void addVariableToTable(SolverVariableVO _variable, QTableWidget * _table, int _row );

    /************************************ FILL Functions ************************************/
    void fillMenuTable(QString _parentName, QString _childName, QTableWidget * _table,
                       QList<SolverVariableVO> _list);

    /************************************ FIND/COMPARE Functions ************************************/
    QDomElement findSectionElementByName(QString _sectionName,QDomNodeList _list);
    bool compareSectionTitle(QString _name,QDomElement _element);

    /************************************ REMOVAL Functions ************************************/
    void removeSectionNode(QString _sectionName, QDomDocument * _tree);
    void removeMenuElementByName(QString _name, QDomDocument * _tree );
    void removeSubsectionElementByName(QString _parentName, QString _childName,
                                                               QDomDocument * _tree);

    /************************************ Getters/Setters/Has/Is Functions ******************/
    int getRowofVariable(QString _variableName, QTableWidget * _table);
    QString getAssociatedHelpText(QString _variableName, QList<SolverVariableVO> _list, bool _withFormat);
    QString getSectionTitle(QDomElement _element);
    QStringList getSubsectionTitle(QDomElement _element);
    void getSectionMenuElements(QString _sectionName, QDomDocument * _confgiDomTree, QDomDocument * _menuToFill);
    QString getAssociatedFileNameValue(SolverVariableVO _solverVO, QString _value);

};

#endif // SOLVERCONFIGMENUHELPER_H
