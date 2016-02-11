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
 *
 *  *  Author: Dec 2016: Carmen Alonso Montes calonso@bcamath.org

***/

#include "solverplotter.h"
#include <qwt_plot_canvas.h>
#include <qwt_legend.h>
#include <qwt_scale_widget.h>
//#include <qwt_legend_label.h>
#include <qwt_plot_item.h>
#include <qwt_symbol.h>
#include <qwt_plot_curve.h>
#include <QPen>
#include <QSize>


#include <QPointF>

#include <stdio.h>
#include <iostream>
using namespace std;

#include "Plotter/plotterbackground.h"

// TO BE REMOVED
#include <qwt_legend_item.h>
#include <qwt_plot_layout.h>

#include <qwt_plot_renderer.h>

// Plotting libraries
#include <QImageWriter>
#include<QFileDialog>


/**
 * @brief Constructor
 *
 * @param parent
 */
SolverPlotter::SolverPlotter(QWidget * parent):
    QwtPlot(parent),
  dataCount( 0 )
{
    setAutoReplot( true );
    canvas()->setBorderRadius( 10 );
    setCanvasBackground(Qt::white);
    plotLayout()->setAlignCanvasToScales(true);
    QwtLegend *legend = new QwtLegend;
    legend->setItemMode(QwtLegend::CheckableItem);
    this->insertLegend(legend, QwtPlot::RightLegend);
    setAxisLayout();
    initCurves();
    showCurves();
    connect(this, SIGNAL(legendChecked(QwtPlotItem *, bool)),
         SLOT(showCurve(QwtPlotItem *, bool)));

}

/**
 * @brief Function to clear the current Canvas
 *
 */
void SolverPlotter::clearCanvas()
{
 /*    data[TimeSim]._points.clear();
     data[TimeSim].curve->setSamples(data[TimeSim]._points);*/
     data[resRho]._points.clear();
     data[resRho].curve->setSamples(data[resRho]._points);
     data[resRhoe]._points.clear();
     data[resRhoe].curve->setSamples(data[resRhoe]._points);
 /*    data[cliftTotal]._points.clear();
     data[cliftTotal].curve->setSamples(data[cliftTotal]._points);
     data[cDragTotal]._points.clear();
     data[cDragTotal].curve->setSamples(data[cDragTotal]._points);*/
     showCurves();
     replot();
}

/**
 * @brief Function that sets up the Axis Layout
 *
 */
void SolverPlotter::setAxisLayout()
{
    setAxisTitle( QwtPlot::xBottom, " Iteration No." );
    setAxisScale( QwtPlot::xBottom, 0, HISTORY );
    setAxisTitle( QwtPlot::yLeft, "Log(Residuals)" );
    setAxisScale( QwtPlot::yLeft, _scaleY, _scaleYPlus );
}

/**
 * @brief Function to show the curves
 *
 */
void SolverPlotter::showCurves()
{
    showCurve( data[resRho].curve, true );
    showCurve( data[resRhoe].curve, true );

}

/**
 * @brief Function that gets the curve names
 *
 * @param _id
 * @return QString
 */
QString SolverPlotter::getCurveName(int _id)
{
    switch(_id)
    {
       case 0:
         return "resRho";
         break;
    case 1:
         return "resRhoe";
         break;
    otherwise:
        return "";
    }
}

/**
 * @brief Function that sets the color
 *
 * @param _curve
 * @param _id
 */
void SolverPlotter::setColor(PlotterCurve * _curve, int _id)
{
    switch(_id)
    {
        case 0:
           _curve->setColor( Qt::red );
           break;
        case 1:
            _curve->setColor( Qt::blue );
            break;
   }
}

/**
 * @brief Function that updates the values of the curve
 *
 * @param curve
 * @param _id
 */
void SolverPlotter::updateValues(PlotterCurve * curve, int _id)
{
    switch(_id)
    {
    case 0:
        data[resRho].curve = curve;
        (data[resRho]._points) << QPointF(0,0);
        break;
    case 1:
        data[resRhoe].curve = curve;
        (data[resRhoe]._points) << QPointF(0,0);
        break;
    }
}

/**
 * @brief Function that inits the curves
 *
 */
void SolverPlotter::initCurves()
{
    PlotterCurve * curve;
    int _numCurves =2; // 5
    for (int i = 0; i < _numCurves; i++)
    {
        QString _nameCurve = getCurveName(i);
        curve = new PlotterCurve( _nameCurve);
        setColor(curve,i);
        curve->setStyle(QwtPlotCurve::Lines);

        curve->attach( this );
        updateValues(curve,i);
    }

}


