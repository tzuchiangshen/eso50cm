#include "TheSky6IF.h"


TheSky6IF::TheSky6IF( const char * device, int baudrate ):
    logger("TcsGui")
{
    //logger.logINFO( "[myRS232::myRS232] Device: %s\n", dev );
    //m_device = new char[strlen(device) + 1];
    strcpy( m_device, device );
    logger.logINFO( "[myRS232::myRS232] device: %s", m_device );
    m_baudrate = baudrate;
    logger.logINFO( "[myRS232::myRS232] baudrate: %d", m_baudrate );
    m_timeout = 500;
    m_microsecs = 250;
    m_tty_is_connected = false;
}

TheSky6IF::~TheSky6IF( void ) {
    //if( m_device != NULL ) {
    //    free( m_device );
    //}
    logger.logINFO( "[myRS232::~myRS232] Good bye!" );
}

/**
 * open_RS232
 */
int TheSky6IF::setDevName( const char * dev ) {
    int retval;
    if( (retval = strlen( dev )) < 32 ) {
        strcpy( m_device, dev );
        logger.logINFO( "[myRS232::setDevName] %s", m_device );
        return retval;
    } else {
        logger.logINFO( "[myRS232::setDevName] Error: \"%s\" too long!", m_device );
        return 0;
    }
}
/**
 * open_RS232
 */
int TheSky6IF::setBaudRate( int b ) {
    return b;
}

/**
 * open_RS232
 */
int TheSky6IF::open_RS232( void ) {
    if( ( m_port = open( m_device, O_RDWR | O_NDELAY ) ) < 0 ) {
        logger.logSEVERE( "[myRS232::open_RS232] open" );
        logger.logSEVERE( "[myRS232::open_RS232] open ERROR" );
        m_tty_is_connected = false;
    } else {
        logger.logINFO( "[myRS232::open_RS232] open OK" );
        m_tty_is_connected = true;
        //if( ioctl( m_port, TCGETA, & oldtio) < 0 ) {
        if( tcgetattr( m_port, & oldtio) < 0 ) {
            logger.logSEVERE( "[myRS232::open_RS232] tcgetattr" ) ;
            logger.logINFO( "[myRS232::open_RS232] tcgetattr ERROR" );
        }
        //if( ioctl( m_port, TCGETA, & newtio) < 0 ) {
        if( tcgetattr( m_port, & newtio) < 0 ) {
            logger.logSEVERE( "[myRS232::open_RS232] tcgetattr" ) ;
            logger.logINFO( "[myRS232::open_RS232] tcgetattr ERROR" );
        }
        
        switch( m_baudrate ) {
            case 0:
                newtio.c_cflag = B0 | CS8 | CLOCAL | CREAD;
                logger.logINFO( "[myRS232::open_RS232] baudrate selected: B0" );
                break;
            case 9600:
                newtio.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
                logger.logINFO( "[myRS232::open_RS232] baudrate selected: B9600" );
                break;
            case 19200:
                newtio.c_cflag = B19200 | CS8 | CLOCAL | CREAD;
                logger.logINFO( "[myRS232::open_RS232] baudrate selected: B19200" );
                break;
            case 38400:
                newtio.c_cflag = B38400 | CS8 | CLOCAL | CREAD;
                logger.logINFO( "[myRS232::open_RS232] baudrate selected: B38400" );
                break;
            case 57600:
                newtio.c_cflag = B57600 | CS8 | CLOCAL | CREAD;
                logger.logINFO( "[myRS232::open_RS232] baudrate selected: B57600" );
                break;
            case 115200:
                newtio.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
                logger.logINFO( "[myRS232::open_RS232] baudrate selected: B115200" );
                break;
            default:
                newtio.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
                logger.logINFO( "[myRS232::open_RS232] baudrate selected: B9600" );
        }
        newtio.c_iflag = IGNPAR;
        newtio.c_oflag = 0;
        newtio.c_lflag = 0;       //ICANON;
        tcflush( m_port, TCIFLUSH );
        if( tcsetattr( m_port, TCSANOW,  & newtio ) < 0 ) {
            logger.logSEVERE( "[myRS232::open_RS232] tcsetattr" ) ;
            logger.logSEVERE( "[myRS232::open_RS232] tcsetattr ERROR\n" );
        }
        //flush_RS232();
    }
    return m_port;
}

