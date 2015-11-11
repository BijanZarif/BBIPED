#ifndef HTMLFORMATTER_H
#define HTMLFORMATTER_H

#include <QString>
#include <QList>
#include <QStringList>

class HtmlFormatter
{
public:
    HtmlFormatter();
    /****************************** Start/End *************************/
    void startFormatting();
    void endFormatting();

    /****************************** Append *************************/
    void appendTitle(QString _title);
    void appendTitleParagraph(QString _title);
    void appendParagraph(QString _paragraph);
    void appendTable(QStringList _headerList,QList<QStringList> _rowContent);

    /****************************** Getter/Setter *************************/
    QString getFormatedText();

private:
   QString _formattedText;

   /****************************** Getter/Setter *************************/
   QString getHeader();
   QString getFooter();
};

#endif // HTMLFORMATTER_H
