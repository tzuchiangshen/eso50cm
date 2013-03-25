#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "ui_encoder.h"
#include "ui_offset.h"
#include "ui_telescope.h"
#include "ui_status.h"
#include "ui_logpanel.h"
#include <QVBoxLayout>
#include <QObject>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QPalette>
#include <QDebug>
#include <QFuture>
#include <QtCore>
#include <QWebView>
#include <QUrl>
#include <QSortFilterProxyModel>


MainWindow::MainWindow(int arg, char** argv, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    uiTelescope(new Ui::TelescopeForm),
    uiEncoder(new Ui::encoderForm),
    uiOffset(new Ui::offsetForm),
    uiStatus(new Ui::statusForm),
    uiLogPanel(new Ui::LogPanel),
    mainController(new MainController)
{




    ui->setupUi(this);

    createOffsetDocking();
    createEncoderDocking();
    createCentralWidget();
    createLoggerDocking(arg, argv);
    createStatusBar();
    createWebcamDocking();


    // toolbar
    //connect( ui->saveAsAction, SIGNAL( triggered()),
    //        this, SLOT( stopTelescope()));

    connect( ui->gotoTargetAction, SIGNAL( triggered()), this, SLOT( gotoTarget() ));
    connect( ui->parkTelescopeAction, SIGNAL(triggered()), this, SLOT( parkTelescope() ));
    connect( ui->parkTelescopeCapAction, SIGNAL(triggered()), this, SLOT( parkTelescopeCap() ));
    connect( ui->trackingAction, SIGNAL(triggered()), this, SLOT( startTracking() ));
    connect( ui->trackingDisableAction, SIGNAL(triggered()), this, SLOT( stopTracking() ));
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

    // menu file
    connect( ui->actionExit, SIGNAL(triggered()), this, SLOT(exit()));
    // menu windows
    connect( ui->logsLevelPanelAction, SIGNAL(triggered()), this, SLOT( openLogsLevelPanel()));
    connect( ui->logsPanelAction, SIGNAL(triggered()), this, SLOT( openLogsPanel()));


    // status bar
    qRegisterMetaType<ProcessStatus>("ProcessStatus");
    qRegisterMetaType<TelescopeStatus>("TelescopeStatus");
    qRegisterMetaType<TrackingStatus>("TrackingStatus");
    connect( mainController->obsControl, SIGNAL( newObsControlStatusTriggered(ProcessStatus)),
             this, SLOT( updateObsControlStatus(ProcessStatus) ));
    connect( mainController->obsControl, SIGNAL( newTelescopeStatusTriggered(TelescopeStatus)),
             this, SLOT( updateTelescopeStatus(TelescopeStatus) ));
    connect( mainController, SIGNAL( newTheSky6StatusTriggered(ProcessStatus)),
             this, SLOT( updateTheSky6Status(ProcessStatus)) );
    connect( mainController->obsControl, SIGNAL( newTrackingStatusTriggered(TrackingStatus)),
             this, SLOT( updateTrackingStatus(TrackingStatus)) );
    connect( mainController->obsControl, SIGNAL( newLCUControlStatusTriggered(ProcessStatus)),
             this, SLOT( updateLCUControlStatus(ProcessStatus)) );

    //set initial condition
    stopTelescope();


}

MainWindow::~MainWindow()
{
    delete ui;
    delete uiTelescope;
    delete uiEncoder;
    delete uiOffset;
    delete uiLogPanel;
}


void MainWindow::testSlots(bool visible) {
    uiTelescope->UT_LineEdit->setText(QString("esto es una prueba2") + QString(visible));
}

// ToolBar

void MainWindow::stopTelescope() {
    mainController->obsControl->stopTracking();
    mainController->obsControl->stopTelescope();
}

void MainWindow::startTracking() {
    mainController->obsControl->startTracking();
}

void MainWindow::stopTracking() {
    mainController->obsControl->stopTracking();
}


void MainWindow::parkTelescope() {
    mainController->obsControl->parkTelescope();
}

void MainWindow::parkTelescopeCap() {
    mainController->obsControl->parkTelescopeCap();
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
    telescopeWidget->setMinimumWidth(550);
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
        table->item(3,0)->setText(QString::number(enc->remAlphaMotor));
        table->item(3,0)->setTextColor(QColor(255,0,0));


        //Delta
        table->item(5,0)->setText(QString::number(enc->lectDeltaAxisE));
        table->item(5,1)->setText(QString::number(enc->posDeltaAxisE));
        table->item(6,0)->setText(QString::number(enc->lectDeltaWormE));
        table->item(6,1)->setText(QString::number(enc->posDeltaWormE));
        table->item(7,0)->setText(QString::number(enc->lectDeltaMotor));
        table->item(7,1)->setText(QString::number(enc->posDeltaMotor));
        table->item(8,0)->setText(QString::number(enc->remDeltaMotor));
        table->item(8,0)->setTextColor(QColor(255,0,0));
    }
}

