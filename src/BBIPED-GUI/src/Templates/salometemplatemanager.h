#ifndef SALOMETEMPLATEMANAGER_H
#define SALOMETEMPLATEMANAGER_H

#include <QString>
#include <QList>
#include <QStringList>
#include <QHash>
#include <QDomDocument>
#include <QDomElement>
#include <QXmlStreamWriter>
#include "../Data/SalomeTemplateData/salometemplatedata.h"

class SalomeTemplateManager
{
public:
    SalomeTemplateManager();
    void loadTemplate();
    void updateTemplateValues(QList<SalomeTemplateData> _templates);
    void updateUserTemplateValues(QList<SalomeTemplateData> _templates);
    void saveTemplate();

    QList<SalomeTemplateData> getTemplates();
    QStringList getTemplateNameList();

    QList<SalomeTemplateData> getUserTemplates();
    QStringList getUserTemplateNameList();

    QString getTemplateFileNameByTemplateName(QString _templateName);

    void deleteUserDefinedTempalte(QString _templateName);

    bool isInitialized();
    void restoreToDefaultTemplate(int i);
    //void setBBIPEDHome(QString _homePath);
    //void restoreToDefaultUserTemplate(int i);

private:

    QList<SalomeTemplateData> _templatesList;
    QList<SalomeTemplateData> _userTemplatesList;
    QStringList _templateNameList;
    QStringList _userTemplateNameList;

    QString _templatePath;
    QString _bbipedHomePath;

    bool _isInitialized;

    enum{
        selectPredefinedTemplates  = 0,
        selectUserDefinedTemplates = 1
    };

    void buildTemplateMenus(QDomDocument * _domTree);
    void buildPredefinedTemplateMenus(QDomElement _domTree);
    void buildUserTemplateMenus(QDomElement _domTree);

    void buildTemplateData(QDomElement _templateNode,int _option);

    void buildVariableList(QDomElement _variableListNode, SalomeTemplateData * _template);

    SalomeVariableData buildVariableData(QDomElement _variableNode);

    void saveTemplateData(QXmlStreamWriter * _xmlWriter, QList<SalomeTemplateData> _template);
    void saveVariableData(QXmlStreamWriter * _xmlWriter, QList<SalomeVariableData> _varList);
    void updateTemplateNameList(int _option);

};

#endif // SALOMETEMPLATEMANAGER_H
