#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>        /** semaphores */
#include <sys/stat.h>

#include <assert.h>

#include <sys/time.h>   //gettimeofday
#include <time.h>       //gmtime
#include <math.h>

#include <stdlib.h>     //free

#include "server.h"

#define SHMKEY   0xFAFAFA00
#define SEMKEY   0xFAFAFA00


//-------------------------------------------------------------
//  Local Sidereal Time
#define C0      280.46061837
#define C1      360.98564736629
#define C2        0.000387933
#define C3     38710000.0
#define JD2000      2451545.0

//-------------------------------------------------------------
//  Site
#define LNGSCL      -70.55
#define LATSCL      -33.26
#define ALTOUC     1450.0
/**
 * Joulian Date
 */
double julianDate( struct tm ut ) {
    int A, B, AA, MM;
    double x1, x2, x3, x4, jd;

    if( ut.tm_mon == 0 || ut.tm_mon == 1 ){
        AA = ut.tm_year + 1899;
        MM = ut.tm_mon + 14; //= mes+1+12=(date->tm_mon + 1) + 1 + 12
    } else {
        AA = ut.tm_year + 1900;
        MM = ut.tm_mon + 2;  //= mes+1= (date->tm_mon + 1) + 1
    }

    A= (int)floor( (double)AA / 100.0 );
    B= 2 - A + (int)floor( A / 4.0 );

    x1  = ( 365.25 * ( AA + 4716.0 ) );
    x2  = ( 30.6001 * MM ); //= ( 30.6001 * mes + 1 )
    x3 =  ( (double)ut.tm_sec + (double)ut.tm_min * 60.0 +
            (double)ut.tm_hour * 3600.0 ) / 86400.0 +
            (int)ut.tm_mday;
    x4  = B - 1524.5;
    jd= floor(x1)+floor(x2)+x3+x4;

    return jd;
}

/**
 * Local Sidereal Time
 */
double localSiderealTime( double jd, struct tm * lstime ) {
    double T, Theta;
    double lst, hrs, hh, mm, ss;
    int    dias;

    T= ( jd - JD2000 ) / 36525.0;
    Theta= ( (C0 + C1 * ( jd - JD2000 ) + C2 * T*T - T*T*T / C3) + LNGSCL );     // lst en grados
    dias = (int)(Theta / 360.0);

    //lst in degrees
    lst =  Theta - ((double) dias) * 360.0;

    //lst in hours
    hrs   = lst / 15.0;

    //current lst in hrs, mins, secs.
    hh    = floor( hrs );
    mm    = floor( (hrs - hh)*60.0 );
    ss    = (hrs - (hh + mm/60.0))*3600.0;

    //copy data into structure
    lstime->tm_hour = (int) hh;
    lstime->tm_min  = (int) mm;
    lstime->tm_sec  = (int) ss;
    if( lstime->tm_sec >= 60 ) {
        lstime->tm_sec -= 60;
        lstime->tm_min ++;
    }

    //return miliseconds of the current lst
    return ( ss - lstime->tm_sec ) * 1000;
}

/**
 *  Sempahore
 */
struct encoder_data_t {
    char i2c_address;
    char i2c_status;
    int data[8];
};

struct telescope_data_t {
    struct timeval  gtime;
    struct timezone tzone;

    struct encoder_data_t encoder[6];

    char last[128];
    char msg[128];
    char quit;
};


union semun {
    int                 val;
    struct semid_ds     * buf;
    unsigned short int  * array;
    struct seminfo      * __buf;
};


int binary_semaphore_allocate( key_t key, int sem_flags ) {
    return semget( key, 1, sem_flags );
}

int binary_semaphore_deallocate( int sem_id ) {
    union semun ignored_argument;
    return semctl( sem_id, 1, IPC_RMID, ignored_argument );
}

int binary_semaphore_initialize( int sem_id ) {
    unsigned short value[1];
    union semun argument;

    value[0] = 1;
    argument.array = value;
    return semctl( sem_id, 0, SETALL, argument );
}

int binary_semaphore_wait( int sem_id ) {
    struct sembuf operations[1];

    operations[0].sem_num = 0;
    operations[0].sem_op  = -1;
    operations[0].sem_flg = SEM_UNDO;

    return semop( sem_id, operations, 1 );
}

int binary_semaphore_post( int sem_id ) {
    struct sembuf operations[1];    //one operation

    operations[0].sem_num = 0;
    operations[0].sem_op  = 1;
    operations[0].sem_flg = SEM_UNDO;

    return semop( sem_id, operations, 1 );
}



/** HTML source for the start of the process listing page.  */
static char* page_start = 
    "<html>\n" 
    "<head>\n"
    "<title>myTelescope</title>"
    "</head>\n"
    " <body>\n"
    "  <h2 align=\"left\">Time Info</h2>\n"
    "  <table cellpadding=\"4\" cellspacing=\"0\" border=\"1\">\n" 
    "   <thead>\n" 
    "    <tr>\n" 
    "     <th>Coordinate</th>\n"
    "     <th>Value</th>\n" 
    "    </tr>\n" 
    "   </thead>\n" 
    "   <tbody>\n";

/** HTML source for the end of the process listing page.  */

static char* page_end = 
    "   </tbody>\n" 
    "  </table>\n" 
    " </body>\n" 
    "</html>\n";

/** Generate an HTML table row for process PID.  The return value is a 
    pointer to a buffer that the caller must deallocate with free, or 
    NULL if an error occurs.  */
