#include <QtGui/QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow *w = new MainWindow(argc, &argv[0]);
    w->setMinimumSize(1200,800);
    w->setMaximumSize(1440, 960);
    w->show();
    return a.exec();
}
