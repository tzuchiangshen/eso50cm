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
    "  <h2 align=\"left\">Goto Target</h2>\n"
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
        printf( "[offset_telescope] Hello world!\n" );
        if( * arguments != 0 ) {
            printf( "[offset_telescope] %s\n", arguments );
        }
    }


    if( verbose) printf( "[offset_telescope] My file descriptor is %d\n", fd );
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
    sprintf( & buffer[buf_len], "Command temporaly disable via HTML!\n" );
    buf_len = strlen( buffer );
    sprintf( & buffer[buf_len], "--------------------------------------------------\n" );
    strftime( & buffer[buf_len], 128, "Page generated at: [%Y-%m-%d, %T]\n", lcu->getLCU_Time() );
    buf_len = strlen( buffer );

    sprintf( & buffer[buf_len], "\r\n\r\n" );
    buf_len = strlen( buffer );

    sprintf( & buffer[buf_len], page_end );

    write( fd, buffer, strlen( buffer ) );

    if( verbose) printf( "[offset_telescope] HTML output done!\n" );
    if( verbose) printf( "[offset_telescope] Good bye!\n" );
}




extern "C" void module_generate_txt( int fd, const char * arguments, myLCU * lcu )
{
    extern int verbose;
    char buffer[1024];
    int  buf_len = 0;
    double lst, ra, dec, alt, az;

    int  alpha_mtr_counts;
    int  delta_mtr_counts;
    char mem_address;

    int no_quit;
    int goto_alpha_flag = false;
    int goto_delta_flag = false;
    double offset_ra   = 0;
    int offset_ra_hrs  = 0;
    int offset_ra_min  = 0;
    int offset_ra_sec  = 0;
    double offset_dec  = 0;
    int offset_dec_deg = 0;
    int offset_dec_min = 0;
    int offset_dec_sec = 0;

    if( verbose )
        printf( "[offset_telescope] Hello world!\n" );
    if( * arguments != 0 ) {
        if( verbose ) printf( "[offset_telescope] has arguments %s\n", arguments );
        char * ptr = NULL;

        if( (ptr = strstr( arguments, "ra=" )) != NULL ) {
            if( sscanf( ptr, "ra=%d:%d:%d", & offset_ra_hrs, & offset_ra_min, & offset_ra_sec ) == 3 ) {
                if( verbose ) printf( "[offset_telescope] ra =%+03d:%02d:%02d\n", offset_ra_hrs, offset_ra_min, offset_ra_sec );
                offset_ra  = fabs( (double) offset_ra_hrs ) + ((double) offset_ra_min / 60.) + ((double) offset_ra_sec / 3600.);
                offset_ra *= 15.0;
                if( offset_ra_hrs < 0 ) {
                    offset_ra *= -1.;
                } else if( offset_ra_hrs == 0 && ( strstr( arguments, "ra=-" ) != NULL) ) {
                    offset_ra *= -1.;
                }
                if( verbose ) printf( "[offset_telescope] offset ra =%lf\n", offset_ra );
            } else {
                if( verbose ) printf( "[offset_telescope] No offset ra data\n" );
            }
            offset_ra *= -1;
        } else if( (ptr = strstr( arguments, "rad=" )) != NULL ) {
            if( sscanf( ptr, "rad=%d:%d:%d", & offset_ra_hrs, & offset_ra_min, & offset_ra_sec ) == 3 ) {
                if( verbose ) printf( "[offset_telescope] rad=%+03d:%02d:%02d\n", offset_ra_hrs, offset_ra_min, offset_ra_sec );
                offset_ra  = fabs( (double) offset_ra_hrs ) + ((double) offset_ra_min / 60.) + ((double) offset_ra_sec / 3600.);
                //offset_ra *= 15.0;
                if( offset_ra_hrs < 0 ) {
                    offset_ra *= -1.;
                } else if( offset_ra_hrs == 0 && ( strstr( arguments, "rad=-" ) != NULL) ) {
                    offset_ra *= -1.;
                }
                if( verbose ) printf( "[offset_telescope] offset rad=%lf\n", offset_ra );
            } else {
                if( verbose ) printf( "[offset_telescope] No offset rad data\n" );
            }
            offset_ra *= -1;
        }
        if( (ptr = strstr( arguments, "dec=" )) != NULL ) {
            if( sscanf( ptr, "dec=%d:%d:%d", & offset_dec_deg, & offset_dec_min, & offset_dec_sec ) == 3 ) {
                if( verbose ) printf( "[offset_telescope] offset dec=%+03d:%02d:%02d\n", offset_dec_deg, offset_dec_min, offset_dec_sec );
                offset_dec  = fabs( (double) offset_dec_deg ) + ((double) offset_dec_min / 60.) + ((double) offset_dec_sec / 3600.);
                if( offset_dec_deg < 0 ) {
                    offset_dec *= -1.;
                } else if( offset_dec_deg == 0 && ( strstr( arguments, "dec=-" ) != NULL) ) {
                    offset_dec *= -1.;
                }
                if( verbose ) printf( "[offset_telescope] offset dec=%lf\n", offset_dec );
            } else {
                if( verbose ) printf( "[offset_telescope] No offset dec data\n" );
            }
        }
    }
    if( verbose) printf( "[offset_telescope] My file descriptor is %d\n", fd );

    /** Create a stream corresponding to the client socket file
        descriptor.  */
    lcu->waitSemaphore();
    lcu->telescope->setIsRunningGoto( true );
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

        sprintf( & buffer[buf_len], "--------------------------------------------------\n" );
        buf_len = strlen( buffer );

        alpha_mtr_counts = lcu->telescope->alpha->offsetAxisInDeg( offset_ra );
        offset_dec *= -1.;
        delta_mtr_counts = lcu->telescope->delta->offsetAxisInDeg( offset_dec );

        sprintf( & buffer[buf_len], "alpha = [%+8.3lf] degs -> [%8d] tics\n", offset_ra, alpha_mtr_counts );
        buf_len = strlen( buffer );
        sprintf( & buffer[buf_len], "delta = [%+8.3lf] degs -> [%8d] tics\n", offset_dec, delta_mtr_counts );
        buf_len = strlen( buffer );

        //HA: 20 = 1/4 [arcsec]
        //if( alpha_mtr_counts < -25 || 25 < alpha_mtr_counts ) {
        if( alpha_mtr_counts < -20 || 20 < alpha_mtr_counts ) {
            lcu->telescope->alpha->Motor->setDeviceMemory( 7, & alpha_mtr_counts  );
            goto_alpha_flag = true;
        } else {
            alpha_mtr_counts = 0;
            lcu->telescope->alpha->Motor->setDeviceMemory( 7, & alpha_mtr_counts  );
            goto_alpha_flag = true;
        }
            //else {
        //    goto_alpha_flag = true;
        //}
        //Dec: 16 = 1/4 [arcsec]
        //if( delta_mtr_counts < -25 || 25 < delta_mtr_counts ) {
        if( delta_mtr_counts < -16 || 16 < delta_mtr_counts ) {
            lcu->telescope->delta->Motor->setDeviceMemory( 7, & delta_mtr_counts  );
            goto_delta_flag = true;
        } //else {
        //    goto_delta_flag = false;
        //}
    }
    lcu->postSemaphore();
    /** LCU Local Time */
    sprintf( & buffer[buf_len], "--------------------------------------------------\n" );
    buf_len = strlen( buffer );
    strftime( & buffer[buf_len], 128, "Page generated at: [%Y-%m-%d, %T]\n", lcu->getLCU_Time() );
    write( fd, buffer, strlen( buffer ) );

    /** Goto target loop */
    no_quit = 180;
    do {
        sleep( 1 );
        buf_len = 0;
        mem_address = 7;
        lcu->waitSemaphore();
        if( goto_alpha_flag ) {
            lcu->telescope->alpha->Motor->readDeviceMemory( mem_address, & alpha_mtr_counts  );
            //if( -25 < alpha_mtr_counts && alpha_mtr_counts < 25 )
            if( alpha_mtr_counts < 0 ) {
                goto_alpha_flag = false;
                if( verbose)
                    printf( "[offset_telescope] alpha < 0\n" );

            }
        }
        if( goto_delta_flag ) {
            lcu->telescope->delta->Motor->readDeviceMemory( mem_address, & delta_mtr_counts  );
            //if( -25 < delta_mtr_counts && delta_mtr_counts < 25 )
            if( delta_mtr_counts < 0 ) {
                goto_delta_flag = false;
                if( verbose)
                    printf( "[offset_telescope] delta < 0\n" );
            }
        }
        lcu->postSemaphore();

        if( ! goto_delta_flag && ! goto_alpha_flag ) {
            no_quit = false;
        } else {
            no_quit --;
        }
        if( verbose)
            printf( "[offset_telescope] To quit (if something was wrong)... %d sec.\n", no_quit );
        sprintf( & buffer[buf_len], "alpha = %d\n", alpha_mtr_counts );
        buf_len = strlen( buffer );
        sprintf( & buffer[buf_len], "delta = %d\n", delta_mtr_counts );
        buf_len = strlen( buffer );
        sprintf( & buffer[buf_len], "--------------------------------------------------\n" );
        buf_len = strlen( buffer );
        strftime( & buffer[buf_len], 128, "Page generated at: [%Y-%m-%d, %T]\n", lcu->getLCU_Time() );
        write( fd, buffer, strlen( buffer ) );
    } while( no_quit );

    buf_len = 0;
    goto_alpha_flag = false;
    goto_delta_flag = false;
    lcu->waitSemaphore();
    {
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
    sprintf( & buffer[buf_len], "--------------------------------------------------\n" );
    buf_len = strlen( buffer );
    strftime( & buffer[buf_len], 128, "Page generated at: [%Y-%m-%d, %T]\n", lcu->getLCU_Time() );
    //buf_len = strlen( buffer );
    //sprintf( & buffer[buf_len], "\r\n\r\n" );
    write( fd, buffer, strlen( buffer ) );

    lcu->waitSemaphore();
    lcu->telescope->setIsRunningGoto( false );
    lcu->postSemaphore();
    buf_len = 0;
    sprintf( & buffer[buf_len], "\r\n\r\n" );
    write( fd, buffer, strlen( buffer ) );
    sleep(1);
    buf_len = 0;
    sprintf( & buffer[buf_len], "Good bye!\r\n\r\n" );
    write( fd, buffer, strlen( buffer ) );
    if( verbose)
        printf( "[offset_telescope] Good bye!\n" );
}


