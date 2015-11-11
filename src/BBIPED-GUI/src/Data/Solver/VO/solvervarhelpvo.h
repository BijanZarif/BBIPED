#ifndef SOLVERVARHELPVO_H
#define SOLVERVARHELPVO_H

#include <QString>
class SolverVarHelpVO
{
public:
    SolverVarHelpVO();

    void setDefinitionText(QString _txt);
    QString getDefinitionText();

    void setValuesText(QString _txt);
    QString getValuesText();

    void setDetailsText(QString _txt);
    QString getDetailsText();

private:

    QString _definitionTxt;
    QString _valuesTxt;
    QString _detailsTxt;
};

#endif // SOLVERVARHELPVO_H
