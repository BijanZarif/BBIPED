#ifndef SOLVERMARKERMENU_H
#define SOLVERMARKERMENU_H
#include <QString>
#include <QStringList>

class SolverMarkerMenu
{
public:
    SolverMarkerMenu();

    QString getMarkerTitle();
    void setMarkerTitle(QString _newTitle);

    QString getMarkerFileName();
    void setMarkerFileName(QString _newFileName);

    QStringList getParametersList();
    void setParametersList(QStringList _newList);

    void setTypeList(QStringList _newType);
    QStringList getTypeList();

    QString getHelp();
    void setHelp(QString _helpText);

    QStringList getUnitList();
    void setUnitList(QStringList _list);

    QStringList getDefaultValueList();
    void setDefaultValueList(QStringList _list);

    bool isEmpty();


private:
    QString _markerTitle;
    QString _markerFileName;
    QStringList _parameterList;
    QStringList _typeList;
    QString _help;
    QStringList _unitList;
    QStringList _defaultValueList;
};

#endif // SOLVERMARKERMENU_H
