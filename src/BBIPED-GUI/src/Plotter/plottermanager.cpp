
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
