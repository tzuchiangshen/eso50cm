#include <QtGui/QApplication>
#include "telescopecli.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TelescopeCli w;
    w.show();

    return a.exec();
}
