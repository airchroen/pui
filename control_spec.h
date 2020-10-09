#ifndef CONTROLSPEC_H
#define CONTROLSPEC_H

#include <QRect>
#include <QList>
#include <QString>
#include <QMessageBox>
#include <QWidget>
#include <QtWidgets>

struct Loadinfo
{
    QString filetype;
    QString filepath;
};

struct ControlSpec
{
    QString widgetName;
    QRect geometry;
};


#endif // CONTROLSPEC_H
