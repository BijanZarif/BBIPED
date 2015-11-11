#ifndef SALOMEPYTHONFILEUTIL_H
#define SALOMEPYTHONFILEUTIL_H
#include <QString>
#include "salometemplatedata.h"
#include <QFile>
#include <QTextStream>
#include <QList>
#include "salomevariabledata.h"


class SalomePythonFileUtil
{
public:
    SalomePythonFileUtil();

    void setFileName (QString _name);
    void setPathProject (QString _path);
    void setTemplateData(SalomeTemplateData _data);
    void setTemplateDataList(QList<SalomeTemplateData> _data);
    void setVariableList(QList< QList<SalomeVariableData> > _variablesList);


    void buildPythonFile();
    void buildPythonFileForOptimization(int _iterations);



private:
    QString _fileName;
    QString _pathProject;
    SalomeTemplateData _templateData;
    QList<SalomeTemplateData> _templateDataList;
    QList< QList<SalomeVariableData> > _variablesList;

    void buildHeader(QTextStream * _textWriter);
    void buildMain(QTextStream * _textWriter);
    void buildMainForOptimization(QTextStream * _textWriter,int _iterations);
    QString getVariablesFromTemplate();
    QString getVariables(int _pos);

};

#endif // SALOMEPYTHONFILEUTIL_H
