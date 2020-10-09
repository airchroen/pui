#ifndef PARSE_FILE_H
#define PARSE_FILE_H

#include "control_spec.h"
#include <QFile>
#include <QtXml/QtXml>
#include <QtXml/QDomDocument>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

class LayoutFileParser
{
public:
    virtual QList<ControlSpec> parse(const QString &filePath) = 0;
    virtual ~LayoutFileParser(){}
};

class XmlLayoutFileParser : public LayoutFileParser
{
    QList<ControlSpec> parse(const QString &filePath);
};

class JsonLayoutFileParser : public LayoutFileParser
{
    QList<ControlSpec> parse(const QString &filePath);
};

class SQLiteLayoutFileParser : public LayoutFileParser
{
    QList<ControlSpec> parse(const QString &filePath);
};

class ParserFactory
{
public:
    static LayoutFileParser* createParser(const QString &fileType)
    {
        if (fileType.compare(QStringLiteral("XML"), Qt::CaseInsensitive) == 0)
        {
            return new XmlLayoutFileParser();
        }
        else if (fileType.compare(QStringLiteral("JSON"), Qt::CaseInsensitive) == 0)
        {
            return new JsonLayoutFileParser();
        }
        else if (fileType.compare(QStringLiteral("SQLite"), Qt::CaseInsensitive) == 0)
        {
            return new SQLiteLayoutFileParser();
        }
        else
        {
            QMessageBox::about(NULL,"Error"," The current file format is not supported,  Please correct and try again ");
            exit(0);
        }
    }
};


#endif // PARSE_FILE_H
