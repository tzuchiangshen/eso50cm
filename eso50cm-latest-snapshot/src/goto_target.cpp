/**
 * 2010 11 18
 * o 23:02 Tracking off after slewing to target.
 *
 */
#include <stdio.h>
#include <sys/uio.h>

#include "myLCU.h"
#include "myUtil.h"

#define NUM_OF_TRY 1


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
        printf( "[goto_target] Hello world!\n" );
        if( * arguments != 0 ) {
            printf( "[goto_target] %s\n", arguments );
        }
    }


    if( verbose) printf( "[goto_target] My file descriptor is %d\n", fd );
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
    buf_len = strlen( buffer );
    strftime( & buffer[buf_len], 128, "Page generated at: [%Y-%m-%d, %T]\n", lcu->getLCU_Time() );
    buf_len = strlen( buffer );

    sprintf( & buffer[buf_len], "\r\n\r\n" );
    buf_len = strlen( buffer );

    sprintf( & buffer[buf_len], page_end );

    write( fd, buffer, strlen( buffer ) );

    if( verbose) printf( "[goto_target] HTML output done!\n" );
    if( verbose) printf( "[goto_target] Good bye!\n" );
}




extern "C" void module_generate_txt( int fd, const char * arguments, myLCU * lcu )
{
    extern int verbose;
    char buffer[1024];
    int  buf_len = 0;
    double lst, ra, dec, alt, az;
    double offset_ra, offset_dec;
    int  alpha_mtr_counts;
    int  delta_mtr_counts;
    char mem_address;
    //int value;
    int no_quit;
    int goto_alpha_flag = false;
    int goto_delta_flag = false;
    int setup_ready;
    char is_telescope_configured = 0;
    double delta_t;

    if( verbose ) {
        printf( "[goto_target] Hello world!\n" );
        if( * arguments != 0 ) {
            printf( "[goto_target] %s\n", arguments );
        }
    }

    if( verbose) printf( "[goto_target] My file descriptor is %d\n", fd );

    /** Create a stream corresponding to the client socket file
        descriptor.  */
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
        if( verbose) printf( "[monitor_position] Telescope not configured... Good bye!\n" );
        return;
    }

    buf_len = 0;
    lcu->waitSemaphore();
    {
        /** Starting goto target */
        lcu->telescope->setIsRunningGoto( true );
        /** Time Elapsed */
        //delta_t = lcu->telescope->getDeltaT();
        //sprintf( & buffer[buf_len], "Time elapsed since last access: %lf sec\n", delta_t );
        //buf_len = strlen( buffer );
        /** Current Position */
        //lcu->telescope->currentPosition( & lst, & ra, & dec, & alt, & az );

        /** Delta T */
        delta_t = lcu->telescope->getDeltaT();
        if( delta_t < 0.8 ) {
            sprintf( & buffer[buf_len], "Time elapsed: %lf sec\n", delta_t );
            buf_len = strlen( buffer );
            lst = lcu->telescope->getLST();
            ra  = lcu->telescope->getRA();
            dec = lcu->telescope->getDec();
            alt = lcu->telescope->getAlt();
            az  = lcu->telescope->getAz();
        } else {
            /** Current Position */
            sprintf( & buffer[buf_len], "Time elapsed: %lf > 0.8 sec: Getting new position\n", delta_t );
            buf_len = strlen( buffer );
            lcu->telescope->currentPosition( & lst, & ra, & dec, & alt, & az );
        }

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
        strfdegs( & buffer[buf_len], 256, "dDec= |%+03d:%02d:%02.0lf|\n\n", offset_dec );
        buf_len = strlen( buffer );

        /** Alt/Az Position */
        sprintf( & buffer[buf_len], "Alt = |%+10.6lf (%+10.6lf)|\n", lcu->telescope->getAlt(), alt  );
        buf_len = strlen( buffer );
        sprintf( & buffer[buf_len], "Az  = |%+10.6lf (%+10.6lf)|\n", lcu->telescope->getAz(), az  );
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

        //if( alpha_mtr_counts < -25 || 25 < alpha_mtr_counts ) {
            //lcu->telescope->alpha->Motor->readDeviceMemory( mem_address, & value  );
            //sprintf( & buffer[buf_len], "alpha.Motor[%d]=%8d\n", mem_address, value );
            //buf_len = strlen( buffer );
            //if( ! lcu->telescope->getIsTracking() ) {
                //value = 0;
                //lcu->telescope->alpha->Motor->setDeviceMemory( 3, & value, 0  );
                //lcu->telescope->setIsTracking( false );
                //sprintf( & buffer[buf_len], "Tracking OFF!\n" );
                //buf_len = strlen( buffer );
            //} else {
            //    lcu->telescope->alpha->Motor->setDeviceMemory( 3, & value, 0  );
            //    sprintf( & buffer[buf_len], "Tracking already ON!\n" );
            //    buf_len = strlen( buffer );
            //}
        // HA 20 = 1/4[arcsec]
        if( alpha_mtr_counts < -20 || 20 < alpha_mtr_counts ) {
            lcu->telescope->alpha->Motor->setDeviceMemory( 7, & alpha_mtr_counts, 0  );
            goto_alpha_flag = true;
        }
        // Dec: 16= 1/4[arcsec]
        //if( delta_mtr_counts < -25 || 25 < delta_mtr_counts ) {
        if( delta_mtr_counts < -16 || 16 < delta_mtr_counts ) {
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
            mem_address = 7;
            lcu->waitSemaphore();
            {
                /** Current Position */
                /** Delta T */
                delta_t = lcu->telescope->getDeltaT();
                if( delta_t < 0.8 ) {
                    sprintf( & buffer[buf_len], "Time elapsed: %lf sec\n", delta_t );
                    buf_len = strlen( buffer );
                    lst = lcu->telescope->getLST();
                    ra  = lcu->telescope->getRA();
                    dec = lcu->telescope->getDec();
                    alt = lcu->telescope->getAlt();
                    az  = lcu->telescope->getAz();
                } else {
                    /** Current Position */
                    sprintf( & buffer[buf_len], "Time elapsed: %lf > 0.8 sec: Getting new position\n", delta_t );
                    buf_len = strlen( buffer );
                    lcu->telescope->currentPosition( & lst, & ra, & dec, & alt, & az );
                }

                if( goto_alpha_flag ) {
                    lcu->telescope->alpha->Motor->readDeviceMemory( 7, & alpha_mtr_counts, 0  );
                    //if( -25 < alpha_mtr_counts && alpha_mtr_counts < 25 )
                    if( alpha_mtr_counts < 0 )
                        goto_alpha_flag = false;
                }
                if( goto_delta_flag ) {
                    lcu->telescope->delta->Motor->readDeviceMemory( 7, & delta_mtr_counts, 0  );
                    //if( -25 < delta_mtr_counts && delta_mtr_counts < 25 )
                    if( delta_mtr_counts < 0 )
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
                //printf( "[goto_target] alpha = %d\n", alpha_mtr_counts );
                //printf( "[goto_target] delta = %d\n", delta_mtr_counts );
                printf( "[goto_target] To quit (if something was wrong)... %d sec.\n", no_quit );
            }
            sprintf( & buffer[buf_len], "alpha = %d\n", alpha_mtr_counts );
            buf_len = strlen( buffer );
            sprintf( & buffer[buf_len], "delta = %d\n", delta_mtr_counts );
            buf_len = strlen( buffer );


            sprintf( & buffer[buf_len], "Telescope [alt = %lf] [az = %lf]\n",
                       lcu->telescope->getAlt(), lcu->telescope->getAz() );

            if( lcu->telescope->getAlt() < lcu->telescope->getLowElevation() + 10.0 ) {
                sprintf( & buffer[buf_len], "Warning!\nTelescope near horizon.\n" );
                buf_len = strlen( buffer );
            } else if( lcu->telescope->getAlt() < lcu->telescope->getLowElevation() ) {
                sprintf( & buffer[buf_len], "Warning!\nTelescope bellow horizon.\n" );
                buf_len = strlen( buffer );
            }

            sprintf( & buffer[buf_len], "--------------------------------------------------\n" );
            //buf_len = strlen( buffer );
            //strftime( & buffer[buf_len], 128, "Page generated at: [%Y-%m-%d, %T]\n", lcu->getLCU_Time() );
            write( fd, buffer, strlen( buffer ) );
        } while( no_quit );

        buf_len = 0;
        goto_alpha_flag = false;
        goto_delta_flag = false;
        lcu->waitSemaphore();
        {
            /** Current Position */
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

            sprintf( & buffer[buf_len], "Telescope [alt = %lf] [az = %lf]\n",
                       lcu->telescope->getAlt(), lcu->telescope->getAz() );
            buf_len = strlen( buffer );

            if( lcu->telescope->getAlt() < lcu->telescope->getLowElevation() + 10.0 ) {
                sprintf( & buffer[buf_len], "Warning!\nTelescope near horizon.\n" );
                buf_len = strlen( buffer );
            } else if( lcu->telescope->getAlt() < lcu->telescope->getLowElevation() ) {
                sprintf( & buffer[buf_len], "Warning!\nTelescope bellow horizon.\n" );
                buf_len = strlen( buffer );
            }

            //if( alpha_mtr_counts < -25 || 25 < alpha_mtr_counts ) {
            // HA 20 = 1/4[arcsec]
            if( alpha_mtr_counts < -20 || 20 < alpha_mtr_counts ) {
                lcu->telescope->alpha->Motor->setDeviceMemory( 7, & alpha_mtr_counts, 0  );
                goto_alpha_flag = true;
            }
            //if( delta_mtr_counts < -25 || 25 < delta_mtr_counts ) {
            // Dec: 16= 1/4[arcsec]
            if( delta_mtr_counts < -16 || 16 < delta_mtr_counts ) {
                lcu->telescope->delta->Motor->setDeviceMemory( 7, & delta_mtr_counts, 0  );
                goto_delta_flag = true;
            }
        }
        lcu->postSemaphore();
        if( ! goto_alpha_flag && ! goto_delta_flag ) {
            setup_ready = 0;
        } else {
            setup_ready --;
        }

        sprintf( & buffer[buf_len], "--------------------------------------------------\n" );
        buf_len = strlen( buffer );
        //strftime( & buffer[buf_len], 128, "Page generated at: [%Y-%m-%d, %T]\n", lcu->getLCU_Time() );
        //write( fd, buffer, strlen( buffer ) );
        if( verbose )
            printf( "[goto_target] Try No %d\n", NUM_OF_TRY - setup_ready );

    } while( setup_ready );

    buf_len = 0;

    //value = 1200;
    //lcu->telescope->alpha->Motor->setDeviceMemory( 3, & value, 0  );
    //lcu->telescope->setIsTracking( true );
    //sprintf( & buffer[buf_len], "Tracking ON!\n" );
    //buf_len = strlen( buffer );
    //sprintf( & buffer[buf_len], "--------------------------------------------------\n" );
    //buf_len = strlen( buffer );
    strftime( & buffer[buf_len], 128, "Page generated at: [%Y-%m-%d, %T]\n", lcu->getLCU_Time() );
    write( fd, buffer, strlen( buffer ) );

    lcu->waitSemaphore();
    lcu->telescope->setIsRunningGoto( false );
    lcu->postSemaphore();


    buf_len = 0;
    sprintf( & buffer[buf_len], "\r\n\r\n" );
    write( fd, buffer, strlen( buffer ) );
    sleep(1);
    buf_len = 0;
    sprintf( & buffer[buf_len], "\r\n\r\n" );
    write( fd, buffer, strlen( buffer ) );



    if( verbose)
        printf( "[goto_target] Good bye!\n" );
}


