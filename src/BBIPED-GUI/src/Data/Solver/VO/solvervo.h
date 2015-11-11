#ifndef SOLVERVO_H
#define SOLVERVO_H

#include <QString>
#include <QHash>


class SolverVO
{
public:

    SolverVO();
    ~SolverVO();

    /******************************* Update/Copy *********************************/
    void updateSolverConfigFileMapValues(QString _varName,QString _varValue);
    void copy(SolverVO _objToCopy);

    /******************************* Reset/Clear *********************************/
    void resetMapValues();
    void clear();

    /******************************* Is/Have *********************************/

    bool isMeshFileEmpty();
    bool isConfigFileEmpty();

    /******************************* Getter and Setters *********************************/
    void setSolverFilePath(QString _path);
    QString getSolverFilePath();

    void setSolverConfigFilePath(QString _path);
    QString getSolverConfigFilePath();

    void setSolverConfigFileMapValues(QHash<QString,QString> _newMapValues);
    QHash<QString,QString> getSolverConfigFileMapValues();
private:
    QString _solverFilePath;
    QString _solverConfigFilePath;
    QHash<QString, QString> _solverConfigFileMapValues;
    QString _solverEngine;


};

#endif // SU2VO_H