// menu
void MainWindow::openLogsLevelPanel() {
    QProcess p;
    p.startDetached("LogLevelPanel");
    p.waitForStarted();
}

void MainWindow::openLogsPanel() {
    QProcess p;
    p.startDetached("runLogPanel");
    p.waitForStarted();
}

void MainWindow::exit() {
    close();
}

//slots for status bar
void MainWindow::updateTelescopeStatus(TelescopeStatus status) {
    QPalette plt = telescopeStatus->palette();

    //ProcessStatus status = (ProcessStatus)type;
    switch(status) {
        case TelescopeStop:
        {
            plt.setColor(QPalette::Button, Qt::green);
            telescopeStatus->setPalette(plt);
            telescopeStatus->setText("Stop");
            break;
        }
        case TelescopeMoving:
        {
            plt.setColor(QPalette::Button, Qt::yellow);
            telescopeStatus->setPalette(plt);
            telescopeStatus->setText("Moving");
            break;
        }
        case TelescopeError:
        {
            plt.setColor(QPalette::Button, Qt::red);
            telescopeStatus->setPalette(plt);
            telescopeStatus->setText("Error");
            break;
        }
        default:
        {
            plt.setColor(QPalette::Button, Qt::gray);
            telescopeStatus->setPalette(plt);
            telescopeStatus->setText("unknown");
            break;
        }
    }
}
void MainWindow::updateObsControlStatus(ProcessStatus status) {
    QPalette plt = obsControlStatus->palette();

    //ProcessStatus status = (ProcessStatus)type;
    switch(status) {
        case ProcessConnected:
        {
            plt.setColor(QPalette::Button, Qt::green);
            obsControlStatus->setPalette(plt);
            this->obsControlStatus->setText("connected");
            break;
        }
        case ProcessDisconnected:
        {
            plt.setColor(QPalette::Button, Qt::red);
            obsControlStatus->setPalette(plt);
            obsControlStatus->setText("disconnected");
            break;
        }
        case ProcessError:
        {
            plt.setColor(QPalette::Button, Qt::red);
            telescopeStatus->setPalette(plt);
            telescopeStatus->setText("Error");
            break;
        }
        default:
        {
            plt.setColor(QPalette::Button, Qt::gray);
            obsControlStatus->setPalette(plt);
            obsControlStatus->setText("unknown");
            break;
        }

    }
}

