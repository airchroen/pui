#include "parse_file.h"

QList<ControlSpec> XmlLayoutFileParser::parse(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(NULL,"Error"," XML file read error,  Please correct and try again ");
        exit(0);
    }

    QDomDocument doc;
    if(!doc.setContent(&file))
    {
        file.close();
        QMessageBox::about(NULL,"Error"," XML file structure error,  Please correct and try again ");
        exit(0);
    }
    file.close();

    ControlSpec controlSpec;
    QList<ControlSpec> controlspecList;

    QDomElement domElemnt = doc.documentElement();
    QDomNode node = domElemnt.firstChild();

    while (!node.isNull())
    {
        if (!node.isElement())
        {
            QMessageBox::about(NULL,"Error"," XML file structure error,  Please correct and try again ");
            exit(0);
        }

        QDomNodeList list= node.toElement().childNodes();
        for (int i=0; i<list.count(); i++)
        {
            QDomNode domNode = list.at(i);
            QString tagName = domNode.toElement().tagName();
            if (tagName.contains("WIDGETNAME"))
            {
                controlSpec.widgetName = domNode.toElement().text();
                continue;
            }
            else if (tagName.contains("GEOMETRY"))
            {
                QString str =  domNode.toElement().text();
                QStringList strList =str.split(",");

                if (strList.size() != 4)
                {
                    QMessageBox::about(NULL,"Error"," Geometry parameters error,  Please correct and try again ");
                    exit(0);
                }
                controlSpec.geometry.setX(strList.at(0).toInt());
                controlSpec.geometry.setY(strList.at(1).toInt());
                controlSpec.geometry.setWidth(strList.at(2).toInt());
                controlSpec.geometry.setHeight(strList.at(3).toInt());

                controlspecList.push_back(controlSpec);
            }
            else
            {
                QMessageBox::about(NULL,"Error"," XML file structure error,  Please correct and try again ");
                exit(0);
            }
        }
        controlspecList.push_back(controlSpec);
        node=node.nextSibling();
    }

    return controlspecList;
}

QList<ControlSpec> JsonLayoutFileParser::parse(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::about(NULL,"Error"," Json file read error,  Please correct and try again ");
        exit(0);
    }

    QByteArray byteArray = file.readAll();
    file.close();

    ControlSpec controlSpec;
    QList<ControlSpec> controlspecList;

    QJsonParseError jsonError;
    QJsonDocument document = QJsonDocument::fromJson(byteArray, &jsonError);
    if (document.isNull() && !(jsonError.error == QJsonParseError::NoError))
    {
        QMessageBox::about(NULL,"Error"," Documents is null or Json files parse error,  Please correct and try again ");
        exit(0);
    }
    if (!(document.isObject()))
    {
        QMessageBox::about(NULL,"Error"," Json files parse error,  Please correct and try again ");
        exit(0);
    }

    QJsonObject object = document.object();
    if (!(object.contains("Widgets")))
    {
        QMessageBox::about(NULL,"Error"," Json files parse error,  Please correct and try again ");
        exit(0);
    }

    QJsonValue valueArray = object.value("Widgets");
    if (!(valueArray.isArray()))
    {
        QMessageBox::about(NULL,"Error"," Some information was missing,  Please correct and try again ");
        exit(0);
    }

    QJsonArray jsonArray = valueArray.toArray();
    for (int i = 0; i < jsonArray.count(); i++)
    {
        QJsonValue childValue = jsonArray[i];
        if (!(childValue.isObject()))
        {
            QMessageBox::about(NULL,"Error"," Json files parse error,  Please correct and try again ");
            exit(0);
        }
        QJsonObject childObject = childValue.toObject();
        if (!(childObject.contains("WidgetName")))
        {
            QMessageBox::about(NULL,"Error"," Some information was missing,  Please correct and try again ");
            exit(0);
        }
        QJsonValue valueWidgetName = childObject.value("WidgetName");
        if (!(valueWidgetName.isString()) && (valueWidgetName.toString().isEmpty()))
        {
            QMessageBox::about(NULL,"Error"," Some information was missing,  Please correct and try again ");
            exit(0);
        }

        controlSpec.widgetName = valueWidgetName.toString();

        if (!(childObject.contains("Geometry")))
        {
            QMessageBox::about(NULL,"Error"," Some information was missing,  Please correct and try again ");
            exit(0);
        }
        QJsonValue valueGeometry = childObject.value("Geometry");
        if (!(valueGeometry.isString()) && (valueWidgetName.toString().isEmpty()))
        {
            QMessageBox::about(NULL,"Error"," Some information was missing,  Please correct and try again ");
            exit(0);
        }

        QString str = valueGeometry.toString();
        QStringList strList = str.split(",");

        if (strList.size() != 4)
        {
            QMessageBox::about(NULL,"Error"," Geometry data parameters error,  Please correct and try again ");
            exit(0);
        }
        controlSpec.geometry.setX(strList.at(0).toInt());
        controlSpec.geometry.setY(strList.at(1).toInt());
        controlSpec.geometry.setWidth(strList.at(2).toInt());
        controlSpec.geometry.setHeight(strList.at(3).toInt());
        controlspecList.push_back(controlSpec);
    }
    return controlspecList;
}

QList<ControlSpec> SQLiteLayoutFileParser::parse(const QString &filePath)
{
    ControlSpec controlSpec;
    QList<ControlSpec> controlspecList;

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(filePath);

    if (!db.open())
    {
        QMessageBox::about(NULL,"Error"," SQLITE file loading failed,  Please correct and try again ");
        exit(0);
    }

    QSqlQuery query;

    bool isOk = query.exec("select * from Widgets");
    if (!isOk)
    {
        QMessageBox::about(NULL,"Error"," SQLITE file loading failed,  Please correct and try again ");
        exit(0);
    }

    while (query.next())
    {
        if (query.value(0).toString().isEmpty())
        {
            QMessageBox::about(NULL,"Error"," Some information was missing,  Please correct and try again ");
            exit(0);
        }
        controlSpec.widgetName = query.value(1).toString();
        QString str = query.value(2).toString();
        QStringList strList = str.split(",");

        if (strList.size() != 4)
        {
            QMessageBox::about(NULL,"Error"," Geometry data parameters error,  Please correct and try again ");
            exit(0);
        }
        controlSpec.geometry.setX(strList.at(0).toInt());
        controlSpec.geometry.setY(strList.at(1).toInt());
        controlSpec.geometry.setWidth(strList.at(2).toInt());
        controlSpec.geometry.setHeight(strList.at(3).toInt());
        controlspecList.push_back(controlSpec);
    }
    db.close();

    return controlspecList;
}


