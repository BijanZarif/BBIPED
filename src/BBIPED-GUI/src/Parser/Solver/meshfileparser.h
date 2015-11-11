#ifndef MESHFILEPARSER_H
#define MESHFILEPARSER_H
#include "Parser/parserfiles.h"
#include <QString>
#include <QHash>
class MeshFileParser: public ParserFiles
{
public:
    MeshFileParser();
    void parseFileProcess();
    bool is3DMesh();
private:
    bool _is3dMesh;
    bool _found3DMesh;

    QHash<QString, QString> _currentParsedValues;
    int _id;
    void analyseLine(QString _line);
    bool containsKeyTagName(QString _line);
    void saveTokenElements(QString _line);

};

#endif // MESHFILEPARSER_H
