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
    "  <h2 align=\"left\">Current Position</h2>\n"
    "  <pre>\n";

/** HTML source for the end of the process listing page.  */
static char * page_end =
    "  </pre>\n"
    "</body>\n"
    "</html>\n";


extern "C" void module_generate( int fd, const char * arguments, myLCU * lcu )
{
    extern int verbose;
    char buffer[1024];
    int  buf_len = 0;
    double lst, ra, dec, alt, az;

    if( verbose ) {
        printf( "[read_encoders] Hello world!\n" );
        if( * arguments != 0 ) {
            printf( "[read_encoders] %s\n", arguments );
        }
    }


    if( verbose) printf( "[read_encoders] My file descriptor is %d\n", fd );
    /** Create a stream corresponding to the client socket file
    descriptor.  */
    sprintf( & buffer[buf_len], page_start );
    buf_len = strlen( buffer );

    lcu->waitSemaphore();
    if( lcu->telescope->getIsConfigured() == 0 ) {
        sprintf( & buffer[buf_len], "Information: Please, config telescope first!\n" );
        buf_len = strlen( buffer );
    } else {
        lcu->telescope->currentPosition( & lst, & ra, & dec, & alt, & az );
        /** Local Time, Local Sidereal Time, JDate */
        strfdegs( & buffer[buf_len], 256, "LST = | %02d:%02d:%02.0lf|  ", lcu->telescope->getLST() / 15.0 );
        buf_len = strlen( buffer );
        strftime( & buffer[buf_len], 256, "LT  = | %T|    LD  = |%Y-%m-%d| ", lcu->telescope->getLocalTime() );
        buf_len = strlen( buffer );
        sprintf( & buffer[buf_len], "JD  = |%lf|\n", lcu->telescope->getJulianDate() );
        buf_len = strlen( buffer );

        /** Telescope RA Dec and HA Dec - Longitude */
        strfdegs( & buffer[buf_len], 256, "RA  = | %02d:%02d:%02.0lf|  ", lcu->telescope->getRA() / 15.0 );
        buf_len = strlen( buffer );
        strfdegs( & buffer[buf_len], 256, "Dec = |%+03d:%02d:%02.0lf|    ", lcu->telescope->getDec() );
        buf_len = strlen( buffer );
        strfdegs( & buffer[buf_len], 256, "HA  = |%+03d:%02d:%02.0lf|  ", lcu->telescope->getHA() / 15.0 );
        buf_len = strlen( buffer );
        strfdegs( & buffer[buf_len], 256, "D-L = |%+03d:%02d:%02.0lf|\n", lcu->telescope->getDec() - lcu->telescope->getLatitude() );
        buf_len = strlen( buffer );

        /** Target RA Dec and Target HA Target Dec - Telescope Longitude */
        strfdegs( & buffer[buf_len], 256, "tRA = | %02d:%02d:%02.0lf|  ", lcu->telescope->getTargetRA() / 15.0 );
        buf_len = strlen( buffer );
        strfdegs( & buffer[buf_len], 256, "tDec= |%+03d:%02d:%02.0lf|    ", lcu->telescope->getTargetDec() );
        buf_len = strlen( buffer );
        strfdegs( & buffer[buf_len], 256, "tHA = |%+03d:%02d:%02.0lf|  ", lcu->telescope->getTargetHA() / 15.0 );
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
    strftime( & buffer[buf_len], 128, "Page generated at: [%Y-%m-%d, %T]\n", lcu->getLCU_Time() );
    buf_len = strlen( buffer );

    sprintf( & buffer[buf_len], "\r\n\r\n" );
    buf_len = strlen( buffer );

    sprintf( & buffer[buf_len], page_end );

    write( fd, buffer, strlen( buffer ) );

    if( verbose) printf( "[read_encoders] HTML output done!\n" );
    if( verbose) printf( "[read_encoders] Good bye!\n" );
}




extern "C" void module_generate_txt( int fd, const char * arguments, myLCU * lcu )
{
    extern int verbose;
    char buffer[1024];
    int  buf_len = 0;
    //double lst, ra, dec, alt, az;
    int value = 0;
    //double tmp;
    //double ha_degrees, dl_degrees;
    char is_telescope_configured = 0;
    double delta_t;

    if( verbose ) {
        printf( "[read_encoders] Hello world!\n" );
        if( * arguments != 0 ) {
            printf( "[read_encoders] %s\n", arguments );
        }
    }

    if( verbose) printf( "[read_encoders] My file descriptor is %d\n", fd );

    /** Create a stream corresponding to the client socket file
        descriptor.  */
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

    sprintf( & buffer[buf_len], "\r\n\r\n" );
    write( fd, buffer, strlen( buffer ) );

    if( verbose) printf( "[read_encoders] Good bye!\n" );
}


