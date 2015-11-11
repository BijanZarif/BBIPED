#ifndef ABSTRACTRULE_H
#define ABSTRACTRULE_H

#include <QStringList>
#include <QList>

class AbstractRule
{
public:
    AbstractRule();
   // virtual void executeRule() = 0;

    void setVariablesToBeDisabled(QStringList _list);
    QStringList getVariablesToBeDisabled();

    void setVariablesToBeEnabled(QStringList _list);
    QStringList getVariablesToBeEnabled();

private:
    QStringList _listVariablesToBeDisabled;
    QStringList _listVariablesToBeEnabled;
};

#endif // ABSTRACTRULE_H