static char * format_time_info( const char * description, const char * info ) {

    size_t result_length;
    char* result;


    /** Compute the length of the string we'll need to hold the result, and 
        allocate memory to hold it.  */
    result_length = strlen( description ) + strlen( info ) + 128;
    result = (char*) xmalloc( result_length );
    /** Format the result.  */
    snprintf( result, result_length,
                "<tr><td align=\"center\">%s</td><td align=\"center\">%s</td></tr>\n",
                description, info );
    /** Clean up.  */

    /** All done.  */
    return result;
}

void module_generate (int fd) {
    int i;

    int segment_id;
    char * shared_memory;
    struct shmid_ds shmbuffer;
    int segment_size;
    const int shared_segment_size = 1024;
    int semaphore_id;

    struct telescope_data_t * telescope;

    struct tm Time;
    double JDate;
    struct tm LSidT;
    char line[32];
    char * time_info;

    /** The number of elements in the array that we've used.  */
    size_t vec_length = 0;
    /** The allocated size of the array.  */
    size_t vec_size = 16;
    /** The array of iovcec elements.  */
    struct iovec* vec = 
        (struct iovec*) xmalloc (vec_size * sizeof (struct iovec));

    /** The first buffer is the HTML source for the start of the page.  */
    vec[vec_length].iov_base = page_start;
    vec[vec_length].iov_len = strlen (page_start);
    ++vec_length;

    /** Allocate a semaphore */
    semaphore_id = binary_semaphore_allocate( SEMKEY,  S_IRUSR | S_IWUSR );

    /** Allocate a shared memory segment */
    segment_id = shmget( SHMKEY, shared_segment_size, S_IRUSR | S_IWUSR );
    if( semaphore_id > 0 &&  segment_id > 0 ) {
        /** Attach the shared memory segment */
        shared_memory = (char *) shmat( segment_id, 0, 0 );
        //printf( "sahred memory attached at adress %p\n", shared_memory );
        telescope = (struct telescope_data_t *) shared_memory;

        /** Determine the segment size */
        shmctl( segment_id, IPC_STAT, & shmbuffer );
        segment_size = shmbuffer.shm_segsz;
        //printf( "segment size: %d\n", segment_size );

        /** Generate the HTML output.  */
        binary_semaphore_wait( semaphore_id );

        gmtime_r( & (telescope->gtime.tv_sec), & Time  );
        JDate = julianDate( Time );
        localSiderealTime( JDate, & LSidT );

        strftime( line, 24, " %Y-%m-%d", & Time );
        time_info = format_time_info( "Universal Date", line );
        if( vec_length == vec_size - 1 ) {
            vec_size *= 2;
            vec = xrealloc (vec, vec_size * sizeof (struct iovec));
        }
        vec[vec_length].iov_base = time_info;
        vec[vec_length].iov_len = strlen( time_info );
        ++vec_length;

        strftime( line, 24, " %T", & Time );
        time_info = format_time_info( "Universal Time", line );
        if( vec_length == vec_size - 1 ) {
            vec_size *= 2;
            vec = xrealloc (vec, vec_size * sizeof (struct iovec));
        }
        vec[vec_length].iov_base = time_info;
        vec[vec_length].iov_len = strlen( time_info );
        ++vec_length;

        localtime_r( & (telescope->gtime.tv_sec), & Time  );
        strftime( line, 24, " %Y-%m-%d", & Time );
        time_info = format_time_info( "Loca Date", line );
        if( vec_length == vec_size - 1 ) {
            vec_size *= 2;
            vec = xrealloc (vec, vec_size * sizeof (struct iovec));
        }
        vec[vec_length].iov_base = time_info;
        vec[vec_length].iov_len = strlen( time_info );
        ++vec_length;

        strftime( line, 24, " %T", & Time );
        time_info = format_time_info( "Local Time", line );
        if( vec_length == vec_size - 1 ) {
            vec_size *= 2;
            vec = xrealloc (vec, vec_size * sizeof (struct iovec));
        }
        vec[vec_length].iov_base = time_info;
        vec[vec_length].iov_len = strlen( time_info );
        ++vec_length;

        //sprintf( line, "%15.6lf", JDate );
        //strftime( line, 24, " %T", & LSidT );

        binary_semaphore_post( semaphore_id );

        /** Detach the shared memory segment */
        shmdt( shared_memory );
    } else {
        time_info = "<tr><td colspan=\"2\" align=\"center\">Unable to Comply!</td></tr>";

        /** Make sure the iovec array is long enough to hold this buffer 
            (plus one more because we'll add an extra element when we're done 
            listing processes).  If not, grow it to twice its current size.  */
        if (vec_length == vec_size - 1) { 
            vec_size *= 2;
            vec = xrealloc (vec, vec_size * sizeof (struct iovec));
        }
        /** Store this buffer as the next element of the array.  */
        vec[vec_length].iov_base = time_info;
        vec[vec_length].iov_len = strlen( time_info );
        ++vec_length;
    }

    /** Add one last buffer with HTML that ends the page.  */
    vec[vec_length].iov_base = page_end;
    vec[vec_length].iov_len = strlen (page_end);
    ++vec_length;

    /** Output the entire page to the client file descriptor all at once.  */
    writev (fd, vec, vec_length);

    /** Deallocate the buffers we created.  The first and last are static 
        and should not be deallocated.  */
    for( i = 1; i < vec_length - 1; ++i )
        free( vec[i].iov_base );
    /** Deallocate the iovec array.  */
    free( vec );
}

