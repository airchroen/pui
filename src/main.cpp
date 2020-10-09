#include "draw_widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    DrawWidget w;
    w.resize(960,640);
    w.show();
    return app.exec();
}
