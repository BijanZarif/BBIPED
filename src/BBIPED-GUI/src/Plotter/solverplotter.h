#ifndef SOLVERPLOTTER_H
#define SOLVERPLOTTER_H
#include <QWidget>
#include <qwt_plot.h>
#include <QPolygonF>

#include "Data/Solver/VO/solverresidualvo.h"
#include "Plotter/plottercurve.h"

#define HISTORY 100 // seconds

class QwtPlotCurve;

class SolverPlotter : public QwtPlot
{
       Q_OBJECT
public:
    enum ResidualData
    {
//        TimeSim,
        resRho,
        resRhoe,
//        cliftTotal,
//        cDragTotal,

        NResidualData
    };

    SolverPlotter( QWidget * = 0 );
    const QwtPlotCurve *plotterCurve( int id ) const
    {
        return data[id].curve;
    }

    void showPoints(SolverResidualVO * _obj);
    void clearCanvas();
    void updateLegendItemsInGraphic(SolverResidualVO * _obj);
    void resetLegend(SolverResidualVO * _obj);
    void exportPlot();

private Q_SLOTS:
    void showCurve(QwtPlotItem *, bool on);
private:
    struct
    {
        QwtPlotCurve *curve;
        QPolygonF _points;
    } data[NResidualData];

    double timeData[HISTORY];

    int dataCount;

    void initCurves();
    void showCurves();
    void setAxisLayout();
    QString getCurveName(int _id);
    void setColor(PlotterCurve * _curve, int _id);
    void updateValues(PlotterCurve * curve, int id);
    void resetAllPointValues(int _iteration);
    void setReadOnlyLegend(QwtLegend *legend,  QwtPlotItem *item, bool on );
};

#endif // SOLVERPLOTTER_H





