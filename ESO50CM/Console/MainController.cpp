#include "MainController.h"

MainController::MainController():
    logger("MainController")
{
    logger.logINFO("MainController::MainController: started");
    //ObsControl Interface
    obsControl = new ObsControlIF();
    obsControl->connect();
    lcu = obsControl->getLCUReference();
    obsControl->start();
    //TheSky6 RS232 Interface
    logger.logINFO("TelescopeCli::TelescopeCli: Connecting to theSky6 through /dev/ttyS0");
    theSky6 = new TheSky6IF( "/dev/ttyS0", 9600);
    theSky6FD = theSky6->open_RS232();
    if( theSky6FD > 0 ) {
        theSkyNotifier = new QSocketNotifier( theSky6FD, QSocketNotifier::Read, this );
        theSkyNotifier->setEnabled( true );
        connect( theSkyNotifier, SIGNAL( activated(int) ), this, SLOT( theSkyMessage() ) );
    }
}

MainController::~MainController() {
    //delete ui;
}

// SLOTS implementations
void MainController::testSlots(bool visible) {
    //ui->UT_LineEdit->setText(QString("esto es una prueba3") + QString(visible));
}

/**
 *  theSkyMessage
 */
void MainController::theSkyMessage( void )
{
    static char buffer[128];
    static int  rx_bytes;
    static QString arguments;

    while( theSky6->status_RS232() > 0 ) {
        rx_bytes += theSky6->read_RS232( & buffer[rx_bytes], 128 - rx_bytes - 1 );
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
            obsControl->getCurrentPositionRA(buffer2, 256);
            //logger.logFINE() << "<<<<<<<<<<<<<<<<<" << buffer2;
            answer.append(tr(buffer2));
            answer.append("#");
            answer.remove( QRegExp(" ") );
            logger.logFINE("[TelescopeCli::theSkyMessage] GR# Response RA %s", answer.toStdString().c_str());
            theSky6->write_RS232( answer.toAscii(), answer.length() );

        } else if( request.contains("GD#") ) {
            char buffer2[256];
            logger.logFINE( "[TelescopeCli::theSkyMessage] GD# Get Telescope Dec (%s)", buffer );
            /** Send answer to The Sky */
            //answer.append( ui->telDecLineEdit->text().toAscii() );
            obsControl->getCurrentPositionDec(buffer2, 256);
            //logger.logFINE() << "<<<<<<<<<<<<<<<<<" << buffer2;
            //answer.append("-44:09:00");
            answer.append(tr(buffer2));
            answer.append("#");
            answer.remove( QRegExp(" ") );
            logger.logFINE("[TelescopeCli::theSkyMessage] GD# Response Dec %s", answer.toStdString().c_str());
            theSky6->write_RS232( answer.toAscii(), answer.length() );
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
                 obsControl->setTargetPositionRA(arguments.toStdString().c_str());
             }
             /** Send answer to The Sky */
             answer.append("1#");    //RA Accepted
             theSky6->write_RS232( answer.toAscii(), answer.length() );
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
                 obsControl->setTargetPositionDec(arguments.toStdString().c_str());
             }
             /** Send answer to The Sky */
             answer.append("1#");    //Dec accepted
             theSky6->write_RS232( answer.toAscii(), answer.length() );
         } else if( request.contains("MS#") ) {
             if( arguments.length() > 0 ) {
                 arguments.insert( 0, "set_target?");
             }
             logger.logFINE("[TelescopeCli::theSkyMessage] MS# Slew to target %s: ",arguments.toStdString().c_str());
             /** Send data to server */
             //command_tread.requestNewConnetion( ui->hostLineEdit->text(),
             //               ui->portLineEdit->text().toInt(), arguments );
             theSky6WaitAnswer = true;
             /** Clear used arguments */
             arguments.clear();
             /** Send answer to The Sky */
             answer.append("0#");    //Slew is possible
             theSky6->write_RS232( answer.toAscii(), answer.length() );
         } else {
             logger.logFINE( "[TelescopeCli::theSkyMessage] Message %s", buffer );
         }
     }

 }


