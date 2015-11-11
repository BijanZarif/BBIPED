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
#include "outputsolverparser.h"
#include <QStringList>
#include <stdio.h>
#include <iostream>
#include <QList>
using namespace std;

#include "../../Defines/General/GeneralDefines.h"
/**
 * @brief Constructor
 *
 */
OutputSolverParser::OutputSolverParser()
{
    _startParsing = false;
    _identifiedNoElementsToPlot = false;
}



/**
 * @brief Returns if the parsing has already started
 *
 * @return bool
 */
bool OutputSolverParser::getStartParsing()
{
    return _startParsing;
}

/**
 * @brief Returns true if the line contains data available to be parsed
 *
 * @param _line
 */
void OutputSolverParser::checkWhenDataToPaintAvailable(QString _line)
{
    if( (!_startParsing) && (!_line.isEmpty()) &&
            ((_line.contains("------------------------------ Start solver"))
             || (_line.contains("------------------------------ Begin solver")))
            || (_line.contains("Begin Solver")))
        _startParsing = true;
}

/**
 * @brief Function that performs the parsing of the line to extract the values
 *
 * @param _residualObj
 * @param _line
 */
void OutputSolverParser::parseOutput(SolverResidualVO * _residualObj, QString _line)
{
    _line = _line.simplified();
   // qDebug( "[1]LINE PARSED" + _line.toLatin1());
    if (!_line.isEmpty())
    {
       // qDebug("[2]Analizando la linea"+ _line.toLatin1() );
        _line = _line.simplified();
        _line.replace(" ",";");
        QStringList _numList = _line.split(";");

        if ( (!_identifiedNoElementsToPlot) && (_line.contains("Iter")) )
            identifyNoElementsToPlot(_numList,_residualObj);

        // Filling the object
        if ( (_identifiedNoElementsToPlot) && (!_line.contains("Iter")) && (!_line.contains("Exit")) )
        {
            //qDebug("Mirando si rellenar el objeto");

            if ( (!_numList.isEmpty())  && (_numList.size() > 1) && (isValidLine(_numList)))
            {
              //  qDebug("Objeto relleno");
                fillResidualObject(_residualObj,_numList);
            }
        }
    }


}


/**
 * @brief
 *
 * @param _list
 * @param _residualObj
 */
void OutputSolverParser::identifyNoElementsToPlot(QStringList _list,SolverResidualVO * _residualObj)
{
    //qDebug("Identificando los elementos");
   /* if (!_list.empty())
        qDebug(QString("Numero elementos -->"+ QString::number(_list.count())).toLatin1());*/
    if ((!_list.isEmpty()) && (_list.count() <= 6))
    {
        //qDebug("Numero <6");
        //  Iter    Time(s)     Res[Rho]     Res[RhoE]   CLift(Total)   CDrag(Total)
        if (!_list.contains("Iter"))
            _residualObj->setIterationEnabled(false);
        if (!_list.contains("Time(s)")) // tengo que poner Time(s)
            _residualObj->setTimeEnabled(false);
        if (!_list.contains("Res[Rho]"))
            _residualObj->setResRhoEnabled(false);
        if (!_list.contains("Res[RhoE]"))
            _residualObj->setResRhoeEnabled(false);
        if (!_list.contains("CLift(Total)"))
            _residualObj->setCliftTotalEnabled(false);
        if (!_list.contains("CDrag(Total)"))
            _residualObj->setDragTotalEnabled(false);

    }else
        if ((!_list.isEmpty()) && (_list.count() >= 6))
        {
              //qDebug("Numero >= 6");
              _residualObj->enableAllValues();
        }
    _identifiedNoElementsToPlot = true;
}

/**
 * @brief
 *
 * @param _list
 * @return bool
 */
bool OutputSolverParser::isValidLine(QStringList _list)
{
    bool _isValid = true;
    //qDebug("Mira si es valida la linea");
    for (int i = 0; (i < _list.size() && (i <= 6));i++)
    {
        bool _isInteger;
        bool _isDouble;
        bool _isInf = false;
        bool _isNan = false;
        if ((QString::compare( _list.at(i),"inf") == StrEqual) || (QString::compare( _list.at(i),"-inf") == StrEqual) )
            _isInf = true;
        if ( (QString::compare( _list.at(i),"nan") == StrEqual) || (QString::compare( _list.at(i),"-nan") == StrEqual) )
            _isNan = true;
       int _val1 = _list.at(i).toInt(&_isInteger);

       double _val2 = _list.at(i).toDouble(&_isDouble);
        if ( (!_isInteger) && (!_isDouble) && (!_isInf) && (!_isNan) )
        {
          //  qDebug("es faaaalso");
            return false;
        }
    }


    return _isValid;
}


