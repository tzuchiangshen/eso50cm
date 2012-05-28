#include <QtGui/QApplication>
#include "mainwindow.h"
#include <iostream>
#include <pthread.h>
pthread_attr_t tattr;
void *arg;
pthread_t tid;
int argcg;
char **argvg;
int ret;


QApplication *qapp;
void *runSubscriber(void *gui)
{
  Subscriber subs((MainWindow *)gui);
  std::cout<<"Starting subscriber"<<std::endl;
  subs.main(argcg, argvg);
}
int main(int argc, char *argv[])
{
    argvg=argv;
    argcg=argc;
    QApplication qtap(argc, argv);
    std::cout<<"Starting GUI"<<std::endl;  
    MainWindow w;
    w.show();
    ret = pthread_create(&tid, NULL, runSubscriber,(void *) &w);
    return qapp->exec();    
}

