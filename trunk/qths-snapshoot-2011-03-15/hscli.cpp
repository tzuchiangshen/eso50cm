#include <QtGui>

#include "hscli.h"
#include "ui_hscli.h"

/**
 *  @brief Constructor.
 */
hsCli::hsCli(QWidget *parent) :
    QMainWindow(parent),
    ui( new Ui::hsCli )
{
    ui->setupUi( this );

    setWindowTitle( tr( "myHS" ) );
    //ui->hostLineEdit->setFocus();

    //timer = new QTimer( this );
    //connect( timer, SIGNAL( timeout() ), this, SLOT( processOneThing() ) );
    //timer->setSingleShot( true );

    //ui->statusLabel->setText( "Not connected" );

    //handset = new myRS232( "/dev/ttyUSB0", 57600 );
    //handset_fd = handset->open_RS232();
    //if( handset_fd > 0 ) {
    //    handset_notifier = new QSocketNotifier( handset_fd, QSocketNotifier::Read, this );
    //    handset_notifier->setEnabled( true );
    //    connect( handset_notifier, SIGNAL( activated(int) ), this, SLOT( handsetMessage() ) );
    //}

    connect( & alpha_thread, SIGNAL( newData( QString ) ),
             this, SLOT( alphaMessage( QString ) ) );
    connect( & delta_thread, SIGNAL( newData( QString ) ),
             this, SLOT( deltaMessage( QString ) ) );

    connect( ui->northPB, SIGNAL( pressed() ),  this, SLOT( northPressed() )  );
    connect( ui->northPB, SIGNAL( released() ), this, SLOT( northReleased() ) );
    connect( ui->southPB, SIGNAL( pressed() ),  this, SLOT( southPressed() )  );
    connect( ui->southPB, SIGNAL( released() ), this, SLOT( southReleased() ) );

    connect( ui->eastPB, SIGNAL( pressed() ),  this, SLOT( eastPressed() )  );
    connect( ui->eastPB, SIGNAL( released() ), this, SLOT( eastReleased() ) );
    connect( ui->westPB, SIGNAL( pressed() ),  this, SLOT( westPressed() )  );
    connect( ui->westPB, SIGNAL( released() ), this, SLOT( westReleased() ) );

    connect( ui->deviceConnectPB, SIGNAL( clicked() ), this, SLOT( connectHSDevice() ) );
    connect( ui->deviceDisconnectPB, SIGNAL( clicked() ), this, SLOT( disconnectHSDevice() ) );
}

/**
 *  @brief Destructor.
 */
hsCli::~hsCli()
{
    //thesky->close_RS232();
    //handset->close_RS232();
    delete ui;
}


/**
 * @brief This slot implemets the required actios when one of the alpha push buttons are pressed.
 */
void hsCli::alphaPressed( const char dir )
{
    qDebug( "alpha: pressed %c", dir );
    QString page;

    page = tr( "handset_command?rate=" );
    page.append( ui->speedComboBox->currentText() );
    page.append( "&dir=" );
    page.append( dir );
	qDebug() << page;
    alpha_thread.requestNewConnetion( ui->hostLineEdit->text(),
                                     ui->portLineEdit->text().toInt(),
                                     page );
}

/**
 * @brief This slot implemets the required actios when one of the alpha push buttons are released
 */
void hsCli::alphaReleased( const char dir  )
{
    qDebug( "alpha: released %c", dir );
    QString page;

    page = tr( "handset_command?stop&dir=" );
    page.append( dir );
    alpha_thread.requestNewConnetion( ui->hostLineEdit->text(),
                                     ui->portLineEdit->text().toInt(),
                                     page );
}

/**
 * deltaPressed
 */
void hsCli::deltaPressed( const char dir )
{
    qDebug( "delta: pressed %c", dir );
    QString page;
    page.clear();
    page = tr( "handset_command?rate=" );
    page.append( ui->speedComboBox->currentText() );
    page.append( "&dir=" );
    page.append( dir );
    delta_thread.requestNewConnetion( ui->hostLineEdit->text(),
                                      ui->portLineEdit->text().toInt(),
                                      page );
}

/**
 * deltaReleased
 */
void hsCli::deltaReleased( const char dir )
{
    qDebug( "delta: released %c", dir );
    QString page;
    page.clear();
    page = tr( "handset_command?stop&dir=" );
    page.append( dir );
    delta_thread.requestNewConnetion( ui->hostLineEdit->text(),
                                      ui->portLineEdit->text().toInt(),
                                      page );
}


/**
 * northPressed
 */
void hsCli::northPressed( void )
{
    deltaPressed( 'N' );
    ui->southPB->setEnabled( false );
}


/**
 * northReleased
 */
void hsCli::northReleased( void )
{
    deltaReleased( 'N' );
    ui->southPB->setEnabled( true );
}


