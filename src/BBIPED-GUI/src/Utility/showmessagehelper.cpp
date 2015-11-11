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
***/
#include "showmessagehelper.h"
#include <QMessageBox>

/**
 * @brief
 *
 */
ShowMessageHelper::ShowMessageHelper()
{
}


/**
 * @brief Show custom message in status bar
 *
 * @param msg
 */
void  ShowMessageHelper::showMessage( QString msg, QStatusBar * _statusBar )
{
  // display message on main window status bar
  _statusBar->showMessage( msg );
}

/**
 * @brief Show comboBox for Saving results
 *
 * @return bool
 */
bool ShowMessageHelper::showMessageForSaving()
{
    QMessageBox msgbox;
    msgbox.setIcon(QMessageBox::Question);
    msgbox.setWindowTitle("YOUR PROJECT NEED TO BE SAVED"); //this is ignore on mac
    msgbox.setText("Your project has changes that have not been saved yet. Do you want to save them, before Closing?"); //set message box text
    msgbox.addButton(QMessageBox::Yes); //add ok button
    msgbox.addButton(QMessageBox::No); //add cancel button
    int status = msgbox.exec(); //display message box
    /* display which option was choosen */
    switch (status) {
        case QMessageBox::Yes:
            return true;
            break;
        case QMessageBox::No:
            return false;
            break;
     }
    return false;
}

/**
 * @brief Show About Message window
 *
 * @param _title
 * @param _message
 * @return bool
 */
void ShowMessageHelper::showMessageWindow(QString _title, QString _message, QWidget * _window)
{
    QMessageBox _msgBox;
    _msgBox.setTextFormat(Qt::RichText);
    QPixmap projectLogo(":/logo/images/icon_31x64.png");
    _msgBox.setIconPixmap(projectLogo);

    _msgBox.setWindowTitle(_title);
    _msgBox.setText(_message);
    _msgBox.exec();


}

/**
 * @brief Show message window to reset the boundary values to the new ones
 *
 * @return bool
 */
bool ShowMessageHelper::showMessageReset()
{
    QMessageBox msgbox;
    msgbox.setIcon(QMessageBox::Question);
    msgbox.setWindowTitle("Reset Configuration File Values"); //this is ignored on mac
    msgbox.setText("You are going to reset all the values to their default ones. Do you wan to continue?"); //set message box text
    msgbox.addButton(QMessageBox::Ok); //add ok button
    msgbox.addButton(QMessageBox::Cancel); //add cancel button
    int status = msgbox.exec(); //display message box
    /* display which option was choosen */
    switch (status) {
        case QMessageBox::Ok:
            return true;
            break;
        case QMessageBox::Cancel:
            return false;
            break;
     }
    return false;
}

/**
 * @brief Default WARNING Message
 *
 * @param _title
 * @param _message
 * @return bool
 */
bool ShowMessageHelper::showWarningMessage(QString _title, QString _message)
{
    QMessageBox msgbox;
    msgbox.setWindowTitle(_title); //this is ignore on mac
    msgbox.setIcon(QMessageBox::Warning);
    msgbox.setText(_message); //set message box text
    msgbox.addButton(QMessageBox::Ok); //add ok button
    msgbox.addButton(QMessageBox::Cancel); //add cancel button
    int status = msgbox.exec(); //display message box
    /* display which option was choosen */
    switch (status) {
        case QMessageBox::Ok:
           return true;
           break;
        case QMessageBox::Cancel:
            return false;
            break;
     }
    return false;
}
