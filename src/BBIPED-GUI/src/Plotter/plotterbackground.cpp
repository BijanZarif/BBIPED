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
#include "plotterbackground.h"

/**
 * @brief Constructor
 *
 */
PlotterBackground::PlotterBackground()
{
    // setZ( 0.0 );
}

/**
 * @brief Function that draws the background
 *
 * @param painter
 * @param
 * @param yMap
 * @param canvasRect
 */
void PlotterBackground::draw( QPainter *painter,
        const QwtScaleMap &, const QwtScaleMap &yMap,
        const QRectF &canvasRect ) const
{
    QColor c( Qt::white );
    QRectF r = canvasRect;

   // for ( int i = 100; i > 0; i -= 10 )
     for ( int i = -20; i < 20; i += 5 )
    {
        r.setBottom( yMap.transform( i + 5 ) );
        r.setTop( yMap.transform( i ) );
        painter->fillRect( r, c );
        c = c.dark( 110 );
    }
}


