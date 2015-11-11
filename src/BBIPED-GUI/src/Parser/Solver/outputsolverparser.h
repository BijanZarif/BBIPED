#ifndef OUTPUTSOLVERPARSER_H
#define OUTPUTSOLVERPARSER_H
#include <QString>
#include <QList>
#include "Data/Solver/VO/solverresidualvo.h"
class OutputSolverParser
{
public:
    OutputSolverParser();
    void parseOutput(SolverResidualVO * _residualObj, QString _line);
    bool getStartParsing();
    void checkWhenDataToPaintAvailable(QString _line);

private:
    bool _startParsing;
    bool _identifiedNoElementsToPlot;
    QList<int> _nanPositions;

    void fillResidualObject(SolverResidualVO * _residualOb,QStringList _list);
    QStringList checkNanValues(SolverResidualVO * _residualOb,QStringList _list);
    bool valueIsNan(QString _val);
    QStringList buildResidualList( QList<bool> _validVal, QStringList _list);
    QList<bool> getValidElementsToPrint(SolverResidualVO * _residualOb);
    bool isValidLine(QStringList _list);
    void identifyNoElementsToPlot(QStringList _list,SolverResidualVO * _residualObj);

};

#endif // OUTPUTSOLVERPARSER_H
