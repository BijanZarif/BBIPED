#ifndef PLOTTERCURVE_H
#define PLOTTERCURVE_H

#include <qwt_plot_curve.h>

class PlotterCurve : public QwtPlotCurve
{
public:
    PlotterCurve(const QString &title);
     void setColor( const QColor &color );
};

#endif // PLOTTERCURVE_H