/**
 * @brief Function that fills the object with the parsed values
 *
 * @param _residualOb
 * @param _list
 */
void OutputSolverParser::fillResidualObject(SolverResidualVO * _residualOb,QStringList _list)
{
    if ((!_list.isEmpty()) && (_list.size() > 1) ) // At least, another parameter more than Iteration to draw
    {
        _list = checkNanValues(_residualOb,_list);
        if ( (!_list.isEmpty()) && (_list.size() > 1) )
            _residualOb->fillValues(_list);

    }

}

/**
 * @brief
 *
 * @param _residualOb
 * @return QList<bool>
 */
QList<bool> OutputSolverParser::getValidElementsToPrint(SolverResidualVO * _residualOb)
{
 QList<bool> _validElements;
 for (int i = 0; i < 6;i++)
 {
     switch (i)
     {
     case 0:
         _validElements.append(_residualOb->getIterationEnabled());
         break;
     case 1:
         _validElements.append(_residualOb->getTimeEnabled());
         break;
     case 2:
         _validElements.append(_residualOb->getResRhoEnabled());
         break;
     case 3:
         _validElements.append(_residualOb->getResRhoeEnabled());
         break;
     case 4:
         _validElements.append(_residualOb->getCliftTotalEnabled());
         break;
     case 5:
         _validElements.append(_residualOb->getDragTotalEnabled());
         break;
     }
 }
 return _validElements;
}

/**
 * @brief
 *
 * @param _validVal
 * @param _list
 * @return QStringList
 */
QStringList OutputSolverParser::buildResidualList(QList<bool> _validVal, QStringList _list)
{
    QStringList _newList;

    int _cPos = 0;

    for (int i = 0; i < _validVal.size();i++)
    {
       bool _printable = _validVal.at(i);
       if ( (_printable) && (_cPos < _list.size()) )
        {
           _newList.append(_list.at(_cPos));
           _cPos++;
        }
        else
            _newList.append("nan");

    }

    return _newList;
}




QStringList OutputSolverParser::checkNanValues(SolverResidualVO * _residualOb,QStringList _list)
{
    // TODO completarlo para cuando la lista sea menor a 6 elementos
    QStringList _newList;
    QList<bool> _printableElements = getValidElementsToPrint(_residualOb);

    // If we only have Iter, we cannot print
    if (_list.size() <=1)
        return _newList;

    if (_list.size()< 6)    
        _list = buildResidualList(_printableElements,_list);

    if (_list.size() >= 6)
    {
    // Adding the correspondent values
        if (_residualOb->getIterationEnabled())
        {
            if (!valueIsNan(_list.at(0)))
                _newList.append(_list.at(0));
            else
                _residualOb->setIterationEnabled(false);
        }
        if (_residualOb->getTimeEnabled())
        {
            if (!valueIsNan(_list.at(1)))
                _newList.append(_list.at(1));
            else
                _residualOb->setTimeEnabled(false);

        }
        if (_residualOb->getResRhoEnabled())
        {
            if (!valueIsNan(_list.at(2)))
                  _newList.append(_list.at(2));
            else
                _residualOb->setResRhoEnabled(false);
        }
        if (_residualOb->getResRhoeEnabled())
        {
            if (!valueIsNan(_list.at(3)))
                _newList.append(_list.at(3));
            else
                _residualOb->setResRhoeEnabled(false);
        }
        if (_residualOb->getCliftTotalEnabled())
        {
            if (!valueIsNan(_list.at(4)))
                _newList.append(_list.at(4));
            else{
                _residualOb->setCliftTotalEnabled(false);
            }
        }
        if (_residualOb->getDragTotalEnabled())
        {
            if (!valueIsNan(_list.at(5)))
                _newList.append(_list.at(5));
            else
                _residualOb->setDragTotalEnabled(false);
        }
    }
    return _newList;



}

bool OutputSolverParser::valueIsNan(QString _val)
{
    bool _result = false;
    bool _isaInteger;
    bool _isADouble;
   _val.toInt(&_isaInteger);
    _val.toDouble(&_isADouble);

    if ((!_isaInteger) && (!_isADouble) )
        return true;

    if ((QString::compare(_val,"inf") == StrEqual) || (QString::compare(_val,"-inf") == StrEqual) )
        return true;
    if ( (QString::compare(_val,"nan") == StrEqual) || (QString::compare(_val,"-nan") == StrEqual) )
        return true;

    return _result;
}