/**
 * southPressed
 */
void hsCli::southPressed( void )
{
    deltaPressed( 'S' );
    ui->northPB->setEnabled( false );
}


/**
 * southReleased
 */
void hsCli::southReleased( void )
{
    deltaReleased( 'S' );
    ui->northPB->setEnabled( true );
}

/**
 * eastPressed
 */
void hsCli::eastPressed( void )
{
    alphaPressed( 'E' );
    ui->westPB->setEnabled( false );
}


/**
 * eastReleased
 */
void hsCli::eastReleased( void )
{
    alphaReleased( 'E' );
    ui->westPB->setEnabled( true );
}

/**
 * westPressed
 */
void hsCli::westPressed( void )
{
    alphaPressed( 'W' );
    ui->eastPB->setEnabled( false );
}


/**
 * westReleased
 */
void hsCli::westReleased( void )
{
    alphaReleased( 'W' );
    ui->eastPB->setEnabled( true );
}







/** ***************************************************************
 * cliConnected
 */
//void hsCli::cliConnected( void )
//{
//    ui->connectButton->setEnabled( false );
//    ui->disconnectButton->setEnabled( true );
//    ui->view->clear();
//}

/**
 * cliDisconnected
 */
//void hsCli::cliDisconnected( void )
//{
//    ui->connectButton->setEnabled( true );
//    ui->disconnectButton->setEnabled( false );
//    ui->statusLabel->setText( "Not connected" );
//}


/**
 * connectToServer
 */
//void hsCli::connectToServer( void )
//{
//    thread.requestNewData( ui->hostLineEdit->text(),
//                                 ui->portLineEdit->text().toInt());
//}

/**
 * disconnectFromServer
 */
//void hsCli::disconnectFromServer( void )
//{
//    thread.quitRun();
//}


/**
 * displayError
 */
void hsCli::displayError( int socketError, const QString & message )
{
    switch( socketError ) {
        case QAbstractSocket::HostNotFoundError:
            QMessageBox::information( this, tr("LCU Client"),
                                      tr("The host was not found. Please check the "
                                         "host and port settings." ) );
            break;
    case QAbstractSocket::ConnectionRefusedError:
            QMessageBox::information( this, tr("LCU Client"),
                                        tr("The connection was refused by the peer. "
                                           "Make sure the fortune server is running, "
                                           "and check that the host name and port "
                                           "settings are correct.") );
            break;
    default:
            QMessageBox::information( this, tr( "LCU Client"),
                                      tr("The following error occurred: %1.").arg(message) );
    }

}

/**
 * showData
 */
//void hsCli::showData( const QString & data )
//{
//    qDebug() << "[hsCli::showData]" << data;
//}

/**
 *  processOneThing
 */
//void hsCli::processOneThing( void )
//{
//    thread.requestNewData( ui->hostLineEdit->text(),
//                           ui->portLineEdit->text().toInt());
//    QMessageBox::information( this, tr("LCU Client"), tr("The Timer is done!" ) );
//}


/**
 *  handsetMessage
 */
