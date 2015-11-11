#ifndef RESIDUALTABHELPER_H
#define RESIDUALTABHELPER_H

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_symbol.h>
#include <qwt_legend.h>

#include "Data/Solver/VO/solverresidualvo.h"
class ResidualTabHelper
{
public:
    ResidualTabHelper();

    QPolygonF showPlot(QwtPlot * _plot, SolverResidualVO * _residualVO,
                  QPolygonF _points);
    void setPlotEnvironment(QwtPlot * _plot);

private:
    QwtPlotCurve *_curve;
    QPolygonF _storedPoints;
};

#endif // RESIDUALTABHELPER_H