/**
 * @brief Function that shows the points of the read curve
 *
 * @param _obj
 */
void SolverPlotter::showPoints(SolverResidualVO * _obj)
{
    int _iteration  =  _obj->getIteration();
    updateLegendItemsInGraphic(_obj);
    if (_iteration < 0 )
        return;
    // Timing
     if (_obj->getResRhoEnabled())
    {
       data[resRho]._points << QPointF(_iteration,_obj->getResRho());
       data[resRho].curve->setSamples( data[resRho]._points);
    }
    if (_obj->getResRhoeEnabled())
    {
       data[resRhoe]._points << QPointF(_iteration,_obj->getResRhoe());
       data[resRhoe].curve->setSamples( data[resRhoe]._points);
    }

    if( (_obj->getResRhoe() > _scaleYPlus) || (_obj->getResRhoe() < _scaleY) ||
           (_obj->getResRho() > _scaleYPlus) || (_obj->getResRho() < _scaleY)  )
        setAxisAutoScale(QwtPlot::yLeft,true);
    if (_iteration > 100)
        setAxisAutoScale(QwtPlot::xBottom,true);
    replot();
}


/**
 * @brief Function to reset all the points
 *
 * @param _iteration
 */
void SolverPlotter::resetAllPointValues(int _iteration)
{
    data[resRho]._points.clear();
    data[resRhoe]._points.clear();

    setAxisScale( QwtPlot::xBottom, dataCount, dataCount + HISTORY );

    replot();

}


/**
 * @brief Function to show the curve
 *
 * @param item
 * @param on
 */
void SolverPlotter::showCurve( QwtPlotItem *item, bool on )
{

    item->setVisible(on);
    QWidget *w = legend()->find(item);
    if ( w && w->inherits("QwtLegendItem") )
        ((QwtLegendItem *)w)->setChecked(on);
    replot();
}



/**
 * @brief
 *
 * @param _obj
 */
void SolverPlotter::updateLegendItemsInGraphic(SolverResidualVO * _obj)
{
    setReadOnlyLegend(this->legend(),data[resRho].curve, _obj->getResRhoEnabled() );
    setReadOnlyLegend(this->legend(),data[resRhoe].curve, _obj->getResRhoeEnabled() );

}


/**
 * @brief
 *
 * @param _obj
 */
void SolverPlotter::resetLegend(SolverResidualVO * _obj)
{

    if (_obj->getResRhoEnabled())
        data[resRho].curve->setItemAttribute(QwtPlotItem::Legend,false);
    if (_obj->getResRhoeEnabled())
         data[resRhoe].curve->setItemAttribute(QwtPlotItem::Legend,false);

    replot();

     data[resRho].curve->setItemAttribute(QwtPlotItem::Legend,true);
    data[resRhoe].curve->setItemAttribute(QwtPlotItem::Legend,true);
     replot();
}



/**
 * @brief
 *
 * @param legend
 * @param item
 * @param on
 */
void SolverPlotter::setReadOnlyLegend(QwtLegend *legend,  QwtPlotItem *item, bool on )
{
    item->setItemAttribute(QwtPlotItem::Legend,on); // This function removes the item from the legend
}

/**
 * @brief
 *
 */
void SolverPlotter::exportPlot()
{
#ifndef QT_NO_PRINTER
    QString fileName = "image.pdf";
#else
    QString fileName = "image.png";
#endif

#ifndef QT_NO_FILEDIALOG
    const QList<QByteArray> imageFormats =
        QImageWriter::supportedImageFormats();

    QStringList filter;
    filter += "PDF Documents (*.pdf)";
#ifndef QWT_NO_SVG
    filter += "SVG Documents (*.svg)";
#endif
    filter += "Postscript Documents (*.ps)";

    if ( imageFormats.size() > 0 )
    {
        QString imageFilter("Images (");
        for ( int i = 0; i < imageFormats.size(); i++ )
        {
            if ( i > 0 )
                imageFilter += " ";
            imageFilter += "*.";
            imageFilter += imageFormats[i];
        }
        imageFilter += ")";

        filter += imageFilter;
    }

    fileName = QFileDialog::getSaveFileName(
        this, "Export File Name", fileName,
        filter.join(";;"), NULL, QFileDialog::DontConfirmOverwrite);
#endif
    if ( !fileName.isEmpty() )
    {
        QwtPlotRenderer renderer;
        renderer.setDiscardFlag(QwtPlotRenderer::DiscardBackground, false);

        renderer.renderDocument(this, fileName, QSizeF(300, 200), 85);
    }
}