void hsCli::handsetMessage( int fd )
{
    static char buffer[128];
    static int rx_bytes;
    static QString slewing_rate;

    if( fd != handset->getPort() ) {
       qDebug( "[TelescopeCli::handsetMessage] Unknown Socket" );
    }

    while( handset->status_RS232() > 0 ) {
        rx_bytes += handset->read_RS232( & buffer[rx_bytes], 128 - rx_bytes - 1 );
        buffer[rx_bytes] = 0;
    }
    if( buffer[rx_bytes-1] == '#' ) {
        rx_bytes = 0;
        //qDebug( "[TelescopeCli::handsetMessage] %s", buffer );

        QString request;
        //QString page;
        //QString answer;
        request = tr( buffer );
        ui->deviceView->append( request );
        //page.clear();
        //answer.clear();
        if( request.contains(":Sw") ) {
            //qDebug( "[TelescopeCli::handsetMessage] Select slewing rate...(%s)", buffer );
            slewing_rate.clear();
            if( request.contains("S#") ) {
                slewing_rate = tr( "rate=0" );
                ui->speedComboBox->setCurrentIndex( 3 );
            } else if( request.contains("s#") ) {
                slewing_rate = tr( "rate=s" );
                ui->speedComboBox->setCurrentIndex( 0 );
            } else if( request.contains("G#") ) {
                slewing_rate = tr( "rate=G" );
                ui->speedComboBox->setCurrentIndex( 1 );
            } else if( request.contains("O#") ) {
                slewing_rate = tr( "rate=O" );
                ui->speedComboBox->setCurrentIndex( 2 );
            } else {
                slewing_rate = tr( "rate=0" );
                ui->speedComboBox->setCurrentIndex( 3 );
            }
            //qDebug() << "[TelescopeCli::handsetMessage] Slewing rate: " << slewing_rate;
        } else if( request.contains(":M") ) {
            //page.clear();
            //qDebug( "[TelescopeCli::handsetMessage] Start slewing to... (%s)", buffer );
            if( ! slewing_rate.contains( "rate=0" ) && slewing_rate.length() > 0 ) {
                //page = tr( "start_slewing?" );
                //page.append( slewing_rate );
                if( request.contains("n#") ) {
                    //qDebug() << tr( "[TelescopeCli::handsetMessage] N" );
                    northPressed();
                    //page.append( "&dir=n" );
                } else if( request.contains("s#") ) {
                    //qDebug() << tr( "[TelescopeCli::handsetMessage] S" );
                    southPressed();
                    //page.append( "&dir=s" );
                } else if( request.contains("e#") ) {
                    //qDebug() << tr( "[TelescopeCli::handsetMessage] E" );
                    eastPressed();
                    //page.append( "&dir=e" );
                } else if( request.contains("w#") ) {
                    //qDebug() << tr( "[TelescopeCli::handsetMessage] W" );
                    westPressed();
                    //page.append( "&dir=w" );
                }
                /** Send data to server */
                //command_tread.requestNewConnetion( ui->hostLineEdit->text(),
                //           ui->portLineEdit->text().toInt(), page );
            }
            //qDebug() << tr( "[TelescopeCli::handsetMessage] " ) << page;
        } else if( request.contains(":Q") ) {
            //page.clear();
            //qDebug( "[TelescopeCli::handsetMessage] Stop slewing to... (%s)", buffer );
            //page = tr( "stop_slewing" );
            if( request.contains("n#") ) {
                //page.append( "?dir=n" );
                //qDebug() << tr( "[TelescopeCli::handsetMessage] N" );
                northReleased();
            } else if( request.contains("s#") ) {
                //page.append( "?dir=s" );
                //qDebug() << tr( "[TelescopeCli::handsetMessage] S" );
                southReleased();
            } else if( request.contains("e#") ) {
                //page.append( "?dir=e" );
                //qDebug() << tr( "[TelescopeCli::handsetMessage] E" );
                eastReleased();
            } else if( request.contains("w#") ) {
                //page.append( "?dir=w" );
                //qDebug() << tr( "[TelescopeCli::handsetMessage] W" );
                westReleased();
            } 
            //command_tread.requestNewConnetion( ui->hostLineEdit->text(),
            //               ui->portLineEdit->text().toInt(), page );
            //qDebug() << tr( "[TelescopeCli::handsetMessage] " ) << page;
        }
    }
}

/**
 * alphaMessage
 */
void hsCli::alphaMessage( QString message )
{
    qDebug( "[TelescopeCli::alphaMessage] New data" );

    //qDebug() << "[TelescopeCli::alphaMessage]\n" << message.toAscii();
    ui->telescopeView->append( message );
    if( message.contains("STOP") ) {
        alpha_thread.quitRun();
    }
    qDebug( "[TelescopeCli::alphaMessage] Good bye!" );
}

/**
 * deltaMessage
 */
void hsCli::deltaMessage( QString message )
{
    qDebug( "[TelescopeCli::deltaMessage] New data" );

    //qDebug() << "[TelescopeCli::deltaMessage]\n" << message.toAscii();
    ui->telescopeView->append( message );
    if( message.contains("STOP") ) {
        delta_thread.quitRun();
    }
    qDebug( "[TelescopeCli::deltaMessage] Good bye!" );
}


/**
 * connectHSDevice
 */
void hsCli::connectHSDevice( void )
{
    handset = new myRS232( ui->deviceLineEdit->text().toAscii().data(), ui->baudsLineEdit->text().toInt() );
    handset_fd = handset->open_RS232();
    if( handset_fd > 0 ){
        handset_notifier = new QSocketNotifier( handset_fd, QSocketNotifier::Read, this );
        handset_notifier->setEnabled( true );
        connect( handset_notifier, SIGNAL( activated(int) ), this, SLOT( handsetMessage(int) ) );
        ui->deviceConnectPB->setEnabled( false );
        ui->deviceDisconnectPB->setEnabled( true );
        qDebug() << tr( "[hsCli::connectHSDevice] OK." );
    } else {
        delete handset;
        qDebug() << tr( "[hsCli::connectHSDevice] ERROR!" );
    }

}

/**
 * disconnectHSDevice
 */
void hsCli::disconnectHSDevice( void )
{
    if( handset->getIsConnected() ) {
        handset->close_RS232();
        delete handset_notifier;
        delete handset;
        ui->deviceDisconnectPB->setEnabled( false );
        ui->deviceConnectPB->setEnabled( true );
        qDebug() << tr( "[hsCli::disconnectHSDevice] OK." );
    }
}
