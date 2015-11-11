#ifndef SOLVERMARKERMENUHELPER_H
#define SOLVERMARKERMENUHELPER_H
#include <QString>
#include <QHash>
#include <QTableWidget>
#include <QStringList>
#include "Data/Solver/VO/solvervariablevo.h"
#include "Data/Solver/VO/solversectionvo.h"

#include "WindowHelper/Dialog/SolverDialog/solverconfigmenuhelper.h"

class SolverMarkerMenuHelper: public SolverConfigMenuHelper
{
public:
    SolverMarkerMenuHelper();

    /************************************ FILL ************************************/

    void fillMarkerMenuTable(QString _parentName,QHash<QString,QString> _meshVariables,
                             QTableWidget * _table, QList<SolverVariableVO> _list);
    /************************************ ADD/UPDATE ************************************/

    void addMarkerVariable( QTableWidget * _table,QString _varName, QStringList _list,bool _isMultizoneMarker);
    void addMarkerMenu(QDomDocument * _rootTree,QDomDocument * _menuToFill);
    void removeMarkerElementsFromConfigFile(QHash<QString,QString> * _mapValues);
    bool checkMarkerMeshVariables(QHash<QString,QString>  _userMarkers,
                                  QHash<QString,QString>  _meshFileMarkers,
                                  QHash<QString,QString> * _markersConfig );
    bool checkMarkerMeshVariables(QHash<QString,QString>  _userMarkers,
                                              QHash<QString,QString>  _meshFileMarkers);
    /************************************ GETTER/SETTER ************************************/

   void addExtraColumn(QTableWidget * _table, int _lastColumn,QString _defaultVal);
   void addEditIconToColumn(QTableWidget * _table, int row, int _column);



};
#endif // SOLVERMARKERMENUHELPER_H
