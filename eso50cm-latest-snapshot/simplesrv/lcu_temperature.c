#include<unistd.h>
#include<sys/mman.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>

#include <sys/time.h>   //gettimeofday
#include <time.h>       //gmtime
#include <assert.h>

#include "spi.c"

/*
Technologic systems TS-7800 temperature sensor.
Updated to utilize spi.c.
*/

#define DIOBASE 0xE8000000
#define CS_PIN 31

#define RW_REG(ptr) *(ptr + 0x08/sizeof(unsigned int))

/** A template for the HTML page this module generates.  */ 
static char* page_template = 
"<html>\n"
"<head>\n"
"<title>myTelescope</title>"
"</head>\n"
" <body>\n"
"  <h2 align=\"left\">LCU Temperature</h2>\n"
"  <p>LT: %s.</p>\n"
"  <p>T: %f[oC].</p>\n"
" </body>\n"
"</html>\n";



float tempC( unsigned int data ) {
    float ret;

    if (data & (1 << 15)) {  /* negative temperatures: */
        ret = -0.0625 * (~((data >> 3) | 0xFFFFE000) + 1);
    } else {               /* positive temperatures: */
        ret = .0625 * (data >> 3);  /* 3 final bits are irrelevant */
    }
    return ret;
}

float CtoF( float x ) {
    return x * 9/5 + 32;
}

void module_generate( int fd ) {
    struct tm LTime;
    struct timeval  gtime;
    struct timezone tzone;
    char time[32];

    FILE* fp;
    unsigned int rawtemp = 0;
    volatile unsigned int * dioptr;
    int mem_fd = open( "/dev/mem", O_RDWR | O_SYNC );

    dioptr = (unsigned int *) mmap( 0, getpagesize(),
                PROT_READ|PROT_WRITE, MAP_SHARED, mem_fd, DIOBASE );

    RW_REG(dioptr) &= ~(1 << CS_PIN); // make sure temp sensor not chip selected
    init_spi(); // call init_spi before selecting an SPI device
    RW_REG(dioptr) ^= (1 << CS_PIN); // temp sensor chip select
    rawtemp = spi8(0) << 8;
    rawtemp |= spi8(0);

    /**  printf("%X\n", tempdata(dioptr)); */
    //printf("The temperature in Fahrenheit is %f\n", CtoF(tempC(rawtemp)));


    /** Create a stream corresponding to the client socket file
        descriptor.  */
    fp = fdopen (fd, "w");
    assert (fp != NULL);

    //local time
    gettimeofday( & gtime, & tzone );
    localtime_r( & (gtime.tv_sec), & LTime  );
    strftime( time, 24, " %T", & LTime );

    /** Generate the HTML output with the temperature. */
    //printf("The temperature in Celcius is... %f\n", tempC(rawtemp));
    fprintf (fp, page_template, time, tempC(rawtemp));


    /** All done; flush the stream.  */
    fflush( fp );
}