void MainWindow::updateLCUControlStatus(ProcessStatus status) {
    QPalette plt = lcuControlStatus->palette();

    //ProcessStatus status = (ProcessStatus)type;
    switch(status) {
        case ProcessConnected:
        {
            plt.setColor(QPalette::Button, Qt::green);
            lcuControlStatus->setPalette(plt);
            lcuControlStatus->setText("connected");
            break;
        }
        case ProcessDisconnected:
        {
            plt.setColor(QPalette::Button, Qt::red);
            lcuControlStatus->setPalette(plt);
            lcuControlStatus->setText("disconnected");
            break;
        }
        case ProcessError:
        {
            plt.setColor(QPalette::Button, Qt::red);
            lcuControlStatus->setPalette(plt);
            lcuControlStatus->setText("Error");
            break;
        }
        default:
        {
            plt.setColor(QPalette::Button, Qt::gray);
            lcuControlStatus->setPalette(plt);
            lcuControlStatus->setText("unknown");
            break;
        }

    }
}

void MainWindow::updateTrackingStatus(TrackingStatus status) {
    QPalette plt = trackingStatus->palette();

    //ProcessStatus status = (ProcessStatus)type;
    switch(status) {
        case TrackingOff:
        {
            plt.setColor(QPalette::Button, Qt::green);
            trackingStatus->setPalette(plt);
            trackingStatus->setText("Off");
            break;
        }
        case TrackingOn:
        {
            plt.setColor(QPalette::Button, Qt::yellow);
            trackingStatus->setPalette(plt);
            trackingStatus->setText("On");
            break;
        }
        case TrackingError:
        {
            plt.setColor(QPalette::Button, Qt::red);
            trackingStatus->setPalette(plt);
            trackingStatus->setText("Error");
            break;
        }
        default:
        {
            plt.setColor(QPalette::Button, Qt::gray);
            trackingStatus->setPalette(plt);
            trackingStatus->setText("unknown");
            break;
        }
    }

}

void MainWindow::updateTheSky6Status(ProcessStatus status) {
    QPalette plt = obsControlStatus->palette();

    //ProcessStatus status = (ProcessStatus)type;
    switch(status) {
        case ProcessConnected:
        {
            plt.setColor(QPalette::Button, Qt::green);
            theSky6Status->setPalette(plt);
            theSky6Status->setText("connected");
            break;
        }
        case ProcessDisconnected:
        {
            plt.setColor(QPalette::Button, Qt::red);
            theSky6Status->setPalette(plt);
            theSky6Status->setText("disconnected");
            break;
        }
        case ProcessError:
        {
            plt.setColor(QPalette::Button, Qt::red);
            theSky6Status->setPalette(plt);
            theSky6Status->setText("Error");
            break;
        }
        default:
        {
            plt.setColor(QPalette::Button, Qt::gray);
            theSky6Status->setPalette(plt);
            theSky6Status->setText("unknown");
            break;
        }
    }
}

// widget creation
void MainWindow::createOffsetDocking() {
    QWidget *offsetWidget = new QWidget;
    //uiTelescope->ui = new Ui::TelescopeForm;
    uiOffset->setupUi(offsetWidget);
    ui->dockOffset->setWidget(offsetWidget);
    ui->dockOffset->setWindowTitle("offset");
    //ui->dockOffset->setMaximumSize(ui->dockOffset->sizeHint());

    ui->dockOffset->setMinimumSize(200, 300);
    ui->dockOffset->setMaximumSize(400, 300);
    qDebug() << "docking size=" << ui->dockOffset->sizeHint();
}

void MainWindow::createEncoderDocking() {
    //docking via Widget
    QWidget *encoderWidget = new QWidget();
    uiEncoder->setupUi(encoderWidget);
    ui->dockEncoder->setWidget(encoderWidget);

    ui->dockEncoder->setMinimumSize(350, 280);
    ui->dockEncoder->setMaximumSize(400, 300);

    QTableWidget *table = encoderWidget->findChild<QTableWidget*>("tableWidget");
    for (int i=0; i < table->rowCount(); i++) {
        table->setRowHeight(i,20);
        for(int j=0; j<table->columnCount(); j++) {
            table->setItem(i,j, new QTableWidgetItem(""));
            table->item(i,j)->setTextAlignment(Qt::AlignRight);
        }
    }
}

