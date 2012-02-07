#include "commandthread.h"

/**
 * Constructor
 */
commandThread::commandThread( QObject * parent )
    : QThread( parent ), m_quit( false )
{
}

/**
 * Destructor
 */
commandThread::~commandThread( void )
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
void commandThread::requestNewConnetion( const QString & hostName, quint16 port, QString command )
{
    QMutexLocker locker( & mutex );
    this->m_hostName = hostName;
    this->m_port = port;
    this->m_command = command;

    if ( ! isRunning() ) {
        qDebug( "[commandThread::requestNewConnetion] Starting" );
        start();
    } else {
        qDebug( "[commandThread::requestNewConnetion] is runnig" );
        cond.wakeOne();
    }
}


/**
 * quitRun
 */
void commandThread::quitRun( void )
{
    mutex.lock();
    m_quit = true;
    cond.wakeOne();
    mutex.unlock();
}

/**
 * wakeUp
 */
void commandThread::wakeUp( void )
{
    mutex.lock();
    cond.wakeOne();
    mutex.unlock();
}

/**
 * run
 */
void commandThread::run( void )
{

    mutex.lock();
    QString serverName = m_hostName;
    quint16 serverPort = m_port;
    int quit = m_quit;
    QString request = m_command;
    mutex.unlock();

    do {
        QTcpSocket socket;
        const int Timeout = 5 * 1000;
    
        if( socket.state() == QAbstractSocket::UnconnectedState ) {
            socket.connectToHost( serverName, serverPort );

            if ( ! socket.waitForConnected( Timeout ) ) {
                emit error( socket.error(), socket.errorString() );
                return;
            }
        }
        quit = 2;
        do {
            /** Request method */
            request.insert( 0, "TXT /" );
            request.append( " HTTP/1.1\r\n\r\n" );
            socket.write( request.toAscii(), request.length()  );
            request = tr( "\r\n\r\n" );
            socket.write( request.toAscii(), request.length()  );

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
                    //qDebug( "[commandThread::run] New Data %d", (int) socket.bytesAvailable()  );
                    bytes_read = in.readRawData( buffer, blockSize - 1 );
                    buffer[bytes_read] = 0;
                    data.append( tr( buffer ) );
                    if( data.contains( "\r\n\r\n" ) ){
                        cont = false;
                    }
                }
            }
            qDebug( "[commandThread::run] New data" );
            mutex.lock();
            emit newData( data );
            cond.wait( & mutex );
            serverName = m_hostName;
            serverPort = m_port;
            request = m_command;
            mutex.unlock();

            quit --;
            qDebug( ) << "[commandThread::run] quit=" << quit;
        } while( quit );

        //mutex.lock();
        //quit = m_quit;
        //mutex.unlock();
    } while ( ! m_quit );
    qDebug( "[commandThread::run] Good bye!" );
}

