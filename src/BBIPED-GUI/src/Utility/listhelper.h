#ifndef LISTHELPER_H
#define LISTHELPER_H

#include <QString>
#include <QHash>
#include <QStringList>


class ListHelper
{
public:
    ListHelper();

    QStringList getListFromQHash(QHash<QString,QString> _map);
    bool isContainedInList(QStringList _list, QString _value);

};

#endif // LISTHELPER_H
