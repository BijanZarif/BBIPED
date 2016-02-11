/***
 *    This file is part of BBIPED GUI.
 *
 *   BBIPED GUI is free software: you can redistribute it and/or modify
 *   it under the terms of the Lesser GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   BBIPED GUI is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   Lesser GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with Foobar. If not, see <http://opensource.org/licenses/LGPL-3.0>.
 *
 *  *  Author: Dec 2016: Carmen Alonso Montes calonso@bcamath.org

***/
#include <QtGui/QApplication>
#include "mainwindow.h"
#include <QTextStream>
#include <QFile>
#include <QDir>
//#include <QProcess>
#include <QCoreApplication>

#include "Plotter/solverplotter.h"

// Solucion de: http://qtway.blogspot.com.es/2009/11/persistent-logging-for-debug-purpose.html
QTextStream *out = 0;

/**
 * @brief
 *
 * @param type
 * @param msg
 */
void logOutput(QtMsgType type, const char *msg)
{
    QString debugdate = QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss");
    switch (type)
    {
    case QtDebugMsg:
        debugdate += "[D]";
        break;
    case QtWarningMsg:
        debugdate += "[W]";
        break;
    case QtCriticalMsg:
        debugdate += "[C]";
        break;
    case QtFatalMsg:
        debugdate += "[F]";
    }
    (*out) << debugdate << " " << msg << endl;

    if (QtFatalMsg == type)
    {
        abort();
    }
}


/**
 * @brief
 *
 * @param argc
 * @param argv[]
 * @return int
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //-----------------------------------------------------------------
    // Logging
    QString _pathApplication = QCoreApplication::applicationDirPath();
    QString fileName = QDir(_pathApplication).filePath("logfile.log");
        QFile *log = new QFile(fileName);
        if (log->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
            out = new QTextStream(log);
            qInstallMsgHandler(logOutput);
        } else {
            qDebug() << "Error opening log file '" << fileName << "'. All debug output redirected to console.";
        }
    //-----------------------------------------------------------------
/*********************************************/
// Adding the Splash functionality
    QPixmap pixmap(":/logo/images/logoBBIPED_Full.png");
    QSplashScreen splash(pixmap);
    splash.show();
    splash.showMessage("Loading Application");
    qApp->processEvents();//This is used to accept a click on the screen so that user can cancel the screen
/*********************************************/
    MainWindow w;
    w.setWindowTitle("BBIPED Software");
    QIcon _logo(":/logo/images/icon_31x64.png");
    a.setWindowIcon(_logo);
    w.show();
/*********************************************/
    splash.finish(&w);
/*-------------------------------------------------------------/
/***** Adding the documentation to the assistant **************/
// TODO - Add the documentation to the assistant

/*-------------------------------------------------------------*/

    return a.exec();

}