void MainWindow::createLoggerDocking(int arg, char** argv) {
    QWidget *logPanelWidget = new QWidget();
    uiLogPanel->setupUi(logPanelWidget);
    ui->dockLogger->setWidget(logPanelWidget);
    ui->dockLogger->setMinimumHeight(200);

//   dummy message for testing
//    LogMessageData msg;
//    msg.data = "hola";
//    //msg.level = 100;
//    msg.method = "probando";
//    model->addMessage(msg);

    //move it to the constructor
    list = new QList<LogMessageQT>;
    model= new MessageTableModel(this);
    model->setList(list,10000);  //   <--- MAX NUMBER OF LOGS!!
    arg = 2;
    argv[1] = "--Ice.Config=/eso50cm/SWROOT/config/loggingService.config";
    Subscriber *subs = new Subscriber(uiLogPanel, model, arg, argv);
    subs->start();

    proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(model);
    proxyModel->setFilterKeyColumn(1);
    setProxyFilter(2); // default value for this is info

    connect(uiLogPanel->cbFilter,SIGNAL(currentIndexChanged(int)),this,SLOT(setProxyFilter(int)));
    connect(uiLogPanel->btStartLogging,SIGNAL(clicked()),this,SLOT(startLogging()));
    connect(uiLogPanel->btStopLogging,SIGNAL(clicked()),this,SLOT(stopLogging()));
    connect(uiLogPanel->btClearLogging,SIGNAL(clicked()),this,SLOT(clearLogging()));

    uiLogPanel->btStartLogging->setEnabled(false);

    uiLogPanel->tableView->setModel(proxyModel);
    //uiLogPanel->tableView->setModel(model);
    uiLogPanel->tableView->setColumnWidth(0,175);
    uiLogPanel->tableView->setColumnWidth(1,80);
    uiLogPanel->tableView->setColumnWidth(2,140);
    uiLogPanel->tableView->setColumnWidth(3,600);
    uiLogPanel->tableView->horizontalHeader()->setStretchLastSection(true);
    uiLogPanel->tableView->show();
    proxyModel->sort(0, Qt::DescendingOrder);
    //connect(uiLogPanel->bNext,SIGNAL(clicked()),this,SLOT(searchNext()));
    //connect(uiLogPanel->bPrevious,SIGNAL(clicked()),this,SLOT(searchPrevious()));
}

void MainWindow::createWebcamDocking() {

    ui->dockWebcam->setVisible(false);
    ui->dockProcess->setVisible(false);
    return;

    QWebView *view = new QWebView();
    view->setStyleSheet("background-color:rgb(150,147,88); padding: 7px ; color:rgb(255,255,255)");
    //QUrl url("http://admin:gUlg2385KaW@observatorio2.astro.puc.cl:1084/admin/view.cgi?profile=2");
    QUrl url("http://admin:gUlg2385KaW@192.168.0.11:1084/admin/view.cgi?profile=2");
    //QUrl url("http://weather.aiv.alma.cl/data/all/images/temperature-30min.png");
    //qDebug() << "url=" << url;
    view->settings()->setAttribute(QWebSettings::PluginsEnabled, true);
    view->settings()->setAttribute(QWebSettings::JavaEnabled, true);
    view->settings()->setAttribute(QWebSettings::AutoLoadImages, true);
    view->load(url);
    view->showMinimized();
    ui->dockWebcam->setWidget(view);
    ui->dockWebcam->setWindowTitle("webcam");
    ui->dockWebcam->setMinimumWidth(300);
    //ui->dockWebcam->setMaximumWidth(400);
}

