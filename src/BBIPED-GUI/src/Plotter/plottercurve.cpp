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
 *  Author: Dec 2016: Carmen Alonso Montes calonso@bcamath.org
***/
#include "plottercurve.h"
/*
#include <qwt_plot_item.h>
#include <qwt_plot.h>
*/
#include <QColor>

/**
 * @brief Constructor
 *
 * @param title
 */
PlotterCurve::PlotterCurve( const QString &title ):
    QwtPlotCurve( title )
{
    setRenderHint( QwtPlotItem::RenderAntialiased );
}

/**
 * @brief Function that sets up the color
 *
 * @param color
 */
void PlotterCurve::setColor( const QColor &color )
{
    QColor c = color;
    c.setAlpha( 150 );

    setPen( c );
    QPen pen;  // creates a default pen

    pen.setStyle(Qt::SolidLine);
    pen.setWidth(3);
    pen.setColor(c);
    this->setPen(pen);
}

