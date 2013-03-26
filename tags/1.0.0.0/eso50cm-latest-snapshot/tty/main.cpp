/** ***************************************
 *  2010 03 25  17:06
 ******************************************/

#include <stdio.h>
#include <unistd.h>     //sleep

#include <string.h> 

#include <sys/select.h> //select

#include <errno.h>

#include <stdlib.h>

#include <myRS232.h>

int stream_status( int m_port ) {
    fd_set fds;
    struct timeval tv;
    int retval;

    FD_ZERO( & fds );
    FD_SET( m_port, & fds );
    tv.tv_sec = 0;
    tv.tv_usec = 100;

    if( ( retval = select( m_port + 1, & fds, NULL, NULL, & tv ) ) < 0  ) {
        printf( "Error on select()\n" );
    }
    return retval;
}

/**
 *
 */
int main( int args, char * argv[] ) {
    int run;
    int retval;
    char c;
    int fd_stdin;
    char buffer[128];
    char tty_buffer[256];
    int baudrate;

    fd_stdin = fileno( stdin );

    myRS232 * tty;
    baudrate = atoi( argv[2] );
    printf( "[tty] Opening %s at %d[bps]\n", argv[1], baudrate );
    tty = new myRS232( argv[1], baudrate );
    int isConnected= tty->open_RS232();
    
    
    c= 0;
    run= true;
    int new_message_flag = false;
    int rx;
    while( run )  {
        if( isConnected ){
            if( (rx = tty->status_RS232()) > 0 ) {
                //printf( "[tty] status %d\n", rx );
                rx = tty->read_RS232( tty_buffer, 256 );
                //for( int k = 0; k < 10 && rx < 0; k ++ ) {
                //    rx = tty->read_RS232( tty_buffer, 256 );
                //    sleep( 1 );
                //}
                if( rx > 0 ) {
                    //tty_buffer[rx] = 0;
                    for( int k = 0; k < rx; k ++ ) {
                        if( 31 < tty_buffer[k] && tty_buffer[k] < 128 ) {
                            if( tty_buffer[k] == '#' ){
                                printf( "%c\n", tty_buffer[k] );
                            } else {
                                printf( "%c", tty_buffer[k] );
                            }
                        } else {
                            printf( "[%3u]", (unsigned char) tty_buffer[k] );
                        }
                    }
                } else {
                    printf( "[tty] read ERROR" );
                }
            }
        } else {
            printf( "[tty] Connection closed, nothing to do, exiting\n" );
            run = false;
        }
        if( new_message_flag ) {
            new_message_flag = false;
            if( strcmp( buffer, "exit" ) == 0 ) {            
                printf( "[tty] %s\n", buffer );
                run = false;
            } else {
                if( isConnected ) {
                    tty->write_RS232( buffer, strlen( buffer ) );
                }
            }
        } else {
            if( ( retval = stream_status( fd_stdin ) ) > 0 ) {
                memset( buffer, 0 , 128 );
                retval = read( fd_stdin, buffer, 128 );
                buffer[retval-1] = 0;  //enter = 0
                fflush( stdin );
                new_message_flag = true;
            }
        }
    }
    if( isConnected ) {
        tty->close_RS232();
    }
    delete tty;
    
    printf( "[tty] Good bye!\n" );

    return 1;
}