void MainWindow::createStatusBar() {

    QLabel *lblTelescopeStatus = new QLabel("Telescope");
    telescopeStatus = new QToolButton();
    QLabel *lblTrackingStatus = new QLabel("Tracking");
    trackingStatus = new QToolButton();
    QLabel *lblTheSky6Status = new QLabel("TheSky6");
    theSky6Status = new QToolButton();
    QLabel *lblObsControlStatus = new QLabel("ObsControl");
    obsControlStatus = new QToolButton();
    QLabel *lblLCUControlStatus = new QLabel("LCUControl");
    lcuControlStatus = new QToolButton();

    statusBar()->addPermanentWidget(lblTelescopeStatus);
    statusBar()->addPermanentWidget(telescopeStatus);
    statusBar()->addPermanentWidget(lblTrackingStatus);
    statusBar()->addPermanentWidget(trackingStatus);
    statusBar()->addPermanentWidget(lblTheSky6Status);
    statusBar()->addPermanentWidget(theSky6Status);
    statusBar()->addPermanentWidget(lblObsControlStatus);
    statusBar()->addPermanentWidget(obsControlStatus);
    statusBar()->addPermanentWidget(lblLCUControlStatus);
    statusBar()->addPermanentWidget(lcuControlStatus);

    //statusBar()->setMinimumHeight(256);

    //telescopeStatus
    //telescopeStatus->setTextColor(0,0,0);
    telescopeStatus->setText("unknown");
    telescopeStatus->setToolTip("telescope");
    telescopeStatus->setEnabled(true);


    //trackingStatus
    trackingStatus->setText("unknown");
    trackingStatus->setToolTip("tracking");
    trackingStatus->setEnabled(true);

    //theSky6Status
    theSky6Status->setText("unknown");
    theSky6Status->setToolTip("TheSky6");
    theSky6Status->setEnabled(true);

    //obsControlStatus
    obsControlStatus->setText("unknown");
    obsControlStatus->setToolTip("ObsControl");
    obsControlStatus->setEnabled(true);

    //lcuControlStatus
    lcuControlStatus->setText("unknown");
    lcuControlStatus->setToolTip("LCUControl");
    lcuControlStatus->setEnabled(true);

    statusBar()->showMessage("eso50cm Console started", 3000);
}


void MainWindow::setProxyFilter( int filter)
{
    qDebug() << " setProxyFilter: filter=" << filter;
    bool current_logging_status = model->isReceivingMessage();
    if(current_logging_status == true)  {
        model->stopReceivingMessage();
        qDebug() << " stop receiving messages ";
    }
    switch(filter) {
      case 0:
            proxyModel->setFilterRegExp("(^Severe$)");
            break;
      case 1:
            proxyModel->setFilterRegExp("(^Severe$|^Warning$)");
            break;
      case 2:
            proxyModel->setFilterRegExp("(^Severe$|^Warning$|^Info$)");
            break;
      case 3:
            proxyModel->setFilterRegExp("(^Severe$|^Warning$|^Info$|^Config$)");
            break;
      case 4:
            proxyModel->setFilterRegExp("(^Severe$|^Warning$|^Info$|^Config$|^Fine$)");
            break;
      case 5:
            proxyModel->setFilterRegExp("(^Severe$|^Warning$|^Info$|^Config$|^Fine$|^Finer$)");
            break;
      case 6:
            proxyModel->setFilterRegExp("(^Severe$|^Warning$|^Info$|^Config$|^Fine$|^Finer$|^Finest$)");
            break;
    }
    if(current_logging_status  == true) {
        model->startReceivingMessage();
        qDebug() << " start receiving messages ";
    }
}


void MainWindow::startLogging() {
    qDebug() << " started to receive messages ";
    uiLogPanel->btStartLogging->setEnabled(false);
    uiLogPanel->btStopLogging->setEnabled(true);
    model->startReceivingMessage();
}

void MainWindow::stopLogging() {
    qDebug() << " stopped to receive messages ";
    uiLogPanel->btStartLogging->setEnabled(true);
    uiLogPanel->btStopLogging->setEnabled(false);
    model->stopReceivingMessage();
}

void MainWindow::clearLogging() {
    qDebug() << " clear logPanel ";
    model->clearList();
}
