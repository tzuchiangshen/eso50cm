#include <QtGui/QApplication>
#include "logpanel.h"
#include <iostream>
#include <pthread.h>
/*int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LogPanel w;
    w.show();
    return a.exec();
}*/

pthread_attr_t tattr;
void *arg;
pthread_t tid;
int argcg;
char **argvg;
int ret;


QApplication *qapp;

void *runSubscriber(void *gui)

{
  Subscriber subs((LogPanel *)gui);
  std::cout<<"Starting subscriber"<<std::endl;
  subs.main(argcg, argvg);
  return NULL;
}
int main(int argc, char *argv[])
{
    argvg=argv;
    argcg=argc;
    QApplication qtap(argc, argv);
    std::cout<<"Starting GUI"<<std::endl;
    LogPanel w;
    w.show();
     ret = pthread_create(&tid, NULL, runSubscriber,(void *) &w);
    return qapp->exec();
}

