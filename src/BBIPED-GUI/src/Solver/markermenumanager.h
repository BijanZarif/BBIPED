#ifndef MARKERMENUMANAGER_H
#define MARKERMENUMANAGER_H

#include <QDomDocument>
#include <QString>
#include <QDomElement>

#include "Data/Solver/VO/solvermarkermenu.h"

class MarkerMenuManager
{
public:
    MarkerMenuManager();
    QList<SolverMarkerMenu> getAllMarkerMenuList();
    SolverMarkerMenu getMarkerNodeByName(QString _nodeName);

private:
  QList<SolverMarkerMenu> _markerMenuList;

};

#endif // MARKERMENUMANAGER_H
