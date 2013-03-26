/**
 *
 *
 */
#include <stdio.h>
#include <sys/uio.h>

#include "myLCU.h"
#include "myUtil.h"

/** HTML source for the start of the process listing page.  */
static char * page_start = 
    "<html>\n"
    "<head>\n"
    " <title>myTelescope</title>\n"
    " <meta http-equiv=\"refresh\"content=\"5\">\n"
    "</head>\n"
    " <body>\n"
    "  <h2 align=\"left\">%s</h2>\n"
    "   <pre>\n";

/** HTML source for the end of the process listing page.  */
static char * page_end = 
    "  </pre>\n"
    " </body>\n"
    "</html>\n";


/**
 *
 */
#define M_SIZE_OF_BUFFER 1024
extern "C" void module_generate( int fd, const char * arguments, myLCU * lcu )
{
    extern int verbose;

    int     quit = true;

    char    is_telescope_configured = 0;
    double  delta_t;
    char    help_flag               = 0;
    char    slew_rate               = 255;
    char    slew_dir                = 255;
    double  degs_per_sec            = 0.;
    int     tics_per_sec            = 0;
    char *  ptr;

    int  retval;
    char html_output_flag = 1;
    if( verbose ) printf( "[handset_command] Hello world!\n" );

    /** Checks for arguments */
    if( * arguments != 0 ) {
        if( verbose ) printf( "[handset_command] has arguments %s\n", arguments );
        char * ptr = NULL;

        if( (ptr = strstr( arguments, "help" )) != NULL ) {
            help_flag = 1;
        }

        if( (ptr = strstr( arguments, "rate=" )) != NULL ) {
            if( sscanf( ptr, "rate=%c", & slew_rate ) == 1 ) {
                if( verbose ) printf( "[handset_command] slew_rate = %c\n", slew_rate );
            }
        }
        if( (ptr = strstr( arguments, "dir=" )) != NULL ) {
            if( sscanf( ptr, "dir=%c", & slew_dir ) == 1 ) {
                if( verbose ) printf( "[handset_command] slew_dir = %c\n", slew_dir );
            }
        }

        if( (ptr = strstr( arguments, "txt" )) != NULL ) {
            html_output_flag = 0;
            if( verbose ) printf( "[handset_command] No HTML output [%s]\n", ptr );
        }
    }
    if( slew_rate == 'S' ) {
        degs_per_sec = 8./15.;         //128x  32['/s]
    } else if( slew_rate == 's' ) {
        degs_per_sec = 1./15.;         //128x  32['/s]
    } else if( slew_rate == 'G' ) {
        degs_per_sec = 1./120.;         //128x  32['/s]
    } else if( slew_rate == 'O' ) {
        degs_per_sec = 1./360.;         //128x  32['/s]
    }
    if( verbose )
        printf( "[handset_command] degs_per_sec = %lf\n", degs_per_sec );

    /** Create a stream corresponding to the client socket file
        descriptor.  */
    if( verbose )
        printf( "[handset_command] My file descriptor is %d\n", fd );

    lcu->waitSemaphore();
    is_telescope_configured = lcu->telescope->getIsConfigured();
    lcu->postSemaphore();

    char    buffer[M_SIZE_OF_BUFFER];
    int     buf_len = 0;
    char    read_buffer[M_SIZE_OF_BUFFER];;
    int     bytes_read = 0;

    if( html_output_flag ) {
        buf_len += sprintf( buffer, page_start, "Handset command  (handset_command)" );
    } else {
        buf_len += sprintf( buffer, "Handset command (handset_command)\n\n" );
    }

    if( is_telescope_configured == 0 ) {
        sprintf( & buffer[buf_len], "Information: Please, config telescope first!\n" );
        buf_len = strlen( buffer );
        sprintf( & buffer[buf_len], "--------------------------------------------------\n" );
        buf_len = strlen( buffer );
        strftime( & buffer[buf_len], 256, "Page generated at: |%Y-%m-%d|%T|\n", lcu->getLCU_Time() );
        buf_len = strlen( buffer );
        //sprintf( & buffer[buf_len], "\r\n\r\n" );
        /* Write buffer to the file descriptor */
        //write( fd, buffer, buf_len );
        //if( verbose ) printf( "[handset_command] Telescope not configured... Good bye!\n" );
        //return;
    } else {
        do {
            sprintf( & buffer[buf_len], "Start slewing to dir=%c at rate=%c\n", slew_dir, slew_rate );
            buf_len = strlen( buffer );
            lcu->waitSemaphore();
            {
                /** Delta T */
                delta_t = lcu->telescope->getDeltaT();

                double tmp;
                if( slew_dir == 'N' ) {
                    tmp = -(lcu->telescope->delta->Motor->getTicsPerRev() *
                            lcu->telescope->delta->Motor->getEncoderToAxis_Reduction() *
                            degs_per_sec / 360.);
                    tics_per_sec = (int) tmp;
                    sprintf( & buffer[buf_len], "N... tics_per_sec = %d\n", tics_per_sec );
                    buf_len = strlen( buffer );
                    lcu->telescope->delta->Motor->setDeviceMemory( 6, & tics_per_sec, 0  );
                } else if( slew_dir == 'S' ) {
                    tmp =  (lcu->telescope->delta->Motor->getTicsPerRev() *
                            lcu->telescope->delta->Motor->getEncoderToAxis_Reduction() *
                            degs_per_sec / 360.);
                    tics_per_sec = (int) tmp;
                    sprintf( & buffer[buf_len], "N... tics_per_sec = %d\n", tics_per_sec );
                    buf_len = strlen( buffer );
                    lcu->telescope->delta->Motor->setDeviceMemory( 6, & tics_per_sec, 0  );
                } else if( slew_dir == 'E' ) {
                    tmp = -(lcu->telescope->alpha->Motor->getTicsPerRev() *
                            lcu->telescope->alpha->Motor->getEncoderToAxis_Reduction() *
                            degs_per_sec / 360.);
                    tics_per_sec = (int) tmp;
                    sprintf( & buffer[buf_len], "N... tics_per_sec = %d\n", tics_per_sec );
                    buf_len = strlen( buffer );
                    lcu->telescope->alpha->Motor->setDeviceMemory( 6, & tics_per_sec, 0  );
                } else if( slew_dir == 'W' ) {
                    tmp =  (lcu->telescope->alpha->Motor->getTicsPerRev() *
                            lcu->telescope->alpha->Motor->getEncoderToAxis_Reduction() *
                            degs_per_sec / 360.);
                    tics_per_sec = (int) tmp;
                    sprintf( & buffer[buf_len], "N... tics_per_sec = %d\n", tics_per_sec );
                    buf_len = strlen( buffer );
                    lcu->telescope->alpha->Motor->setDeviceMemory( 6, & tics_per_sec, 0  );
                }
                sprintf( & buffer[buf_len], "tics_per_sec = %d\n", tics_per_sec );
                buf_len = strlen( buffer );

            }
            lcu->postSemaphore();

            sprintf( & buffer[buf_len], "\r\n\r\n" );
            buf_len = strlen( buffer );

            write( fd, buffer, buf_len );
            buf_len = 0;

            int num_timouts_toquit = 60;
            bytes_read = 0;
            do {
                int retval;
                fd_set rfds;
                struct timeval tv;

                FD_ZERO( & rfds );
                FD_SET( fd, & rfds );
                tv.tv_sec = 1;
                tv.tv_usec= 0;
                retval = select( fd + 1, & rfds, NULL, NULL, & tv );
                if( retval < 0 ) {
                    perror( "[handset_command] select" );
                    if( verbose )
                        printf( "[handset_command] select ERROR!\n" );
                    num_timouts_toquit = 0;
                    quit = true;
                } else if( retval == 0 ) {
                    num_timouts_toquit --;
                    quit = true;
                    if( verbose )
                        printf( "[handset_command] Timeout %d...\n", 60 - num_timouts_toquit );
                } else {
                    bytes_read += read( fd, & read_buffer[bytes_read], M_SIZE_OF_BUFFER - bytes_read - 1 );
                    read_buffer[bytes_read] = 0;
                    if( bytes_read > 0 ) {
                        //if( verbose )
                        //    printf( "[handset_command] Received %d bytes, request: %s\n", buf_len, buffer );
                        if( strstr( buffer, "stop" ) != NULL ){
                            if( verbose )
                                printf( "[handset_command] STOP received\n" );
                            num_timouts_toquit = 0;
                            quit = true;
                        } else {
                            num_timouts_toquit --;
                            quit = true;
                            if( verbose )
                                printf( "[handset_command] unknown commmand\n Timeout %d...\n", 10 - num_timouts_toquit );
                        }

                    } else {
                        num_timouts_toquit = 0;
                        quit = true;
                    }
                }
            } while( num_timouts_toquit );


            char stop_dir;
            if( strstr( read_buffer, "stop" ) != NULL ){
                if( (ptr = strstr( read_buffer, "dir=" )) != NULL ) {
                    if( sscanf( ptr, "dir=%c", & stop_dir ) == 1 ) {
                        if( verbose )
                            printf( "[handset_command] stop_dir = %c\n", stop_dir );
                    }
                    sprintf( & buffer[buf_len], "STOP accepted with dir = %c\n", stop_dir );
                    buf_len = strlen( buffer );
                }
            } else {
                sprintf( & buffer[buf_len], "STOP anyway!\n" );
                buf_len = strlen( buffer );
            }

            sprintf( & buffer[buf_len], "Stop slewing to %c\n", slew_dir );
            buf_len = strlen( buffer );
            lcu->waitSemaphore();
            {
                tics_per_sec = 0;
                if( slew_dir == 'N' ) {
                    sprintf( & buffer[buf_len], "N... tics_per_sec = %d\n", tics_per_sec );
                    buf_len = strlen( buffer );
                    lcu->telescope->delta->Motor->setDeviceMemory( 6, & tics_per_sec, 0  );
                } else if( slew_dir == 'S' ) {
                    sprintf( & buffer[buf_len], "S... tics_per_sec = %d\n", tics_per_sec );
                    buf_len = strlen( buffer );
                    lcu->telescope->delta->Motor->setDeviceMemory( 6, & tics_per_sec, 0  );
                } else if( slew_dir == 'E' ) {
                    sprintf( & buffer[buf_len], "E... tics_per_sec = %d\n", tics_per_sec );
                    buf_len = strlen( buffer );
                    lcu->telescope->alpha->Motor->setDeviceMemory( 6, & tics_per_sec, 0  );
                } else if( slew_dir == 'W' ) {
                    sprintf( & buffer[buf_len], "W... tics_per_sec = %d\n", tics_per_sec );
                    buf_len = strlen( buffer );
                    lcu->telescope->alpha->Motor->setDeviceMemory( 6, & tics_per_sec, 0  );
                }
                sprintf( & buffer[buf_len], "tics_per_sec = %d\n", tics_per_sec );
                buf_len = strlen( buffer );

                /** Delta T */
                delta_t = lcu->telescope->getDeltaT();
                sprintf( & buffer[buf_len], "Time elapsed: %lf sec\n", delta_t );
                buf_len = strlen( buffer );

            }
            lcu->postSemaphore();
            /** LCU Local Time */
            sprintf( & buffer[buf_len], "--------------------------------------------------\n" );
            buf_len = strlen( buffer );
            strftime( & buffer[buf_len], 256, "Page generated at: |%Y-%m-%d|%T|\n", lcu->getLCU_Time() );
            buf_len = strlen( buffer );

            sprintf( & buffer[buf_len], "\r\n\r\n" );
            buf_len = strlen( buffer );

            /** Write buffer to the file descriptor */
            //if( verbose) printf( "[handset_command] Writing to fd %d bytes.\n", buf_len );
            write( fd, buffer, buf_len );
            buf_len = 0;

        } while( ! quit );
    }

    if( html_output_flag ) {
        buf_len += sprintf( & buffer[buf_len], page_end );
    } else {
        buf_len += sprintf( & buffer[buf_len], "\r\n\r\n" );
    }

    retval = write( fd, buffer, buf_len );
    if( retval > 0 ) {
        if( verbose ) printf( "[handset_command] %d Bytes written in fd=%d \n", retval, fd );
    } else {
        perror( "[handset_command] write" );
        if( verbose ) printf( "[handset_command] write ERROR\n" );
    }

    if( verbose) printf( "[handset_command] Good bye!\n" );
}


