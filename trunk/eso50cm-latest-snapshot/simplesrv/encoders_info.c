#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>        /** semaphores */
#include <sys/stat.h>

#include <assert.h>

#include <sys/time.h>   //gettimeofday
#include <time.h>       //gmtime

#include <stdlib.h>     //free

#include "server.h"

#define SHMKEY   0xFAFAFA00
#define SEMKEY   0xFAFAFA00


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


/**
 * Semaphore
 */
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
    "  <h2 align=\"left\">Encoders Info</h2>\n"
    "  <table cellpadding=\"3\" cellspacing=\"0\" border=\"1\">\n"
    "   <thead>\n" 
    "    <tr>\n" 
    "     <th>I2C Address</th>\n"
    "     <th>Description</th>\n" 
    "     <th>Position</th>\n" 
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
static char * format_encoder_info( unsigned int i2c_address, const char * description, int data ) { 

    size_t result_length;
    char* result;


    /** Compute the length of the string we'll need to hold the result, and 
        allocate memory to hold it.  */
    result_length = strlen( description ) + 128;
    result = (char*) xmalloc( result_length );
    /** Format the result.  */
    snprintf( result, result_length, 
            "<tr><td align=\"center\">0x%02X</td><td><tt>%s</tt></td><td align=\"right\">%d</td></tr>\n", 
            i2c_address, description, data );
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

    char * encoder_info;

    /** The number of elements in the array that we've used.  */
    size_t vec_length = 0;
    /** The allocated size of the array.  */
    size_t vec_size = 16;
    /** The array of iovcec elements.  */
    struct iovec * vec = 
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
        for( i = 0; i < 6; i ++ ) {
            encoder_info = format_encoder_info( telescope->encoder[i].i2c_address, "...", telescope->encoder[i].data[4] );
            if( vec_length == vec_size - 1 ) { 
                vec_size *= 2;
                vec = xrealloc (vec, vec_size * sizeof (struct iovec));
            }
            vec[vec_length].iov_base = encoder_info;
            vec[vec_length].iov_len = strlen( encoder_info );
            ++vec_length;
        }

        binary_semaphore_post( semaphore_id );

        /** Detach the shared memory segment */
        shmdt( shared_memory );
    } else {
        encoder_info = "<tr><td colspan=\"3\" align=\"center\">Unable to Comply!</td></tr>";

        /** Make sure the iovec array is long enough to hold this buffer 
            (plus one more because we'll add an extra element when we're done 
            listing processes).  If not, grow it to twice its current size.  */
        if (vec_length == vec_size - 1) { 
            vec_size *= 2;
            vec = xrealloc (vec, vec_size * sizeof (struct iovec));
        }
        /** Store this buffer as the next element of the array.  */
        vec[vec_length].iov_base = encoder_info;
        vec[vec_length].iov_len = strlen( encoder_info );
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

