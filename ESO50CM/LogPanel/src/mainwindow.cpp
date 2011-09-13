#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <fstream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->statusBar->showMessage("Application Started",3000);
    if (!db.connect("tcp://127.0.0.1:3306","logmgr","logpass"))
    {
        ui->dbConnection->setChecked(false);
        ui->statusBar->showMessage("Error connecting to the DB",3000);
    }
    else {
        ui->statusBar->showMessage("Succesfully connected to the DB",3000);
        ui->dbConnection->setChecked(true);
    }
    levelDesc[0]="UNKNOWNLevel0";
    levelDesc[1]="FINEST";
    levelDesc[2]="FINER";
    levelDesc[3]="FINE";
    levelDesc[4]="CONFIG";
    levelDesc[5]="INFO";
    levelDesc[6]="WARNING";
    levelDesc[7]="SEVERE";
    levelDesc[9]="UNKNOWN";
    // sets the initial sizes
    ui->logTable->setColumnWidth(0,200);
    ui->logTable->setColumnWidth(1,90);
    ui->logTable->setColumnWidth(2,100);
    ui->logTable->setColumnWidth(3,500);


     // DETAIL table
    QTableWidgetItem *prototype = new QTableWidgetItem();
    // setup my prototype
    prototype->setFlags(Qt::ItemIsEnabled);
    // add all my items using the prototype->clone() method

    QTableWidgetItem *levelItem=prototype->clone();
    QTableWidgetItem *timeItem=prototype->clone();
    QTableWidgetItem *sourceItem=prototype->clone();
    ui->detailsTable->setColumnWidth(0,200);
    ui->detailsTable->setItem(0,0,timeItem);
    ui->detailsTable->setItem(1,0,levelItem);
    ui->detailsTable->setItem(2,0,sourceItem);
    //ui->detailsTable->horizontalHeader()->hide();
    ui->logTable->setAlternatingRowColors(true);


}

int MainWindow::levelDescToNumber(QString level) {

  /*  levelDesc[0]="UNKNOWNLevel0";
    levelDesc[1]="FINEST";
    levelDesc[2]="FINER";
    levelDesc[3]="FINE";
    levelDesc[4]="CONFIG";
    levelDesc[5]="INFO";
    levelDesc[6]="WARNING";
    levelDesc[7]="SEVERE";
    levelDesc[9]="UNKNOWN";
    */
    if (level.compare("All")==0)
        return 0;
    if (level.compare("FINEST")==0)
        return 1;
    if (level.compare("FINER")==0)
        return 2;
    if (level.compare("FINE")==0)
        return 3;
    if (level.compare("CONFIG")==0)
        return 4;
    if (level.compare("INFO")==0)
        return 5;
    if (level.compare("WARNING")==0)
        return 6;
    if (level.compare("SEVERE")==0)
        return 7;
    return 9;
}

void MainWindow::addLog(int level, double timestamp, int sourceId,string data)
{
    QTableWidgetItem *prototype = new QTableWidgetItem();
    // setup my prototype
    prototype->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    // add all my items using the prototype->clone() method


    QTableWidgetItem *levelItem=prototype->clone();
    QTableWidgetItem *timeItem=prototype->clone();
    QTableWidgetItem *sourceItem=prototype->clone();
    QTableWidgetItem *dataItem=prototype->clone();

    levelItem->setText(getLevelDesc(level).c_str());
    timeItem->setText(getTimeString(timestamp).c_str());
    sourceItem->setText(getSourceDesc(sourceId).c_str());
    dataItem->setText(data.c_str());

    ui->logTable->setSortingEnabled (false);

    ui->logTable->insertRow(0);
    ui->logTable->setItem(0,0,timeItem);
    if (level>5)
        levelItem->setForeground(Qt::red);
    ui->logTable->setItem(0,1,levelItem);
    ui->logTable->setItem(0,2,sourceItem);
    ui->logTable->setItem(0,3,dataItem);
    // if the level is under the filter, we hide the row
    if (level<ui->logLevelFilter->currentIndex())
           ui->logTable->hideRow(0);

    ui->logTable->setSortingEnabled (true);

}
void MainWindow::addLog(int level, double timestamp, string source,string data)
{
    QTableWidgetItem *prototype = new QTableWidgetItem();
    // setup my prototype
    prototype->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    // add all my items using the prototype->clone() method


    QTableWidgetItem *levelItem=prototype->clone();
    QTableWidgetItem *timeItem=prototype->clone();
    QTableWidgetItem *sourceItem=prototype->clone();
    QTableWidgetItem *dataItem=prototype->clone();

    levelItem->setText(getLevelDesc(level).c_str());
    timeItem->setText(getTimeString(timestamp).c_str());
    sourceItem->setText(source.c_str());
    dataItem->setText(data.c_str());

    ui->logTable->setSortingEnabled (false);

    ui->logTable->insertRow(0);
    ui->logTable->setItem(0,0,timeItem);
    if (level>5)
            levelItem->setForeground(Qt::red);
    ui->logTable->setItem(0,1,levelItem);
    ui->logTable->setItem(0,2,sourceItem);
    ui->logTable->setItem(0,3,dataItem);
    // if the level is under the filter, we hide the row
    if (level<ui->logLevelFilter->currentIndex())
           ui->logTable->hideRow(0);

    ui->logTable->setSortingEnabled (true);
}

