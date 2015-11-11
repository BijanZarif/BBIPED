
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
#include "plotterhelper.h"
#include <QString>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

/**
 * @brief Constructor
 *
 */
PlotterHelper::PlotterHelper()
{
}


/**
 * @brief Function that sets up the plot layout
 *
 * @param _parent
 * @param _tab
 * @param plot
 */
void PlotterHelper::setResidualPlotLayout(QWidget * _parent, QTabWidget * _tab, SolverPlotter * plot)
{
    QWidget * vBox = new QWidget(_parent);

    vBox->setWindowTitle( "Residual Plot" );

    plot = new SolverPlotter( vBox );
    plot->setTitle( "Residual Evolution" );

    const int margin = 5;
    plot->setContentsMargins( margin, margin, margin, margin );

    QString info( "Press the legend to en/disable a curve" );

    QLabel *label = new QLabel( info, vBox );

    QVBoxLayout *layout = new QVBoxLayout( vBox );
    layout->addWidget( plot );
    layout->addWidget( label );

    vBox->resize( 600, 400 );
    vBox->show();

    _tab->addTab(vBox,"Residual");
}


/**
 * @brief Plotting function example -- > TODO
 *
 */
/*void PlotterHelper::enableQPlot(QwtPlot * _plot)
{
    //QwtPlot plot;

    _plot->setTitle( "Plot Demo" );
    _plot->setCanvasBackground( Qt::white );
    _plot->setAxisScale( QwtPlot::yLeft, 0.0, 10.0 );
    _plot->insertLegend( new QwtLegend() );

    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->attach( _plot );

    QwtPlotCurve *curve = new QwtPlotCurve();
    curve->setTitle( "Some Points" );
    curve->setPen( Qt::blue, 4 ),
    curve->setRenderHint( QwtPlotItem::RenderAntialiased, true );
////////////////////////////////////////////////
    QwtSymbol *symbol = new QwtSymbol( QwtSymbol::Ellipse,
        QBrush( Qt::yellow ), QPen( Qt::red, 2 ), QSize( 8, 8 ) );
    curve->setSymbol( symbol );

    QPolygonF points;
    points << QPointF( 0.0, 4.4 ) << QPointF( 1.0, 3.0 )
        << QPointF( 2.0, 4.5 ) << QPointF( 3.0, 6.8 )
        << QPointF( 4.0, 7.9 ) << QPointF( 5.0, 7.1 );
    curve->setSamples( points );

    curve->attach(_plot );

    //ui->residualPlot->resize( 600, 400 );
     _plot->show();
}

void PlotterHelper::setPlotEnvironment(QwtPlot * _plot)
{
    _plot->setTitle( "Solver Graphical Evolution" );
    _plot->setCanvasBackground( Qt::white );
    _plot->setAxisTitle(QwtPlot::xBottom,"Iteration");
    _plot->setAxisScale(QwtPlot::xBottom,10,20,0);
    _plot->setAxisScale(QwtPlot::yLeft,10,20,0);
   _plot->show();

}
*/
