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
    "  <h2 align=\"left\">Telescope Status</h2>\n"
    "  <table cellpadding=\"2\" cellspacing=\"0\" border=\"1\">\n"
    "   <thead>\n" 
    "    <tr>\n" 
    "     <th>Vaiable</th>\n"
    "     <th>Value</th>\n" 
    "    </tr>\n"
    "   </thead>\n" 
    "   <tbody>\n";

static char * line_format =
    "<tr><td align=\"left\">%s</td><td align=\"right\">%s</td></tr>\n";

/** HTML source for the end of the process listing page.  */
static char* page_end = 
    "   </tbody>\n"
    "  </table>\n"
    " </body>\n"
    "</html>\n";

/** Generate an HTML table for the information.  */
static char * format_info( const char * name, const char * value )
{
    size_t result_length;
    char * result;

    /** Compute the length of the string we'll need to hold the result, and
        allocate memory to hold it.  */
    result_length = strlen( name ) + strlen( value ) + strlen( line_format ) + 16 ;
    result = (char *) malloc( result_length );
    /** Format the result.  */
    snprintf( result, result_length, line_format, name, value );

    /** All done.  */
    return result;
}


extern "C" void module_generate ( int fd, const char * arguments, myLCU * lcu )
{
    extern int verbose;
    char string_data[32];

    if( verbose ) {
        printf( "[module_generate] Hello world!\n" );
        if( * arguments != 0 ) {
            printf( "[module_generate] %s\n", arguments );
        }
    }
    //myLCU * lcu;
    //lcu = new myLCU();
    //lcu->createTelescope();
    double lst, ra, dec;

    /** Create a stream corresponding to the client socket file
        descriptor.  */
    if( verbose) printf( "[module_generate] My file descriptor is %d\n", fd );
    /** Attach Instrument Shared memory segment.
        We will need it to access instrument low level data.*/
    //lcu->telescope->attachInstrumentMemory();
    /** We will wait untill we get the access to the
        User Shared Memory segment. */
    lcu->waitSemaphore();
    /** We have gor access to the User Shared Memory segment
        then we can translate low level telescope data
        to user's format.
        The corresponding telescope routine waits until it
        have access to Instrument Shared memory
        segment, and after read it will free the acces. */
    lcu->telescope->currentPosition( & lst, & ra, & dec );
    /** We will generate the output: for fd > 0  */
    if( fd > 0 ) {
        /** The number of elements in the array that we've used.  */
        size_t vec_length = 0;
        /** The allocated size of the array.  */
        size_t vec_size = 32;
        /** The array of iovcec elements.  */
        struct iovec * vec = (struct iovec *) malloc( vec_size * sizeof( struct iovec ) );
        char * formated_line;

        if( vec != NULL ) {
            if( verbose) printf( "[module_generate] Generating HTML output!\n" );
            /** The first buffer is the HTML source for the start of the page.  */
            vec[vec_length].iov_base = page_start;
            vec[vec_length].iov_len  = strlen( page_start );
            ++vec_length;

            /** The buffers with the HTML source for the each line.  */

            strftime( string_data, 32, "%Y-%m-%d, %T", lcu->telescope->getLocalTime() );
            formated_line = format_info( "LT", string_data );
            vec[vec_length].iov_base = formated_line;
            vec[vec_length].iov_len  = strlen( formated_line );
            ++vec_length;

            /** Alpha Worm */
            snprintf( string_data, 32, "%+10.0lf", lcu->telescope->alpha->WormE->getPosition() );
            formated_line = format_info( "Alpha Worm", string_data );
            vec[vec_length].iov_base = formated_line;
            vec[vec_length].iov_len  = strlen( formated_line );
            ++vec_length;
            /** Alpha Axis */
            snprintf( string_data, 32, "%+10.0lf", lcu->telescope->alpha->AxisE->getPosition() );
            formated_line = format_info( "Alpha Axis", string_data );
            vec[vec_length].iov_base = formated_line;
            vec[vec_length].iov_len  = strlen( formated_line );
            ++vec_length;
            /** Delta Worm */
            snprintf( string_data, 32, "%+10.0lf", lcu->telescope->delta->WormE->getPosition() );
            formated_line = format_info( "Delta Worm", string_data );
            vec[vec_length].iov_base = formated_line;
            vec[vec_length].iov_len  = strlen( formated_line );
            ++vec_length;
            /** Delta Axis */
            snprintf( string_data, 32, "%+10.0lf", lcu->telescope->delta->AxisE->getPosition() );
            formated_line = format_info( "Delta Axis", string_data );
            vec[vec_length].iov_base = formated_line;
            vec[vec_length].iov_len  = strlen( formated_line );
            ++vec_length;
            /** LUC local time */
            strftime( string_data, 32, "%Y-%m-%d, %T", lcu->getLCU_Time() );
            formated_line = format_info( "Page Generated at", string_data );
            vec[vec_length].iov_base = formated_line;
            vec[vec_length].iov_len  = strlen( formated_line );
            ++vec_length;
            /** Add one last buffer with HTML that ends the page.  */
            vec[vec_length].iov_base = page_end;
            vec[vec_length].iov_len = strlen (page_end);
            ++vec_length;

            /** Output the entire page to the client file descriptor all at once.  */
            writev( fd, vec, vec_length );

            /** Deallocate the buffers we created.  The first and last are static 
                and should not be deallocated.  */
            for( size_t i = 1; i < vec_length - 1; ++i ) {
                free( vec[i].iov_base );
            }
            /** Deallocate the iovec array.  */
            free( vec );

            if( verbose) printf( "[module_generate] HTML output done!\n" );
        }
    } else {
        printf( "[module_generate] No HTML output!\n" );
        /** Local Time */
        strftime( string_data, 32, "%Y-%m-%d, %T", lcu->telescope->getLocalTime() );
        printf( "[module_generate] LT  = [%s]\n", string_data );
        printf( "[module_generate] AW Position  = [%+10.0lf]\n", lcu->telescope->alpha->WormE->getPosition() );
        printf( "[module_generate] AA Position  = [%+10.0lf]\n", lcu->telescope->alpha->AxisE->getPosition() );
        printf( "[module_generate] DW Position  = [%+10.0lf]\n", lcu->telescope->delta->WormE->getPosition() );
        printf( "[module_generate] DA Position  = [%+10.0lf]\n", lcu->telescope->delta->AxisE->getPosition() );

        /** LCU Local Time */
        printf( "[module_generate] --------------------------------\n" );
        strftime( string_data, 32, "%Y-%m-%d, %T", lcu->getLCU_Time() );
        printf( "[module_generate] Page generated at: [%s]\n", string_data );
    }
    lcu->postSemaphore();
    //delete lcu;

    if( verbose) printf( "[module_generate] Good bye!\n" );
}

