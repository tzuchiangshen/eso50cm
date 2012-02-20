#include <QtGui>

#include "telescopecli.h"
#include "ui_telescopecli.h"

TelescopeCli::TelescopeCli(QWidget *parent) :
    QMainWindow(parent),
    ui( new Ui::TelescopeCli ),
    logger("TcsGui")
{
    ui->setupUi( this );

    logger.logINFO("TcsGUI started.");

    setWindowTitle(tr("myLCU client"));
    //ui->hostLineEdit->setFocus();

	controller = new TcsGuiController;
	controller->connect();
	lcu = controller->getLCUReference();
	//controller->test();
	controller->start();
	///controller->setTargetPositionRA("ra=+05:15:05");


    //connect( ui->hostLineEdit, SIGNAL( textChanged(QString) ),
    //         this, SLOT( enableConnectButton() ) );
    //connect( ui->portLineEdit, SIGNAL( textChanged(QString) ),
    //         this, SLOT( enableConnectButton() ) );
    //ui->connectButton->setEnabled( true );
    //ui->disconnectButton->setEnabled( false );
    //connect( ui->connectButton, SIGNAL( clicked() ),
    //         this, SLOT( connectToServer() ) );
    //connect( ui->disconnectButton, SIGNAL( clicked() ),
    //         this, SLOT( disconnectFromServer() ) );

    connect( controller, SIGNAL( newData(int, OUC::TelescopeData* ) ),
             this, SLOT( showData(int, OUC::TelescopeData* ) ) );
    //connect( & thread, SIGNAL( error( int, QString ) ),
    ///         this, SLOT( displayError( int, QString ) ) );
    //connect( & thread, SIGNAL( runConnected() ),
    //         this, SLOT( cliConnected() ) );
    //connect( & thread, SIGNAL( runDisconnected() ),
    //         this, SLOT( cliDisconnected() ) );

    timer = new QTimer( this );
    connect( timer, SIGNAL( timeout() ), this, SLOT( processOneThing() ) );
    timer->setSingleShot( true );

    ui->statusLabel->setText( "Not connected" );

    logger.logINFO("TelescopeCli::TelescopeCli: Connecting to theSky6 through /dev/ttyS0");
    thesky = new myRS232( "/dev/ttyS0", 9600);
    thesky_fd = thesky->open_RS232();
    if( thesky_fd > 0 ) {
       thesky_notifier = new QSocketNotifier( thesky_fd, QSocketNotifier::Read, this );
       thesky_notifier->setEnabled( true );
       connect( thesky_notifier, SIGNAL( activated(int) ), this, SLOT( theSkyMessage() ) );
    }

    logger.logINFO("TelescopeCli::TelescopeCli: Connecting to handset through /dev/ttyUSB");
    handset = new myRS232( "/dev/ttyUSB0", 57600 );
    handset_fd = handset->open_RS232();
    if( handset_fd > 0 ) {
        handset_notifier = new QSocketNotifier( handset_fd, QSocketNotifier::Read, this );
        handset_notifier->setEnabled( true );
        connect( handset_notifier, SIGNAL( activated(int) ), this, SLOT( handsetMessage() ) );
    }

    //connect( & command_tread, SIGNAL( newData( QString ) ),
    //         this, SLOT( informationMessage( QString ) ) );
    
    m_thesky_waitanswer = false;
}

TelescopeCli::~TelescopeCli()
{
    thesky->close_RS232();
    handset->close_RS232();
    delete ui;
}

/**
 * cliConnected
 */
//void TelescopeCli::cliConnected( void )
//{
//    ui->connectButton->setEnabled( false );
//    ui->disconnectButton->setEnabled( true );
//    ui->view->clear();
//}

/**
 * cliDisconnected
 */
//void TelescopeCli::cliDisconnected( void )
//{
//    ui->connectButton->setEnabled( true );
//    ui->disconnectButton->setEnabled( false );
//    ui->statusLabel->setText( "Not connected" );
//}


/**
 * connectToServer
 */
void TelescopeCli::connectToServer( void )
{
    //thread.requestNewData( ui->hostLineEdit->text(),
    //                             ui->portLineEdit->text().toInt());
}

