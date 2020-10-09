#include "draw_widget.h"

#include <QDebug>
#include <stdlib.h>

DrawWidget::DrawWidget(QWidget *parent)
    : QWidget(parent)
{
    m_settingFilePath = QApplication::applicationDirPath() + "/app-config.ini";

    Loadinfo loadInfo = loadSetting();

    setupUI(parseFile(loadInfo));
}

DrawWidget::~DrawWidget(){}

Loadinfo DrawWidget::loadSetting()
{
    QSettings settings(m_settingFilePath, QSettings::IniFormat);

    QString sFiletype = settings.value("FileType/type").toString();
    QString sFilepath = settings.value("FilePath/path").toString();

    if (sFiletype.isEmpty() || sFilepath.isEmpty())
    {
        QMessageBox::information(NULL,"Warning"," Configuration file filling error,  Please correct and try again ");
        exit(0);
    }

    return {sFiletype, sFilepath};
}

QList<ControlSpec> DrawWidget::parseFile(const Loadinfo &info)
{
    LayoutFileParser* fileParser = ParserFactory::createParser(info.filetype);
    auto parseResult = fileParser->parse(info.filepath);

    return parseResult;
}

void DrawWidget::setupUI(const QList<ControlSpec> &specList)
{
    if (specList.isEmpty())
        return;

    for (int i = 0; i < specList.size(); i++)
    {
        if (specList[i].widgetName.compare(QStringLiteral("Label"), Qt::CaseInsensitive) == 0)
        {
            auto label = new QLabel(this);
            label->setGeometry(specList[i].geometry);
        }
        else if (specList[i].widgetName.compare(QStringLiteral("Pushbutton"), Qt::CaseInsensitive) == 0)
        {
            auto pushButton = new QPushButton(this);
            pushButton->setGeometry(specList[i].geometry);
        }
        else if (specList[i].widgetName.compare(QStringLiteral("Slider"), Qt::CaseInsensitive) == 0)
        {
            auto slider = new QSlider(this);
            slider->setGeometry(specList[i].geometry);
            slider->setOrientation(Qt::Horizontal);
        }
        else if (specList[i].widgetName.compare(QStringLiteral("CheckBox"), Qt::CaseInsensitive) == 0)
        {
            auto checkBox = new QCheckBox(this);
            checkBox->setGeometry(specList[i].geometry);
        }
        else if (specList[i].widgetName.compare(QStringLiteral("TextEdit"), Qt::CaseInsensitive) == 0)
        {
            auto textEdit = new QTextEdit(this);
            textEdit->setGeometry(specList[i].geometry);
        }
        else if (specList[i].widgetName.compare(QStringLiteral("ProgressBar"), Qt::CaseInsensitive) == 0)
        {
            auto progressBar = new QProgressBar(this);
            progressBar->setGeometry(specList[i].geometry);
            progressBar->setValue(24);
        }
        else if (specList[i].widgetName.compare(QStringLiteral("LCDNumber"), Qt::CaseInsensitive) == 0)
        {
            auto lcdNumber = new QLCDNumber(this);
            lcdNumber->setGeometry(specList[i].geometry);
        }
        else if (specList[i].widgetName.compare(QStringLiteral("Dial"), Qt::CaseInsensitive) == 0)
        {
            auto dial = new QDial(this);
            dial->setGeometry(specList[i].geometry);
        }
        else if (specList[i].widgetName.compare(QStringLiteral("DialogButtonBox"), Qt::CaseInsensitive) == 0)
        {
            auto buttonBox = new QDialogButtonBox(this);
            buttonBox->setGeometry(specList[i].geometry);
            buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        }
    }
}
