#ifndef SALOMETEMPLATEDATA_H
#define SALOMETEMPLATEDATA_H
#include <QString>
#include <QList>
#include "salomevariabledata.h"
class SalomeTemplateData
{
public:
    SalomeTemplateData();

    QString getTemplateName();
    void setTemplateName(QString _name);
    QString getTemplateFileName();
    void setTemplateFileName(QString _name);

    QList<SalomeVariableData> getVariableList();
    void setVariableList(QList<SalomeVariableData> _list);

    bool isEmpty();

    SalomeVariableData findByName(QString _name);


private:
    QString _templateName;
    QString _templateFileName;
    QList<SalomeVariableData> _variableList;

    QString removeWhiteSpaces(QString _str);

};

#endif // SALOMETEMPLATEDATA_H
