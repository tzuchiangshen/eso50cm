#include "MainController.h"

MainController::MainController():
    logger("MainController")
{
    logger.logINFO("MainController::MainController: started");
    obsControl = new ObsControlIF();
    obsControl->connect();
    lcu = obsControl->getLCUReference();
    obsControl->start();
}

MainController::~MainController() {
    //delete ui;
}

void MainController::testSlots(bool visible) {
    //ui->UT_LineEdit->setText(QString("esto es una prueba3") + QString(visible));
}