/**
 * disconnectFromServer
 */
void TelescopeCli::disconnectFromServer( void )
{
    //thread.quitRun();
}


/**
 * changeLocation
 */
void TelescopeCli::showData(const int type,  OUC::TelescopeData *data ) {
	//logger.logFINE() << "TelescopeCli::showData type=" << type << " data=" << data->currentPos.RA;

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
		ui->UT_LineEdit->setText( info );
		//LT
		ts = *localtime(&now);
		strftime(buf, sizeof(buf), "%H:%M:%S", &ts);
		info = QString(buf);
		ui->LT_LineEdit->setText( info );
		//JD
		info = QString("%1").arg(data->julianDate, 0, 'f', 0);
		ui->JD_LineEdit->setText( info );
		//LST
		controller->formatRAPosition(data->currentPos.localSideralTime, buf, sizeof(buf));
		info = QString(tr(buf));
		ui->LST_LineEdit->setText( info );

		//current position : RA
		controller->formatRAPosition(data->currentPos.RA, buf, sizeof(buf));
		info = QString(tr(buf));
		ui->telRALineEdit->setText( info );
		//current position : Dec
		controller->formatDecPosition(data->currentPos.Dec, buf, sizeof(buf));
		info = QString(tr(buf));
		ui->telDecLineEdit->setText( info );
		//current position : HA
		controller->formatRAPosition(data->currentPos.HA, buf, sizeof(buf));
		info = QString(tr(buf));
		ui->telHALineEdit->setText( info );

		//target position : RA
		controller->formatRAPosition(data->targetPos.RA, buf, sizeof(buf));
		info = QString(tr(buf));
		ui->trgRALineEdit->setText( info );
		//target position : Dec
		controller->formatDecPosition(data->targetPos.Dec, buf, sizeof(buf));
		info = QString(tr(buf));
		ui->trgDecLineEdit->setText( info );
		//target position : HA
		controller->formatRAPosition(data->targetPos.HA, buf, sizeof(buf));
		info = QString(tr(buf));
		ui->trgHALineEdit->setText( info );
		//differences: RA
		controller->formatRAPosition(data->differencePos.RA, buf, sizeof(buf));
		info = QString(tr(buf));
		ui->difRALineEdit->setText( info );
		//difference position : Dec
		controller->formatDecPosition(data->differencePos.Dec, buf, sizeof(buf));
		info = QString(tr(buf));
		ui->difDecLineEdit->setText( info );
		//difference position : HA
		controller->formatRAPosition(data->differencePos.HA, buf, sizeof(buf));
		info = QString(tr(buf));
		ui->difHALineEdit->setText( info );

		//Azimuth
		controller->formatRAPosition(data->currentPos.Az, buf, sizeof(buf));
		info = QString(tr(buf));
		ui->telAzLineEdit->setText( info );
		//Elevation
		controller->formatDecPosition(data->currentPos.Alt, buf, sizeof(buf));
		info = QString(tr(buf));
		ui->telAltLineEdit->setText( info );

        info = QString("<font color='green'>Connected<font>");
        ui->statusLabel->setText( info );

	} else if ( type == 2) {
        //error happended in the controller.
        info = QString("<font color='red'>Not Connected</font>");
        ui->statusLabel->setText( info );
    }

}

