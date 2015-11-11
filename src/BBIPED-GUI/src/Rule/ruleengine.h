#ifndef RULEENGINE_H
#define RULEENGINE_H
#include <QStringList>
#include <QList>
#include "Data/Solver/VO/solversectionvo.h"
#include "Rule/variablerule.h"
#include "Solver/solverconfigmanager.h"
class RuleEngine
{
public:
    RuleEngine();
    //void setSectionList(QList<SolverSectionVO> * _list);
    void executeRule(VariableRule _rule);
    void setSolverConfigManager(SolverConfigManager * _manager);

private:
    QStringList _disabledVariables;
    QStringList _activatedVariables;
    SolverConfigManager * _solverManager;
    //QList<SolverSectionVO> * _sectionList;

    void changeVisibility(QString _name, bool _visibilityVal);
    void setVariableVisibility(QStringList _list, bool _visibility);
    void buildVisibilityList(bool _visibility,QString _varName);
    void updateVisibilitySolverManager();


};

#endif // RULEENGINE_H
