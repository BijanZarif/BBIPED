#ifndef SOLVERCONFIGREADER_H
#define SOLVERCONFIGREADER_H
#include <QHash>
#include <QString>
#include <QDomElement>
#include <QDomDocument>
#include <QList>

#include "Solver/solverconfigmanager.h"
#include "Data/Solver/VO/solversectionvo.h"
#include "Data/Solver/VO/solvervarhelpvo.h"

class SolverConfigReader
{
public:
    struct childIds
    {
        int _sectionId;
        int _variableId;
    };

    SolverConfigReader();
    /******************************** Update/Change *****************************/
    void updateValueElement(QString _valueName, QString _newValue, QDomDocument * _domTree);
    void updateAllDomTree(QHash<QString,QString> _newMapValues, QDomDocument * _domTree);

    /******************************** Getter/Setter *****************************/
    QList<SolverSectionVO> getAllSectionVariableMenus(QDomDocument * _tree);
    QStringList getComboValues(QDomElement _element);

private:

    /******************************** Find/Search *****************************/
    childIds findSectionChildId(QString _varName, QDomNodeList  _sectionNodeList);

    /******************************** Create *****************************/
    SolverVarHelpVO * createHelpElement(QDomElement _element);
    void createComboValues(QDomElement _element,SolverVariableVO * _solverVO);
    /******************************** Create *****************************/
    QList<SolverSectionVO> createSectionNode(QDomElement _element);
    QList<SolverVariableVO> createVariableSection(QDomElement _element);
    void changeValueElement(QString _varName, QString _newValue, QDomDocument * _domTree);

};

#endif // SOLVERCONFIGREADER_H
