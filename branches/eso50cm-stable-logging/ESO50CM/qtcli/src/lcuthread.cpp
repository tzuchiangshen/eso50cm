
#include "lcuthread.h"

/**
 * Constructor
 */
lcuThread::lcuThread( QObject * parent )
    : QThread( parent ), m_quit( false )
{
}

/**
 * Destructor
 */
lcuThread::~lcuThread( void )
{
    mutex.lock();
    m_quit = true;
    cond.wakeOne();
    mutex.unlock();
    wait();
}

/**
 * requestNewConnetion
 */
void lcuThread::requestNewData( const QString & hostName, quint16 port )
{
    QMutexLocker locker( & mutex );
    this->hostName = hostName;
    this->port = port;

    if ( ! isRunning() )
        start();
    else
        cond.wakeOne();
}

void lcuThread::quitRun( void )
{
    mutex.lock();
    m_quit = true;
    mutex.unlock();
}

/**
 * run
 */
void lcuThread::run( void )
{

    mutex.lock();
    QString serverName = hostName;
    quint16 serverPort = port;
    m_quit = false;
    mutex.unlock();

    QTcpSocket socket;
    const int Timeout = 5 * 1000;

    socket.connectToHost( serverName, serverPort );

    if ( ! socket.waitForConnected( Timeout ) ) {
        emit error( socket.error(), socket.errorString() );
        return;
    }
    emit runConnected();

    /** Request method */
    QString request;
    request = tr( "TXT /monitor_position HTTP/1.1\r\n\r\n" );
    socket.write( request.toAscii(), request.length()  );
    request = tr( "\r\n\r\n" );
    socket.write( request.toAscii(), request.length()  );

    int no_quit = 10;
    while ( no_quit ) {

        /** Wait for data */
        QString data;
        QDataStream in( & socket );
        in.setVersion( QDataStream::Qt_4_0 );
        data.clear();
        int bytes_read;
        char buffer[1024];
        int blockSize = 1024;

        bool cont = true;
        while( cont ) {
            //while( socket.bytesAvailable() < blockSize )
            //&& socket.error() != QAbstractSocket::RemoteHostClosedError
            if( ! socket.waitForReadyRead( Timeout ) ) {
                if( socket.error() != QAbstractSocket::SocketTimeoutError  ) {
                    emit error( socket.error(), socket.errorString() );
                    return;
                }
            } else {
                //qDebug( "[lcuThread::run] New Data %d", (int) socket.bytesAvailable()  );
                bytes_read = in.readRawData( buffer, blockSize - 1 );
                buffer[bytes_read] = 0;
                data.append( tr( buffer ) );
                if( data.contains( "\r\n\r\n" ) ){
                    //qDebug( "[lcuThread::run] End of data" );
                    cont = false;
                }
            }
        }

        mutex.lock();
        emit newData( data );

        //cond.wait( & mutex );
        //qDebug( "End of wait" );
        serverName = hostName;
        serverPort = port;
        no_quit = ! m_quit;
        mutex.unlock();
        sleep( 1 );
        //no_quit --;
        if( no_quit > 0 ) {
            request = tr( "data" );
            socket.write( request.toAscii(), request.length()  );
        } //else {
        //    request = tr( "quit" );
        //    socket.write( request.toAscii(), request.length()  );
        //}
        //qDebug( "[lcuThread::run] To quit %d", no_quit );
    }
    //qDebug( "[lcuThread::run] Good bye!" );
    emit runDisconnected();
}

/**
 * old_run
 */
void lcuThread::old_run( void )
{

    mutex.lock();
    QString serverName = hostName;
    quint16 serverPort = port;
    mutex.unlock();

    while ( ! m_quit ) {
        QTcpSocket socket;
        const int Timeout = 5 * 1000;

        socket.connectToHost( serverName, serverPort );

        if ( ! socket.waitForConnected( Timeout ) ) {
            emit error( socket.error(), socket.errorString() );
            return;
        }
        /** Request new data */
        QString request;
        request = tr( "TXT /monitor_position HTTP/1.1\r\n\r\n" );
        socket.write( request.toAscii(), request.length()  );
        request = tr( "\r\n\r\n" );
        socket.write( request.toAscii(), request.length()  );

        /** Wait for data */
        QString data;
        QDataStream in( & socket );
        in.setVersion( QDataStream::Qt_4_0 );
        data.clear();
        int bytes_read;
        char buffer[256];
        int blockSize = 256;

        bool cont = true;
        while( cont ) {
            //while( socket.bytesAvailable() < blockSize )
            //&& socket.error() != QAbstractSocket::RemoteHostClosedError
            if( ! socket.waitForReadyRead( Timeout ) ) {
                if( socket.error() != QAbstractSocket::SocketTimeoutError  ) {
                    emit error( socket.error(), socket.errorString() );
                    return;
                }
            } else {
                //qDebug( "New Data %d", (int) socket.bytesAvailable()  );
                bytes_read = in.readRawData( buffer, blockSize - 1 );
                buffer[bytes_read] = 0;
                data.append( tr( buffer ) );
                if( data.contains( "\r\n\r\n" ) ){
                    //qDebug( "End of data" );
                    cont = false;
                }
            }
        }

        mutex.lock();
        emit newData( data );

        cond.wait( & mutex );
        //qDebug( "End of wait" );
        serverName = hostName;
        serverPort = port;
        mutex.unlock();

    }
}
