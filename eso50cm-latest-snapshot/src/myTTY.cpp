#include "myTTY.h"

myTTY::myTTY( const char * device, int baudrate )
: myRS232( device, baudrate )  
{
    m_tty_is_connected = false;
    m_tty_is_running = false;
    
    m_incomingMessage = false;
    m_newMessage = false;
    m_buffer_length = 0;

    //alpha = new myServerSocket( "/tmp/alpha" );
    //alpha->start();
    //delta = new myServerSocket( "/tmp/delta" );
    //delta->start();
 
    _tid = 0;

}

/**
 * myTTY::~myTTY()
 */
myTTY::~myTTY() {
    printf( "[myTTY::~myTTY] Exiting...\n" );
    //if( stop() == 0 ) {
    //    printf( "[myTTY::~myTTY] tty stoped succesfully\n" );
    //} else {
    //    printf( "[myTTY::~myTTY] error when stoping tty\n" );
    //}
    //printf( "[myTTY::~myTTY] deliting alpha...\n" );
    //delete alpha;
    //printf( "[myTTY::~myTTY] deliting delta...\n" );
    //delete delta;

    printf( "[myTTY::~myTTY] Good bye!\n" );
}

/**
 * myTTY::stop()
 */
int myTTY::stop( void ) {
    int retval = -1;
    printf( "[myTTY::stop] I am Stop...\n" );
    
    m_tty_end = false;
    m_tty_is_running = false;
    while( ! m_tty_end ) {
        printf( "[myTTY::stop] thread still runing...\n" );
        sleep(1);
    }
    
    if ( _tid ) {
        printf( "[myTTY::stop] Stoping thread (tid=%lu)...\n", _tid );
        retval = pthread_join( _tid, NULL );
        if( retval == 0 ) {
            printf( "[myTTY::stop] pthread joined succesfuly\n" );
        } else {
            perror( "[myTTY::stop] pthread_join " );
            printf( "[myTTY::stop] pthread_join error, returned %d\n", retval );
        }
    } else {
        printf( "[myTTY::stop] No TTY to stop\n" );
    }
    
    return retval;
}

/**
 * myTTY::start()
 */
int myTTY::start( void ) {
    int retval;
    if( (retval = pthread_create( & _tid, NULL, gate, this ) ) == 0 ) {
        printf( "[myTTY::start] pthread_create OK (return code = %d)\n", retval );
    } else {
        perror( "[myTTY::start] pthread_create " );
        printf( "[myTTY::start] pthread_create error (return code = %d)\n", retval );
    }
    return retval;
}

void * myTTY::gate( void * p ) {
    ( (myTTY *)p )->run();
    return NULL;
}

/**
 * @brief Serial comm. main loop.
 * This loop receive and transmit data through USB link.
 * Received data are stored according to i2c_address.
 *
 */
void myTTY::run( void ) {
    //int retval;
    //char tty_line[128];
    //char tty_message[128];
    //int tty_message_bytes = 0;
    //int new_tty_message = false;
    
    printf( "[myTTY::run] Hello world\n" );
    printf( "[myTTY::run] PID = %d (%u)\n", (int) getpid(), (int) pthread_self() );

    m_tty_is_connected= false;
    m_tty_is_running = true;

    while( m_tty_is_running ) {

        /**
         * Send messages
         */
        /*
        if( alpha->getIsConnected() ) {
            if( (retval = alpha->clientSocketStatus()) > 0 ) {
                if( (retval = alpha->readSocket( tty_line, 128 )) > 0 ) {
                    printf( "[myTTY::run] sending alpha command\n" );
                    write_RS232( tty_line, retval );
                }
            }
        }
        if( delta->getIsConnected() ) {
            if( delta->clientSocketStatus() > 0 ) {
                if( (retval = delta->readSocket( tty_line, 128 )) > 0 ) {
                    printf( "[myTTY::run] sending delta command\n" );
                    write_RS232( tty_line, retval );
                }
            }
        }
        */
        /**
         * Test connection
         */
        if( ! m_tty_is_connected ) {
            if( open_RS232() < 0  ) {
                printf( "[myTTY::run] Can't open  %s\n", getDeviceName() );
                sleep(10);
            } else {
                printf( "[myTTY::run] %s is open\n", getDeviceName() );
                sleep(1);
                m_tty_is_connected = true;
            }
        } else {
            //printf( "[myTTY::run] %s is open\n", getDeviceName() );
            sleep( 1 );
        }
    }
    close_RS232();
    m_tty_end = true;
    printf( "[myTTY::run] The End (tid=%lu).\n", pthread_self() );
}

/**
 * getIsConnected returns m_isConnected value
 * isConeccted value is seted true when open conection was sucsess. 
 */
int myTTY::getIsConnected( void ) {
    return m_tty_is_connected;
}





