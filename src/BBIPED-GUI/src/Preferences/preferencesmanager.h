#ifndef PREFERENCESMANAGER_H
#define PREFERENCESMANAGER_H

#include "../Data/PreferencesData/userpreferencesdata.h"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

class PreferencesManager
{
public:
    PreferencesManager();
    UserPreferencesData getUserPreferences();
    void saveUserPreferences(UserPreferencesData _newPreferences);

private:

    /**************************************** READ ***************************************************/
    void readPreferencesFile(UserPreferencesData * _userData);

    /**************************************** WRITE ***************************************************/

     void writeRecentProjects(QXmlStreamWriter * _xmlWriter, UserPreferencesData  _userData);
     void writeUserPreferences(QXmlStreamWriter * _xmlWriter, UserPreferencesData _userData);
     void writeWarningPreferences(QXmlStreamWriter * _xmlWriter, UserPreferencesData  _userData);
     void writeTemplatePreferences(QXmlStreamWriter * _xmlWriter, UserPreferencesData  _userData);

    /**************************************** Getter/Setter ***************************************************/
    void getRecentProjects(QXmlStreamReader *_xmlReader, UserPreferencesData * _userData);
    void getUserToolPreferences(QXmlStreamReader *_xmlReader, UserPreferencesData * _userData);
    void getUserTemplatePreferences(QXmlStreamReader *_xmlReader, UserPreferencesData * _userData);
    QStringList getProjectList(QXmlStreamReader *_xmlReader);
    QString getViewerInformation(QXmlStreamReader *_xmlReader, QString _keyName, QString _attName);
    void setSolverEngineInformation(QXmlStreamReader *_xmlReader, UserPreferencesData * _userData);
    bool getWarningPreferences(QXmlStreamReader *_xmlReader, QString _warningType,QString _warningAtt);
    void getUserWarningPreferences(QXmlStreamReader *_xmlReader, UserPreferencesData * _userData);

};

#endif // PREFERENCESMANAGER_H
