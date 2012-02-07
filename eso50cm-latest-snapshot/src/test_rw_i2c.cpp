/**
 *
 *
 */
#include <stdio.h>
#include <sys/uio.h>

#include "myLCU.h"
#include "myUtil.h"

/** HTML source for the start of the process listing page.  */
static char* page_start = 
    "<html>\n" 
    "<head>\n"
    "<title>myTelescope</title>"
    "</head>\n"
    " <body>\n"
    "  <h2 align=\"left\">Set Target</h2>\n"
    "  <pre>\n";

/** HTML source for the end of the process listing page.  */
static char* page_end = 
    "  </pre>\n"
    " </body>\n"
    "</html>\n";


extern "C" void module_generate( int fd, const char * arguments, myLCU * lcu )
{
    extern int verbose;


    if( verbose ) printf( "[test_rw_i2c] Hello world!\n" );
    if( * arguments != 0 ) {
        if( verbose ) printf( "[test_rw_i2c] has arguments %s\n", arguments );

    }

    char buffer[1024];
    int  buf_len = 0;

    if( verbose) printf( "[test_rw_i2c] My file descriptor is %d\n", fd );
    /** Create a stream corresponding to the client socket file
        descriptor.  */
    sprintf( & buffer[buf_len], page_start );
    buf_len = strlen( buffer );

    lcu->waitSemaphore();
    if( lcu->telescope->getIsConfigured() == 0 ) {
        sprintf( & buffer[buf_len], "Information: Please, config telescope first!\n" );
        buf_len = strlen( buffer );
    }
    lcu->postSemaphore();

    /** LCU Local Time */
    sprintf( & buffer[buf_len], "--------------------------------------------------\n" );
    buf_len = strlen( buffer );
    strftime( & buffer[buf_len], 128, "Page generated at: [%Y-%m-%d, %T]\n", lcu->getLCU_Time() );
    buf_len = strlen( buffer );
    sprintf( & buffer[buf_len], "\r\n\r\n" );
    buf_len = strlen( buffer );

    sprintf( & buffer[buf_len], page_end );

    write( fd, buffer, strlen( buffer ) );

    if( verbose) printf( "[test_rw_i2c] Good bye!\n" );
}



extern "C" void module_generate_txt( int fd, const char * arguments, myLCU * lcu )
{
    extern int verbose;

    char    buffer[1024];
    int     buf_len                 = 0;
    char    help_flag               = 0;
    char    is_telescope_configured = 0;
    int     i;
    int     value;
    int     address                 = 0;

    if( verbose ) printf( "[test_rw_i2c] Hello world!\n" );
    if( * arguments != 0 ) {
        if( verbose ) printf( "[test_rw_i2c] has arguments %s\n", arguments );
        char * ptr = NULL;

        if( (ptr = strstr( arguments, "help" )) != NULL ) {
            help_flag = 1;
        }

        if( (ptr = strstr( arguments, "address" )) != NULL ) {
            if( sscanf( ptr, "address=%d", & address ) == 1 ) {
                if( verbose ) printf( "[test_rw_i2c] address=%d\n", address );
            } else {
                if( verbose ) printf( "[test_rw_i2c] No address specified.\n" );
            }
        }
    }


    lcu->waitSemaphore();
    is_telescope_configured = lcu->telescope->getIsConfigured();
    lcu->postSemaphore();

    buf_len = 0;
    if( is_telescope_configured == 0 ) {
        sprintf( & buffer[buf_len], "Information: Please, config telescope first!\n" );
        buf_len = strlen( buffer );
    } else {
        lcu->waitSemaphore();
        {
            sprintf( & buffer[buf_len], "Read Test\n" );
            buf_len = strlen( buffer );

            for( i = 0; i < 8; i ++ ) {
                if( address == 120 ) {
                    lcu->telescope->alpha->Motor->readDeviceMemory( i, & value  );
                    sprintf( & buffer[buf_len], "alpha.Motor[%d]=%8d\n", i, value );
                } else if( address == 122 ) {
                    lcu->telescope->delta->Motor->readDeviceMemory( i, & value  );
                    sprintf( & buffer[buf_len], "delta.Motor[%d]=%8d\n", i, value );
                }
                buf_len = strlen( buffer );
            }
            sprintf( & buffer[buf_len], "\nWrite Test\n" );
            buf_len = strlen( buffer );
            for( i = 0; i < 8; i ++ ) {
                if( address == 120 ) {
                    value = 1200 + i;
                    lcu->telescope->alpha->Motor->setDeviceMemory( i, & value  );
                } else if( address == 122 ) {
                    value = 1220 + i;
                    lcu->telescope->delta->Motor->setDeviceMemory( i, & value  );
                }
                //sprintf( & buffer[buf_len], "alpha.Motor[%d]=%8d\n", i, value );
                //buf_len = strlen( buffer );
            }
            for( i = 0; i < 8; i ++ ) {
                if( address == 120 ) {
                    lcu->telescope->alpha->Motor->readDeviceMemory( i, & value  );
                    sprintf( & buffer[buf_len], "alpha.Motor[%d]=%8d\n", i, value );
                } else if( address == 122 ) {
                    lcu->telescope->delta->Motor->readDeviceMemory( i, & value  );
                    sprintf( & buffer[buf_len], "delta.Motor[%d]=%8d\n", i, value );
                }
                buf_len = strlen( buffer );
            }
        }
        lcu->postSemaphore();
    }

    /** LCU Local Time */
    sprintf( & buffer[buf_len], "--------------------------------------------------\n" );
    buf_len = strlen( buffer );
    strftime( & buffer[buf_len], 128, "Page generated at: [%Y-%m-%d, %T]\n", lcu->getLCU_Time() );
    buf_len = strlen( buffer );
    sprintf( & buffer[buf_len], "\r\n\r\n" );

    write( fd, buffer, strlen( buffer ) );

    if( verbose) printf( "[test_rw_i2c] Good bye!\n" );
}


