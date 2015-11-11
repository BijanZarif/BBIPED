#ifndef SALOMEVARIABLEDATA_H
#define SALOMEVARIABLEDATA_H
#include <QString>
class SalomeVariableData
{
public:
    SalomeVariableData();

    QString getName();
    void setName(QString _newName);

    QString getFileName();
    void setFileName(QString _newName);

    QString getDefaultValue();
    void setDefaultValue(QString _val);

    QString getCurrentValue();
    void setCurrentValue(QString _val);

    QString getOptimizedValue();
    void setOptimizedValue(QString _val);

private:
    QString _name;
    QString _fileName;
    QString _defaultValue;
    QString _currentValue;
    QString _optimizedValue;

    QString removeWhiteSpaces(QString _str);

};

#endif // SALOMEVARIABLEDATA_H
