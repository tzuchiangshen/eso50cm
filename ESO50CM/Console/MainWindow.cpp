#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "ui_encoder.h"
#include "ui_telescope.h"
#include <QVBoxLayout>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    uiTelescope(new Ui::TelescopeForm),
    uiEncoder(new Ui::encoderForm),
    mainController(new MainController)
{
    ui->setupUi(this);

    //createOffsetDocking();
    //createEncoderDocking();
    createCentralWidget();

    connect( ui->saveAsAction, SIGNAL( triggered(bool)),
            this, SLOT( testSlots(bool)));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete uiTelescope;
    delete uiEncoder;
}


//void MainWindow::createEncoderDocking2() {
//    //docking via GroupBox
//    QGroupBox *telescopeGroupBox = new QGroupBox();
//    uiTelescope->setupUi(telescopeGroupBox);
//    ui->dockLogger->setWidget(telescopeGroupBox);
//    ui->dockLogger->setWindowTitle("telescope");

//}

void MainWindow::testSlots(bool visible) {
    uiTelescope->UT_LineEdit->setText(QString("esto es una prueba2") + QString(visible));
}

void MainWindow::createEncoderDocking() {
    //docking via Widget
    QWidget *encoder = new QWidget();
    uiEncoder->setupUi(encoder);
    ui->dockProcess->setWidget(encoder);
}

void MainWindow::createCentralWidget() {
    QWidget *telescopeWidget = new QWidget;
    //uiTelescope->ui = new Ui::TelescopeForm;
    uiTelescope->setupUi(telescopeWidget);
    setCentralWidget(telescopeWidget);
    uiTelescope->UT_LineEdit->setText(QString("esto es una prueba"));
}

void MainWindow::createOffsetDocking() {
    textEdit1 = new QTextEdit();
    textEdit1->setText("hola");
    textEdit2 = new QTextEdit();
    textEdit2->setText("chao");

    QGroupBox *offsetGroupBox = new QGroupBox();
    QVBoxLayout *myLayout = new QVBoxLayout;
    myLayout->addWidget(textEdit1);
    myLayout->addWidget(textEdit2);
    offsetGroupBox->setLayout(myLayout);

    ui->dockOffset->setWidget(offsetGroupBox);
    ui->dockOffset->setWindowTitle("offset");

}
