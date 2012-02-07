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
    char ttyS0_buffer[256];
    char ttyS1_buffer[256];
    int  ttyS0_newline = false;
    int  ttyS1_newline = false;
    int  ttyS0_isolate = false;
    int  ttyS1_isolate = false;
    int  baudrate;
    fd_stdin = fileno( stdin );

    myRS232 * ttyS0;
    myRS232 * ttyS1;
    baudrate = atoi( argv[3] );
    printf( "[ttspy] Opening %s and %s at %d[Bps]\n", argv[1], argv[2], baudrate );
    
    ttyS0 = new myRS232( argv[1], baudrate );
    int is_ttyS0_connected= ttyS0->open_RS232();
    ttyS1 = new myRS232( argv[2], baudrate );
    int is_ttyS1_connected= ttyS1->open_RS232();
    
    FILE * fp;
    if( (fp = fopen( argv[3], "w" )) == NULL ) {
        perror( "[ttspy] fopen" );
        printf( "[ttspy] fopen ERROR" );
    }
    
    
    c= 0;
    run= true;
    int new_message_flag = false;
    int rx;
    int tx;
    while( run )  {
        if( is_ttyS0_connected && is_ttyS1_connected ){
            if( (rx = ttyS0->status_RS232()) > 0 ) {
                if( (rx = ttyS0->read_RS232( ttyS0_buffer, 256 )) > 0 ) {
                    if( ! ttyS0_newline ) {
                        printf( "\n[ttyspy::0->1] " );
                        fprintf( fp, "\n[ttyspy::0->1] " );
                        ttyS0_newline = true;
                    }
                    for( int k = 0; k < rx; k ++ ) {
                        if( (31 < ttyS0_buffer[k]) && (ttyS0_buffer[k] < 127) ) {
                            printf( "%c", ttyS0_buffer[k] );
                            fprintf( fp, "%c", ttyS0_buffer[k] );
                            if( ttyS0_buffer[k] == '#' ) {
                                printf( "\n[ttyspy::0->1] " );
                                fprintf( fp, "\n[ttyspy::0->1] " );
                            }
                        } else if( ttyS0_buffer[k] == 223 ) {
                            printf( "*" );
                            fprintf( fp, "*" );
                        } else {
                            printf( "[%d]", (int) ttyS0_buffer[k] );
                            fprintf( fp, "[%d]", (int) ttyS0_buffer[k] );
                        }
                    }
                } else {
                    printf( "[ttspy] read (%s) ERROR\n", argv[1] );
                }
                if( ! ttyS1_isolate ) {
                    tx = ttyS1->write_RS232( ttyS0_buffer, rx );
                }
            }
            if( (rx = ttyS1->status_RS232()) > 0 ) {
                if( (rx = ttyS1->read_RS232( ttyS1_buffer, 256 )) > 0 ) {
                    for( int k = 0; k < rx; k ++ ) {
                        if( ! ttyS1_newline ) {
                            printf( "\n[ttyspy::1->0] " );
                            fprintf( fp, "\n[ttyspy::1->0] " );
                            ttyS1_newline = true;
                        }
                        if( (31 < ttyS1_buffer[k]) && (ttyS1_buffer[k] < 127) ) {
                            printf( "%c", ttyS1_buffer[k] );
                            fprintf( fp, "%c", ttyS1_buffer[k] );
                            if( ttyS1_buffer[k] == '#' ) {
                                printf( "\n[ttyspy::1->0] " );
                                fprintf( fp, "\n[ttyspy::1->0] " );
                            }
                        } else if( ttyS1_buffer[k] == 223 ) {
                            printf( "*" );
                            fprintf( fp, "*" );
                        } else {
                            printf( "[%d]", (int) ttyS1_buffer[k] );
                            fprintf( fp, "[%d]", (int) ttyS1_buffer[k] );
                        }
                    }
                } else {
                    printf( "[ttspy] read (%s) ERROR\n", argv[2] );
                }
                if( ! ttyS0_isolate ) {
                    tx = ttyS0->write_RS232( ttyS1_buffer, rx );
                }
            }
        } else {
            printf( "[ttspy] Connection closed, nothing to do, exiting\n" );
            run = false;
        }
        if( new_message_flag ) {
            new_message_flag = false;
            if( strcmp( buffer, "exit" ) == 0 ) {            
                printf( "[ttspy] %sing...\n", buffer );
                run = false;
            } else if( strcmp( buffer, "isolate S0" ) == 0 ) {            
                ttyS0_isolate = true;
                printf( "[ttspy] ttyS0_isolate = %d \n", ttyS0_isolate );
            } else if( strcmp( buffer, "unisolate S0" ) == 0 ) {            
                ttyS0_isolate = false;
                printf( "[ttspy] ttyS0_isolate = %d \n", ttyS0_isolate );
            } else if( strcmp( buffer, "isolate S1" ) == 0 ) {            
                ttyS1_isolate = true;
                printf( "[ttspy] ttyS1_isolate = %d \n", ttyS1_isolate );
            } else if( strcmp( buffer, "unisolate S1" ) == 0 ) {            
                ttyS1_isolate = false;
                printf( "[ttspy] ttyS1_isolate = %d \n", ttyS1_isolate );
            } else {
                if( is_ttyS1_connected ) {
                    printf( "[ttspy::stdin->1] %s\n", buffer );
                    fprintf( fp, "[ttspy::stdin->1] %s\n", buffer );
                    ttyS1->write_RS232( buffer, strlen( buffer ) );
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
    if( is_ttyS0_connected ) {
        ttyS0->close_RS232();
    }
    delete ttyS0;
    if( is_ttyS1_connected ) {
        ttyS1->close_RS232();
    }
    delete ttyS1;
    
    fclose( fp );
    
    printf( "[ttspy] Thank you for using ttspy...\nGood bye!\n" );

    return 1;
}

