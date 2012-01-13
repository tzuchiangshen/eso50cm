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
    "  <title>myTelescope</title>"
    "</head>\n"
    "<body>\n"
    "  <h2 align=\"left\">%s</h2>\n"
    "  <pre>\n";

/** HTML source for the end of the process listing page.  */
static char * page_end =
    "  </pre>\n"
    "</body>\n"
    "</html>\n";


extern "C" void module_generate( int fd, const char * arguments, myLCU * lcu )
{
    extern int verbose;

    int value = 0;
    char is_telescope_configured = 0;
    double delta_t;

    int  retval;
    char html_output_flag = 1;
    if( verbose ) printf( "[read_encoders] Hello world!\n" );
    /** Checks for arguments */
    if( * arguments != 0 ) {
        if( verbose ) printf( "[read_encoders] Has arguments: %s\n", arguments );
        char * ptr = NULL;

        if( (ptr = strstr( arguments, "txt" )) != NULL ) {
            html_output_flag = 0;
            if( verbose ) printf( "[read_encoders] No HTML output [%s]\n", ptr );
        }
    }
    /** Create a stream corresponding to the client socket file
    descriptor.  */
    if( verbose) printf( "[read_encoders] My file descriptor is %d\n", fd );

    char buffer[1024];
    int  buf_len = 0;

    if( html_output_flag ) {
        buf_len += sprintf( buffer, page_start, "Read Encoders (read_encoders)" );
    }

    lcu->waitSemaphore();
    is_telescope_configured = lcu->telescope->getIsConfigured();
    lcu->postSemaphore();

    if( is_telescope_configured == 0 ) {
        sprintf( & buffer[buf_len], "Information: Please, config telescope first!\n" );
        buf_len = strlen( buffer );
    } else {
        lcu->waitSemaphore();
        {
            /** Time Elapsed */
            delta_t = lcu->telescope->getDeltaT();
            sprintf( & buffer[buf_len], "Time elapsed since last access: %lf sec\n", delta_t );
            buf_len = strlen( buffer );
            sprintf( & buffer[buf_len], "--------------------------------------------------\n" );
            buf_len = strlen( buffer );
            sprintf( & buffer[buf_len], "            Lecture       Position\n" );
            buf_len = strlen( buffer );
            /** Alpha Axis Encoder */
            lcu->telescope->alpha->AxisE->readDeviceMemory( 2, & value, 0 );
            sprintf( & buffer[buf_len], "Alpha AxisE: %6d    ", value );
            buf_len = strlen( buffer );
            lcu->telescope->alpha->AxisE->readDeviceMemory( 4, & value, 0 );
            sprintf( & buffer[buf_len], "%+11d\n", value );
            buf_len = strlen( buffer );
            /** Alpha Worm Encoder */
            lcu->telescope->alpha->WormE->readDeviceMemory( 2, & value, 0  );
            sprintf( & buffer[buf_len], "Alpha WormE: %6d    ", value );
            buf_len = strlen( buffer );
            lcu->telescope->alpha->WormE->readDeviceMemory( 4, & value, 0  );
            sprintf( & buffer[buf_len], "%+11d\n", value );
            buf_len = strlen( buffer );
            /** Alpha Motor Encoder */
            lcu->telescope->alpha->Motor->readDeviceMemory( 2, & value, 0  );
            sprintf( & buffer[buf_len], "Alpha Motor: %6d    ", value );
            buf_len = strlen( buffer );
            lcu->telescope->alpha->Motor->readDeviceMemory( 4, & value, 0  );
            sprintf( & buffer[buf_len], "%+11d\n\n", value );
            buf_len = strlen( buffer );

            /** Delta Axis Encoder */
            lcu->telescope->delta->AxisE->readDeviceMemory( 2, & value, 0  );
            sprintf( & buffer[buf_len], "Delta AxisE: %6d    ", value );
            buf_len = strlen( buffer );
            lcu->telescope->delta->AxisE->readDeviceMemory( 4, & value, 0  );
            sprintf( & buffer[buf_len], "%+11d\n", value );
            buf_len = strlen( buffer );
            /** Delta Worm Encoder */
            lcu->telescope->delta->WormE->readDeviceMemory( 2, & value, 0  );
            sprintf( & buffer[buf_len], "Delta WormE: %6d    ", value );
            buf_len = strlen( buffer );
            lcu->telescope->delta->WormE->readDeviceMemory( 4, & value, 0  );
            sprintf( & buffer[buf_len], "%+11d\n", value );
            buf_len = strlen( buffer );
            /** Delta Motor Encoder */
            lcu->telescope->delta->Motor->readDeviceMemory( 2, & value, 0  );
            sprintf( & buffer[buf_len], "Delta Motor: %6d    ", value );
            buf_len = strlen( buffer );
            lcu->telescope->delta->Motor->readDeviceMemory( 4, & value, 0  );
            sprintf( & buffer[buf_len], "%+11d\n", value );
            buf_len = strlen( buffer );
        }
        lcu->postSemaphore();
    }
    /** LCU Local Time */
    sprintf( & buffer[buf_len], "--------------------------------------------------\n" );
    buf_len = strlen( buffer );
    strftime( & buffer[buf_len], 128, "Page generated at: [%Y-%m-%d, %T]\n", lcu->getLCU_Time() );
    buf_len = strlen( buffer );

    if( html_output_flag ) {
        buf_len += sprintf( & buffer[buf_len], page_end );
    } else {
        buf_len += sprintf( & buffer[buf_len], "\r\n\r\n" );
    }

    retval = write( fd, buffer, buf_len );
    if( retval > 0 ) {
        if( verbose ) printf( "[read_encoders] %d Bytes written in fd=%d \n", retval, fd );
    } else {
        perror( "[read_encoders] write" );
        if( verbose ) printf( "[read_encoders] write ERROR\n" );
    }

    if( verbose) printf( "[read_encoders] Good bye!\n" );
}


