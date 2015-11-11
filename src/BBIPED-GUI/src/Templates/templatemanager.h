#ifndef TEMPLATEMANAGER_H
#define TEMPLATEMANAGER_H

#include <QString>
#include <QHash>
#include <QXmlStreamWriter>
#include <QStringList>
#include <QList>
#include "../Data/Solver/VO/solversectionvo.h"

class TemplateManager
{
public:
    TemplateManager();
    void saveTemplate(QString _templatePath,QHash<QString,QString> _selVar);
    void setSectionTreeList( QList<SolverSectionVO> _list);

private:
    int _sectionId;
    int _subsectionId;
    int _variableId;
    QList<SolverSectionVO> _sectionTreeList;
    QHash<QString,QString> _selectedVariables;

    /*********************************** Getter/Setter ****************************************************/
    QStringList getSectionNames();
    QStringList getVariableNames(QString _subSectionName);
    QStringList getSubSectionNames();
    QStringList getSubSectionNamesBySectionName(QString _sectionName);
    QList<SolverVariableVO> getVariables(QString _subSectionName);


    /*********************************** Create ****************************************************/
    void createTemplateFile(QStringList _sectionList, QXmlStreamWriter * _xmlWriter);
    void createSectionElement(QString _sectionName, QXmlStreamWriter * _xmlWriter);
    void createXmlElement(QString _tagName, QString _value,QXmlStreamWriter * _xmlWriter);

    void createSubsectionList(QString _sectionName,  QXmlStreamWriter * _xmlWriter);
    void createVariableList(QString _sectionName, QString _subSectionName, QXmlStreamWriter * _xmlWriter);

    void createHelpElement(SolverVariableVO _variableVO, QXmlStreamWriter * _xmlWriter);
    void createComboValues(SolverVariableVO _variableVO, QXmlStreamWriter * _xmlWriter);
    void createValueTag(SolverVariableVO _variableVO, QXmlStreamWriter * _xmlWriter);
    void createVariableList(QString _subSectionName, QXmlStreamWriter * _xmlWriter);
    void createVariable(SolverVariableVO _variableVO, QXmlStreamWriter * _xmlWriter);

};

#endif // TEMPLATEMANAGER_H
