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
#include "ruleengine.h"
#include "Defines/General/GeneralDefines.h"
//#include "../Data/solversectiondao.h"
#define VARIABLE_TO_BE_DISABLED false
#define VARIABLE_TO_BE_ENABLED true

/**
 * @brief
 *
 */
RuleEngine::RuleEngine()
{
}
/*void RuleEngine::setSectionList(QList<SolverSectionVO> * _list)
{
    _sectionList=   _list;
}*/
/**
 * @brief Set the associated config manager (since it contains all the lists that are used in the views)
 *
 * @param _manager
 */
void RuleEngine::setSolverConfigManager(SolverConfigManager * _manager)
{
    _solverManager = _manager;
}

/**
 * @brief This function gets the list of variables to be enabled/disabled and it analyses it in order to add it to
 * the final list in order to change their visibility
 *
 * @param _rule
 */
void RuleEngine::executeRule(VariableRule _rule)
{
        QStringList _listToDisable = _rule.getVariablesToBeDisabled();
        QStringList _listToEnable = _rule.getVariablesToBeEnabled();

        if (!_listToDisable.isEmpty())
            setVariableVisibility(_listToDisable,false);
        if (!_listToEnable.isEmpty())
            setVariableVisibility(_listToEnable,true);
        updateVisibilitySolverManager();
}

/**
 * @brief This fucntion builds the list of variables according its visibility
 *
 * @param _list
 * @param _visibility
 */
void RuleEngine::setVariableVisibility(QStringList _list, bool _visibility)
{

    for (int i = 0; i < _list.size(); i++)
    {
        QString _variableToSet = _list.at(i);
        buildVisibilityList(_visibility,_variableToSet);

    }

}


/**
 * @brief This function passes the already computed lists to the solver config manager to update the visibilities
 * according their nature
 *
 */
void RuleEngine::updateVisibilitySolverManager()
{
    _solverManager->updateVisibility(_disabledVariables,false);
    _solverManager->updateVisibility(_activatedVariables,true);

}

// If the variable is already in one of the places I just remove it for consistency
/**
 * @brief This functionadds the variable to the different lists according the visibility to be set
 *
 * @param _visibility
 * @param _varName
 */
void RuleEngine::buildVisibilityList(bool _visibility,QString _varName)
{
    switch(_visibility)
    {
        case VARIABLE_TO_BE_DISABLED:
            {
                if ((!_activatedVariables.isEmpty()) &&
                    (_activatedVariables.contains(_varName)))
                        _activatedVariables.removeOne(_varName);
                _disabledVariables.append(_varName);
            }
            break;
        case VARIABLE_TO_BE_ENABLED:
            {
                if ( (!_disabledVariables.isEmpty()) &&
                    (_disabledVariables.contains(_varName)) )
                       _disabledVariables.removeOne(_varName);
                _activatedVariables.append(_varName);
            }
            break;
    }
}