void TelescopeCli::showData( const QString & data ) {
    //ui->view->append( data );
    //data.indexOf( "LT");

	

    QString str;
    QString info;
    int i = 0;
    str = data.section( '\n', i, i );
	logger.logFINE("showData: str=%s", str.toStdString().c_str()) ;
    do {
        if( str.contains("LT  =") ) {
            info = str.section( "|", 1, 1 );
            ui->LT_LineEdit->setText( info  );
            //info = str.section( "|", 3, 3 );
            //ui->UD_LineEdit->setText( info  );
        } else if( str.contains("UT  =") ) {
            info = str.section( "|", 1, 1 );
            ui->UT_LineEdit->setText( info  );
            //info = str.section( "|", 3, 3 );
            //ui->UD_LineEdit->setText( info  );
        } else if( str.contains("LST =") ) {
            info = str.section( "|", 1, 1 );
            ui->LST_LineEdit->setText( info  );
            info = str.section( "|", 3, 3 );
            ui->JD_LineEdit->setText( info  );
        } else if( str.contains("RA  =") ) {
            info = str.section( "|", 1, 1 );
            ui->telRALineEdit->setText( info );
            info = str.section( "|", 3, 3 );
            ui->telHALineEdit->setText( info );
        } else if( str.contains("Dec =") ) {
            info = str.section( "|", 1, 1 );
            ui->telDecLineEdit->setText( info );
        } else if( str.contains("tRA =") ) {
            info = str.section( "|", 1, 1 );
            ui->trgRALineEdit->setText( info );
            info = str.section( "|", 3, 3 );
            ui->trgHALineEdit->setText( info );
        } else if( str.contains("tDec=") ) {
            info = str.section( "|", 1, 1 );
            ui->trgDecLineEdit->setText( info );
        } else if( str.contains("dRA =") ) {
            info = str.section( "|", 1, 1 );
            ui->difRALineEdit->setText( info );
            int idx;
            if( (idx = info.indexOf("-")) >= 0 ) {
                info.replace( idx, 1, "+");
            } else if( (idx = info.indexOf("+")) >= 0 ) {
                info.replace( idx, 1, "-");
            }
            ui->difHALineEdit->setText( info );
            info = str.section( "|", 3, 3 );
            ui->difDecLineEdit->setText( info );
        } else if( str.contains("Alt =") ) {
            info = str.section( "|", 1, 1 );
            ui->telAltLineEdit->setText( info );
            info = str.section( "|", 3, 3 );
            ui->telAzLineEdit->setText( info );
        } else if( str.contains( "generated", Qt::CaseInsensitive ) ) {
            info = str.section( "|", 2, 2 );
            ui->statusLabel->setText( info );
        } else if( str.contains( "info", Qt::CaseInsensitive ) ) {
            //ui->view->append( str );
        }

        i ++ ;
        str = data.section( '\n', i, i );
    } while( str.length() > 0 );

    //timer->start( 5000 );
}

//void TelescopeCli::displayError( int socketError, const QString & message )
//{
//    switch( socketError ) {
//        case QAbstractSocket::HostNotFoundError:
//            QMessageBox::information( this, tr("LCU Client"),
//                                      tr("The host was not found. Please check the "
//                                         "host and port settings." ) );
//            break;
//    case QAbstractSocket::ConnectionRefusedError:
//            QMessageBox::information( this, tr("LCU Client"),
//                                        tr("The connection was refused by the peer. "
//                                           "Make sure the fortune server is running, "
//                                           "and check that the host name and port "
//                                           "settings are correct.") );
//            break;
//    default:
//            QMessageBox::information( this, tr( "LCU Client"),
//                                      tr("The following error occurred: %1.").arg(message) );
//    }
//
//}

/**
 *  processOneThing
 */
void TelescopeCli::processOneThing( void )
{
    //thread.requestNewData( ui->hostLineEdit->text(),
    //                             ui->portLineEdit->text().toInt());

    //QMessageBox::information( this, tr("LCU Client"), tr("The Timer is done!" ) );

}


/**
 *  theSkyMessage
 */
