#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <sys/time.h>
#include <time.h>
#include <stdio.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->statusBar->showMessage("Application Started",3000);
    if (!db.connect("tcp://127.0.0.1:3306","logmgr","logpass"))
        ui->statusBar->showMessage("Error connecting to the DB",5000);
    else
        ui->statusBar->showMessage("Succesfully connected to the DB",5000);

    levelDesc[0]="UnknownLevel0";
    levelDesc[1]="Finest";
    levelDesc[2]="Finer";
    levelDesc[3]="Fine";
    levelDesc[4]="Config";
    levelDesc[5]="Info";
    levelDesc[6]="Warning";
    levelDesc[7]="Severe";
    levelDesc[9]="Unknown";
}
void MainWindow::addLog(int level, double timestamp, int sourceId,string data)
{
    ui->logTable->setSortingEnabled (false);
    ui->logTable->insertRow(0);
    ui->logTable->setItem(0,1,new QTableWidgetItem(getLevelDesc(level).c_str()));
    ui->logTable->setItem(0,0,new QTableWidgetItem(getTimeString(timestamp).c_str()));
    ui->logTable->setItem(0,2,new QTableWidgetItem(getSourceDesc(sourceId).c_str()));
    ui->logTable->setItem(0,3,new QTableWidgetItem(data.c_str()));
    ui->logTable->setSortingEnabled (true);
}
void MainWindow::addLog(int level, double timestamp, string source,string data)
{
    ui->logTable->setSortingEnabled (false);
    ui->logTable->insertRow(0);
    ui->logTable->setItem(0,1,new QTableWidgetItem(getLevelDesc(level).c_str()));
    ui->logTable->setItem(0,0,new QTableWidgetItem(getTimeString(timestamp).c_str()));
    ui->logTable->setItem(0,2,new QTableWidgetItem(source.c_str()));
    ui->logTable->setItem(0,3,new QTableWidgetItem(data.c_str()));
    ui->logTable->setSortingEnabled (true);
}

/*void MainWindow::logEvent(const LogMessageData &message, const Ice::Current& c){
  //printf("\n received event...\n");
  //ui->statusBar->showMessage("Received event");
  addLog(message.level, message.timestamp, message.source,message.message);
}*/
string MainWindow::getSourceDesc(int sourceId){
    return db.getSourceDesc(sourceId);
}

string MainWindow::getTimeString(double timestamp){
    char buffer[30];
    char timestr[50];
    time_t timesec;
    timesec=(int)timestamp;
    strftime(buffer,30,"%m-%d-%Y  %T.",localtime(&timesec));
    sprintf(timestr,"%s%06ld\n",buffer,(long int)((timestamp-timesec)*1e6));
    return string(timestr);
}
string MainWindow::getLevelDesc(int level)
{
    if (level>9||level<0)
        return "Unknown";
    else
        return levelDesc[level];
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_logLevelFilter_currentIndexChanged(int index)
{
    for(int i=0;i< ui->logTable->rowCount();i++)
        if(index==0)
           ui->logTable->showRow(i);
        else if(index==1) {
            if(ui->logTable->itemAt(i,1)->text().compare("Finest"))
                ui->logTable->hideRow(i);
            else
                ui->logTable->showRow(i);
        }
        else if(index==2) {
            if(ui->logTable->itemAt(i,1)->text().compare("Finest") ||ui->logTable->itemAt(i,1)->text().compare("Finer") )
                ui->logTable->hideRow(i);
            else
                ui->logTable->showRow(i);
        }
        else if(index==3) {
            if(ui->logTable->itemAt(i,1)->text().compare("Finest") ||ui->logTable->itemAt(i,1)->text().compare("Finer") ||ui->logTable->itemAt(i,1)->text().compare("Fine")  )
                ui->logTable->hideRow(i);
            else
                ui->logTable->showRow(i);
        }
        else if(index==4) {
            if(ui->logTable->itemAt(i,1)->text().compare("Finest") ||ui->logTable->itemAt(i,1)->text().compare("Finer") ||ui->logTable->itemAt(i,1)->text().compare("Fine")
               ||ui->logTable->itemAt(i,1)->text().compare("Config") )
                ui->logTable->hideRow(i);
            else
                ui->logTable->showRow(i);
        }
        else if(index==5) {
            if(ui->logTable->itemAt(i,1)->text().compare("Finest") ||ui->logTable->itemAt(i,1)->text().compare("Finer") ||ui->logTable->itemAt(i,1)->text().compare("Fine")
               ||ui->logTable->itemAt(i,1)->text().compare("Config") ||ui->logTable->itemAt(i,1)->text().compare("Info")  )
                ui->logTable->hideRow(i);
            else
                ui->logTable->showRow(i);
        }
        else if(index==5) {
            if(ui->logTable->itemAt(i,1)->text().compare("Finest") ||ui->logTable->itemAt(i,1)->text().compare("Finer") ||ui->logTable->itemAt(i,1)->text().compare("Fine")
               ||ui->logTable->itemAt(i,1)->text().compare("Config") ||ui->logTable->itemAt(i,1)->text().compare("Info") ||ui->logTable->itemAt(i,1)->text().compare("Warning")  )
                ui->logTable->hideRow(i);
            else
                ui->logTable->showRow(i);
        }
                else if(index==6) {
            if(ui->logTable->itemAt(i,1)->text().compare("Finest") ||ui->logTable->itemAt(i,1)->text().compare("Finer") ||ui->logTable->itemAt(i,1)->text().compare("Fine")
               ||ui->logTable->itemAt(i,1)->text().compare("Config") ||ui->logTable->itemAt(i,1)->text().compare("Info") ||ui->logTable->itemAt(i,1)->text().compare("Warning")
               ||ui->logTable->itemAt(i,1)->text().compare("Severe")  )
                ui->logTable->hideRow(i);
            else
                ui->logTable->showRow(i);
        }

}
