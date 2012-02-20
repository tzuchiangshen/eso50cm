#ifndef _MYRS232_H_
#define _MYRS232_H_

#include <cstring>
#include <iostream>
#include <exception>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>                 //read
#include <termios.h>
#include <fcntl.h>
#include <sys/times.h>
#include <sys/ioctl.h>

#include <errno.h>
#include "LoggerHelper.h"

using namespace std;

class myRS232  {
    public:
       
        myRS232( const char * device, int baudrate);
        ~myRS232( void );

        int setDevName( const char * dev );
        int setBaudRate( int b );

        int open_RS232( void );
        int close_RS232( void );

        int read_RS232( char * str, int max );
        int write_RS232( const char * s, int l );

        void flush_RS232();
        int status_RS232();
        char * getDeviceName( void );
        int getPort( void );
        int getIsConnected( void );

    private:
        struct termios  oldtio;
        struct termios  newtio;

        char            m_device[32];
        int             m_baudrate;
        int             m_port;
        int             m_timeout;
        long            m_microsecs;
        int             m_tty_is_connected;
        LoggerHelper logger;

};

#endif // _MYRS232_H_

