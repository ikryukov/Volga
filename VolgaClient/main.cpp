#include "volgamainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    VolgaMainWindow w;
    w.show();

    return a.exec();
}
