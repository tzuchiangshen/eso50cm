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

    double lst, ra, dec, alt, az;
    int value = 0;
    double tmp;
    double ha_degrees, dl_degrees;
    char is_telescope_configured = 0;
    double delta_t;

    int  retval;
    char html_output_flag = 1;
    if( verbose ) printf( "[sync_to_zenith] Hello world!\n" );
    /** Checks for arguments */
    if( * arguments != 0 ) {
        if( verbose ) printf( "[sync_to_zenith] Has arguments: %s\n", arguments );
        char * ptr = NULL;

        if( (ptr = strstr( arguments, "txt" )) != NULL ) {
            html_output_flag = 0;
            if( verbose ) printf( "[sync_to_zenith] No HTML output [%s]\n", ptr );
        }
    }
    /** Create a stream corresponding to the client socket file
        descriptor.  */
    if( verbose) printf( "[sync_to_zenith] My file descriptor is %d\n", fd );

    char buffer[1024];
    int  buf_len = 0;

    if( html_output_flag ) {
        buf_len += sprintf( buffer, page_start, "Sync. Current Position to Zenith (sync_to_zenith)" );
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
            /** Alpha */
            ha_degrees = 0.0;
            if( verbose )
                printf( "[save_position] delta = %lf\n", ha_degrees );
            /** Alpha Axis Encoder */
            lcu->telescope->alpha->AxisE->readDeviceMemory( 4, & value  );
            tmp = ha_degrees;
            tmp /= 360.0;
            tmp *= lcu->telescope->alpha->AxisE->getEncoderToAxis_Reduction();
            tmp *= lcu->telescope->alpha->AxisE->getTicsPerRev();
            tmp -= (double) value;
            tmp *= -1.;
            sprintf( & buffer[buf_len], "Alpha AxisE Home Position: old=%5.0lf  new=%5.0lf\n",
                    lcu->telescope->alpha->AxisE->getHomePosition(), tmp );
            buf_len = strlen( buffer );
            lcu->telescope->alpha->AxisE->setHomePosition( round( tmp ) );
            /** Alpha Worm Encoder */
            lcu->telescope->alpha->WormE->readDeviceMemory( 4, & value  );
            tmp = ha_degrees;
            tmp /= 360.0;
            tmp *= lcu->telescope->alpha->WormE->getEncoderToAxis_Reduction();
            tmp *= lcu->telescope->alpha->WormE->getTicsPerRev();
            tmp -= (double) value;
            tmp *= -1.;
            sprintf( & buffer[buf_len], "Alpha WormE Home Position: old=%5.0lf  new=%5.0lf\n",
                    lcu->telescope->alpha->WormE->getHomePosition(), tmp );
            buf_len = strlen( buffer );
            lcu->telescope->alpha->WormE->setHomePosition( round( tmp ) );
            /** Alpha Motor Encoder */
            lcu->telescope->alpha->Motor->readDeviceMemory( 4, & value  );
            tmp = ha_degrees;
            tmp /= 360.0;
            tmp *= lcu->telescope->alpha->Motor->getEncoderToAxis_Reduction();
            tmp *= lcu->telescope->alpha->Motor->getTicsPerRev();
            tmp -= (double) value;
            tmp *= -1.;
            sprintf( & buffer[buf_len], "Alpha Motor Home Position: old=%5.0lf  new=%5.0lf\n",
                    lcu->telescope->alpha->Motor->getHomePosition(), tmp );
            buf_len = strlen( buffer );
            lcu->telescope->alpha->Motor->setHomePosition( round( tmp ) );

            /** Delta  */
            dl_degrees = 0.0;//lcu->telescope->getLatitude();
            if( verbose )
                printf( "[save_position] delta = %lf\n", dl_degrees );
            /** Delta Axis Encoder */
            lcu->telescope->delta->AxisE->readDeviceMemory( 4, & value  );
            tmp = dl_degrees;
            tmp /= 360.0;
            tmp *= lcu->telescope->delta->AxisE->getEncoderToAxis_Reduction();
            tmp *= lcu->telescope->delta->AxisE->getTicsPerRev();
            tmp -= (double) value;
            tmp *= -1.;
            sprintf( & buffer[buf_len], "Delta AxisE Home Position: old=%5.0lf  new=%5.0lf\n",
                    lcu->telescope->delta->AxisE->getHomePosition(), tmp );
            buf_len = strlen( buffer );
            lcu->telescope->delta->AxisE->setHomePosition( round( tmp ) );
            /** Delta Worm Encoder */
            lcu->telescope->delta->WormE->readDeviceMemory( 4, & value  );
            tmp = dl_degrees;
            tmp /= 360.0;
            tmp *= lcu->telescope->delta->WormE->getEncoderToAxis_Reduction();
            tmp *= lcu->telescope->delta->WormE->getTicsPerRev();
            tmp -= (double) value;
            tmp *= -1.;
            sprintf( & buffer[buf_len], "Delta WormE Home Position: old=%5.0lf  new=%5.0lf\n",
                    lcu->telescope->delta->WormE->getHomePosition(), tmp );
            buf_len = strlen( buffer );
            lcu->telescope->delta->WormE->setHomePosition( round( tmp ) );
            /** Delta Motor Encoder */
            lcu->telescope->delta->Motor->readDeviceMemory( 4, & value  );
            tmp = dl_degrees;
            tmp /= 360.0;
            tmp *= lcu->telescope->delta->Motor->getEncoderToAxis_Reduction();
            tmp *= lcu->telescope->delta->Motor->getTicsPerRev();
            tmp -= (double) value;
            tmp *= -1.;
            sprintf( & buffer[buf_len], "Delta Motor Home Position: old=%5.0lf  new=%5.0lf\n",
                    lcu->telescope->delta->Motor->getHomePosition(), tmp );
            buf_len = strlen( buffer );
            lcu->telescope->delta->Motor->setHomePosition( round( tmp ) );

            sprintf( & buffer[buf_len], "--------------------------------------------------\n" );
            buf_len = strlen( buffer );

            /** Time Elapsed */
            delta_t = lcu->telescope->getDeltaT();
            sprintf( & buffer[buf_len], "Time elapsed since last access: %lf sec\n", delta_t );
            buf_len = strlen( buffer );
            sprintf( & buffer[buf_len], "--------------------------------------------------\n" );
            buf_len = strlen( buffer );
            /** Current Position */
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
            strfdegs( & buffer[buf_len], 256, "HA  = |%+03d:%02d:%02.0lf|  ", (lcu->telescope->getHA()) / 15.0 );
            buf_len = strlen( buffer );
            strfdegs( & buffer[buf_len], 256, "D-L = |%+03d:%02d:%02.0lf|\n", (lcu->telescope->getDec() - lcu->telescope->getLatitude()) );
            buf_len = strlen( buffer );

            /** Target RA Dec and Target HA Target Dec - Telescope Longitude */
            strfdegs( & buffer[buf_len], 256, "tRA = | %02d:%02d:%02.0lf|  ", lcu->telescope->getTargetRA() / 15.0 );
            buf_len = strlen( buffer );
            strfdegs( & buffer[buf_len], 256, "tDec= |%+03d:%02d:%02.0lf|    ", lcu->telescope->getTargetDec() );
            buf_len = strlen( buffer );
            strfdegs( & buffer[buf_len], 256, "tHA = |%+03d:%02d:%02.0lf|  ", (ha_degrees = lcu->telescope->getTargetHA())/ 15.0 );
            buf_len = strlen( buffer );
            strfdegs( & buffer[buf_len], 256, "tD-L= |%+03d:%02d:%02.0lf|\n", (dl_degrees = lcu->telescope->getTargetDec() - lcu->telescope->getLatitude()) );
            buf_len = strlen( buffer );

            /** Differesnce RA Dec  */
            strfdegs( & buffer[buf_len], 256, "dRA = |%+03d:%02d:%02.0lf|  ", lcu->telescope->getDifferenceRA() / 15.0  );
            buf_len = strlen( buffer );
            strfdegs( & buffer[buf_len], 256, "dDec= |%+03d:%02d:%02.0lf|\n", lcu->telescope->getDifferenceDec() );
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
        if( verbose ) printf( "[sync_to_zenith] %d Bytes written in fd=%d \n", retval, fd );
    } else {
        perror( "[sync_to_zenith] write" );
        if( verbose ) printf( "[sync_to_zenith] write ERROR\n" );
    }

    if( verbose) printf( "[sync_to_zenith] Good bye!\n" );
}


