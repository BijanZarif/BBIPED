#ifndef USERMESHVO_H
#define USERMESHVO_H

#include <QString>
#include <QStringList>

class UserMeshVO
{
public:
    UserMeshVO();
    QString getFilename();
    QString getUserName();
    QStringList getBoundaryNameList();
    QStringList getBoundaryAssociatedValues();

    void setFilename(QString _name);
    void setUserName(QString _name);
    void setBoundaryNameList(QStringList _list);
    void setBoundaryAssociatedValues(QStringList _list);

    void updateBndValue(QString _boundName, QString _newVal);
    void addBoundary(QString _boundName, QString _val);
    void resetValues();

    bool containsBoundary(QString _bndName);
    void deleteBoundary(QString _bndName);

    void setParentMenu(QString _menu);
    QString getParentMenu();

    void setAssociatedMeshVariables(QStringList _list);
    QStringList getAssociatedMeshVariables();

    bool hasAssociatedMeshVariables();
    void setHasAssociatedMeshVariables(bool _val);

    void initializeBndAssociatedValuesToDefault(QString _val,int _noElements);

private:
    QString _filename;
    QString _userName;
    QStringList _boundaryNameList;
    QStringList _boundaryAssociatedValues;
    QString _parentMenu;

    QStringList _associatedMeshVariables;
    bool _hasAssociatedMeshVariables;
};

#endif // USERMESHVO_H
