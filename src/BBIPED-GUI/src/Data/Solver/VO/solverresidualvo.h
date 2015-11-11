#ifndef SOLVERRESIDUALVO_H
#define SOLVERRESIDUALVO_H

#include <QStringList>

class SolverResidualVO
{
public:
    SolverResidualVO();

    int getIteration();
    double getTimeValue();
    double getResRho();
    double getResRhoe();
    double getCliftTotal();
    double getCDragTotal();

    void setIteration(int _newIteration);
    void setTimeValue(double _newVal);
    void setResRho(double _newVal);
    void setResRhoe(double _newVal);
    void setCliftTotal(double _newVal);
    void setCDragTotal(double _newVal);
    void clearAllData();

    void setIterationEnabled(bool _val);
    void setTimeEnabled(bool _val);
    void setResRhoEnabled(bool _val);
    void setResRhoeEnabled(bool _val);
    void setCliftTotalEnabled(bool _val);
    void setDragTotalEnabled(bool _val);

    bool getIterationEnabled();
    bool getTimeEnabled();
    bool getResRhoEnabled();
    bool getResRhoeEnabled();
    bool getCliftTotalEnabled();
    bool getDragTotalEnabled();

    void enableAllValues();
    void fillValues(QStringList _list);

private:
    int _iteration;
    double _time;
    double _resRho;
    double _resRhoe;
    double _cliftTotal;
    double _cDragTotal;
    bool _iterationEnabled;
    bool _timeEnabled;
    bool _resRhoEnabled;
    bool _resRhoeEnabled;
    bool _cliftTotalEnabled;
    bool _cDragTotalEnabled;

};

#endif // SOLVERRESIDUALVO_H
