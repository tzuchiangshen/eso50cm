/**
 *
 *
 */
#include <stdio.h>

#include "myLCU.h"
#include <sys/uio.h>


/** HTML source for the start of the process listing page.  */
static char* page_start = 
    "<html>\n" 
    "<head>\n"
    "<title>myTelescope</title>"
    "</head>\n"
    " <body>\n"
    "  <h2 align=\"left\">Current Configuration</h2>\n"
    "  <pre>\n";

/** HTML source for the end of the process listing page.  */
static char* page_end = 
    "  </pre>\n"
    " </body>\n"
    "</html>\n";

extern "C" void module_generate( int fd, const char * arguments, myLCU * lcu )
{
    extern int verbose;
    char string_data[32];
    char buffer[128];

    if( verbose ) {
        printf( "[get_config] Hello world!\n" );
        if( * arguments != 0 ) {
            printf( "[get_config] %s\n", arguments );
        }
    }

    if( verbose) printf( "[get_config] My file descriptor is %d\n", fd );

    /** Create a stream corresponding to the client socket file
        descriptor.  */
    write( fd, page_start, strlen( page_start ) );

    lcu->waitSemaphore();

    sprintf( buffer, "Is Config.=      %d\n", (int) lcu->telescope->getIsConfigured() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "Lat       = %+11.4lf \n", lcu->telescope->getLatitude() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "Lng       = %+11.4lf \n", lcu->telescope->getLongitude() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "Alt       = %+11.4lf \n", lcu->telescope->getAltitude() );
    write( fd, buffer, strlen( buffer ) );

    sprintf( buffer, "AMT       = %+11.4lf \n", lcu->telescope->alpha->Motor->getTicsPerRev() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "AMH       = %+11.4lf \n", lcu->telescope->alpha->Motor->getHomePosition() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "AMR       = %+11.4lf \n", lcu->telescope->alpha->Motor->getEncoderToAxis_Reduction() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "AWT       = %+11.4lf \n", lcu->telescope->alpha->WormE->getTicsPerRev() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "AWH       = %+11.4lf \n", lcu->telescope->alpha->WormE->getHomePosition() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "AWR       = %+11.4lf \n", lcu->telescope->alpha->WormE->getEncoderToAxis_Reduction() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "AAT       = %+11.4lf \n", lcu->telescope->alpha->AxisE->getTicsPerRev() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "AAH       = %+11.4lf \n", lcu->telescope->alpha->AxisE->getHomePosition() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "AAR       = %+11.4lf \n", lcu->telescope->alpha->AxisE->getEncoderToAxis_Reduction() );
    write( fd, buffer, strlen( buffer ) );

    sprintf( buffer, "DMT       = %+11.4lf \n", lcu->telescope->delta->Motor->getTicsPerRev() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "DMH       = %+11.4lf \n", lcu->telescope->delta->Motor->getHomePosition() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "DMR       = %+11.4lf \n", lcu->telescope->delta->Motor->getEncoderToAxis_Reduction() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "DWT       = %+11.4lf \n", lcu->telescope->delta->WormE->getTicsPerRev() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "DWH       = %+11.4lf \n", lcu->telescope->delta->WormE->getHomePosition() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "DWR       = %+11.4lf \n", lcu->telescope->delta->WormE->getEncoderToAxis_Reduction() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "DAT       = %+11.4lf \n", lcu->telescope->delta->AxisE->getTicsPerRev() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "DAH       = %+11.4lf \n", lcu->telescope->delta->AxisE->getHomePosition() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "DAR       = %+11.4lf \n", lcu->telescope->delta->AxisE->getEncoderToAxis_Reduction() );
    write( fd, buffer, strlen( buffer ) );
    lcu->postSemaphore();

    /** LCU Local Time */
    sprintf( buffer, "--------------------------------------------------\n" );
    write( fd, buffer, strlen( buffer ) );
    strftime( string_data, 32, "%Y-%m-%d, %T", lcu->getLCU_Time() );
    sprintf( buffer, "Information generated at: [%s]\n", string_data );
    write( fd, buffer, strlen( buffer ) );

    write( fd, page_end, strlen( page_end ) );

    if( verbose) printf( "[get_config] Good bye!\n" );
}

