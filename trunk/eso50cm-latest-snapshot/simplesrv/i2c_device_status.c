#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>        /** semaphores */
#include <sys/stat.h>

#include <assert.h>

#include <sys/time.h>   //gettimeofday

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



/** A template for the HTML page this module generates.  */ 
"<html>\n"
"<head>\n"
"<title>myTelescope</title>"
"</head>\n"
" <body>\n"
"  <h2 align=\"left\">%s</h2>\n"
"  <p>0x%02X %d.</p>\n"
"  <p>0x%02X %d.</p>\n"
"  <p>0x%02X %d.</p>\n"
"  <p>0x%02X %d.</p>\n"
"  <p>0x%02X %d.</p>\n"
"  <p>0x%02X %d.</p>\n"
"  <p>%s</p>\n"
" </body>\n"
"</html>\n";

static char* page_error = 
"<html>\n"
"<head>\n"
"<title>myTelescope</title>"
"</head>\n"
" <body>\n"
"  <h2 align=\"left\">Telescope Devices Status</h2>\n"
"  <p> Unable to comply.</p>\n" 
"  <p> seamaphore_id: %d</p>\n" 
"  <p> segment_id   : %d</p>\n" 
" </body>\n"
"</html>\n";

void module_generate (int fd) {
    FILE* fp;
    int segment_id;
    char * shared_memory;
    struct shmid_ds shmbuffer;
    int segment_size;
    const int shared_segment_size = 1024;
    int semaphore_id;

    struct telescope_data_t * telescope;

    /** Create a stream corresponding to the client socket file
        descriptor.  */
    fp = fdopen (fd, "w");
    assert (fp != NULL);

    /** Allocate a semaphore */
    semaphore_id = binary_semaphore_allocate( SEMKEY,  S_IRUSR | S_IWUSR );

    /** Allocate a shared memory segment */
    segment_id = shmget( SHMKEY, shared_segment_size, S_IRUSR | S_IWUSR );
    if( semaphore_id > 0 &&  segment_id > 0 ) {
        /** Attach the shared memory segment */
        shared_memory = (char *) shmat( segment_id, 0, 0 );
        printf( "sahred memory attached at adress %p\n", shared_memory );
        telescope = (struct telescope_data_t *) shared_memory;

        /** Determine the segment size */
        shmctl( segment_id, IPC_STAT, & shmbuffer );
        segment_size = shmbuffer.shm_segsz;
        printf( "segment size: %d\n", segment_size );

        /** Generate the HTML output.  */
        binary_semaphore_wait( semaphore_id );
        localtime_r( & (telescope->gtime.tv_sec), & LTime  );
        strftime( infoline, 24, "%Y-%m-%d %T", & LTime );
        printf( page_template,
                    "I2C Devices status",
                    (unsigned int) telescope->encoder[0].i2c_address, telescope->encoder[0].i2c_status,
                    (unsigned int) telescope->encoder[1].i2c_address, telescope->encoder[1].i2c_status,
                    (unsigned int) telescope->encoder[2].i2c_address, telescope->encoder[2].i2c_status,
                    (unsigned int) telescope->encoder[3].i2c_address, telescope->encoder[3].i2c_status,
                    (unsigned int) telescope->encoder[4].i2c_address, telescope->encoder[4].i2c_status,
                    (unsigned int) telescope->encoder[5].i2c_address, telescope->encoder[5].i2c_status,
                    infoline );
        binary_semaphore_post( semaphore_id );    

        /** Detach the shared memory segment */
        shmdt( shared_memory );
    } else {
        fprintf (fp, page_error, semaphore_id,  segment_id );
    }

    /** All done; flush the stream.  */
    fflush (fp);
}

