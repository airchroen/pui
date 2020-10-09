#ifndef DRAWUI_H
#define DRAWUI_H

#include "control_spec.h"
#include "parse_file.h"



class DrawWidget : public QWidget
{
    Q_OBJECT

public:
    DrawWidget(QWidget *parent = 0);
    ~DrawWidget();

private:
    Loadinfo loadSetting();
    QList<ControlSpec> parseFile(const Loadinfo &info);
    void setupUI(const QList<ControlSpec> &specList);

private:
    QString m_settingFilePath;
};

#endif // DRAWUI_H
