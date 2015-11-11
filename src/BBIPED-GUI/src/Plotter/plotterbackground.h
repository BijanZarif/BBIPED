#ifndef PLOTTERBACKGROUND_H
#define PLOTTERBACKGROUND_H

#include <qwt_plot_item.h>
#include <QPainter>
#include <qwt_scale_map.h>
#include <QRectF>

class PlotterBackground :  public QwtPlotItem
{
public:
    PlotterBackground();
     virtual int rtti() const
    {
      //  return QwtPlotItem::Rtti_PlotUserItem;
    }
    virtual void draw( QPainter *painter,
        const QwtScaleMap &, const QwtScaleMap &yMap,
        const QRectF &canvasRect ) const;
};

#endif // PLOTTERBACKGROUND_H
