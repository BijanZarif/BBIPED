#ifndef CONFIGURATIONFILEPARSER_H
#define CONFIGURATIONFILEPARSER_H
#include "Parser/parserfiles.h"
#include <QHash>
#include <QString>

#include <QList>
#include "Data/Solver/VirtualMRF/virtualmrfvo.h"
#include "Defines/Tool/Solver/SolverDefines.h"
class ConfigurationFileParser: public ParserFiles
{
public:
    ConfigurationFileParser();
    void parseFileProcess();

    QList<VirtualMrfVO> getParsedVirtualMRFData();

 private:
    QHash<QString, QString> _currentParsedValues;
    QList<VirtualMrfVO> _parsedVirtualMRFData;

    void analyseLine(QString _line);
    bool isCommentedLine(QString _line);
    void saveTokenElements(QString _line);
    QString removeCommentedPart(QString _text);
    void saveVirtualMRFValue(QString _keyName,QString _valueVal);
    int findVirtualMRFPositionByZoneName(QString _name);
    void saveDomainClippingValue(QString _keyName,QString _valueVal);
};

#endif // CONFIGURATIONFILEPARSER_H