extern "C" void module_generate_txt( int fd, const char * arguments, myLCU * lcu )
{
    extern int verbose;
    char string_data[32];
    char buffer[128];

    if( verbose ) {
        printf( "[get_config] Hello world!\n" );
        if( * arguments != 0 ) {
            printf( "[get_config] %s\n", arguments );
        }
    }
    //myLCU * lcu;
    //lcu = new myLCU();
    //lcu->createTelescope();

    if( verbose) printf( "[get_config] My file descriptor is %d\n", fd );

    /** Create a stream corresponding to the client socket file
        descriptor.  */
    //sprintf( buffer, "\r\n\r\n" );
    //write( fd, buffer, strlen( buffer ) );

    lcu->waitSemaphore();

    sprintf( buffer, "Is Config.=      %d\n", (int) lcu->telescope->getIsConfigured() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "Lat       = %+11.4lf \n", lcu->telescope->getLatitude() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "Lng       = %+11.4lf \n", lcu->telescope->getLongitude() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "Alt       = %+11.4lf \n", lcu->telescope->getAltitude() );
    write( fd, buffer, strlen( buffer ) );

    sprintf( buffer, "AMT       = %+11.4lf \n", lcu->telescope->alpha->Motor->getTicsPerRev() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "AMH       = %+11.4lf \n", lcu->telescope->alpha->Motor->getHomePosition() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "AMR       = %+11.4lf \n", lcu->telescope->alpha->Motor->getEncoderToAxis_Reduction() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "AWT       = %+11.4lf \n", lcu->telescope->alpha->WormE->getTicsPerRev() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "AWH       = %+11.4lf \n", lcu->telescope->alpha->WormE->getHomePosition() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "AWR       = %+11.4lf \n", lcu->telescope->alpha->WormE->getEncoderToAxis_Reduction() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "AAT       = %+11.4lf \n", lcu->telescope->alpha->AxisE->getTicsPerRev() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "AAH       = %+11.4lf \n", lcu->telescope->alpha->AxisE->getHomePosition() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "AAR       = %+11.4lf \n", lcu->telescope->alpha->AxisE->getEncoderToAxis_Reduction() );
    write( fd, buffer, strlen( buffer ) );

    sprintf( buffer, "DMT       = %+11.4lf \n", lcu->telescope->delta->Motor->getTicsPerRev() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "DMH       = %+11.4lf \n", lcu->telescope->delta->Motor->getHomePosition() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "DMR       = %+11.4lf \n", lcu->telescope->delta->Motor->getEncoderToAxis_Reduction() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "DWT       = %+11.4lf \n", lcu->telescope->delta->WormE->getTicsPerRev() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "DWH       = %+11.4lf \n", lcu->telescope->delta->WormE->getHomePosition() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "DWR       = %+11.4lf \n", lcu->telescope->delta->WormE->getEncoderToAxis_Reduction() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "DAT       = %+11.4lf \n", lcu->telescope->delta->AxisE->getTicsPerRev() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "DAH       = %+11.4lf \n", lcu->telescope->delta->AxisE->getHomePosition() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "DAR       = %+11.4lf \n", lcu->telescope->delta->AxisE->getEncoderToAxis_Reduction() );
    write( fd, buffer, strlen( buffer ) );
    lcu->postSemaphore();

    /** LCU Local Time */
    sprintf( buffer, "--------------------------------------------------\n" );
    write( fd, buffer, strlen( buffer ) );
    strftime( string_data, 32, "%Y-%m-%d, %T", lcu->getLCU_Time() );
    sprintf( buffer, "Page generated at: [%s]\n", string_data );
    write( fd, buffer, strlen( buffer ) );

    sprintf( buffer, "\r\n\r\n" );
    write( fd, buffer, strlen( buffer ) );

    if( verbose) printf( "[get_config] Good bye!\n" );
}

