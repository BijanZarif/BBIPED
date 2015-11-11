#ifndef PLOTTERMANAGER_H
#define PLOTTERMANAGER_H
#include "Plotter/solverplotter.h"
#include <QList>
#include <QTabWidget>
#include <QWidget>
#include <QString>
class PlotterManager
{
public:
    PlotterManager();
    void createPlotTab(QWidget * vBox, QTabWidget * _tabWidget, SolverPlotter * plot, QString _title, QString _infoLegend);
    void setVariablesIndexToShow(SolverPlotter * plot,QList<int> _listIndex);


};

#endif // PLOTTERMANAGER_H
