#include <QtGui/QApplication>
#include "loglevelpanel.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LogLevelPanel w;
    w.show();
    return a.exec();
}
