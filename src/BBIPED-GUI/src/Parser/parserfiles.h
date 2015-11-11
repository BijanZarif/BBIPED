#ifndef PARSERFILES_H
#define PARSERFILES_H
#include <QString>
#include <QHash>



class ParserFiles
{
public:
    ParserFiles();

    virtual void parseFileProcess() = 0;

   /****************************** Getter/Setter *******************************/
    QString getFilePath();
    void setFilePath (QString _newPath);
    QHash<QString,QString> getParsedValues();
    void setParsedValues(QHash<QString,QString> _newValues);

    /****************************** Remove/Clear *******************************/
    QString removeSpaces(QString _value);

private:
    QString _filePath;
    QHash<QString,QString> _parsedValues;


};

#endif // PARSERFILES_H
