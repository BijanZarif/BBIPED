#ifndef PLOTTERHELPER_H
#define PLOTTERHELPER_H

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_symbol.h>
#include <qwt_legend.h>
#include <QTabWidget>
#include "Plotter/solverplotter.h"

class PlotterHelper
{
public:
    PlotterHelper();
    void setResidualPlotLayout(QWidget  * _parent, QTabWidget * _tab, SolverPlotter * plot);

};

#endif // PLOTTERHELPER_H
