#include "plottermanager.h"

#include <QLabel>
#include <QVBoxLayout>

PlotterManager::PlotterManager()
{
}


void PlotterManager::createPlotTab(QWidget * vBox, QTabWidget * _tabWidget, SolverPlotter * plot, QString _title, QString _infoLegend)
{
    vBox->setWindowTitle( _title);

    const int margin = 10;

    plot->setContentsMargins( margin, margin, margin, margin );

    QString info(_infoLegend);

    QLabel *label = new QLabel( info, vBox );
    QVBoxLayout *layout = new QVBoxLayout( vBox );
    layout->addWidget( plot );
    layout->addWidget( label );
    vBox->show();

    _tabWidget->addTab(vBox,_title);
}


void PlotterManager::setVariablesIndexToShow(SolverPlotter * plot, QList<int> _listIndex)
{

}
