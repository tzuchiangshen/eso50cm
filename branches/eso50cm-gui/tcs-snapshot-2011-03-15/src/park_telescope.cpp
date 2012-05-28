/**
 *
 *
 */
#include <stdio.h>
#include <sys/uio.h>

#include "myLCU.h"
#include "myUtil.h"

#define NUM_OF_TRY 4

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
    double offset_ra, offset_dec;
    int  alpha_mtr_counts;
    int  delta_mtr_counts;
    char mem_address;
    int value;
    int no_quit;
    int goto_alpha_flag = false;
    int goto_delta_flag = false;
    int setup_ready;
    double trg_ra = 0;
    double trg_dec = 0;
    double trg_alt = 0;
    double trg_az = 0;
    char is_telescope_configured = 0;
    double delta_t;

    int  retval;
    char html_output_flag = 1;
    if( verbose ) printf( "[park_telescope] Hello world!\n" );
    /** Checks for arguments */
    if( * arguments != 0 ) {
        if( verbose ) printf( "[park_telescope] Has arguments: %s\n", arguments );
        char * ptr = NULL;

        if( (ptr = strstr( arguments, "txt" )) != NULL ) {
            html_output_flag = 0;
            if( verbose ) printf( "[park_telescope] No HTML output [%s]\n", ptr );
        }
    }

    /** Create a stream corresponding to the client socket file
    descriptor.  */
    if( verbose) printf( "[park_telescope] My file descriptor is %d\n", fd );

    char buffer[1024];
    int  buf_len = 0;

    if( html_output_flag ) {
        buf_len += sprintf( buffer, page_start, "Park Telescope (park_telescope)" );
    } else {
        buf_len += sprintf( buffer, "Park Telescope (park_telescope)\n\n" );
    }

    lcu->waitSemaphore();
    is_telescope_configured = lcu->telescope->getIsConfigured();
    lcu->postSemaphore();
    if( is_telescope_configured == 0 ) {
        buf_len = 0;
        sprintf( & buffer[buf_len], "Information: Please, config telescope first!\n" );
        buf_len = strlen( buffer );
        sprintf( & buffer[buf_len], "--------------------------------------------------\n" );
        buf_len = strlen( buffer );
        strftime( & buffer[buf_len], 256, "Page generated at: |%Y-%m-%d|%T|\n", lcu->getLCU_Time() );
        buf_len = strlen( buffer );
        sprintf( & buffer[buf_len], "\r\n\r\n" );
        /** Write buffer to the file descriptor */
        write( fd, buffer, buf_len );
        if( verbose) printf( "[park_telescope] Telescope not configured... Good bye!\n" );
        return;
    }

    lcu->waitSemaphore();
    {
        /** Time Elapsed */
        delta_t = lcu->telescope->getDeltaT();
        sprintf( & buffer[buf_len], "Time elapsed since last access: %lf sec\n", delta_t );
        buf_len = strlen( buffer );
        sprintf( & buffer[buf_len], "--------------------------------------------------\n" );
        buf_len = strlen( buffer );
        /** Current Position */
        lcu->telescope->currentPosition( & lst, & ra, & dec, & alt, & az );
        /** Set target = zenith */
        trg_ra = lst;
        trg_dec = lcu->telescope->getLatitude();
        lcu->telescope->setTarget( trg_ra, trg_dec, & trg_alt, & trg_az  );
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
        offset_ra = lcu->telescope->getDifferenceRA();
        strfdegs( & buffer[buf_len], 256, "dRA = |%+03d:%02d:%02.0lf|  ", offset_ra / 15.0  );
        buf_len = strlen( buffer );
        offset_dec = lcu->telescope->getDifferenceDec();
        strfdegs( & buffer[buf_len], 256, "dDec= |%+03d:%02d:%02.0lf|\n", offset_dec );
        buf_len = strlen( buffer );

        sprintf( & buffer[buf_len], "--------------------------------------------------\n" );
        buf_len = strlen( buffer );

        mem_address = 7;

        offset_ra *= -1.;
        alpha_mtr_counts = lcu->telescope->alpha->offsetAxisInDeg( offset_ra );
        offset_dec *= -1.;
        delta_mtr_counts = lcu->telescope->delta->offsetAxisInDeg( offset_dec );

        sprintf( & buffer[buf_len], "alpha = [%+8.3lf] degs -> [%8d] tics\n", offset_ra, alpha_mtr_counts );
        buf_len = strlen( buffer );
        sprintf( & buffer[buf_len], "delta = [%+8.3lf] degs -> [%8d] tics\n", offset_dec, delta_mtr_counts );
        buf_len = strlen( buffer );

        if( lcu->telescope->getIsTracking() ) {
            value = 0;
            lcu->telescope->alpha->Motor->setDeviceMemory( 3, & value  );
            lcu->telescope->setIsTracking( false );
            sprintf( & buffer[buf_len], "Tracking OFF!\n" );
            buf_len = strlen( buffer );
        } else {
            sprintf( & buffer[buf_len], "Tracking already OFF!\n" );
            buf_len = strlen( buffer );
        }
        if( alpha_mtr_counts < -50 || 50 < alpha_mtr_counts ) {
            //lcu->telescope->alpha->Motor->readDeviceMemory( mem_address, & value  );
            //sprintf( & buffer[buf_len], "alpha.Motor[%d]=%8d\n", mem_address, value );
            //buf_len = strlen( buffer );
            lcu->telescope->alpha->Motor->setDeviceMemory( 7, & alpha_mtr_counts, 0  );
            goto_alpha_flag = true;
        }
        if( delta_mtr_counts < -50 || 50 < delta_mtr_counts ) {
            //lcu->telescope->delta->Motor->readDeviceMemory( mem_address, & value  );
            //sprintf( & buffer[buf_len], "delta.Motor[%d]=%8d\n", mem_address, value );
            //buf_len = strlen( buffer );
            lcu->telescope->delta->Motor->setDeviceMemory( 7, & delta_mtr_counts, 0  );
            goto_delta_flag = true;
        }
    }
    lcu->postSemaphore();
    /** LCU Local Time */
    sprintf( & buffer[buf_len], "--------------------------------------------------\n" );
    buf_len = strlen( buffer );
    strftime( & buffer[buf_len], 128, "Page generated at: [%Y-%m-%d, %T]\n", lcu->getLCU_Time() );
    //buf_len = strlen( buffer );
    //sprintf( & buffer[buf_len], "\r\n\r\n" );
    write( fd, buffer, strlen( buffer ) );

    /** Goto target loop*/
    setup_ready = NUM_OF_TRY;
    do {
        no_quit = 180;
        do {
            sleep( 1 );
            buf_len = 0;
            lcu->waitSemaphore();
            {
                if( goto_alpha_flag ) {
                    lcu->telescope->alpha->Motor->readDeviceMemory( 7, & alpha_mtr_counts, 0  );
                    if( -50 < alpha_mtr_counts && alpha_mtr_counts < 50 )
                        goto_alpha_flag = false;
                }
                if( goto_delta_flag ) {
                    lcu->telescope->delta->Motor->readDeviceMemory( 7, & delta_mtr_counts, 0  );
                    if( -50 < delta_mtr_counts && delta_mtr_counts < 50 )
                        goto_delta_flag = false;
                }
            }
            lcu->postSemaphore();

            if( ! goto_delta_flag && ! goto_alpha_flag ) {
                no_quit = false;
            } else {
                no_quit --;
            }
            if( verbose) {
                //printf( "[park_telescope] alpha = %d\n", alpha_mtr_counts );
                //printf( "[park_telescope] delta = %d\n", delta_mtr_counts );
                printf( "[park_telescope] To quit (if something was wrong)... %d sec.\n", no_quit );
            }
            sprintf( & buffer[buf_len], "alpha = %d\n", alpha_mtr_counts );
            buf_len = strlen( buffer );
            sprintf( & buffer[buf_len], "delta = %d\n", delta_mtr_counts );
            buf_len = strlen( buffer );
            sprintf( & buffer[buf_len], "--------------------------------------------------\n" );
            buf_len = strlen( buffer );
            strftime( & buffer[buf_len], 128, "Page generated at: [%Y-%m-%d, %T]\n", lcu->getLCU_Time() );
            buf_len = strlen( buffer );

            write( fd, buffer, buf_len );
        } while( no_quit );

        buf_len = 0;
        goto_alpha_flag = false;
        goto_delta_flag = false;
        lcu->waitSemaphore();
        {
            /** Time Elapsed */
            delta_t = lcu->telescope->getDeltaT();
            sprintf( & buffer[buf_len], "Time elapsed since last access: %lf sec\n", delta_t );
            buf_len = strlen( buffer );
            /** Current Position */
            lcu->telescope->currentPosition( & lst, & ra, & dec, & alt, & az );
            /** Set target = zenith */
            trg_ra = lst;
            trg_dec = lcu->telescope->getLatitude();
            lcu->telescope->setTarget( trg_ra, trg_dec, & trg_alt, & trg_az  );
            /** Differesnce RA Dec  */
            offset_ra = lcu->telescope->getDifferenceRA();
            strfdegs( & buffer[buf_len], 256, "dRA = |%+03d:%02d:%02.0lf|  ", offset_ra / 15.0  );
            buf_len = strlen( buffer );
            offset_dec = lcu->telescope->getDifferenceDec();
            strfdegs( & buffer[buf_len], 256, "dDec= |%+03d:%02d:%02.0lf|\n", offset_dec );
            buf_len = strlen( buffer );

            mem_address = 7;
            offset_ra *= -1.;
            alpha_mtr_counts = lcu->telescope->alpha->offsetAxisInDeg( offset_ra );
            offset_dec *= -1.;
            delta_mtr_counts = lcu->telescope->delta->offsetAxisInDeg( offset_dec );

            sprintf( & buffer[buf_len], "alpha = [%+8.3lf] degs -> [%8d] tics\n", offset_ra, alpha_mtr_counts );
            buf_len = strlen( buffer );
            sprintf( & buffer[buf_len], "delta = [%+8.3lf] degs -> [%8d] tics\n", offset_dec, delta_mtr_counts );
            buf_len = strlen( buffer );

            /** Alpha error smaller than 5000, then start tracking */
            //if( -10000 < alpha_mtr_counts && alpha_mtr_counts < 10000 ) {
            //    if( ! lcu->telescope->getIsTracking() ) {
            //        value = 600;
            //        lcu->telescope->alpha->Motor->setDeviceMemory( 3, & value  );
            //        lcu->telescope->setIsTracking( true );
            //        sprintf( & buffer[buf_len], "Tracking ON!\n" );
            //        buf_len = strlen( buffer );
            //   }
            //}
            if( alpha_mtr_counts < -50 || 50 < alpha_mtr_counts ) {
            //lcu->telescope->alpha->Motor->readDeviceMemory( mem_address, & value  );
            //sprintf( & buffer[buf_len], "alpha.Motor[%d]=%8d\n", mem_address, value );
            //buf_len = strlen( buffer );
                lcu->telescope->alpha->Motor->setDeviceMemory( 7, & alpha_mtr_counts, 0  );
                goto_alpha_flag = true;
            }
            if( delta_mtr_counts < -50 || 50 < delta_mtr_counts ) {
            //lcu->telescope->delta->Motor->readDeviceMemory( mem_address, & value  );
            //sprintf( & buffer[buf_len], "delta.Motor[%d]=%8d\n", mem_address, value );
            //buf_len = strlen( buffer );
                lcu->telescope->delta->Motor->setDeviceMemory( 7, & delta_mtr_counts, 0  );
                goto_delta_flag = true;
            }
        }
        lcu->postSemaphore();

        if( ! goto_alpha_flag && ! goto_delta_flag ) {
            setup_ready = false;
        } else {
            setup_ready --;
        }

        sprintf( & buffer[buf_len], "--------------------------------------------------\n" );
        buf_len = strlen( buffer );
        strftime( & buffer[buf_len], 128, "Page generated at: [%Y-%m-%d, %T]\n", lcu->getLCU_Time() );
        buf_len = strlen( buffer );
        buf_len += sprintf( & buffer[buf_len], "\r\n\r\n" );

        write( fd, buffer, buf_len );

        if( verbose )
            printf( "[park_telescope] Try No %d\n", NUM_OF_TRY - setup_ready );

    } while( setup_ready );

    buf_len = 0;
    if( html_output_flag ) {
        buf_len += sprintf( & buffer[buf_len], page_end );
    } else {
        buf_len += sprintf( & buffer[buf_len], "\r\n\r\n" );
    }

    retval = write( fd, buffer, buf_len );
    if( retval > 0 ) {
        if( verbose ) printf( "[park_telescope] %d Bytes written in fd=%d \n", retval, fd );
    } else {
        perror( "[park_telescope] write" );
        if( verbose ) printf( "[park_telescope] write ERROR\n" );
    }


    lcu->waitSemaphore();
    lcu->telescope->setIsRunningGoto( false );
    lcu->postSemaphore();

    if( verbose)
        printf( "[park_telescope] Good bye!\n" );
}