void TelescopeCli::theSkyMessage( void )
{
    static char buffer[128];
    static int  rx_bytes;
    static QString arguments;

    while( thesky->status_RS232() > 0 ) {
        rx_bytes += thesky->read_RS232( & buffer[rx_bytes], 128 - rx_bytes - 1 );
        buffer[rx_bytes] = 0;
    }
    if( buffer[rx_bytes-1] == '#' ) {
        rx_bytes  = 0;

        QString request;
        QString answer;
        request = tr( buffer );
        answer.clear();
        logger.logFINE(" commands arrived from theSkySix: (%s)", buffer);

        if( request.contains("GR#") ) {
			char buffer2[256];
            logger.logFINE( "[TelescopeCli::theSkyMessage] GR# Get Telescope RA (%s)", buffer );
            /** Send answer to The Sky */
            /* answer.append( ui->telRALineEdit->text().toAscii() ); */
	   		//answer.append("06:18:10");
			controller->getCurrentPositionRA(buffer2, 256);
			//logger.logFINE() << "<<<<<<<<<<<<<<<<<" << buffer2;
			answer.append(tr(buffer2));
            answer.append("#");
            answer.remove( QRegExp(" ") );
            logger.logFINE("[TelescopeCli::theSkyMessage] GR# Response RA %s", answer.toStdString().c_str());
            thesky->write_RS232( answer.toAscii(), answer.length() );

        } else if( request.contains("GD#") ) {
			char buffer2[256];
            logger.logFINE( "[TelescopeCli::theSkyMessage] GD# Get Telescope Dec (%s)", buffer );
            /** Send answer to The Sky */
            //answer.append( ui->telDecLineEdit->text().toAscii() );
			controller->getCurrentPositionDec(buffer2, 256);
			//logger.logFINE() << "<<<<<<<<<<<<<<<<<" << buffer2;
	    	//answer.append("-44:09:00");
			answer.append(tr(buffer2));
            answer.append("#");
            answer.remove( QRegExp(" ") );
            logger.logFINE("[TelescopeCli::theSkyMessage] GD# Response Dec %s", answer.toStdString().c_str());
            thesky->write_RS232( answer.toAscii(), answer.length() );
        } else if( request.contains("Q#") ) {
            logger.logFINE( "[TelescopeCli::theSkyMessage] Q# Halt all current slewing (%s)", buffer );
            /** Send answer to The Sky */
            //Returns nothing
            //answer.append("0#");
            //thesky->write_RS232( answer.toAscii(), answer.length() );
        } else if( request.contains("Sr") ) {
            if( arguments.length() > 0 ) {
                arguments.append( "&" );
            }
            if( ! arguments.contains( "ra=") ) {
                arguments.append( "ra=" );
                arguments.append( & buffer[3] );
                arguments.remove( QRegExp(" ") );
                arguments.remove( QRegExp("#") );
                logger.logINFO("[TelescoeCli::theSkyMessage] Sr# Set target ra: %s", arguments.toStdString().c_str()); //.toLatin1();
				controller->setTargetPositionRA(arguments.toStdString().c_str());
            }
            /** Send answer to The Sky */
            answer.append("1#");    //RA Accepted
            thesky->write_RS232( answer.toAscii(), answer.length() );
        } else if( request.contains("Sd") ) {
            if( arguments.length() > 0 ) {
                arguments.append( "&" );
            }
            if( ! arguments.contains( "dec=") ) {
                arguments.append( "dec=" );
                arguments.append( & buffer[3] );
                arguments.remove( QRegExp(" ") );
                arguments.remove( QRegExp("#") );
                logger.logINFO("[TelescopeCli::theSkyMessage] Sd# Set target dec: %s", arguments.toStdString().c_str()); //.toLatin1();
				controller->setTargetPositionDec(arguments.toStdString().c_str());
            }
            /** Send answer to The Sky */
            answer.append("1#");    //Dec accepted
            thesky->write_RS232( answer.toAscii(), answer.length() );
        } else if( request.contains("MS#") ) {
            if( arguments.length() > 0 ) {
                arguments.insert( 0, "set_target?");
            }
            logger.logFINE("[TelescopeCli::theSkyMessage] MS# Slew to target %s: ",arguments.toStdString().c_str());
            /** Send data to server */
            //command_tread.requestNewConnetion( ui->hostLineEdit->text(),
            //               ui->portLineEdit->text().toInt(), arguments );
            m_thesky_waitanswer = true;
            /** Clear used arguments */
            arguments.clear();
            /** Send answer to The Sky */
            answer.append("0#");    //Slew is possible
            thesky->write_RS232( answer.toAscii(), answer.length() );
        } else {
            logger.logFINE( "[TelescopeCli::theSkyMessage] Message %s", buffer );
        }
    }

}

/**
 *  handsetMessage
 */
