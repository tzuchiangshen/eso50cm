#include "MainController.h"

MainController::MainController(QWidget *parent):
    logger("MainController")
{
    logger.logINFO("LLEgue!");
}

MainController::~MainController() {
    //delete ui;
}

//void MainController::testSlots(bool visible) {
//    ui->UT_LineEdit->setText(QString("esto es una prueba3") + QString(visible));
//}
