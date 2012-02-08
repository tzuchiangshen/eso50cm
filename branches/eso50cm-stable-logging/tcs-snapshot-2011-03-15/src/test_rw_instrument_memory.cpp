/**
 * Compile using:
 * g++ -Wall -g -fPIC -shared -I../include -lm
 *      ../obj/myLCU.o
 *      ../obj/myTelescope.o
 *      ../obj/myTClock.o
 *      ../obj/myTAxis.o
 *      ../obj/myTMotor.o
 *      ../obj/myTEncoder.o
 *      ../obj/myBSemaphore.o
 *      -o example1/lcu_test.so lcu_test.cpp
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
    "  <h2 align=\"left\">test_rw_instrument_memory</h2>\n"
    "  <pre>\n";

/** HTML source for the end of the process listing page.  */
static char* page_end = 
    "  </pre>\n"
    " </body>\n"
    "</html>\n";

extern "C" void module_generate ( int fd, const char * arguments, myLCU * lcu )
{
    extern int verbose;
    char string_data[32];
    char buffer[128];
    char mem_address;
    int value;

    if( verbose ) {
        printf( "[test_rw_instrument_memory] Hello world!\n" );
        if( * arguments != 0 ) {
            printf( "[test_rw_instrument_memory] %s\n", arguments );
        }
    }
    if( verbose) printf( "[test_rw_instrument_memory] My file descriptor is %d\n", fd );

    /** Create a stream corresponding to the client socket file
    descriptor.  */
    write( fd, page_start, strlen( page_start ) );

    sprintf( buffer, "Read Test:\n" );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "----------\n\n" );
    write( fd, buffer, strlen( buffer ) );
    mem_address = 4;
    lcu->telescope->alpha->Motor->readDeviceMemory( mem_address, & value  );
    sprintf( buffer, "alpha.Motor[%d]=%8d  ", mem_address, value );
    write( fd, buffer, strlen( buffer ) );
    lcu->telescope->delta->Motor->readDeviceMemory( mem_address, & value  );
    sprintf( buffer, "delta.Motor[%d]=%8d\n", mem_address, value );
    write( fd, buffer, strlen( buffer ) );

    lcu->telescope->alpha->WormE->readDeviceMemory( mem_address, & value  );
    sprintf( buffer, "alpha.WormE[%d]=%8d  ", mem_address, value );
    write( fd, buffer, strlen( buffer ) );
    lcu->telescope->delta->WormE->readDeviceMemory( mem_address, & value  );
    sprintf( buffer, "delta.WormE[%d]=%8d\n", mem_address, value );
    write( fd, buffer, strlen( buffer ) );

    lcu->telescope->alpha->AxisE->readDeviceMemory( mem_address, & value  );
    sprintf( buffer, "alpha.AxisE[%d]=%8d  ", mem_address, value );
    write( fd, buffer, strlen( buffer ) );
    lcu->telescope->delta->AxisE->readDeviceMemory( mem_address, & value  );
    sprintf( buffer, "delta.AxisE[%d]=%8d\n", mem_address, value );
    write( fd, buffer, strlen( buffer ) );

    sprintf( buffer, "\nRead/Write Test:\n" );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "----------------\n\n" );
    write( fd, buffer, strlen( buffer ) );
    mem_address = 0;
    lcu->telescope->alpha->Motor->readDeviceMemory( mem_address, & value  );
    sprintf( buffer, "alpha.Motor[%d]=%8d  ", mem_address, value );
    write( fd, buffer, strlen( buffer ) );
    value += 1024;
    lcu->telescope->alpha->Motor->setDeviceMemory( mem_address, & value  );
    lcu->telescope->alpha->Motor->readDeviceMemory( mem_address, & value  );
    sprintf( buffer, "alpha.Motor[%d]=%8d\n", mem_address, value );
    write( fd, buffer, strlen( buffer ) );

    lcu->telescope->delta->Motor->readDeviceMemory( mem_address, & value  );
    sprintf( buffer, "delta.Motor[%d]=%8d  ", mem_address, value );
    write( fd, buffer, strlen( buffer ) );
    value += 2048;
    lcu->telescope->delta->Motor->setDeviceMemory( mem_address, & value  );
    lcu->telescope->delta->Motor->readDeviceMemory( mem_address, & value  );
    sprintf( buffer, "delta.Motor[%d]=%8d\n", mem_address, value );
    write( fd, buffer, strlen( buffer ) );

    /** LCU Local Time */
    sprintf( buffer, "\n--------------------------------------------------\n" );
    write( fd, buffer, strlen( buffer ) );
    strftime( string_data, 32, "%Y-%m-%d, %T", lcu->getLCU_Time() );
    sprintf( buffer, "Information generated at: [%s]\n", string_data );
    write( fd, buffer, strlen( buffer ) );

    write( fd, page_end, strlen( page_end ) );

    if( verbose) printf( "[test_rw_instrument_memory] Good bye!\n" );
}

