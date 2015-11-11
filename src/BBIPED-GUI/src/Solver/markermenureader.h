#ifndef MARKERMENUREADER_H
#define MARKERMENUREADER_H

#include <QDomDocument>
#include <QList>

#include "Data/Solver/VO/solvermarkermenu.h"
class MarkerMenuReader
{
public:
    MarkerMenuReader();
    ~MarkerMenuReader();

    /*********************** Build/Create **************************************/
    void buildAllMarkerMenuNodes();
    void loadAllFile();

    /*********************** Getter/Setter **************************************/
    QString getMarkerName(QDomElement _element, QString tagName);
    QList<SolverMarkerMenu>  getMarkerMenuList();



private:
    QDomDocument * _domtree;
    QList<SolverMarkerMenu>  _markerMenuList;
    bool _isInitialized;

    QStringList createParameterList(QDomElement _element);
    SolverMarkerMenu createMarkerNode(QDomNode _node);
    QStringList createTypeList(QDomElement _element);
    QStringList createUnitList(QDomElement _element);
    QStringList createDefaultValueList(QDomElement _element);


};

#endif // MARKERMENUREADER_H
