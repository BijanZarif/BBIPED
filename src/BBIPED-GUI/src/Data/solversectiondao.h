#ifndef SOLVERSECTIONDAO_H
#define SOLVERSECTIONDAO_H
#include <QString>
#include <QList>

#include "Data/Solver/VO/solversectionvo.h"
#include "Data/Solver/VO/solvervariablevo.h"

class SolverSectionDAO
{
public:
    SolverSectionDAO();
    // CREATE ELEMENTS
    SolverSectionVO * createSectionElement(QString _sectionName);
    SolverSectionVO * createSectionElement(QString _sectionName, QList<SolverVariableVO> _listVariables);

    // FIND ELEMENTS
    SolverSectionVO findSection(QString _sectionName,QList<SolverSectionVO> _list);
    QStringList findSubSectionNamesBySectionName(QString _sectionName,QList<SolverSectionVO> _list);
    QString findSectionNameBySubSectionName(QString _subSectionName, QList<SolverSectionVO> _list);
    SolverSectionVO findSubSection(QString _subSectionName, QList<SolverSectionVO> _list);
    QString findValueNameByVariableName(QList<SolverSectionVO> _solverList, QString _varName);

    QString findValueNameByVariableName(SolverSectionVO _solverVO,QString _varName);
    SolverVariableVO findVariableByFileName(QList<SolverSectionVO> _list, QString _fileName);
    SolverVariableVO findVariableByFileName(QList<SolverVariableVO> _list, QString _fileName);
    QString findVariableNameByValueName(SolverSectionVO _solverVO,QString _valueName);

    //QString findComboFileValueFromComboName(SolverSectionVO _sectionVO, QString _variableName, QString _comboName);

    bool isDefaultValueByFileName(QList<SolverSectionVO> _list, QString _varFileName, QString _cValue);

    // GETTER /SETTER
    QStringList getVariableNameList(QList<SolverVariableVO> _list);

    QStringList getOptionalVariables(QList<SolverSectionVO> _listSection);

    SolverVariableVO findVariableByValueName(SolverSectionVO _solverVO,QString _valueName);


};

#endif // SOLVERSECTIONDAO_H
