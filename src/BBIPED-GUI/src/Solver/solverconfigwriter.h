#ifndef SOLVERCONFIGWRITER_H
#define SOLVERCONFIGWRITER_H

#include <QString>
#include <QHash>
#include "Solver/solverconfigmanager.h"
#include <QTextStream>
#include "../Data/ProjectData/VO/projectdetails.h"
#include "Data/Solver/VirtualMRF/virtualmrfvo.h"
#include <QList>
#include <QStringList>

class SolverConfigWriter
{
public:
    SolverConfigWriter();

    /******************************** Write *****************************/
    void writeToConfigurationFile(QString _filePath, QDomDocument *  _configTree, ProjectDetails _projectDetails);
    void writeDOMTree(QDomDocument *  _configTree, QTextStream * _fileOutput);
    void writeHeaderFile(QTextStream * _fileOutput, ProjectDetails _projectDetails);

    /******************************** Write Section/Subsection *****************************/
    void writeSectionElement(QDomElement _sectionElement, QTextStream * _fileOutput);
    void writeSectionList(QDomElement _sectionList, QTextStream * _fileOutput);
    void writeSubsectionList(QDomElement _subSectionList, QTextStream * _fileOutput);
    void writeSubsectionElement(QDomElement _subSectionElement, QTextStream * _fileOutput);

    /******************************** Write Variable*****************************/
    void writeVariableElement(QDomElement _varElement, QTextStream * _fileOutput,bool _isVirtualMRF);
    void writeVariableValue(QDomElement _valueElement, QTextStream * _fileOutput,bool _isVirtualMRF);
    void writeVariableList(QDomElement _variableList, QTextStream * _fileOutput, bool _isVirtualMRF);

    void setUserValues(QHash<QString,QString> _values);
    void setVirtualMRFUserValues(QList<VirtualMrfVO> _values);
    void setOptionalVariableList(QStringList _list);


private:
    QHash<QString,QString> _userValues;
    QList<VirtualMrfVO> _virtualMRFUserValues;
    QStringList _optionalVariables;

    //QStringList _repeatableVariables;

void printVirtualMRFValues(QDomElement _variableList, QTextStream * _fileOutput);

};

#endif // SOLVERCONFIGWRITER_H
