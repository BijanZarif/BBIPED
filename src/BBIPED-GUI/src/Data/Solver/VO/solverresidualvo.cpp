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
#include "solverresidualvo.h"

/**************************************************************************************/
/**
 * @brief Constructor
 *
 */
SolverResidualVO::SolverResidualVO()
{
    _iteration = 0;
    _time = 0;
    _resRho = 0;
    _resRhoe = 0;
    _cDragTotal = 0;
    _cliftTotal = 0;

    _iterationEnabled = true;
    _timeEnabled = true;
    _resRhoEnabled = true;
    _resRhoeEnabled = true;
    _cliftTotalEnabled = true;
    _cDragTotalEnabled = true;


}
/****************************** GETTER/SETTER/IS **********************************/
/**
 * @brief Return time Value
 *
 * @return double
 */
double SolverResidualVO::getTimeValue()
{
    return _time;
}

/**
 * @brief Return ResRho
 *
 * @return double
 */
double SolverResidualVO::getResRho()
{
    return _resRho;
}

/**
 * @brief Return ResRhoe
 *
 * @return double
 */
double SolverResidualVO::getResRhoe()
{
   return _resRhoe;
}

/**
 * @brief Return Clift Total
 *
 * @return double
 */
double SolverResidualVO::getCliftTotal()
{
    return _cliftTotal;
}

/**
 * @brief Return CDrag Total
 *
 * @return double
 */
double SolverResidualVO::getCDragTotal()
{
    return _cDragTotal;
}

/**
 * @brief set Time Val
 *
 * @param _newVal
 */
void SolverResidualVO::setTimeValue(double _newVal)
{
    _time = _newVal;
}

/**
 * @brief Set ResRho
 *
 * @param _newVal
 */
void SolverResidualVO::setResRho(double _newVal)
{
    _resRho = _newVal;

}

/**
 * @brief Set ResRhoe
 *
 * @param _newVal
 */
void SolverResidualVO::setResRhoe(double _newVal)
{
    _resRhoe = _newVal;

}

/**
 * @brief Set Clift Total
 *
 * @param _newVal
 */
void SolverResidualVO::setCliftTotal(double _newVal)
{
    _cliftTotal = _newVal;

}

/**
 * @brief Set CDrag Total
 *
 * @param _newVal
 */
void SolverResidualVO::setCDragTotal(double _newVal)
{
    _cDragTotal = _newVal;

}

/**
 * @brief return Number of iteration
 *
 * @return int
 */
int SolverResidualVO::getIteration()
{
    return _iteration;
}

/**
 * @brief Set number of iteration
 *
 * @param _newIteration
 */
void SolverResidualVO::setIteration(int _newIteration)
{
    _iteration = _newIteration;
}


/**
 * @brief Set if the iteration no. will appear in the graphic (true, false otherwise)
 *
 * @param _val
 */
void  SolverResidualVO::setIterationEnabled(bool _val)
{
    _iterationEnabled = _val;
}


/**
 * @brief Set if the time will appear in the graphic (true, false otherwise)
 *
 * @param _val
 */
void  SolverResidualVO::setTimeEnabled(bool _val)
{
    _timeEnabled = _val;
}


/**
 * @brief  Set if the ResRho will appear in the graphic (true, false otherwise)
 *
 * @param _val
 */
void  SolverResidualVO::setResRhoEnabled(bool _val)
{
    _resRhoEnabled = _val;
}

/**
 * @brief Set if the ResRhoe will appear in the graphic (true, false otherwise)
 *
 * @param _val
 */
void  SolverResidualVO::setResRhoeEnabled(bool _val)
{
    _resRhoeEnabled = _val;
}

/**
 * @brief Set if the CliftTotal will appear in the graphic (true, false otherwise)
 *
 * @param _val
 */
void  SolverResidualVO::setCliftTotalEnabled(bool _val)
{
    _cliftTotalEnabled = _val;
}

/**
 * @brief Set if the Drag Total will appear in the graphic (true, false otherwise)
 *
 * @param _val
 */
void  SolverResidualVO::setDragTotalEnabled(bool _val)
{
   _cDragTotalEnabled = _val;
}



/**
 * @brief Returns if the Iteration will appear in the graphic (true, false otherwise)
 *
 * @return bool
 */
bool SolverResidualVO::getIterationEnabled()
{
    return _iterationEnabled;

}


/**
 * @brief Returns if the Time will appear in the graphic (true, false otherwise)
 *
 * @return bool
 */
bool SolverResidualVO::getTimeEnabled()
{
    return _timeEnabled;
}


/**
 * @brief Returns if the ResRho will appear in the graphic (true, false otherwise)
 *
 * @return bool
 */
bool SolverResidualVO::getResRhoEnabled()
{
    return _resRhoEnabled;
}


/**
 * @brief Returns if the ResRhoe will appear in the graphic (true, false otherwise)
 *
 * @return bool
 */
bool SolverResidualVO::getResRhoeEnabled()
{
    return _resRhoeEnabled;

}


/**
 * @brief Returns if the Clift Total will appear in the graphic (true, false otherwise)
 *
 * @return bool
 */
bool SolverResidualVO::getCliftTotalEnabled()
{
    return _cliftTotalEnabled;
}


/**
 * @brief Returns if the Drag Total will appear in the graphic (true, false otherwise)
 *
 * @return bool
 */
bool SolverResidualVO::getDragTotalEnabled()
{
    return _cDragTotalEnabled;
}

/**************************************************** FILL / ENABLE **********************************/
/**
 * @brief This function allows to fill in the Residual object according to the values stored in _list and
 * their visibility
 *
 * @param _list
 */
void SolverResidualVO::fillValues(QStringList _list)
{
    if (!_list.isEmpty())
    {
        int _posList = 0;
        if (!_list.contains("Negative"))
           {
                if  (_iterationEnabled)
                {
                   this->setIteration(_list.at(_posList).toInt());
                    _posList++;
                }
                if  (_timeEnabled)
                {
                   this->setTimeValue(_list.at(_posList).toDouble());
                    _posList++;
                }
                if  (_resRhoEnabled)
                {
                   this->setResRho(_list.at(_posList).toDouble());
                    _posList++;
                }
                if  (_resRhoeEnabled)
                {
                   this->setResRhoe(_list.at(_posList).toDouble());
                    _posList++;
                }
                if  (_cliftTotalEnabled)
                {
                   this->setCliftTotal(_list.at(_posList).toDouble());
                    _posList++;
                }
                if  (_cDragTotalEnabled)
                {
                   this->setCDragTotal(_list.at(_posList).toDouble());
                    _posList++;
                }

           }
    }

}


/**
 * @brief Restore all values visibility to true
 *
 */
void SolverResidualVO::enableAllValues()
{
    setIterationEnabled(true);
    setTimeEnabled(true);
    setResRhoEnabled(true);
    setResRhoeEnabled(true);
    setCliftTotalEnabled(true);
    setDragTotalEnabled(true);
}

/*********************************** CLEAR *********************************/
/**
 * @brief  Clear all the data
 *
 */
void SolverResidualVO::clearAllData()
{
    this->setIteration(0);
    this->setTimeValue(0);
    this->setResRho(0);
    this->setResRhoe(0);
    this->setCliftTotal(0);
    this->setCDragTotal(0);

    this->_iterationEnabled  = true;
    this->_timeEnabled = true;
    this->_resRhoEnabled = true;
    this->_resRhoeEnabled = true;
    this->_cliftTotalEnabled = true;
    this->_cDragTotalEnabled = true;
}
