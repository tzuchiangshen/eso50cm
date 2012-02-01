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
extern "C" void module_generate( int fd, const char * arguments, myLCU * lcu )
{
    extern int verbose;

    int quit = true;
    int count = 10;
    double lst, ra, dec, alt, az;

    char is_telescope_configured = 0;
    double delta_t;

    int  retval;
    char html_output_flag = 1;
    if( verbose ) printf( "[monitor_position] Hello world!\n" );
    /** Checks for arguments */
    if( * arguments != 0 ) {
        if( verbose ) printf( "[monitor_position] Has arguments: %s\n", arguments );
        char * ptr = NULL;

        if( (ptr = strstr( arguments, "count=" )) != NULL ) {
            if( sscanf( ptr, "count=%d", & count ) == 1 ) {
                if( verbose ) printf( "[monitor_position] count = %d\n", count );
            }
        }

        if( (ptr = strstr( arguments, "txt" )) != NULL ) {
            html_output_flag = 0;
            if( verbose ) printf( "[monitor_position] No HTML output [%s]\n", ptr );
        }
    }
    /** Create a stream corresponding to the client socket file
    descriptor.  */
    if( verbose) printf( "[monitor_position] My file descriptor is %d\n", fd );

    char buffer[1024];
    int  buf_len = 0;

    if( html_output_flag ) {
        buf_len += sprintf( buffer, page_start, "Monitor Position (monitor_position)" );
    }

    lcu->waitSemaphore();
    is_telescope_configured = lcu->telescope->getIsConfigured();
    lcu->postSemaphore();

    if( is_telescope_configured == 0 ) {
        if( verbose) printf( "[monitor_position] Telescope not configured...\n" );

        sprintf( & buffer[buf_len], "Information: Please, config telescope first!\n" );
        buf_len = strlen( buffer );
        sprintf( & buffer[buf_len], "--------------------------------------------------\n" );
        buf_len = strlen( buffer );
        strftime( & buffer[buf_len], 256, "Page generated at: |%Y-%m-%d|%T|\n", lcu->getLCU_Time() );
        buf_len = strlen( buffer );

        if( html_output_flag ) {
            buf_len += sprintf( & buffer[buf_len], page_end );
        } else {
            buf_len += sprintf( & buffer[buf_len], "\r\n\r\n" );
        }

        retval = write( fd, buffer, buf_len );
        if( retval > 0 ) {
            if( verbose ) printf( "[monitor_position] %d Bytes written in fd=%d \n", retval, fd );
        } else {
            perror( "[monitor_position] write" );
            if( verbose ) printf( "[monitor_position] write ERROR\n" );
        }

        if( verbose) printf( "[monitor_position] Good bye!\n" );
        return;
    }

    do {
        lcu->waitSemaphore();
        {
            /** Delta T */
            delta_t = lcu->telescope->getDeltaT();
            if( delta_t < 0.8 ) {
                sprintf( & buffer[buf_len], "Time elapsed: %lf sec\n", delta_t );
                buf_len = strlen( buffer );
            } else {
                /** Current Position */
                sprintf( & buffer[buf_len], "Time elapsed: %lf > 0.8 sec: Getting new position\n", delta_t );
                buf_len = strlen( buffer );
                lcu->telescope->currentPosition( & lst, & ra, & dec, & alt, & az );
            }
            sprintf( & buffer[buf_len], "--------------------------------------------------\n" );
            buf_len = strlen( buffer );
            /** Local Time */
            strftime( & buffer[buf_len], 256, "LT  = | %T|  LD  = |%Y-%m-%d|\n", lcu->telescope->getLocalTime() );
            buf_len = strlen( buffer );
            /** Local Sidereal Time JDate */
            strfdegs( & buffer[buf_len], 256, "LST = | %02d:%02d:%02.0lf|  ", lcu->telescope->getLST() / 15.0 );
            buf_len = strlen( buffer );
            sprintf( & buffer[buf_len], "JD  = |%lf|\n", lcu->telescope->getJulianDate() );
            buf_len = strlen( buffer );
            /** Telescope RA and HA */
            strfdegs( & buffer[buf_len], 256, "RA  = | %02d:%02d:%02.0lf|  ", lcu->telescope->getRA() / 15.0 );
            buf_len = strlen( buffer );
            strfdegs( & buffer[buf_len], 256, "HA  = |%+03d:%02d:%02.0lf|\n", lcu->telescope->getHA() / 15.0 );
            buf_len = strlen( buffer );
            /** Telescope Dec Dec - Telescope Longitude */
            strfdegs( & buffer[buf_len], 256, "Dec = |%+03d:%02d:%02.0lf|  ", lcu->telescope->getDec() );
            buf_len = strlen( buffer );
            //write( fd, buffer, strlen(buffer) );
            strfdegs( & buffer[buf_len], 256, "D-L = |%+03d:%02d:%02.0lf|\n", lcu->telescope->getDec() - lcu->telescope->getLatitude() );
            buf_len = strlen( buffer );
            /** Target RA and HA */
            strfdegs( & buffer[buf_len], 256, "tRA = | %02d:%02d:%02.0lf|  ", lcu->telescope->getTargetRA() / 15.0 );
            buf_len = strlen( buffer );
            strfdegs( & buffer[buf_len], 256, "tHA = |%+03d:%02d:%02.0lf|\n", lcu->telescope->getTargetHA() / 15.0 );
            buf_len = strlen( buffer );
            /** Target Dec  Target Dec - Telescope Longitude */
            strfdegs( & buffer[buf_len], 256, "tDec= |%+03d:%02d:%02.0lf|  ", lcu->telescope->getTargetDec() );
            buf_len = strlen( buffer );
            strfdegs( & buffer[buf_len], 256, "tD-L= |%+03d:%02d:%02.0lf|\n", lcu->telescope->getTargetDec() - lcu->telescope->getLatitude() );
            buf_len = strlen( buffer );
            /** Differesnce RA Dec  */
            strfdegs( & buffer[buf_len], 256, "dRA = |%+03d:%02d:%02.0lf|  ", lcu->telescope->getDifferenceRA() / 15.0  );
            buf_len = strlen( buffer );
            strfdegs( & buffer[buf_len], 256, "dDec= |%+03d:%02d:%02.0lf|\n", lcu->telescope->getDifferenceDec() );
            buf_len = strlen( buffer );
        }
        lcu->postSemaphore();

        /** LCU Local Time */
        sprintf( & buffer[buf_len], "--------------------------------------------------\n" );
        buf_len = strlen( buffer );
        strftime( & buffer[buf_len], 256, "Page generated at: |%Y-%m-%d|%T|\n", lcu->getLCU_Time() );
        buf_len = strlen( buffer );

        if( html_output_flag ) {
            buf_len += sprintf( & buffer[buf_len], page_end );
            /** Write buffer to the file descriptor */
            retval = write( fd, buffer, buf_len );
            if( retval > 0 ) {
                if( verbose ) printf( "[monitor_position] %d Bytes written in fd=%d \n", retval, fd );
            } else {
                perror( "[monitor_position] write" );
                if( verbose ) printf( "[monitor_position] write ERROR\n" );
            }
            quit = true;
        } else {
            buf_len += sprintf( & buffer[buf_len], "\r\n\r\n" );
            /** Write buffer to the file descriptor */
            retval = write( fd, buffer, buf_len );
            if( retval <= 0 ) {
                perror( "[monitor_position] write" );
                if( verbose ) printf( "[monitor_position] write ERROR\n" );
            }
            /** Wait for request. */
            int bytes_read = 0;
            do {
                int retval;
                fd_set rfds;
                struct timeval tv;

                FD_ZERO( & rfds );
                FD_SET( fd, & rfds );
                tv.tv_sec = 2;
                tv.tv_usec= 0;
                retval = select( fd + 1, & rfds, NULL, NULL, & tv );
                if( retval < 0 ) {
                    perror( "[monitor_position] select" );
                    if( verbose ) printf( "[monitor_position] select ERROR!\n" );
                    quit = true;
                } else if( retval == 0 ) {
                    if( verbose ) printf( "[monitor_position] Timeout\n" );
                    quit = true;
                } else {
                    bytes_read += read( fd, & buffer[bytes_read], sizeof( buffer ) - 1 );
                    buffer[bytes_read] = 0;
                    //if( verbose ) printf( "[monitor_position] Received %d bytes, request: %s\n", bytes_read, buffer );
                    if( bytes_read > 0 ) {
                        if( strstr( buffer, "quit" ) ){
                            quit = true;
                        } else {
                            quit = false;
                        }
                    } else {
                        quit = true;
                    }
                }
            } while( 0 );

            buf_len = 0;
        }
    } while( ! quit );

    buf_len = 0;
    buf_len += sprintf( & buffer[buf_len], "\r\n\r\n" );
    /** Write buffer to the file descriptor */
    retval = write( fd, buffer, buf_len );

    if( verbose) printf( "[monitor_position] Good bye!\n" );
}


