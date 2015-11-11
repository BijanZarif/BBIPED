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
#include "residualtabhelper.h"

/**
 * @brief Constructor
 *
 */
ResidualTabHelper::ResidualTabHelper()
{
}


/**
 * @brief Function to show the plot
 *
 * @param _plot
 * @param _residualVO
 * @param _points
 * @return QPolygonF
 */
QPolygonF ResidualTabHelper::showPlot(QwtPlot * _plot, SolverResidualVO * _residualVO,
                                 QPolygonF _points)
{
    if (_points.isEmpty())
         _points  << QPointF(0,0) ;

    int _iteration = _residualVO->getIteration();
    _points << QPointF(50+_iteration,100+_iteration );
    _curve->setSamples(_points);
    _curve->attach(_plot );
    _plot->replot();
    _plot->show();
    return _points;
}



/**
 * @brief Function to set up the Plot Environment
 *
 * @param _plot
 */
void ResidualTabHelper::setPlotEnvironment(QwtPlot * _plot)
{
    _plot->setTitle( "Solver Graphical Evolution" );
    _plot->setCanvasBackground( Qt::white );
    _plot->setAxisTitle(QwtPlot::xBottom,"Iteration");
    _plot->setAxisScale(QwtPlot::xBottom,0,1000,0);
    _plot->setAxisScale(QwtPlot::yLeft,0,1000,0);

    _curve = new QwtPlotCurve();
    _curve->setTitle( "Some Points" );
    //_curve->setPen( Qt::blue, 4 ),
    _curve->setRenderHint( QwtPlotItem::RenderAntialiased, true );
    QwtSymbol *symbol = new QwtSymbol( QwtSymbol::XCross);
    _curve->setSymbol( symbol );
    _plot->show();


}
