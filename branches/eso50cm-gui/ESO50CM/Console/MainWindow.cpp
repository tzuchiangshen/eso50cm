#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "ui_encoder.h"
#include "ui_offset.h"
#include "ui_telescope.h"
#include <QVBoxLayout>
#include <QObject>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    uiTelescope(new Ui::TelescopeForm),
    uiEncoder(new Ui::encoderForm),
    uiOffset(new Ui::offsetForm),
    mainController(new MainController)
{
    ui->setupUi(this);

    createOffsetDocking();
    //createEncoderDocking();
    createCentralWidget();

    connect( ui->saveAsAction, SIGNAL( triggered(bool)),
            this, SLOT( testSlots(bool)));

    connect( mainController->obsControl, SIGNAL( newData(int, OUC::TelescopeData* ) ),
             this, SLOT( showData(int, OUC::TelescopeData* ) ) );
}

MainWindow::~MainWindow()
{
    delete ui;
    delete uiTelescope;
    delete uiEncoder;
    delete uiOffset;
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

void MainWindow::showData(int type, OUC::TelescopeData *data ) {
    QString info;

    if (type == 1) {

        //Time
        struct tm ts;
        time_t now;
        now = (time_t)data->lcuTime;

        char buf[256];
        //UT
        ts = *gmtime(&now);
        strftime(buf, sizeof(buf), "%H:%M:%S", &ts);
        info = QString(buf);
        uiTelescope->UT_LineEdit->setText( info );
        //LT
        ts = *localtime(&now);
        strftime(buf, sizeof(buf), "%H:%M:%S", &ts);
        info = QString(buf);
        uiTelescope->LT_LineEdit->setText( info );
        //JD
        info = QString("%1").arg(data->julianDate, 0, 'f', 0);
        uiTelescope->JD_LineEdit->setText( info );
        //LST
        mainController->obsControl->formatRAPosition(data->currentPos.localSideralTime, buf, sizeof(buf));
        info = QString(tr(buf));
        uiTelescope->LST_LineEdit->setText( info );

        //current position : RA
        mainController->obsControl->formatRAPosition(data->currentPos.RA, buf, sizeof(buf));
        info = QString(tr(buf));
        uiTelescope->telRALineEdit->setText( info );
        //current position : Dec
        mainController->obsControl->formatDecPosition(data->currentPos.Dec, buf, sizeof(buf));
        info = QString(tr(buf));
        uiTelescope->telDecLineEdit->setText( info );
        //current position : HA
        mainController->obsControl->formatRAPosition(data->currentPos.HA, buf, sizeof(buf));
        info = QString(tr(buf));
        uiTelescope->telHALineEdit->setText( info );

        //target position : RA
        mainController->obsControl->formatRAPosition(data->targetPos.RA, buf, sizeof(buf));
        info = QString(tr(buf));
        uiTelescope->trgRALineEdit->setText( info );
        //target position : Dec
        mainController->obsControl->formatDecPosition(data->targetPos.Dec, buf, sizeof(buf));
        info = QString(tr(buf));
        uiTelescope->trgDecLineEdit->setText( info );
        //target position : HA
        mainController->obsControl->formatRAPosition(data->targetPos.HA, buf, sizeof(buf));
        info = QString(tr(buf));
        uiTelescope->trgHALineEdit->setText( info );
        //differences: RA
        mainController->obsControl->formatRAPosition(data->differencePos.RA, buf, sizeof(buf));
        info = QString(tr(buf));
        uiTelescope->difRALineEdit->setText( info );
        //difference position : Dec
        mainController->obsControl->formatDecPosition(data->differencePos.Dec, buf, sizeof(buf));
        info = QString(tr(buf));
        uiTelescope->difDecLineEdit->setText( info );
        //difference position : HA
        mainController->obsControl->formatRAPosition(data->differencePos.HA, buf, sizeof(buf));
        info = QString(tr(buf));
        uiTelescope->difHALineEdit->setText( info );

        //Azimuth
        mainController->obsControl->formatRAPosition(data->currentPos.Az, buf, sizeof(buf));
        info = QString(tr(buf));
        uiTelescope->telAzLineEdit_2->setText( info );
        //Elevation
        mainController->obsControl->formatDecPosition(data->currentPos.Alt, buf, sizeof(buf));
        info = QString(tr(buf));
        uiTelescope->telAltLineEdit_2->setText( info );

        //info = QString("<font color='green'>Connected<font>");
        //uiTelescope->statusLabel->setText( info );

    } else if ( type == 2) {
        //error happended in the controller.
        //info = QString("<font color='red'>Not Connected</font>");
        //ui->statusLabel->setText( info );

        //should emit the reconnect signal to the auto-reconnect thread
    }

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

//void MainWindow::createOffsetDocking() {
//    textEdit1 = new QTextEdit();
//    textEdit1->setText("hola");
//    textEdit2 = new QTextEdit();
//    textEdit2->setText("chao");

//    QGroupBox *offsetGroupBox = new QGroupBox();
//    QVBoxLayout *myLayout = new QVBoxLayout;
//    myLayout->addWidget(textEdit1);
//    myLayout->addWidget(textEdit2);
//    offsetGroupBox->setLayout(myLayout);

//    ui->dockOffset->setWidget(offsetGroupBox);
//    ui->dockOffset->setWindowTitle("offset");

//}

void MainWindow::createOffsetDocking() {
    QWidget *offsetWidget = new QWidget;
    //uiTelescope->ui = new Ui::TelescopeForm;
    uiOffset->setupUi(offsetWidget);
    ui->dockOffset->setWidget(offsetWidget);
    ui->dockOffset->setWindowTitle("offset");
    //ui->dockOffset->setMaximumSize(ui->dockOffset->sizeHint());

    ui->dockOffset->setMinimumWidth(200);
    qDebug() << "size=" << ui->dockOffset->sizeHint();
}
