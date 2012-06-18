#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "ui_encoder.h"
#include "ui_offset.h"
#include "ui_telescope.h"
#include <QVBoxLayout>
#include <QObject>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QDebug>
#include <QWebView>
#include <QUrl>


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
    createEncoderDocking();
    createCentralWidget();

    // toolbar
    //connect( ui->saveAsAction, SIGNAL( triggered()),
    //        this, SLOT( stopTelescope()));

    connect( ui->gotoTargetAction, SIGNAL( triggered()),
            this, SLOT( gotoTarget() ));

    connect( ui->stopTelescopeAction, SIGNAL(triggered()), this, SLOT( stopTelescope()));

    // telescope position
    connect( mainController->obsControl, SIGNAL( newData(int, OUC::TelescopeData* ) ),
             this, SLOT( showData(int, OUC::TelescopeData* ) ) );

    // offset panel
    connect( uiOffset->northPB, SIGNAL( pressed() ),  this, SLOT( northButtonPressed() ));
    connect( uiOffset->northPB, SIGNAL( released() ), this, SLOT( northButtonReleased() ));
    connect( uiOffset->southPB, SIGNAL( pressed() ),  this, SLOT( southButtonPressed() ));
    connect( uiOffset->southPB, SIGNAL( released() ), this, SLOT( southButtonReleased() ));
    connect( uiOffset->eastPB, SIGNAL( pressed() ),  this, SLOT( eastButtonPressed() ));
    connect( uiOffset->eastPB, SIGNAL( released() ), this, SLOT( eastButtonReleased() ));
    connect( uiOffset->westPB, SIGNAL( pressed() ),  this, SLOT( westButtonPressed() ));
    connect( uiOffset->westPB, SIGNAL( released() ), this, SLOT( westButtonReleased() ));

    //encoder panel
    connect( mainController->obsControl, SIGNAL( newEncData(int,OUC::RawEncoderData*)),
             this, SLOT(showEncData(int,OUC::RawEncoderData*) ));

    QWebView *view = new QWebView();
    view->setStyleSheet("background-color:rgb(150,147,88); padding: 7px ; color:rgb(255,255,255)");
    //view->url().setUserName("operador");
    //view->url().setPassword("la810KaW");
    //QUrl url("http://operador:operador@192.168.0.11:1084/admin/view.cgi?profile=2");
    QUrl url("http://weather.aiv.alma.cl/data/all/images/temperature-30min.png");
    qDebug() << "url=" << url;
    view->load(url);
    view->show(); // Minimized();
    ui->dockWebcam->setWidget(view);
    ui->dockWebcam->setWindowTitle("webcam");

}

MainWindow::~MainWindow()
{
    delete ui;
    delete uiTelescope;
    delete uiEncoder;
    delete uiOffset;
}


void MainWindow::testSlots(bool visible) {
    uiTelescope->UT_LineEdit->setText(QString("esto es una prueba2") + QString(visible));
}

// ToolBar

void MainWindow::stopTelescope() {
    mainController->obsControl->stopTelescope();
}

void MainWindow::parkTelescope() {
    mainController->obsControl->parkTelescope();
}

void MainWindow::parkTelescopeCap() {
    //to be implemented
    qDebug() << "Not implemented yet";
}

void MainWindow::gotoTarget() {
    mainController->obsControl->gotoTarget();
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

// SLOTs for offset panel

void MainWindow::northButtonPressed() {
    //uiOffset->northPB->setEnabled(false);
    QString rateName = uiOffset->speedComboBox->currentText();
    string dir("N");
    mainController->obsControl->handset_slew(rateName.toStdString(), dir);
}

void MainWindow::southButtonPressed() {
    //uiOffset->southPB->setEnabled(false);
    QString rateName = uiOffset->speedComboBox->currentText();
    string dir("S");
    mainController->obsControl->handset_slew(rateName.toStdString(), dir);
}

void MainWindow::eastButtonPressed() {
    //uiOffset->eastPB->setEnabled(false);
    QString rateName = uiOffset->speedComboBox->currentText();
    string dir("E");
    mainController->obsControl->handset_slew(rateName.toStdString(), dir);
}

void MainWindow::westButtonPressed() {
    //uiOffset->westPB->setEnabled(false);
    QString rateName = uiOffset->speedComboBox->currentText();
    string dir("W");
    mainController->obsControl->handset_slew(rateName.toStdString(), dir);
}

void MainWindow::northButtonReleased() {
    qDebug() << "north released";
    //uiOffset->northPB->setEnabled(true);
    string rate("stop");
    string dir("N");
    mainController->obsControl->handset_slew(rate, dir);
}

void MainWindow::southButtonReleased() {
    //uiOffset->southPB->setEnabled(true);
    string rate("stop");
    string dir("S");
    mainController->obsControl->handset_slew(rate, dir);
}

void MainWindow::eastButtonReleased() {
    //uiOffset->eastPB->setEnabled(true);
    string rate("stop");
    string dir("E");
    mainController->obsControl->handset_slew(rate, dir);
}

void MainWindow::westButtonReleased() {
    //uiOffset->westPB->setEnabled(true);
    string rate("stop");
    string dir("W");
    mainController->obsControl->handset_slew(rate, dir);
}



void MainWindow::createCentralWidget() {
    QWidget *telescopeWidget = new QWidget;
    //uiTelescope->ui = new Ui::TelescopeForm;
    uiTelescope->setupUi(telescopeWidget);
    setCentralWidget(telescopeWidget);
    uiTelescope->UT_LineEdit->setText(QString("esto es una prueba"));
}

void MainWindow::showEncData(int status,OUC::RawEncoderData *enc) {
    if(status ==1) {

        QTableWidget *table = uiEncoder->tableWidget;

        //Alpha
        table->item(0,0)->setText(QString::number(enc->lectAlphaAxisE));
        table->item(0,1)->setText(QString::number(enc->posAlphaAxisE));
        table->item(1,0)->setText(QString::number(enc->lectAlphaWormE));
        table->item(1,1)->setText(QString::number(enc->posAlphaWormE));
        table->item(2,0)->setText(QString::number(enc->lectAlphaMotor));
        table->item(2,1)->setText(QString::number(enc->posAlphaMotor));

        //Delta
        table->item(4,0)->setText(QString::number(enc->lectDeltaAxisE));
        table->item(4,1)->setText(QString::number(enc->posDeltaAxisE));
        table->item(5,0)->setText(QString::number(enc->lectDeltaWormE));
        table->item(5,1)->setText(QString::number(enc->posDeltaWormE));
        table->item(6,0)->setText(QString::number(enc->lectDeltaMotor));
        table->item(6,1)->setText(QString::number(enc->posDeltaMotor));

    }
}

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

void MainWindow::createEncoderDocking() {
    //docking via Widget
    QWidget *encoderWidget = new QWidget();
    uiEncoder->setupUi(encoderWidget);
    ui->dockEncoder->setWidget(encoderWidget);
    ui->dockEncoder->setMinimumWidth(300);
    QTableWidget *table = encoderWidget->findChild<QTableWidget*>("tableWidget");
    for (int i=0; i < table->rowCount(); i++) {
        table->setRowHeight(i,20);
        for(int j=0; j<table->columnCount(); j++) {
            table->setItem(i,j, new QTableWidgetItem(""));
            table->item(i,j)->setTextAlignment(Qt::AlignRight);
        }
    }
}

