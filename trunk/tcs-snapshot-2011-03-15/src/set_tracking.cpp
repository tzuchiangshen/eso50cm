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
    //char string_data[32];
    int trg_ra_hrs;
    int trg_ra_min;
    int trg_ra_sec;

    int trg_dec_deg;
    int trg_dec_min;
    int trg_dec_sec;

    double trg_ra = 0;
    double trg_dec = 0;

    if( verbose ) printf( "[set_tracking] Hello world!\n" );
    if( * arguments != 0 ) {
        if( verbose ) printf( "[set_tracking] has arguments %s\n", arguments );
        char * ptr = NULL;

        if( (ptr = strstr( arguments, "ra=" )) != NULL ) {
            if( sscanf( ptr, "ra=%d:%d:%d", & trg_ra_hrs, & trg_ra_min, & trg_ra_sec ) == 3 ) {
                if( verbose ) printf( "[set_tracking] trg ra =%+03d:%02d:%02d\n", trg_ra_hrs, trg_ra_min, trg_ra_sec );
                trg_ra  = fabs( (double) trg_ra_hrs ) + ((double) trg_ra_min / 60.) + ((double) trg_ra_sec / 3600.);
                trg_ra *= 15.0;
                if( trg_ra_hrs < 0 ) {
                    trg_ra *= -1.;
                } else if( trg_ra_hrs == 0 && ( strstr( arguments, "ra=-" ) != NULL) ) {
                    trg_ra *= -1.;
                }
                if( verbose ) printf( "[set_tracking] trg ra =%lf\n", trg_ra );
            } else {
                if( verbose ) printf( "[set_tracking] No target ra data\n" );
            }
        }
        if( (ptr = strstr( arguments, "dec=" )) != NULL ) {
            if( sscanf( ptr, "dec=%d:%d:%d", & trg_dec_deg, & trg_dec_min, & trg_dec_sec ) == 3 ) {
                if( verbose ) printf( "[set_tracking] trg dec=%+03d:%02d:%02d\n", trg_dec_deg, trg_dec_min, trg_dec_sec );
                trg_dec  = fabs( (double) trg_dec_deg ) + ((double) trg_dec_min / 60.) + ((double) trg_dec_sec / 3600.);
                if( trg_dec_deg < 0 ) {
                    trg_dec *= -1.;
                } else if( trg_dec_deg == 0 && ( strstr( arguments, "dec=-" ) != NULL) ) {
                    trg_dec *= -1.;
                }
                if( verbose ) printf( "[set_tracking] trg dec=%lf\n", trg_dec );
            } else {
                if( verbose ) printf( "[set_tracking] No target dec data\n" );
            }
        }
    }

    char buffer[1024];
    int  buf_len = 0;
    double trg_alt;
    double trg_az;

    if( verbose) printf( "[set_tracking] My file descriptor is %d\n", fd );
    /** Create a stream corresponding to the client socket file
        descriptor.  */
    sprintf( & buffer[buf_len], page_start );
    buf_len = strlen( buffer );

    lcu->waitSemaphore();
    if( lcu->telescope->getIsConfigured() == 0 ) {
        sprintf( & buffer[buf_len], "Information: Please, config telescope first!\n" );
        buf_len = strlen( buffer );
    } else {
        /** Local Sidereal Time */
        strfdegs( & buffer[buf_len], 128, "LST = [%+03d:%02d:%02.0lf]  ", lcu->telescope->getLST() / 15.0 );
        buf_len = strlen( buffer );
        /** Local Time */
        strftime( & buffer[buf_len], 128, "LT  = [%Y-%m-%d, %T]\n", lcu->telescope->getLocalTime() );
        buf_len = strlen( buffer );
        sprintf( & buffer[buf_len], "--------------------------------------------------\n" );
        buf_len = strlen( buffer );
        if( lcu->telescope->setTarget( trg_ra, trg_dec, & trg_alt, & trg_az  ) == 0 ) {
            /** Target bellow Horizon */
            sprintf( & buffer[buf_len], "Target bellow horizon!\n" );
            buf_len = strlen( buffer );
            sprintf( & buffer[buf_len], "Alt = [%+lf] < %lf\n", trg_alt, lcu->telescope->getLowElevation() );
            buf_len = strlen( buffer );
            sprintf( & buffer[buf_len], "Az  = [%+lf]\n", trg_az );
            buf_len = strlen( buffer );
        } else {
            /** Target above Horizon */
            sprintf( & buffer[buf_len], "Target accepted!\n" );
            buf_len = strlen( buffer );
            sprintf( & buffer[buf_len], "--------------------------------------------------\n" );
            buf_len = strlen( buffer );
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

    if( verbose) printf( "[set_tracking] Good bye!\n" );
}



extern "C" void module_generate_txt( int fd, const char * arguments, myLCU * lcu )
{
    extern int verbose;

    char    buffer[1024];
    int     buf_len                 = 0;
    char    help_flag               = 0;
    char    tracking_flag           = 255;
    char    is_telescope_configured = 0;
    //double  delta_t                 = 0.;
    double  degs_per_sec            = 0.;
    int     tics_per_sec            = 0;

    if( verbose ) printf( "[set_tracking] Hello world!\n" );
    if( * arguments != 0 ) {
        if( verbose ) printf( "[set_tracking] has arguments %s\n", arguments );
        char * ptr = NULL;

        if( (ptr = strstr( arguments, "help" )) != NULL ) {
            help_flag = 1;
        }

        if( (ptr = strstr( arguments, "on" )) != NULL ) {
            tracking_flag = 1;
        } else if( (ptr = strstr( arguments, "off" )) != NULL ) {
            tracking_flag = 0;
        }
        if( (ptr = strstr( arguments, "dps=" )) != NULL ) {
            if( sscanf( ptr, "dps=%lf", & degs_per_sec ) == 1 ) {
                if( verbose ) printf( "[set_tracking] Requesting Traking Speed = %lf degs/sec\n", degs_per_sec );
            } else {
                if( verbose ) printf( "[set_tracking] No degs/sec specified.\n" );
            }
        }
        if( (ptr = strstr( arguments, "tps=" )) != NULL ) {
            if( sscanf( ptr, "tps=%d", & tics_per_sec ) == 1 ) {
                if( verbose ) printf( "[set_tracking] Requesting Traking Speed = %d tics/sec\n", tics_per_sec );
            } else {
                if( verbose ) printf( "[set_tracking] No tics/sec specified.\n" );
            }
        }
    }

    if( tracking_flag == 1 ) {
        tics_per_sec = 600;
    } else if( tracking_flag == 1 ) {
        tics_per_sec = 0;
    } else {
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
            lcu->telescope->alpha->Motor->setDeviceMemory( 3, & tics_per_sec, 0  );
            if( tics_per_sec > 0 ) {
                lcu->telescope->setIsTracking( true );
                sprintf( & buffer[buf_len], "Tracking ON!\n  tps=%d\n", tics_per_sec );
                buf_len = strlen( buffer );
            } else {
                lcu->telescope->setIsTracking( false );
                sprintf( & buffer[buf_len], "Tracking OFF!\n" );
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

    if( verbose) printf( "[set_tracking] Good bye!\n" );
}


