#ifndef SHOWMESSAGEHELPER_H
#define SHOWMESSAGEHELPER_H

#include <QString>
#include <QStatusBar>
//#include <mainwindow.h>
#include <QWidget>

class ShowMessageHelper
{
public:
    ShowMessageHelper();

    void  showMessage( QString msg, QStatusBar * _statusBar );
    bool showMessageForSaving();
    void showMessageWindow(QString _title, QString _message, QWidget * _window);
    bool showMessageReset();
    bool showWarningMessage(QString _title, QString _message);


};

#endif // SHOWMESSAGEHELPER_H
