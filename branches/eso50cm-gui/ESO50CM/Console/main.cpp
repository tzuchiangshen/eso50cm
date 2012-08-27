#include <QtGui/QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow *w = new MainWindow(argc, &argv[0]);
    w->setMinimumHeight(1024);
    w->setMinimumWidth(1440);
    w->show();
    return a.exec();
}
