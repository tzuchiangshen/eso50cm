#include "myRS232.h"


myRS232::myRS232( const char * device, int baudrate ) {
    //printf( "[myRS232::myRS232] Device: %s\n", dev );
    //m_device = new char[strlen(device) + 1];
    strcpy( m_device, device );
    printf( "[myRS232::myRS232] device: %s\n", m_device );
    m_baudrate = baudrate;
    printf( "[myRS232::myRS232] baudrate: %d\n", m_baudrate );
    m_timeout = 500;
    m_microsecs = 250;
    m_tty_is_connected = false;
}
/**
 * Destructor
 */
myRS232::~myRS232( void ) {
    //if( m_device != NULL ) {
    //    free( m_device );
    //}
    printf( "[myRS232::~myRS232] Good bye!\n" );
}

/**
 * setDevName
 */
int myRS232::setDevName( const char * dev )
{
    int retval;
    if( (retval = strlen( dev )) < 32 ) {
        strcpy( m_device, dev );
        printf( "[myRS232::setDevName] %s\n", m_device );
        return retval;
    } else {
        printf( "[myRS232::setDevName] Error: \"%s\" too long!\n", m_device );
        return 0;
    }
}
/**
 * setBaudRate
 */
int myRS232::setBaudRate( int baudrate )
{
    m_baudrate = baudrate;
    return baudrate;
}

/**
 * open_RS232
 */
int myRS232::open_RS232( void )
{
    if( ( m_port = open( m_device, O_RDWR | O_NDELAY ) ) < 0 ) {
        perror( "[myRS232::open_RS232] open" );
        printf( "[myRS232::open_RS232] open ERROR\n" );
        m_tty_is_connected = false;
    } else {
        printf( "[myRS232::open_RS232] open OK\n" );
        m_tty_is_connected = true;
        //if( ioctl( m_port, TCGETA, & oldtio) < 0 ) {
        if( tcgetattr( m_port, & oldtio) < 0 ) {
            perror( "[myRS232::open_RS232] tcgetattr" ) ;
            printf( "[myRS232::open_RS232] tcgetattr ERROR\n" );
        }
        //if( ioctl( m_port, TCGETA, & newtio) < 0 ) {
        if( tcgetattr( m_port, & newtio) < 0 ) {
            perror( "[myRS232::open_RS232] tcgetattr" ) ;
            printf( "[myRS232::open_RS232] tcgetattr ERROR\n" );
        }
        
        switch( m_baudrate ) {
            case 0:
                newtio.c_cflag = B0 | CS8 | CLOCAL | CREAD;
                printf( "[myRS232::open_RS232] baudrate selected: B0\n" );
                break;
            case 9600:
                newtio.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
                printf( "[myRS232::open_RS232] baudrate selected: B9600\n" );
                break;
            case 19200:
                newtio.c_cflag = B19200 | CS8 | CLOCAL | CREAD;
                printf( "[myRS232::open_RS232] baudrate selected: B19200\n" );
                break;
            case 38400:
                newtio.c_cflag = B38400 | CS8 | CLOCAL | CREAD;
                printf( "[myRS232::open_RS232] baudrate selected: B38400\n" );
                break;
            case 57600:
                newtio.c_cflag = B57600 | CS8 | CLOCAL | CREAD;
                printf( "[myRS232::open_RS232] baudrate selected: B57600\n" );
                break;
            case 115200:
                newtio.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
                printf( "[myRS232::open_RS232] baudrate selected: B115200\n" );
                break;
            default:
                newtio.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
                printf( "[myRS232::open_RS232] baudrate selected: B9600\n" );
        }
        newtio.c_iflag = IGNPAR;
        newtio.c_oflag = 0;
        newtio.c_lflag = 0;       //ICANON;
        tcflush( m_port, TCIFLUSH );
        if( tcsetattr( m_port, TCSANOW,  & newtio ) < 0 ) {
            perror( "[myRS232::open_RS232] tcsetattr" ) ;
            printf( "[myRS232::open_RS232] tcsetattr ERROR\n" );
        }
        //flush_RS232();
    }
    return m_port;
}

/**
 * close_RS232
 */
int myRS232::close_RS232( void )
{
    int retval;
    
    printf( "[myRS232::close_RS232] Reseting tty \n" );
    //if( ioctl( m_port, TCSETA, & newtio ) < 0 ) {
    if( tcsetattr( m_port, TCSANOW, & oldtio ) < 0 ) {
        perror( "[myRS232::open_RS232] tcsetattr" ) ;
        printf( "[myRS232::open_RS232] tcsetattr ERROR\n" );
    }
    printf( "[myRS232::close_RS232] Closing tty \n" );
    if( (retval  = close( m_port )) < 0 ) {
    //    printf("[myRS232::close_RS232] close OK\n");
    //} else {
        perror( "[myRS232::close_RS232] close" );
        printf( "[myRS232::close_RS232] close ERROR\n" );
    }
    m_tty_is_connected = false;
    return retval;
}


/**
 * read_RS232
 */
int myRS232::read_RS232( char * str, int max )
{
    int retval;
    if( (retval=read( m_port, str, max ) ) > 0 ) {
        //printf( "[myRS232::read_RS232] read OK (%d)\n", retval );
    } else {
        if( errno == EAGAIN ) {
            printf( "[myRS232::read_RS232] read ERROR: EAGAIN\n" );
        } else {
            perror( "[myRS232::read_RS232] read" );
            printf( "[myRS232::read_RS232] read ERROR\n" );
        }
    }
    return retval; 
}


/**
 * write_RS232
 * Writes a buffer to the port.
 * @param s buffer to be written (char *)
 * @param l lenght of the buffer to be written (int)
 */
int myRS232::write_RS232( const  char * s, int length)
{
    int retval;
    
    if( (retval=write(m_port, s, length) ) < 0 ) {
    //if( (retval=write(m_port, s, length) ) > 0 ) {
    //    printf( "[myRS232::write_RS232] write OK (%d)\n", retval );
    //} else {
        perror( "[myRS232::write_RS232] write" );
        printf( "[myRS232::write_RS232] write ERROR\n" );
    }
    return retval;
}

/**
 * flush_RS232
 */
void myRS232::flush_RS232()
{
    //if( ioctl(m_port, TCFLSH, 0) == -1 ) {
    if( tcflush(m_port, TCIOFLUSH ) == -1 ) {
        perror( "[myRS232::flush_RS232] ioctl" );
        printf( "[myRS232::flush_RS232] ioctl ERROR\n" );
    }
}

/**
 * status_RS232
 */
int myRS232::status_RS232( void )
{
    fd_set read_fds;
    struct timeval tv;
    int retval;

    FD_ZERO( & read_fds );
    FD_SET( m_port, & read_fds);
    tv.tv_sec = 0;
    tv.tv_usec = m_microsecs;

    if( ( retval = select( m_port+1, & read_fds, NULL, NULL, & tv) ) < 0  ) {
        perror( "[myRS232::status_RS232] select" );
        printf( "[myRS232::status_RS232] select ERROR\n" );
    }
    return retval;
}

/**
 * getDeviceName
 * @return m_dev (char *): device name
 */
char * myRS232::getDeviceName( void )
{
    return m_device;
}


int myRS232::getPort( void )
{
    return m_port;
}

/**
 * getIsConnected returns m_isConnected value
 * isConeccted value is seted true when open conection was sucsess. 
 */
int myRS232::getIsConnected( void )
{
    return m_tty_is_connected;
}

