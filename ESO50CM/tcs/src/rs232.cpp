#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>                 //read
#include <termios.h>
#include <fcntl.h>
#include <sys/times.h>
#include <sys/ioctl.h>

#include <errno.h>
#include "telescope.h"

/**
 * open_RS232
 */
int open_RS232( const char * m_device, speed_t m_baudrate ) {
    int m_port;
    struct termios  oldtio;
    struct termios  newtio;

    if( ( m_port = open( m_device, O_RDWR | O_NDELAY ) ) < 0 ) {
        logger.logFINE( "rs232::open_RS232 open ERROR\n" );
    } else {
        logger.logFINE( "rs232::open_RS232 open OK\n" );
        //if( ioctl( m_port, TCGETA, & oldtio) < 0 ) {
        if( tcgetattr( m_port, & oldtio) < 0 ) {
            logger.logFINE("rs232::open_RS232 tcgetattr ERROR\n" );
        }
        //if( ioctl( m_port, TCGETA, & newtio) < 0 ) {
        if( tcgetattr( m_port, & newtio) < 0 ) {
    	    logger.logFINE("rs232::open_RS232 tcgetattr ERROR\n" );
        }

        newtio.c_cflag = m_baudrate | CS8 | CLOCAL | CREAD;
        newtio.c_iflag = IGNPAR;
        newtio.c_oflag = 0;
        newtio.c_lflag = 0;       //ICANON;
        tcflush( m_port, TCIFLUSH );
        if( tcsetattr( m_port, TCSANOW,  & newtio ) < 0 ) {
            logger.logFINE( "rs232::open_RS232 tcsetattr ERROR\n" );
        }
    }
    return m_port;
}

/**
 * close_RS232
 */
int close_RS232( int m_port ) {
    int retval;

    if( (retval  = close( m_port )) < 0 ) {
        logger.logFINE( "rs232::close_RS232 close ERROR\n" );
    }

    return retval;
}


/**
 * read_RS232
 */
int read_RS232( int m_port, char * str, int max ) {
    int retval;
 
    if( (retval=read( m_port, str, max ) ) > 0 ) {
        //printf( "[myRS232::read_RS232] read OK (%d)\n", retval );
    } else {
        if( errno == EAGAIN ) {
            logger.logFINE( "rs232::read_RS232 read ERROR: EAGAIN\n" );
        } else {
            logger.logFINE( "rs232::read_RS232 read ERROR\n" );
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
int write_RS232( int m_port, const char * s, int length ) {
    int retval;
 
    if( (retval = write( m_port, s, length ) ) < 0 ) {
        logger.logFINE( "rs232::write_RS232 write ERROR\n" );
    }
    return retval;
}

/**
 * flush_RS232
 */
int flush_RS232( int m_port ) {
    int retval;

    if( ( retval = tcflush(m_port, TCIOFLUSH ) ) == -1 ) {
        logger.logFINE( "rs232::flush_RS232 ioctl ERROR\n" );
    }
    return retval;
}

/**
 * status_RS232
 */
int status_RS232( int m_port, int m_microsecs ) {
    fd_set read_fds;
    struct timeval tv;
    int retval;

    FD_ZERO( & read_fds );
    FD_SET( m_port, & read_fds);
    tv.tv_sec = 0;
    tv.tv_usec = m_microsecs;

    if( ( retval = select( m_port+1, & read_fds, NULL, NULL, & tv) ) < 0  ) {
        logger.logFINE( "rs232::status_RS232 select ERROR\n" );
    }
    return retval;
}

