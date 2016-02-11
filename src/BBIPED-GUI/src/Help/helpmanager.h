#ifndef HELPMANAGER_H
#define HELPMANAGER_H

#include <QString>

class HelpManager
{
public:
    HelpManager();

    void openHelp(QString _file, QString _folder);
    void openQtAssistant();
    void openHelpURL(QString _externalURL);

};

#endif // HELPMANAGER_H
