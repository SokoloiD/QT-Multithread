#include "multithreadwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MultiThreadWindow w;
    w.show();
    return a.exec();
}