extern "C" void module_generate_txt( int fd, const char * arguments, myLCU * lcu )
{
    extern int verbose;

    int     quit = true;

    char    is_telescope_configured = 0;
    double  delta_t;
    char    help_flag               = 0;
    char    slew_rate               = 255;
    char    slew_dir                = 255;
    double  degs_per_sec            = 0.;
    int     tics_per_sec            = 0;
    char *  ptr;

    int  retval;
    //char html_output_flag = 1;
    if( verbose ) printf( "[handset_command] Hello world!\n" );

    /** Checks for arguments */
    if( * arguments != 0 ) {
        if( verbose ) printf( "[handset_command] has arguments %s\n", arguments );
        char * ptr = NULL;

        if( (ptr = strstr( arguments, "help" )) != NULL ) {
            help_flag = 1;
        }

        if( (ptr = strstr( arguments, "rate=" )) != NULL ) {
            if( sscanf( ptr, "rate=%c", & slew_rate ) == 1 ) {
                if( verbose ) printf( "[handset_command] slew_rate = %c\n", slew_rate );
            }
        }
        if( (ptr = strstr( arguments, "dir=" )) != NULL ) {
            if( sscanf( ptr, "dir=%c", & slew_dir ) == 1 ) {
                if( verbose ) printf( "[handset_command] slew_dir = %c\n", slew_dir );
            }
        }

        //if( (ptr = strstr( arguments, "txt" )) != NULL ) {
        //    html_output_flag = 0;
        //    if( verbose ) printf( "[handset_command] No HTML output [%s]\n", ptr );
        //}
    }
    if( slew_rate == 'S' ) {
        degs_per_sec = 8./15.;          // 32.0[arcmin/s]
    } else if( slew_rate == 's' ) {
        degs_per_sec = 1./120.;         // 30.0[arcsec/s]
    } else if( slew_rate == 'G' ) {
        degs_per_sec = 1./480.;         //  7.5[arcsec/s]
    } else if( slew_rate == 'O' ) {
        degs_per_sec = 1./720.;         //  5.0[arcsec/s]
    }
    if( verbose )
        printf( "[handset_command] degs_per_sec = %lf\n", degs_per_sec );

    /** Create a stream corresponding to the client socket file
    descriptor.  */
    if( verbose )
        printf( "[handset_command] My file descriptor is %d\n", fd );

    lcu->waitSemaphore();
    is_telescope_configured = lcu->telescope->getIsConfigured();
    lcu->postSemaphore();

    char    buffer[M_SIZE_OF_BUFFER];
    int     buf_len = 0;
    char    read_buffer[M_SIZE_OF_BUFFER];;
    int     bytes_read = 0;

    //if( html_output_flag ) {
    //    buf_len += sprintf( buffer, page_start, "Handset command  (handset_command)" );
    //} else {
        buf_len += sprintf( buffer, "Handset command (handset_command)\n\n" );
    //}

    if( is_telescope_configured == 0 ) {
        sprintf( & buffer[buf_len], "Information: Please, config telescope first!\n" );
        buf_len = strlen( buffer );
        sprintf( & buffer[buf_len], "--------------------------------------------------\n" );
        buf_len = strlen( buffer );
        strftime( & buffer[buf_len], 256, "Page generated at: |%Y-%m-%d|%T|\n", lcu->getLCU_Time() );
        buf_len = strlen( buffer );
        //sprintf( & buffer[buf_len], "\r\n\r\n" );
        /* Write buffer to the file descriptor */
        //write( fd, buffer, buf_len );
        //if( verbose ) printf( "[handset_command] Telescope not configured... Good bye!\n" );
        //return;
    } else {
        do {
            sprintf( & buffer[buf_len], "Start slewing to dir=%c at rate=%c\n", slew_dir, slew_rate );
            buf_len = strlen( buffer );
            lcu->waitSemaphore();
            {
                /** Delta T */
                delta_t = lcu->telescope->getDeltaT();

                double tmp;
                if( slew_dir == 'N' ) {
                    tmp = -(lcu->telescope->delta->Motor->getTicsPerRev() *
                            lcu->telescope->delta->Motor->getEncoderToAxis_Reduction() *
                            degs_per_sec / 360.);
                    tics_per_sec = (int) tmp;
                    sprintf( & buffer[buf_len], "N... tics_per_sec = %d\n", tics_per_sec );
                    buf_len = strlen( buffer );
                    lcu->telescope->delta->Motor->setDeviceMemory( 6, & tics_per_sec, 0  );
                } else if( slew_dir == 'S' ) {
                    tmp =  (lcu->telescope->delta->Motor->getTicsPerRev() *
                            lcu->telescope->delta->Motor->getEncoderToAxis_Reduction() *
                            degs_per_sec / 360.);
                    tics_per_sec = (int) tmp;
                    sprintf( & buffer[buf_len], "N... tics_per_sec = %d\n", tics_per_sec );
                    buf_len = strlen( buffer );
                    lcu->telescope->delta->Motor->setDeviceMemory( 6, & tics_per_sec, 0  );
                } else if( slew_dir == 'E' ) {
                    tmp = -(lcu->telescope->alpha->Motor->getTicsPerRev() *
                            lcu->telescope->alpha->Motor->getEncoderToAxis_Reduction() *
                            degs_per_sec / 360.);
                    tics_per_sec = (int) tmp;
                    sprintf( & buffer[buf_len], "N... tics_per_sec = %d\n", tics_per_sec );
                    buf_len = strlen( buffer );
                    lcu->telescope->alpha->Motor->setDeviceMemory( 6, & tics_per_sec, 0  );
                } else if( slew_dir == 'W' ) {
                    tmp =  (lcu->telescope->alpha->Motor->getTicsPerRev() *
                            lcu->telescope->alpha->Motor->getEncoderToAxis_Reduction() *
                            degs_per_sec / 360.);
                    tics_per_sec = (int) tmp;
                    sprintf( & buffer[buf_len], "N... tics_per_sec = %d\n", tics_per_sec );
                    buf_len = strlen( buffer );
                    lcu->telescope->alpha->Motor->setDeviceMemory( 6, & tics_per_sec, 0  );
                }
                sprintf( & buffer[buf_len], "tics_per_sec = %d\n", tics_per_sec );
                buf_len = strlen( buffer );

            }
            lcu->postSemaphore();

            sprintf( & buffer[buf_len], "\r\n\r\n" );
            buf_len = strlen( buffer );

            write( fd, buffer, buf_len );
            buf_len = 0;
            if( verbose )
                printf( "[handset_command] OK. waiting for stop\n" );
            
            int num_timouts_toquit = 10;
            bytes_read = 0;
            do {
                int retval;
                fd_set rfds;
                struct timeval tv;

                FD_ZERO( & rfds );
                FD_SET( fd, & rfds );
                tv.tv_sec = 1;
                tv.tv_usec= 0;
                retval = select( fd + 1, & rfds, NULL, NULL, & tv );
                if( retval < 0 ) {
                    perror( "[handset_command] select" );
                    if( verbose )
                        printf( "[handset_command] select ERROR!\n" );
                    num_timouts_toquit = 0;
                    quit = true;
                } else if( retval == 0 ) {
                    num_timouts_toquit --;
                    quit = true;
                    if( verbose )
                        printf( "[handset_command] Timeout %d...\n", 10 - num_timouts_toquit );
                } else {
                    bytes_read += read( fd, & read_buffer[bytes_read], M_SIZE_OF_BUFFER - bytes_read - 1 );
                    read_buffer[bytes_read] = 0;
                    if( bytes_read > 0 ) {
                        //if( verbose )
                        //    printf( "[handset_command] Received %d bytes, request: %s\n", buf_len, buffer );
                        if( strstr( read_buffer, "stop" ) != NULL ){
                            if( verbose )
                                printf( "[handset_command] STOP received\n" );
                            num_timouts_toquit = 0;
                            quit = true;
                        } else {
                            num_timouts_toquit --;
                            quit = true;
                            if( verbose )
                                printf( "[handset_command] unknown commmand %s\n Timeout %d...\n", buffer, 10 - num_timouts_toquit );
                        }

                    } else {
                        num_timouts_toquit = 0;
                        quit = true;
                    }
                }
            } while( num_timouts_toquit );


            char stop_dir;
            if( strstr( read_buffer, "stop" ) != NULL ){
                if( (ptr = strstr( read_buffer, "dir=" )) != NULL ) {
                    if( sscanf( ptr, "dir=%c", & stop_dir ) == 1 ) {
                        if( verbose )
                            printf( "[handset_command] stop_dir = %c\n", stop_dir );
                    }
                    sprintf( & buffer[buf_len], "STOP accepted with dir = %c\n", stop_dir );
                    buf_len = strlen( buffer );
                }
            } else {
                sprintf( & buffer[buf_len], "STOP anyway!\n" );
                buf_len = strlen( buffer );
            }

            sprintf( & buffer[buf_len], "Stop slewing to %c\n", slew_dir );
            buf_len = strlen( buffer );
            lcu->waitSemaphore();
            {
                tics_per_sec = 0;
                if( slew_dir == 'N' ) {
                    sprintf( & buffer[buf_len], "N... tics_per_sec = %d\n", tics_per_sec );
                    buf_len = strlen( buffer );
                    lcu->telescope->delta->Motor->setDeviceMemory( 6, & tics_per_sec, 0  );
                } else if( slew_dir == 'S' ) {
                    sprintf( & buffer[buf_len], "S... tics_per_sec = %d\n", tics_per_sec );
                    buf_len = strlen( buffer );
                    lcu->telescope->delta->Motor->setDeviceMemory( 6, & tics_per_sec, 0  );
                } else if( slew_dir == 'E' ) {
                    sprintf( & buffer[buf_len], "E... tics_per_sec = %d\n", tics_per_sec );
                    buf_len = strlen( buffer );
                    lcu->telescope->alpha->Motor->setDeviceMemory( 6, & tics_per_sec, 0  );
                } else if( slew_dir == 'W' ) {
                    sprintf( & buffer[buf_len], "W... tics_per_sec = %d\n", tics_per_sec );
                    buf_len = strlen( buffer );
                    lcu->telescope->alpha->Motor->setDeviceMemory( 6, & tics_per_sec, 0  );
                }
                sprintf( & buffer[buf_len], "tics_per_sec = %d\n", tics_per_sec );
                buf_len = strlen( buffer );

                /** Delta T */
                delta_t = lcu->telescope->getDeltaT();
                sprintf( & buffer[buf_len], "Time elapsed: %lf sec\n", delta_t );
                buf_len = strlen( buffer );

            }
            lcu->postSemaphore();
            /** LCU Local Time */
            sprintf( & buffer[buf_len], "--------------------------------------------------\n" );
            buf_len = strlen( buffer );
            strftime( & buffer[buf_len], 256, "Page generated at: |%Y-%m-%d|%T|\n", lcu->getLCU_Time() );
            buf_len = strlen( buffer );

            sprintf( & buffer[buf_len], "\r\n\r\n" );
            buf_len = strlen( buffer );

            /** Write buffer to the file descriptor */
            //if( verbose) printf( "[handset_command] Writing to fd %d bytes.\n", buf_len );
            write( fd, buffer, buf_len );
            buf_len = 0;

        } while( ! quit );
    }

    //if( html_output_flag ) {
    //    buf_len += sprintf( & buffer[buf_len], page_end );
    //} else {
        buf_len += sprintf( & buffer[buf_len], "\r\n\r\n" );
    //}

    retval = write( fd, buffer, buf_len );
    if( retval > 0 ) {
        if( verbose ) printf( "[handset_command] %d Bytes written in fd=%d \n", retval, fd );
    } else {
        perror( "[handset_command] write" );
        if( verbose ) printf( "[handset_command] write ERROR\n" );
    }

    buf_len = 0;
    sprintf( & buffer[buf_len], "\r\n\r\n" );
    write( fd, buffer, strlen( buffer ) );
    sleep(1);
    buf_len = 0;
    sprintf( & buffer[buf_len], "Good bye!\r\n\r\n" );
    write( fd, buffer, strlen( buffer ) );

    if( verbose) printf( "[handset_command] Good bye!\n" );
}


