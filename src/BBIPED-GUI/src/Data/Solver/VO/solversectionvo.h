#ifndef SOLVERSECTIONVO_H
#define SOLVERSECTIONVO_H
#include <QString>
#include <QStringList>
#include "Data/Solver/VO/solvervariablevo.h"

class SolverSectionVO
{
public:
    SolverSectionVO();
    ~SolverSectionVO();
    /************************************ Getters/Setters************************************/

    void setSectionName(QString _name);
    QString getSectionName();

    void setSubsectionName(QString _name);
    QString getSubsectionName();

    void setSubsectionVarList(QList<SolverVariableVO> _list);
    QList<SolverVariableVO> getSubsectionVarList();

    //void changeVariableVisibility(QString _name, bool _visibility,int _pos );
private:

    QString _sectionName;
    QString _subsectionName;
    QList<SolverVariableVO> _variableList;

};

#endif // SOLVERSECTIONVO_H
