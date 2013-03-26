/** ***************************************
 *  2010 03 25  17:06
 ******************************************/
#include <stdio.h>
#include <unistd.h>     //sleep
#include <string.h> 
#include <sys/select.h> //select
#include <errno.h>
#include <stdlib.h>

#include "myClientTCP.h"
#include "myNCurses.h"
#include "myRS232.h"

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
    char buffer[256];
    int rx_bytes;
    char msg[256];
    int msg_len;
    char line[128];
    //int new_time_data = false;

    //fd_stdin = fileno( stdin );

    //myClientSocket  * sck;
    myClientTCP     * tcp;
    
    tcp = new myClientTCP( argv[1], argv[2] );
    tcp->connectSocket();
    
    
    
    //myTimer * timer;
    //printf( "[client] Timer...\n" );
    //timer = new myTimer( -33.15 );
    //if( timer->start() == 0 ) {
    //    printf( "[client] OK\n" );
    //} else {
    //    printf( "[cliet] ERROR\n" );
    //}
    
    //thesky
    
    myRS232 * thesky;
    int thesky_is_connected = false;
    int thesky_command_bytes = 0;
    char thesky_command[32];
    char thesky_retline[32];
    int thesky_command_flag = false;
    
    thesky = new myRS232( "/dev/ttyS1", 9600 );
    if( thesky->open_RS232() ) {
        printf( "Ready TheSky6 port\n" );
    } else {
        perror( "TheSky6" );
        printf( "Unable to open TheSky6 port\n" );
    }
    
    int ncurese_flag = false;
    
    if( strcmp( argv[3], "ncurses" )== 0 ) {
        ncurese_flag = true;
    }
    
    myNcurses * window;
    if( ncurese_flag ) {

        window = new myNcurses();
        
        window->DibujaPanelHora(); 
        window->DibujaPanelAlpha();
        window->DibujaPanelDelta();
        window->DibujaPanelPosicion();
        window->showPanel( 0 );
        window->showPanel( 1 );
        window->showPanel( 2 );
        window->hidePanel( 3 );
        window->hidePanel( 4 );
        window->showPanel( 5 );
        window->pflush();
    } else {
        fd_stdin = fileno( stdin );
    }
    c= 0;
    run= true;
    int new_message_flag = false;

    //int k = 0;
    msg_len = 0;
    while( run  )  {
        if( tcp->getIsConnected() ) {
            if( tcp->socketStatus() > 0 ) {
                rx_bytes = tcp->readSocket( buffer, 256 );
                if( rx_bytes > 0 ) {
                    for( int i = 0; i < rx_bytes; i ++ ) {
                        if( msg_len == 0 ) {
                            //msg[msg_len] = buffer[i];
                            //msg_len ++;
                            if( buffer[i] == '?' || buffer[i] == '!' ) {
                                msg[msg_len] = buffer[i];
                                msg_len ++;
                            }
                        } else {
                            msg[msg_len] = buffer[i];
                            msg_len ++;
                            if( buffer[i] == '#' ) {
                                msg[msg_len] = 0;
                                //printf( "%s\n", msg );
                                if( ncurese_flag ) {
                                    if( msg[1] == 'T' ) {
                                        //printf( "%s\n", msg );
                                        window->ActualizaPanelHora( msg );
                                    } else if( msg[1] == 'P' ) {
                                        //printf( "%s\n", msg );
                                        //printf( "%s\r", msg );
                                        window->ActualizaPanelPosicion( msg );
                                    } else if( msg[1] == 'E' ) {
                                        //printf( "%s\r", msg );
                                        window->ActualizaPanelAlpha( msg );
                                    } else if( msg[1] == ' ' ) {
                                        
                                        if( thesky_command_flag ) {
                                            thesky_command_flag = false;
                                            
                                            int i;
                                            for( i = 2; i <= msg_len; i ++ ){
                                                if( msg[i] == '*' ) {
                                                    thesky_retline[i-2] = 223;
                                                } else {
                                                    thesky_retline[i-2] = msg[i];
                                                }
                                                
                                            }
                                            rx_bytes = thesky->write_RS232( thesky_retline, msg_len - 2 );
                                        //} else {
                                        //    window->ActualizaPanelInput( & msg[2] );
                                        }
                                    }
                                    window->pflush();
                                } else {
                                    if( msg[1] == 'T' ) {
                                        //printf( "tcp rx> %s\n" , & msg[2] );
                                    } else if( msg[1] == 'P' ) {
                                        //printf( "tcp rx> %s\n" , & msg[2] );
                                    } else if( msg[1] == 'E' ) {
                                        //printf( "tcp rx> %s\n" , & msg[2] );
                                    } else if( msg[1] == ' ' ) {
                                        
                                        if( thesky_command_flag ) {
                                            thesky_command_flag = false;

                                            int i;
                                            for( i = 2; i <= msg_len; i ++ ){
                                                if( msg[i] == '*' ) {
                                                    thesky_retline[i-2] = 223;
                                                } else {
                                                    thesky_retline[i-2] = msg[i];
                                                }
                                                
                                            }
                                            if( ! ncurese_flag ) {  
                                                printf( "ESO50cm>%s<\n", thesky_retline );
                                            }
                                            rx_bytes = thesky->write_RS232( thesky_retline, strlen( thesky_retline )  );
                                        } else {
                                            printf( "tcp rx> %s\n" , & msg[2] );
                                        }
                                        
                                    }
                                }
                               msg_len = 0;
                            }
                        }
                    }
                }
            }
        } else {
            //printf( "[client] Connection closed, nothing to do, exiting\n" );
            run = false;
        }
        
        //*********************************************************************
        // TheSky6
        if( thesky->getIsConnected() ) {
            if( ( retval = thesky->status_RS232() ) > 0 ) {
                memset( line, 0, 128 );
                rx_bytes = thesky->read_RS232( line, 128 );
                for( int i = 0; i < rx_bytes; i ++ ) {
                    if( thesky_command_bytes == 0 ) {
                        if( line[i] == ':' ) {
                            thesky_command[thesky_command_bytes] = line[i];
                            thesky_command_bytes ++;
                        }
                    } else {
                        thesky_command[thesky_command_bytes] = line[i];
                        thesky_command_bytes ++;
                        if( line[i] == '#' ) {
                            thesky_command[thesky_command_bytes] = 0;
                            if( ! ncurese_flag ) {
                                printf( "TheSky6> %s\n", thesky_command );
                            }
                            if( strcmp( thesky_command, ":MS#" ) != 0 && strcmp( thesky_command, ":Q#" ) != 0 ) {
                                tcp->writeSocket( thesky_command, thesky_command_bytes );
                            } else {
                                thesky->write_RS232( "0#", 2 );
                            }
                            thesky_command_flag = true;
                            thesky_command_bytes = 0;
                        }
                    }
                }
            }
        }        
        
        //*********************************************************************
        // Keyboard
        if( new_message_flag ) {
            new_message_flag = false;
            if( tcp->getIsConnected() ) {
                tcp->writeSocket( line, strlen( line ) );
            }
            if( strcmp( line, ":ZX#" ) == 0 ) {            
                run = false;
            } else if( strcmp( line, "exit" ) == 0 ) {            
                run = false;
            }
        } else {
            if( ncurese_flag ) {
                char ch;
                if( ( ch = getch() ) != ERR ){
                    if( window->ProcesaInputTeclado( ch, line ) ) {
                        if( strcmp( line, "exit" ) == 0 ) {            
                            run = false;
                        } else if( strcmp( line, "top T") == 0 ) { 
                            window->showPanel( 0 );
                            window->showPanel( 1 );
                            window->showPanel( 2 );
                            window->hidePanel( 3 );
                            window->hidePanel( 4 );
                            window->hidePanel( 5 );
                            window->pflush();
                        } else if( strcmp( line, "top A") == 0 ) { 
                            window->showPanel( 0 );
                            window->showPanel( 1 );
                            window->showPanel( 2 );
                            window->showPanel( 3 );
                            window->hidePanel( 4 );
                            window->hidePanel( 5 );
                            window->pflush();
                        } else if( strcmp( line, "top D") == 0 ) { 
                            window->showPanel( 0 );
                            window->showPanel( 1 );
                            window->showPanel( 2 );
                            window->hidePanel( 3 );
                            window->showPanel( 4 );
                            window->hidePanel( 5 );
                            window->pflush();
                        } else if( strcmp( line, "top P") == 0 ) { 
                            window->showPanel( 0 );
                            window->showPanel( 1 );
                            window->showPanel( 2 );
                            window->hidePanel( 3 );
                            window->hidePanel( 4 );
                            window->showPanel( 5 );
                            window->pflush();
                        } else {
                            new_message_flag = true;
                        }
                    }
                }
            } else {
                if( ( retval = stream_status( fd_stdin ) ) > 0 ) {
                    memset( buffer, 0 , 128 );
                    retval = read( fd_stdin, buffer, 128 );
                    buffer[retval-1] = 0;  //enter = 0
                    fflush( stdin );
                    //printf( "HOLA\n" );
                    strcpy( line, buffer );
                    new_message_flag = true;
                }
            } // end Keyboard
        }
        //if( timer->getIsTimerTic() ) {
        //    if( tmr % 4 == 0 ) {
        //        //tcp->writeSocket( ":GL#", 4 );
        //    }
        //    tmr ++;
        //}
    }
    if( ncurese_flag ) {
        delete window;
        ncurese_flag = false;
    }
    
    if( tcp->getIsConnected() ) {
        tcp->closeSocket();
    }
    delete tcp;
    
    if( thesky->getIsConnected() ) {
        thesky->close_RS232();
    }
    delete thesky;
    
    //printf( "[client] Deleting timer\n" );
    //delete timer;
    printf( "[client] Good bye!\n" );

    return 1;
}