void TelescopeCli::handsetMessage( void )
{
    static char buffer[128];
    static int rx_bytes;
    static QString slewing_rate;
    while( handset->status_RS232() > 0 ) {
        rx_bytes += handset->read_RS232( & buffer[rx_bytes], 128 - rx_bytes - 1 );
        buffer[rx_bytes] = 0;
    }
    if( buffer[rx_bytes-1] == '#' ) {
        rx_bytes = 0;
        //logger.logFINE( "[TelescopeCli::handsetMessage] %s", buffer );

        QString request;
        QString page;
        QString answer;
        request = tr( buffer );
        page.clear();
        answer.clear();
        if( request.contains(":Sw") ) {
            logger.logFINE( "[TelescopeCli::handsetMessage] Select slewing rate...(%s)", buffer );
            slewing_rate.clear();
            if( request.contains("S#") ) {
                slewing_rate = tr( "rate=S" );
            } else if( request.contains("s#") ) {
                slewing_rate = tr( "rate=s" );
            } else if( request.contains("G#") ) {
                slewing_rate = tr( "rate=G" );
            } else if( request.contains("O#") ) {
                slewing_rate = tr( "rate=O" );
            } else if( request.contains("0#") ) {
                slewing_rate = tr( "rate=0" );
            }
            logger.logFINE("[TelescopeCli::handsetMessage] Slewing rate: %s", slewing_rate.toStdString().c_str());
        } else if( request.contains(":M") ) {
            page.clear();
            logger.logFINE( "[TelescopeCli::handsetMessage] Start slewing to... (%s)", buffer );
            if( ! slewing_rate.contains( "rate=0" ) && slewing_rate.length() > 0 ) {
                page = tr( "start_slewing?" );
                page.append( slewing_rate );
                if( request.contains("n#") ) {
                    logger.logFINE("[TelescopeCli::handsetMessage] N");
                    page.append( "&dir=n" );
                } else if( request.contains("s#") ) {
                    logger.logFINE("[TelescopeCli::handsetMessage] S");
                    page.append( "&dir=s" );
                } else if( request.contains("e#") ) {
                    logger.logFINE("[TelescopeCli::handsetMessage] E");
                    page.append( "&dir=e" );
                } else if( request.contains("w#") ) {
                    logger.logFINE("[TelescopeCli::handsetMessage] W");
                    page.append( "&dir=w" );
                }
                /** Send data to server */
                //command_tread.requestNewConnetion( ui->hostLineEdit->text(),
                //           ui->portLineEdit->text().toInt(), page );
            }
            logger.logFINE("[TelescopeCli::handsetMessage] %s", page.toStdString().c_str());
        } else if( request.contains(":Q") ) {
            page.clear();
            logger.logFINE( "[TelescopeCli::handsetMessage] Stop slewing to... (%s)", buffer );
            page = tr( "stop_slewing" );
            if( request.contains("n#") ) {
                page.append( "?dir=n" );
                 logger.logFINE("[TelescopeCli::handsetMessage] N");
            } else if( request.contains("s#") ) {
                page.append( "?dir=s" );
                 logger.logFINE("[TelescopeCli::handsetMessage] S");
            } else if( request.contains("e#") ) {
                page.append( "?dir=e" );
                 logger.logFINE("[TelescopeCli::handsetMessage] E");
            } else if( request.contains("w#") ) {
                page.append( "?dir=w" );
                 logger.logFINE("[TelescopeCli::handsetMessage] W");
            } 
            //command_tread.requestNewConnetion( ui->hostLineEdit->text(),
            //               ui->portLineEdit->text().toInt(), page );
            logger.logFINE("[TelescopeCli::handsetMessage] %s", page.toStdString().c_str());
        }
    }
}

/**
 * informationMessage
 */
//void TelescopeCli::informationMessage( QString message )
//{
//    logger.logFINE( "[TelescopeCli::informationMessage] New data" );
//
//    if( m_thesky_waitanswer ) {
//        m_thesky_waitanswer = false;
//        message.insert( 0, "<pre>" );
//        message.append( "</pre>" );
//        QMessageBox::StandardButton reply;
//        reply = QMessageBox::information( this, tr("myTelescope"), message );
//        if (reply == QMessageBox::Ok)
//            ui->infoLabel->setText(tr("OK"));
//        else
//            ui->infoLabel->setText(tr("Escape"));
//    } else {
//        logger.logFINE() << message.toLatin1();
//    }
//    //command_tread.wakeUp();
//    logger.logFINE( "[TelescopeCli::informationMessage] Good bye!" );
//}