/**
 * close_RS232
 */
int TheSky6IF::close_RS232( void ) {
    int retval;
    
    logger.logINFO( "[myRS232::close_RS232] Reseting tty " );
    //if( ioctl( m_port, TCSETA, & newtio ) < 0 ) {
    if( tcsetattr( m_port, TCSANOW, & oldtio ) < 0 ) {
        logger.logSEVERE( "[myRS232::open_RS232] tcsetattr" ) ;
        logger.logSEVERE( "[myRS232::open_RS232] tcsetattr ERROR" );
    }
    logger.logINFO( "[myRS232::close_RS232] Closing tty \n" );
    if( (retval  = close( m_port )) < 0 ) {
    //    logger.logINFO("[myRS232::close_RS232] close OK\n");
    //} else {
        logger.logSEVERE( "[myRS232::close_RS232] close" );
        logger.logSEVERE( "[myRS232::close_RS232] close ERROR" );
    }
    m_tty_is_connected = false;
    return retval;
}


/**
 * read_RS232
 */
int TheSky6IF::read_RS232( char * str, int max ) {
    int retval;
    if( (retval=read( m_port, str, max ) ) > 0 ) {
        //logger.logINFO( "[myRS232::read_RS232] read OK (%d)\n", retval );
    } else {
        if( errno == EAGAIN ) {
            //logger.logINFO( "[myRS232::read_RS232] read ERROR: EAGAIN\n" );
        } else {
            logger.logSEVERE( "[myRS232::read_RS232] read" );
            logger.logSEVERE( "[myRS232::read_RS232] read ERROR" );
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
int TheSky6IF::write_RS232( const  char * s, int length) {
    int retval;
    
    if( (retval=write(m_port, s, length) ) < 0 ) {
    //if( (retval=write(m_port, s, length) ) > 0 ) {
    //    logger.logINFO( "[myRS232::write_RS232] write OK (%d)\n", retval );
    //} else {
        logger.logSEVERE( "[myRS232::write_RS232] write" );
        logger.logSEVERE( "[myRS232::write_RS232] write ERROR" );
    } else {
        logger.logFINE("[myRS232::write_RS232] sending '%s' to the serial port", s);
    }
    return retval;
}

/**
 * flush_RS232
 */
void TheSky6IF::flush_RS232() {
    //if( ioctl(m_port, TCFLSH, 0) == -1 ) {
    if( tcflush(m_port, TCIOFLUSH ) == -1 ) {
        logger.logSEVERE( "[myRS232::flush_RS232] ioctl" );
        logger.logSEVERE( "[myRS232::flush_RS232] ioctl ERROR" );
    }
}

/**
 * status_RS232
 */
int TheSky6IF::status_RS232( void ) {
    fd_set read_fds;
    struct timeval tv;
    int retval;

    FD_ZERO( & read_fds );
    FD_SET( m_port, & read_fds);
    tv.tv_sec = 0;
    tv.tv_usec = m_microsecs;

    if( ( retval = select( m_port+1, & read_fds, NULL, NULL, & tv) ) < 0  ) {
        logger.logSEVERE( "[myRS232::status_RS232] select" );
        logger.logSEVERE( "[myRS232::status_RS232] select ERROR" );
    }
    return retval;
}

/**
 * getDeviceName
 * @return m_dev (char *): device name
 */
char * TheSky6IF::getDeviceName( void ) {
    return m_device;
}


int TheSky6IF::getPort( void ) {
    return m_port;
}

/**
 * getIsConnected returns m_isConnected value
 * isConeccted value is seted true when open conection was sucsess. 
 */
int TheSky6IF::getIsConnected( void ) {
    return m_tty_is_connected;
}