void MainWindow::logEvent(const LogMessageData &message, const Ice::Current& c){
  //printf("\n received event...\n");
  //ui->statusBar->showMessage("Received event");
  addLog(message.level, message.timestamp, message.source,message.message);
}
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
        return "UNKNOWN";
    else
        return levelDesc[level];
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_logLevelFilter_currentIndexChanged(int index)
{

  for(int i=0;i< ui->logTable->rowCount();i++){
      // index==0 -> All, so we just show all the rows
     if(index==0)
           ui->logTable->showRow(i);
     else {
       // if not, we should see if the row is under the limit
       // first, we convert the level description to a number
       int currentRowLevel=levelDescToNumber(ui->logTable->item(i,1)->text());
       if (currentRowLevel<index)
           ui->logTable->hideRow(i);
       else
           ui->logTable->showRow(i);
      }
    }
}


void MainWindow::on_dbConnection_clicked(bool checked)
{
    ui->dbConnection->setChecked(!checked);
}


void MainWindow::on_logTable_cellClicked(int row, int column)
{

ui->detailsTable->item(0,0)->setText(ui->logTable->item(row,0)->text());
ui->detailsTable->item(1,0)->setText(ui->logTable->item(row,1)->text());
ui->detailsTable->item(2,0)->setText(ui->logTable->item(row,2)->text());
ui->detailText->setText(ui->logTable->item(row,3)->text());
}

void MainWindow::on_detailsTable_cellClicked(int row, int column)
{
    char texto[1000];
    sprintf(texto,"row: %i  col: %i",row,column);
    ui->statusBar->showMessage(texto);
}


void MainWindow::on_actionSave_logs_triggered()
{

     QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                            ".",
                            tr("Text (*.txt)"));
    // first we check if the file exist
    fstream fin;
    fin.open(fileName.toStdString().c_str(),fstream::in);
    bool file_exist=false;
    if( fin.is_open() )
    {
        file_exist=true;
        fin.close();
    }

    FILE *logFile=fopen(fileName.toStdString().c_str(),"a+");
    if (!file_exist)
    {
        // if the file doesn't exist, we add a header
         fprintf(logFile,"|\ttimestamp\t | \tlevel\t | \tsource\t | \tmessage\t |\n");
    }
    for(int i=0;i< ui->logTable->rowCount();i++){
          fprintf(logFile,"| %s | %s | %s | %s |\n",ui->logTable->item(i,0)->text().toStdString().c_str(),ui->logTable->item(i,1)->text().toStdString().c_str(),ui->logTable->item(i,2)->text().toStdString().c_str(),ui->logTable->item(i,3)->text().toStdString().c_str());
    }
    fclose(logFile);
}

void MainWindow::on_searchButton_clicked()
{
    QVariant tmp(ui->logTable->currentRow());
  //   ui->statusBar->showMessage(tmp.toString());
    if (ui->logTable->currentRow()<0 || ui->logTable->currentRow()>ui->logTable->rowCount())
        return;
    if (ui->backCB->isChecked())
    {
         // look back
        for (int i=ui->logTable->currentRow()-1;i>=0;i--){
            if (ui->caseCB->isChecked()){
                // case sensitive
                if (ui->logTable->item(i,3)->text().contains(ui->searchEdit->text()))
                {
                    ui->logTable->setCurrentCell(i,3);
                    QVariant tmp(i);
                    ui->statusBar->showMessage(tmp.toString());
                    return;
                }
            } else {
                // no case sensitive
                if (ui->logTable->item(i,3)->text().toUpper().contains(ui->searchEdit->text().toUpper()))
                                    {
                    ui->logTable->setCurrentCell(i,3);
                    QVariant tmp(i);
                    ui->statusBar->showMessage(tmp.toString());
                    return;
                }

            }

        }

    } else {
        // look forward
        for (int i=ui->logTable->currentRow()+1;i<ui->logTable->rowCount();i++){
            if (ui->caseCB->isChecked()){
                // case sensitive
                if (ui->logTable->item(i,3)->text().contains(ui->searchEdit->text()))
                {
                    ui->logTable->setCurrentCell(i,3);
                    QVariant tmp(i);
                    ui->statusBar->showMessage(tmp.toString());
                    return;
                }
            } else {
                // no case sensitive
                if (ui->logTable->item(i,3)->text().toUpper().contains(ui->searchEdit->text().toUpper()))
                                    {
                    ui->logTable->setCurrentCell(i,3);
                    QVariant tmp(i);
                    ui->statusBar->showMessage(tmp.toString());
                    return;
                }

            }

         }
    }
}
