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
#include "htmlformatter.h"

/**
 * @brief Constructor
 *
 */
HtmlFormatter::HtmlFormatter()
{
    _formattedText ="";
}


/**
 * @brief Destructor
 *
 */
void HtmlFormatter::startFormatting()
{
    _formattedText += getHeader();
}



/**
 * @brief This fucntion adds a title (header 1) with the correspondent html tags
 *
 * @param _title
 */
void HtmlFormatter::appendTitle(QString _title)
{
    _formattedText += "<h1>";
    _formattedText +=  "<span style= \" color:#000080; \"> ";
    _formattedText += _title    ;
    _formattedText += "</span> ";
    _formattedText += "</h1>";
}

/**
 * @brief This fucntion adds a paragraph (header 2, image, title) with the correspondent html tags
 *
 * @param _title
 */
void HtmlFormatter::appendTitleParagraph(QString _title)
{
    _formattedText += "<h2>";
    _formattedText += "<img src=\":/helpicons/images/bullet_blue.png\" />";
    _formattedText +=  "<span style= \" color:#000080; \"> ";
    _formattedText += _title    ;
    _formattedText += "</span> ";
    _formattedText += "</h2>";
}


/**
 * @brief This fucntion adds a paragraph with the correspondent html tags
 *
 * @param _paragraph
 */
void HtmlFormatter::appendParagraph(QString _paragraph)
{
    _formattedText +=  "<p> ";
    _formattedText += _paragraph;
    _formattedText += "</p> ";
}

/**
 * @brief This fucntion adds a the footer of the html document
 *
 */
void HtmlFormatter::endFormatting()
{
    _formattedText += getFooter();
}

/**
 * @brief This function returns the Html formated text
 *
 * @return QString
 */
QString HtmlFormatter::getFormatedText()
{
    return _formattedText;
}


/**
 * @brief This fucntion gets the Html header
 *
 * @return QString
 */
QString HtmlFormatter::getHeader()
{
    QString _headerText;
    _headerText = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\"><html>";
    _headerText += "<head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">";
    _headerText += "p, li { white-space: pre-wrap; } </style></head>";
    _headerText += "<body> ";
    return _headerText;
}

/**
 * @brief This fucntion gets the Html footer
 *
 * @return QString
 */
QString HtmlFormatter::getFooter()
{
    QString _footerText;
    _footerText +=  "</body></html>";
    return _footerText;
}


/**
 * @brief This fucntion adds a table with the correspondent html tags
 *
 * @param _headerList
 * @param _rowContent
 */
void HtmlFormatter::appendTable(QStringList _headerList,QList<QStringList> _rowContent)
{
    _formattedText += "<p><table border=1>";
    // Setting the header
    if (!_headerList.isEmpty())
    {
        _formattedText += "<tr>";
        for (int j = 0; j < _headerList.count();j++)
        {
            _formattedText += "<th>";
            _formattedText += _headerList.at(j);
            _formattedText += "</th>";
        }
        _formattedText += "</tr>";
    }

    // Setting Row Contents
    if (!_rowContent.isEmpty())
    {
        for (int i = 0; i < _rowContent.count(); i++)
        {
            QStringList _row = _rowContent.at(i);
            if (!_row.isEmpty())
            {
                _formattedText += "<tr>";
                 for (int k = 0; k<_row.count();k++)
                 {
                     _formattedText += "<td>";
                     _formattedText += _row.at(k);
                     _formattedText += "</td>";

                 }
                 _formattedText += "</tr>";
            }
        }
    }
    _formattedText += "</table></p>";

}


