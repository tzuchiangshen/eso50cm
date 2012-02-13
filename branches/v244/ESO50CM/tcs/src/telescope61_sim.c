#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/stat.h>

#include <sys/time.h>   //gettimeofday
#include <time.h>       //gmtime

#include <math.h>
#include <string.h>
#include <signal.h>     // sigaction
#include <sys/wait.h>   // wait

#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <errno.h>      //errno
//-------------------------------------------------------------
//  Instrument Shared memory and semaphore
#define SHMKEY   0xFAFAFA00
#define SEMKEY   0xFAFAFA00
#define RDSEMKEY 0xFAFAFA02
//-------------------------------------------------------------
//  User Shared memory and semaphore
#define USRSHMKEY   0xFAFAFA01
#define USRSEMKEY   0xFAFAFA01

#include "./telescope.h"

/* Global variable to exit from main loop */
char quit;

struct encoder_data_t {
    char i2c_address;
    char i2c_status;
    char message[16];
    char answer[16];
    int data[8];
};

struct telescope_data_t {
    struct timeval  gtime;
    struct timezone tzone;

    struct encoder_data_t encoder[6];

    char last[128];
    char msg[128];
    char new_data;
    char quit;
};

/**
 *  Stream status
 */
int stream_status( int m_port ) {
    fd_set fds;
    struct timeval tv;
    int retval;

    FD_ZERO( & fds );
    FD_SET( m_port, & fds );
    tv.tv_sec = 0;
    tv.tv_usec = 100;


    if( ( retval = select( m_port + 1, & fds, NULL, NULL, & tv ) ) < 0  ) {
        printf( "Error on select()\n" );
    }
    return retval;
}

/**
 * Handler for SIGCHLD, to clean up child processes that have
 * terminated.
 */
static void clean_up_child_process( int signal_number ) {
    int status;
    wait( & status );
}

/**
 * Handle a client connection on the file descriptor CONNECTION_FD.
 */
void handle_connection (int connection_fd)
{
}

static void exit_handler(int s)
{
  if(verbose)
    printf("telescope61::exit_handler Ctrl+C signal catched!!!");
  quit = 1; 
}

void myMemcpy(char *dst, char *src, unsigned int size) {
    //TSH: don't understand why memcpy doesn't work. Until so, I will use mine version
    //memcpy(telescope_sim->encoder[i].data[mem_address], &telescope->encoder[i].message[3], 4);
    char *ptr = (char*)dst;
    int i=0;
    for( i=0; i<size; i++) {
       *ptr = *src;
       ptr++;
       src++;
    }   
}

/**
 *  abs_alpha motor_enc_to_worm_enc
 */
int abs_alpha_motor_enc_to_worm_enc(int motor_enc) {
    //see configuration file
    return (int)(281.0 + (motor_enc * 1024.0 * 360.0 / 17280.0 / 3000.0));
}

/**
 *  abs_delta motor_enc_to_worm_enc
 */
int abs_delta_motor_enc_to_worm_enc(int motor_enc) {
    //see configuration files for delta 
    return (int)(794.0 + (motor_enc * 1024.0 * -288.0 / 13824.0 / 3000.0));
}   

/**
 *  abs_alpha motor_enc_to_axis_enc
 */
int abs_alpha_motor_enc_to_axis_enc(int motor_enc) {
    //see configuration file
    return (int)(510.0 + (motor_enc * 1024.0 * 1.5 / 17280.0 / 3000.0));
}

/**
 *  abs_delta motor_enc_to_axis_enc
 */
int abs_delta_motor_enc_to_axis_enc(int motor_enc) {
    //see configuration files for delta 
    return (int)(741.0 + (motor_enc * 1024 * 1.0 / 13824.0 / 3000.0));
}

/**
 *  rel_alpha motor_enc_to_worm_enc
 */
int rel_alpha_motor_enc_to_worm_enc(int motor_enc) {
    //see configuration file
    return (int)(motor_enc * 1024.0 * 360.0 / 17280.0 / 3000.0);
}

/**
 *  rel_delta motor_enc_to_worm_enc
 */
int rel_delta_motor_enc_to_worm_enc(int motor_enc) {
    //see configuration files for delta 
    return (int)(motor_enc * 1024.0 * -288.0 / 13824.0 / 3000.0);
}   

/**
 *  rel_alpha motor_enc_to_axis_enc
 */
int rel_alpha_motor_enc_to_axis_enc(int motor_enc) {
    //see configuration file
    return (int)(motor_enc * 1024.0 * 1.5 / 17280.0 / 3000.0);
}

/**
 *  delta motor_enc_to_axis_enc
 */
int rel_delta_motor_enc_to_axis_enc(int motor_enc) {
    //see configuration files for delta 
    return (int)(motor_enc * 1024 * 1.0 / 13824.0 / 3000.0);
}/**
 *  set_encoders_home_position
 */
void set_encoders_home_positions(struct telescope_data_t * telescope) {

    //TODO: read it from the conf file.
    int alpha_axis_home = 510;
    int alpha_worm_home = 281;
    int delta_axis_home = 741;
    int delta_worm_home = 794;
    char *src, *dst;

    //A-A
    dst = (char*)&telescope->encoder[3].data[4];
    src = (char*)&alpha_axis_home;
    myMemcpy(dst, src, 4);
    dst = (char*)&telescope->encoder[3].data[2];
    src = (char*)&alpha_axis_home;
    myMemcpy(dst, src, 4);
    printf("[telescope_run] Set alpha axis home position, value = %d\n", alpha_axis_home);
    //A-W
    dst = (char*)&telescope->encoder[2].data[4];
    src = (char*)&alpha_worm_home;
    myMemcpy(dst, src, 4);
    dst = (char*)&telescope->encoder[2].data[2];
    src = (char*)&alpha_worm_home;
    myMemcpy(dst, src, 4);
    printf("[telescope_run] Set alpha axis home position, value = %d\n", alpha_worm_home);
    //D-A
    dst = (char*)&telescope->encoder[5].data[4];
    src = (char*)&delta_axis_home;
    myMemcpy(dst, src, 4);
    dst = (char*)&telescope->encoder[5].data[2];
    src = (char*)&delta_axis_home;
    myMemcpy(dst, src, 4);
    printf("[telescope_run] Set delta axis home position, value = %d\n", delta_axis_home);
    //D-W
    dst = (char*)&telescope->encoder[4].data[4];
    src = (char*)&delta_worm_home;
    myMemcpy(dst, src, 4);
    dst = (char*)&telescope->encoder[4].data[2];
    src = (char*)&delta_worm_home;
    myMemcpy(dst, src, 4);
    printf("[telescope_run] Set delta worm home position, value = %d\n", delta_worm_home);
}

/**
 *  telescope run
 */
void telescope_run( const char * device, speed_t baudrate, const char * socket_name ) {
    //int verbose;
    const size_t shared_segment_size = 1024;

    int semaphore_id;
    int read_semaphore_id;
    int segment_id;
    char * shared_memory;
    size_t segment_size;


    int user_semaphore_id;
    int user_segment_id;
    char * user_shared_memory;
    size_t  user_segment_size;

    struct shmid_ds shmbuffer;

    int retval;


    struct tm UTime;
    struct tm LTime;
    struct timeval  gtime;
    struct timezone tzone;
    double JDate;
    struct tm LSTime;
    struct timeval  start_time;
    struct timeval  up_time;
    struct tm StartTime;

    struct timespec req, rem;

    struct telescope_data_t * telescope;
    struct telescope_data_t * telescope_sim;

    int i;
    int j;
    int k;
    char * line;

    int fd_stdin;
    int fd_rs232;
    char infoline[64];
    char buffer[128];
    char tty_buffer[256];
    char msg_buffer[256];
    int rx_msg_length;
    char * shm_message;

    int new_timep_flag;
    int new_message_flag;
    int new_ttymssg_flag;
    int checksum_error_flag;
    int timeout_error_flag;
    int message_length;
    int next_bin_msg;
    int tty_msg_len;
    int rx;

    char bin_message[128];
    int  bin_message_len;

    int wait_for_message;
    int nums_of_time_outs;

    int * ptr_to_int;
    char mem_address;
    int reply_flag;
    int sec;

    double startT, endT;

    //int usr2bin_fd;
    //int bin2usr_fd;
    char m_line[128];
    int bytes_read;
    int length;
    unsigned int uint_tmp;

    struct sigaction sigchld_action;
    struct sigaction sigExitHandler;

    int init_ok_flag;


    do {
        /* Manage CTRL+C and kill signals */
        sigExitHandler.sa_handler = & exit_handler;
        sigemptyset(&sigExitHandler.sa_mask);
        sigExitHandler.sa_flags = 0;
        sigaction(SIGINT, &sigExitHandler, NULL);
    sigaction(SIGTERM, &sigExitHandler, NULL);

        init_ok_flag = 1;
        //shared_memory = NULL;
        //segment_id   = 0;
        //semaphore_id = 0;
        //read_semaphore_id = 0;
        //user_shared_memory = NULL;
        //user_semaphore_id = 0;
        //user_segment_id   = 0;
        //fd_rs232 = 0;

        /** Install a handler for SIGCHLD that cleans up child processes that
        have terminated.  */
        if( verbose )
            printf( "[telescope_run] Installing a handler for SIGCHLD...\n" );
        memset( & sigchld_action, 0, sizeof( sigchld_action ) );
        sigchld_action.sa_handler = & clean_up_child_process;
        if( sigaction( SIGCHLD, & sigchld_action, NULL ) < 0 ) {
            perror( "[telescope_run] sigaction" );
            if( verbose )
                printf( "[telescope_run] sigaction ERROR!\n" );
            init_ok_flag = 0;
        }
        if( verbose )
            printf( "[telescope_run] sigaction OK!\n" );

        /**
        * INSTRUMENT SHARED MEMORY
        */
        /** Allocate a semaphore (write) */
        if( verbose )
            printf( "[telescope_run] allocate semaphore for Instrument Memory (write)...\n" );
        semaphore_id = binary_semaphore_allocate( SEMKEY, 
                            IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR );
        if( semaphore_id == 0 ) {
            if( verbose )
                printf( "[telescope_run] semaphore_id = 0\n" );
            init_ok_flag = 0;
        } else  if( semaphore_id < 0 ) {
            perror( "[telescope_run] semget" );
            if( verbose )
                printf( "[telescope_run] semget ERROR!\n" );
            init_ok_flag = 0;
        }
        if( verbose )
            printf( "[telescope_run] semaphore_id = %d\n", semaphore_id );

        /** Semaphore initialization (write) */
        retval = binary_semaphore_initialize( semaphore_id );

        /** Allocate a semaphore (read) */
        if( verbose )
            printf( "[telescope_run] allocate semaphore for Instrument Memory (read)...\n" );
        read_semaphore_id = binary_semaphore_allocate( RDSEMKEY,
                                IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR );
        if( read_semaphore_id == 0 ) {
            if( verbose )
                printf( "[telescope_run] read_semaphore_id = 0\n" );
            init_ok_flag = 0;
        } else if( read_semaphore_id < 0 ) {
            perror( "[telescope_run] semget" );
            if( verbose )
                printf( "[telescope_run] semget ERROR!\n" );
            init_ok_flag = 0;
        }
        if( verbose )
            printf( "[telescope_run] semaphore_id = %d\n", read_semaphore_id );

        /** Semaphore initialization */
        retval = binary_semaphore_initialize( read_semaphore_id );

        /** Allocate and deallocate a shared memory segment */
        if( verbose )
            printf( "[telescope_run] Allocate and deallocate a shared memory segment.\n" );
        if( verbose )
            printf( "[telescope_run] shmget for Instrument Memory...\n" );
        segment_id = shmget( SHMKEY, shared_segment_size,
                             IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR );
        if( verbose )
            printf( "[telescope_run] shmctl for Instrument Memory...\n" );
        if( shmctl( segment_id, IPC_RMID, 0 ) < 0 ) {
            perror( "[telescope_run] shmctl");
            if( verbose )
                printf( "[telescope_run] shmctl ERROR.\n");
        }

        /** Allocate a shared memory segment */
        if( verbose )
            printf( "[telescope_run] shmget for Instrument Memory...\n" );
        segment_id = shmget( SHMKEY, shared_segment_size, 
                            IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR );
        if( segment_id == 0 ) {
            if( verbose )
                printf( "[telescope_run] segment_id = 0\n" );
            init_ok_flag = 0;
        } else if( segment_id < 0 ) {
            perror( "[telescope_run] shmget" );
            if( verbose )
                printf( "[telescope_run] shmget ERROR!\n" );
            init_ok_flag = 0;
        }
        if( verbose )
            printf( "[telescope_run] segment_id = %d\n", segment_id );

        /** Attach the shared memory segment */
        if( verbose )
            printf( "[telescope_run] shmat for Instrument Memory...\n" );
        shared_memory = (char *) shmat( segment_id, 0, 0 );
        if( (void *) user_shared_memory == NULL ) {
            perror( "[telescope_run] shmat" );
            if( verbose )
                printf( "[telescope_run] shmat ERROR!\n" );
            init_ok_flag = 0;
        }
        init_ok_flag = 1;
        if( verbose )
            printf( "[telescope_run] Instrument Shared Memory attached at adress %p\n", (void *) shared_memory );

        /** Determine the segment size */
        shmctl( segment_id, IPC_STAT, & shmbuffer );
        segment_size = shmbuffer.shm_segsz;
        if( verbose ) {
            printf( "[telescope_run] instrument segment size          = %d\n", segment_size );
            printf( "[telescope_run] sizeof( struct telescope_data_t )= %d\n", sizeof( struct telescope_data_t ) );
        }
        telescope = (struct telescope_data_t *) shared_memory;
        telescope_sim = (struct telescope_data_t*)malloc(sizeof(struct telescope_data_t));
        memset(telescope_sim, 0, sizeof(struct telescope_data_t));

        /**
         * USER SHARED MEMORY
         */
        /** Allocate a semaphore */
        if( verbose )
            printf( "[telescope_run] allocate semaphore for User Memory...\n" );
        user_semaphore_id = binary_semaphore_allocate( USRSEMKEY,
                                                IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR );
        if( user_semaphore_id == 0 ) {
            if( verbose )
                printf( "[telescope_run] user_semaphore_id = 0\n" );
            init_ok_flag = 0;
        } else if( user_semaphore_id < 0 ) {
            perror( "[telescope_run] semget" );
            if( verbose )
                printf( "[telescope_run] semget ERROR!\n" );
            init_ok_flag = 0;
        }
        if( verbose )
            printf( "[telescope_run] user_semaphore_id = %d\n", user_semaphore_id );

        /** Initialize semaphore */
        retval = binary_semaphore_initialize( user_semaphore_id );

        /** Allocate a shared memory segment */
        if( verbose )
            printf( "[telescope_run] shmget for User Memory...\n" );
        user_segment_id = shmget( USRSHMKEY, shared_segment_size,
                            IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR );
        if( user_segment_id < 0 ) {
            if( verbose )
                printf( "[telescope_run] user_segment_id = 0\n" );
            init_ok_flag = 0;
        } else if( user_segment_id < 0 ) {
            perror( "[telescope_run] shmget" );
            if( verbose )
                printf( "[telescope_run] shmget ERROR!\n" );
            init_ok_flag = 0;
        }
        if( verbose )
            printf( "[telescope_run] user_segment_id = %d\n", user_segment_id );

        /** Attach the shared memory segment */
        if( verbose )
            printf( "[telescope_run] shmat for User Memory...\n" );
        user_shared_memory = (char *) shmat( user_segment_id, 0, 0 );
        if( (void *) user_shared_memory == NULL ) {
            perror( "[telescope_run] shmat" );
            if( verbose )
                printf( "[telescope_run] shmat ERROR!\n" );
            init_ok_flag = 0;
        }
        if( verbose )
            printf( "[telescope_run] User Shared Memory attached at adress %p\n", (void *) user_shared_memory );

        /** Determine the segment size */
        shmctl( user_segment_id, IPC_STAT, & shmbuffer );
        user_segment_size = shmbuffer.shm_segsz;
        if( verbose ) {
            printf( "[telescope_run] user segment size: %d\n", segment_size );
        }

        printf("-------------->init_ok_flag=%d\n", init_ok_flag);


        /**
         * File descriptor for standard in
         */
        fd_stdin = fileno( stdin );

        if( fd_stdin == 0 ) {
            init_ok_flag = 1;
            if( verbose )
                printf( "[telescope_run] fd_stdin = 0\n" );
        } else if( fd_stdin < 0 ) {
            init_ok_flag = 0;
            perror( "[telescope_run] fileno" );
            if( verbose )
                printf( "[telescope_run] fileno ERROR.\n" );
        }

    } while( 0 );

    if( init_ok_flag ) {
        if( verbose )
            printf( "[telescope_run] Everything looks OK!\n[telescope_run] Starting main loop\n" );

        /** User Shared Memory Initialization */
        binary_semaphore_wait( user_semaphore_id );
        memset( user_shared_memory, 0, user_segment_size );
        binary_semaphore_post( user_semaphore_id );

        /** Instrument Shared Memory Initialization*/
        quit = 0;
        binary_semaphore_wait( semaphore_id );

        gettimeofday( & (telescope->gtime), & (telescope->tzone) );
        localtime_r( & (telescope->gtime.tv_sec), & LTime  );
        strftime( infoline, 24, "%Y-%m-%d %T", & LTime );
        if( verbose ) printf( "[telescope_run] Hello Simulated World! %s\n", infoline );

        telescope->encoder[0].i2c_address = 0xA2;
        telescope->encoder[1].i2c_address = 0xA4;
        telescope->encoder[2].i2c_address = 0xA6;
        telescope->encoder[3].i2c_address = 0xA8;
        telescope->encoder[4].i2c_address = 0xAA;
        telescope->encoder[5].i2c_address = 0xAC;

        telescope->encoder[0].i2c_status = 1;
        telescope->encoder[1].i2c_status = 1;
        telescope->encoder[2].i2c_status = 1;
        telescope->encoder[3].i2c_status = 1;
        telescope->encoder[4].i2c_status = 1;
        telescope->encoder[5].i2c_status = 1;

        telescope->new_data = 0;
        telescope->quit = quit;
        binary_semaphore_post( semaphore_id );


        /** MAIN LOOP */
        new_message_flag    = 0;
        new_ttymssg_flag    = 0;
        reply_flag          = 0;
        checksum_error_flag = 0;
        new_timep_flag      = 0;

        rx_msg_length       = 0;
        message_length      = 0;
        bin_message_len     = 0;
        sec                 = 120;
        j                   = 0;


        set_encoders_home_positions(telescope);
        do {
            /** This is the main loop */

            /** Check if there are availabe bytes in the /dev/ttySX */
            new_ttymssg_flag    = 0;
            wait_for_message    = 0;
            nums_of_time_outs   = 0;
            timeout_error_flag  = 0;
            rx_msg_length       = 0;
            /**
            * Checks for a message in the instrument shared memory... SIMULATION
            */
            //int enc_count;
            //enc_count = 0;
            char *ptr;
            char *src;
            char *dst;
            if( bin_message_len == 0 ) {
                for( i = 0; i < 6; i ++ ) {
                    binary_semaphore_wait( semaphore_id );
                    if( telescope->encoder[i].message[0] == ':' ) {
                        gettimeofday( & gtime, & tzone );
                        startT  = ((double) gtime.tv_usec)/1000000.;
                        startT += (double) gtime.tv_sec;

                        int check_sum = 0;

                        int message_type = telescope->encoder[i].message[1] % 2;
                        int mem_address  = telescope->encoder[i].message[2];
                        if(message_type == 0) {
                            printf("\n[telescope_run] --------------------SET ---------------------\n");
                        } else {
                            printf("\n[telescope_run] --------------------GET----------------------\n");
                        }
                        printf( "[telescope_run] New message received from LCUControl, to be sent to PIC for 0x%02X mem_address=%d message=%d\n", telescope->encoder[i].message[1], mem_address, *(int*)&telescope->encoder[i].message[3]);
                        if(message_type == 0) {
                            //even => comes from setDeviceMemory, save the encoder value for later the next readDeviceMemory()
                            //Emulate sending data to the PIC (RS232)
                            //it will save the target encoder value in a internal memory structure 
                            //when the readMemory is received, it will return the target encoder value
                            length = 0;
                            nums_of_time_outs = 0;
                            timeout_error_flag = 0;

                            //copy message to data
                            dst = (char*)&telescope->encoder[i].data[mem_address];
                            src = (char*)&telescope->encoder[i].message[3];
                            myMemcpy(dst, src, 4);
							if (ultra_verbose)
                                printf( "[telescope_run] saving received meesage to telescope->encoder[%d].data[%d] = %d\n", 
                                                          i, 
                                                          mem_address, 
                                                          *(int *)&telescope->encoder[i].data[mem_address]);
                            //copy message to answer
                            dst = (char*)&telescope->encoder[i].answer[3];
                            src = (char*)&telescope->encoder[i].message[3];
                            myMemcpy(dst, src, 4);
							if (ultra_verbose)
                                printf( "[telescope_run] preparing answer to the requested message, telescope->encoder[%d].message = 0x%X 0x%X 0x%X 0x%X val=%d\n", 
                                                          i, 
                                                          telescope->encoder[i].message[3], 
                                                          telescope->encoder[i].message[4], 
                                                          telescope->encoder[i].message[5], 
                                                          telescope->encoder[i].message[6], 
                                                          *(int *)&telescope->encoder[i].message[3]);

                            if ( mem_address == 7)  {
                                //it's a command to move the ALPHA motor, we need to adjust the equivalent values in the enc of the WORM in order to 
                                //simluate the telescope movement, but the encoder count needs to be transform from motor->worm
                                if ( telescope->encoder[i].message[1] == 0xA2) {
                                    int worm_enc = 0;
                                    int motor_enc = 0;
                                    int axis_enc = 0;
                                    int current_val = 0; 
                                    //alpha-motor, then set the alpha-worm encoder 
                                    // 1.1. save the incremental motor enc movement at 0xA2, mem_address=4
                                    dst = (char*)&motor_enc;
                                    src = (char*)&telescope->encoder[i].message[3];
                                    myMemcpy(dst, src, 4);
                                    current_val = 0; 
                                    dst = (char*)&current_val;
                                    src = (char*)&telescope->encoder[0].data[4];
                                    myMemcpy(dst, src, 4);
                                    current_val += motor_enc;
                                    dst = (char*)&telescope->encoder[0].data[4];
                                    src = (char*)&current_val;
                                    myMemcpy(dst, src, 4);
							        if (ultra_verbose)
                                        printf("[telescope_run] Simulate abs movement of aplha-motor at motor encoder (0xA2 mem_address=4), value = %d\n", motor_enc);
                                    // 1.2. save the incremental motor enc movement at 0xA2, mem_address=2
                                    dst = (char*)&motor_enc;
                                    src = (char*)&telescope->encoder[i].message[3];
                                    myMemcpy(dst, src, 4);
                                    current_val = 0; 
                                    dst = (char*)&current_val;
                                    src = (char*)&telescope->encoder[0].data[2];
                                    myMemcpy(dst, src, 4);
                                    current_val += motor_enc;
                                    dst = (char*)&telescope->encoder[0].data[2];
                                    src = (char*)&current_val;
                                    myMemcpy(dst, src, 4);
							        if (ultra_verbose)
                                        printf("[telescope_run] Simulate abs movement of aplha-motor at motor encoder (0xA2 mem_address=2), value = %d\n", motor_enc);
                                    // 1.3. convert the incremental motor enc into worm enc 
                                    worm_enc = rel_alpha_motor_enc_to_worm_enc(motor_enc);
							        if (ultra_verbose)
                                        printf("[telescope_run] converted alpha motor enc to alpha worm enc (0xA6 mem_address=4), motor enc = %d, worm enc=%d\n", motor_enc,  worm_enc);
                                    // 1.4. save the incremental worm enc movement at 0xA6, mem_address=4
                                    current_val = 0; 
                                    dst = (char*)&current_val;
                                    src = (char*)&telescope->encoder[2].data[4];
                                    myMemcpy(dst, src, 4);
                                    current_val += worm_enc;
                                    dst = (char*)&telescope->encoder[2].data[4];
                                    src = (char*)&current_val;
                                    myMemcpy(dst, src, 4);
							        if (ultra_verbose)
                                        printf("[telescope_run] Simulate movement at worm encoder (0xA6 mem_address=4), value = %d\n", worm_enc);
                                    // 1.5. save the incremental worm enc movement at 0xA6, mem_address=2
                                    current_val = 0; 
                                    dst = (char*)&current_val;
                                    src = (char*)&telescope->encoder[2].data[2];
                                    myMemcpy(dst, src, 4);
                                    current_val += worm_enc;
                                    dst = (char*)&telescope->encoder[2].data[2];
                                    src = (char*)&current_val;
                                    myMemcpy(dst, src, 4);
							        if (ultra_verbose)
                                        printf("[telescope_run] Simulate abs movement of at alpha worm encoder (0xA6 mem_address=2), value = %d\n", worm_enc);
                                    // 1.6. convert the incremental motor enc into axis enc 
                                    axis_enc = rel_alpha_motor_enc_to_axis_enc(motor_enc);
							        if (ultra_verbose)
                                        printf("[telescope_run] converted alpha motor enc to alpha axis enc (0xA6 mem_address=4), motor enc = %d, axis enc=%d\n", motor_enc,  axis_enc);
                                    // 1.7. save the incremental axis enc movement at 0xA8, mem_address=4
                                    current_val = 0; 
                                    dst = (char*)&current_val;
                                    src = (char*)&telescope->encoder[3].data[4];
                                    myMemcpy(dst, src, 4);
                                    current_val += axis_enc;
                                    dst = (char*)&telescope->encoder[3].data[4];
                                    src = (char*)&current_val;
                                    myMemcpy(dst, src, 4);
							        if (ultra_verbose)
                                        printf("[telescope_run] Simulate movement at alpha axis encoder (0xA8 mem_address=4), value = %d current=%d \n", axis_enc, current_val);
                                    // 1.8. save the incremental axis enc movement at 0xA8, mem_address=2
                                    current_val = 0;
                                    dst = (char*)&current_val;
                                    src = (char*)&telescope->encoder[3].data[2];
                                    myMemcpy(dst, src, 4);
                                    current_val += axis_enc;
                                    dst = (char*)&telescope->encoder[3].data[2];
                                    src = (char*)&current_val;
                                    myMemcpy(dst, src, 4);
							        if (ultra_verbose)
                                        printf("[telescope_run] Simulate movement at alpha axis encoder (0xA8 mem_address=2), value = %d\n", axis_enc);
                                } else if( telescope->encoder[i].message[1] == 0xA4 ) {
                                    int worm_enc = 0;
                                    int motor_enc = 0;
                                    int axis_enc = 0;
                                    int current_val = 0; 
                                    //delta-motor, then set the alpha-worm encoder 
                                    // 1.1. save the incremental motor enc movement at 0xA4, mem_address=4
                                    dst = (char*)&motor_enc;
                                    src = (char*)&telescope->encoder[i].message[3];
                                    myMemcpy(dst, src, 4);
                                    current_val = 0; 
                                    dst = (char*)&current_val;
                                    src = (char*)&telescope->encoder[1].data[4];
                                    myMemcpy(dst, src, 4);
                                    printf ("current = %d, motor_enc=%d\n", current_val, motor_enc);
                                    current_val += motor_enc;
                                    dst = (char*)&telescope->encoder[1].data[4];
                                    src = (char*)&current_val;
                                    myMemcpy(dst, src, 4);
							        if (ultra_verbose)
                                        printf("[telescope_run] Simulate abs movement at delta motor encoder (0xA4 mem_address=4), value = %d\n", motor_enc);
                                    // 1.2. save the incremental motor enc movement at 0xA4, mem_address=2
                                    dst = (char*)&motor_enc;
                                    src = (char*)&telescope->encoder[i].message[3];
                                    myMemcpy(dst, src, 4);
                                    current_val = 0; 
                                    dst = (char*)&current_val;
                                    src = (char*)&telescope->encoder[1].data[2];
                                    myMemcpy(dst, src, 4);
                                    current_val += motor_enc;
                                    dst = (char*)&telescope->encoder[1].data[2];
                                    src = (char*)&current_val;
                                    myMemcpy(dst, src, 4);
							        if (ultra_verbose)
                                        printf("[telescope_run] Simulate abs movement at delta motor encoder (0xA4 mem_address=2), value = %d\n", motor_enc);
                                    // 1.3. convert the incremental motor enc into worm enc 
                                    worm_enc = rel_delta_motor_enc_to_worm_enc(motor_enc);
							        if (ultra_verbose)
                                        printf("[telescope_run] converted alpha motor enc to alpha worm enc (0xA6 mem_address=4), motor enc = %d, worm enc=%d\n", motor_enc,  worm_enc);
                                    // 1.4. save the incremental worm enc movement at 0xAA, mem_address=4
                                    current_val = 0; 
                                    dst = (char*)&current_val;
                                    src = (char*)&telescope->encoder[4].data[4];
                                    myMemcpy(dst, src, 4);
                                    current_val += worm_enc;
                                    dst = (char*)&telescope->encoder[4].data[4];
                                    src = (char*)&current_val;
                                    myMemcpy(dst, src, 4);
							        if (ultra_verbose)
                                        printf("[telescope_run] Simulate movement at worm encoder (0xA6 mem_address=4), value = %d\n", worm_enc);
                                    // 1.5. save the incremental worm enc movement at 0xAA, mem_address=2
                                    current_val = 0; 
                                    dst = (char*)&current_val;
                                    src = (char*)&telescope->encoder[4].data[2];
                                    myMemcpy(dst, src, 4);
                                    current_val += worm_enc;
                                    dst = (char*)&telescope->encoder[4].data[2];
                                    src = (char*)&current_val;
                                    myMemcpy(dst, src, 4);
							        if (ultra_verbose)
                                        printf("[telescope_run] Simulate abs movement of at delta worm encoder (0xAA mem_address=2), value = %d\n", worm_enc);
                                    // 1.6. convert the incremental motor enc into axis enc 
                                    axis_enc = rel_delta_motor_enc_to_axis_enc(motor_enc);
							        if (ultra_verbose)
                                        printf("[telescope_run] converted delta motor enc to delta axis enc (0xAC mem_address=4), motor enc = %d, axis enc=%d\n", motor_enc,  axis_enc);
                                    // 1.7. save the incremental axis enc movement at 0xAC, mem_address=4
                                    current_val = 0; 
                                    dst = (char*)&current_val;
                                    src = (char*)&telescope->encoder[5].data[4];
                                    myMemcpy(dst, src, 4);
                                    current_val += axis_enc;
                                    dst = (char*)&telescope->encoder[5].data[4];
                                    src = (char*)&current_val;
                                    myMemcpy(dst, src, 4);
							        if (ultra_verbose)
                                        printf("[telescope_run] Simulate movement at delta axis encoder (0xAC mem_address=4), value = %d\n", axis_enc);
                                    // 1.8. save the incremental axis enc movement at 0xA8, mem_address=2
                                    current_val = 0;
                                    dst = (char*)&current_val;
                                    src = (char*)&telescope->encoder[5].data[2];
                                    myMemcpy(dst, src, 4);
                                    current_val += axis_enc;
                                    dst = (char*)&telescope->encoder[5].data[2];
                                    src = (char*)&current_val;
                                    myMemcpy(dst, src, 4);
							        if (ultra_verbose)
                                        printf("[telescope_run] Simulate movement at delta axis encoder (0xAC mem_address=2), value = %d\n", axis_enc);
                                }                               
                            }
                        }  else {
                            //odd => read 
                            if (mem_address == 7) {
                               int val = telescope->encoder[i].data[mem_address];
                               //simulate the movement is finished, 
                               //TSH: move to thread and add some delay before reaching stop condition (< 50 enc)
                               val = 103; 
                               dst = (char*)&telescope->encoder[i].answer[3];
                               src = (char*)&val;
                               myMemcpy(dst, src, 4);
                            } else {
                               int val = telescope->encoder[i].data[mem_address];
                               dst = (char*)&telescope->encoder[i].answer[3];
                               src = (char*)&val;
                               myMemcpy(dst, src, 4);
                            }

							if (ultra_verbose)
                                printf("[telescope_run] current values of telescope->encoder[%d].data[%d] = %d\n", 
                                                          i, 
                                                          mem_address, 
                                                          telescope->encoder[i].data[mem_address]);
							if (ultra_verbose)
                                printf( "[telescope_run] answering the requested message, telescope->encoder[%d].answer = 0x%X 0x%X 0x%X 0x%X val=%d\n", 
                                                          i, 
                                                          telescope->encoder[i].answer[3], 
                                                          telescope->encoder[i].answer[4], 
                                                          telescope->encoder[i].answer[5], 
                                                          telescope->encoder[i].answer[6], 
                                                          *(int *)&telescope->encoder[i].answer[3]);

                            int z;
                            for (z=0; z<7; z++) 
                                check_sum += telescope->encoder[i].answer[z];

                            telescope->encoder[i].answer[7] = check_sum;
                            telescope->encoder[i].answer[9] = '#';
                        }

                        //Emulate the response of the PIC, which control the encoders and servos
                        //if you don't reset the message, it will be process again by the telescope61 for ever!!!!!!!!!
                        memset( telescope->encoder[i].message, 0, 16 );

                        if( timeout_error_flag ) {
                            printf( "[telescope_run] timeout ERROR\n" );
                        } else {
                            if( verbose ) { 
                                if (message_type  == 0) { 
                                    printf( "[telescope_run] Replying answer (received from PIC) to LCUControl for 0x%02X enc_value= 0x%X 0x%X 0x%X 0x%X val=%d\n", 
                                                          telescope->encoder[i].i2c_address, 
                                                          telescope->encoder[i].answer[3], 
                                                          telescope->encoder[i].answer[4], 
                                                          telescope->encoder[i].answer[5], 
                                                          telescope->encoder[i].answer[6],
                                                          telescope->encoder[i].data[mem_address]);
                                } else {
                                    printf( "[telescope_run] Received the answer from PIC for 0x%02X enc_value= 0x%X 0x%X 0x%X 0x%X val=%d\n", 
                                                          telescope->encoder[i].i2c_address+1, 
                                                          telescope->encoder[i].answer[3], 
                                                          telescope->encoder[i].answer[4], 
                                                          telescope->encoder[i].answer[5], 
                                                          telescope->encoder[i].answer[6],
                                                          telescope->encoder[i].data[mem_address]);
                                }
                            }
                        }
                        binary_semaphore_post( read_semaphore_id );
                        gettimeofday( & gtime, & tzone );
                        endT  = ((double) gtime.tv_usec)/1000000.;
                        endT += (double) gtime.tv_sec;
                        if( verbose ) 
                            printf( "[telescope_run] message processed in dT=%10.6lf[s]\n", endT - startT );
                    }
                    binary_semaphore_post( semaphore_id );

                }
            }


            /**
            * Get Message to TX from stdin
            */
            //printf( "[telescope_run] checking for a stdio message...\n" );
            if( ! new_message_flag ) {

                if( ( retval = stream_status( fd_stdin ) ) > 0 ) {
                    memset( buffer, 0 , 128 );
                    retval = read( fd_stdin, buffer, 128 );
                    buffer[retval-1] = 0;  //enter = 0
                    fflush( stdin );
                    new_message_flag = 1;
                    message_length = strlen( buffer );
                }
            }//END if( ! new_message_flag )


            /**
            * Process stdin message
            */
            //printf( "[telescope_run] processing  a stdio message...\n" );
            if( new_message_flag ) {
                new_message_flag = 0;
                if( strcmp( buffer, "exit" ) == 0 ) {
                    message_length = 0;
                    if( verbose )
                        printf( "[telescope_run] %s\n", buffer );
                    quit = 1;
                } else if( strcmp( buffer, "reply" ) == 0 ) {
                    binary_semaphore_wait( semaphore_id );
                    localtime_r( & (telescope->gtime.tv_sec), & LTime  );
                    binary_semaphore_post( semaphore_id );     
                    strftime( infoline, 24, "%Y-%m-%d %T", & LTime );
                    printf( "[telescope_run] Reply. (%s)\n", infoline );
                    message_length = 0;
                    reply_flag = 1;
                } else if( strcmp( buffer, "noreply" ) == 0 ) {
                    binary_semaphore_wait( semaphore_id );
                    localtime_r( & (telescope->gtime.tv_sec), & LTime  );
                    binary_semaphore_post( semaphore_id );     
                    strftime( infoline, 24, "%Y-%m-%d %T", & LTime );
                    printf( "[telescope_run] No Reply. (%s)\n", infoline );
                    message_length = 0;
                    reply_flag = 0;
                } else if( strcmp( buffer, "date" ) == 0 ) {
                    message_length = 0;
                    binary_semaphore_wait( semaphore_id );
                    //gettimeofday( & (telescope->gtime), & (telescope->tzone) );
                    localtime_r( & (telescope->gtime.tv_sec), & LTime  );
                    strftime( infoline, 24, "%Y-%m-%d %T", & LTime );
                    printf( "[telescope_run] Date: %s\n", infoline );
                    binary_semaphore_post( semaphore_id );
                } else if( strcmp( buffer, "vn" ) == 0 ) {
                    message_length = 0;
                    printf( "[telescope_run] Geting version number.\n" );
                    msg_buffer[0] = ':';
                    msg_buffer[1] = 0xA1;
                    msg_buffer[2] = 'N';
                    msg_buffer[3] = 0;
                    msg_buffer[4] = 0;
                    msg_buffer[5] = 0;
                    msg_buffer[6] = 0;
                    msg_buffer[7] = 0;
                    msg_buffer[8] = 0;
                    msg_buffer[9] = '#';
                    //retval = write_RS232( fd_rs232, msg_buffer, 10  );
                } else if( strcmp( buffer, "vd" ) == 0 ) {
                    message_length = 0;
                    printf( "[telescope_run] Geting version date.\n" );
                    msg_buffer[0] = ':';
                    msg_buffer[1] = 0xA1;
                    msg_buffer[2] = 'D';
                    msg_buffer[3] = 0;
                    msg_buffer[4] = 0;
                    msg_buffer[5] = 0;
                    msg_buffer[6] = 0;
                    msg_buffer[7] = 0;
                    msg_buffer[8] = 0;
                    msg_buffer[9] = '#';
                    //retval = write_RS232( fd_rs232, msg_buffer, 10  );
                } else if( strcmp( buffer, "vt" ) == 0 ) {
                    message_length = 0;
                    printf( "[telescope_run] Geting version time.\n" );
                    msg_buffer[0] = ':';
                    msg_buffer[1] = 0xA1;
                    msg_buffer[2] = 'T';
                    msg_buffer[3] = 0;
                    msg_buffer[4] = 0;
                    msg_buffer[5] = 0;
                    msg_buffer[6] = 0;
                    msg_buffer[7] = 0;
                    msg_buffer[8] = 0;
                    msg_buffer[9] = '#';
                    //retval = write_RS232( fd_rs232, msg_buffer, 10  );
                } else if( strcmp( buffer, "gp" ) == 0 ) {
                    message_length = 0;
                    printf( "[telescope_run] Geting position.\n" );
                    msg_buffer[0] = ':';
                    msg_buffer[1] = 0xA1;
                    msg_buffer[2] = 'P';
                    msg_buffer[3] = 0;
                    msg_buffer[4] = 0;
                    msg_buffer[5] = 0;
                    msg_buffer[6] = 0;
                    msg_buffer[7] = 0;
                    msg_buffer[8] = 0;
                    msg_buffer[9] = '#';
                    //retval = write_RS232( fd_rs232, msg_buffer, 10  );
                } else {
                    message_length = 0;
                    if( verbose )
                        printf( "[telescope_run] Unknown command %s.\n", buffer );
                }// END
            } else if( reply_flag ) { // END if( new_message_flag )
                if( new_timep_flag ) {
                    new_timep_flag = 0;
                    printf( "[telescope_run] Replying...\n" );
                    msg_buffer[0] = ':';
                    msg_buffer[1] = 0xA1;
                    msg_buffer[2] = 'P';
                    msg_buffer[3] = 0;
                    msg_buffer[4] = 0;
                    msg_buffer[5] = 0;
                    msg_buffer[6] = 0;
                    msg_buffer[7] = 0;
                    msg_buffer[8] = 0;
                    msg_buffer[9] = '#';
                    //retval = write_RS232( fd_rs232, msg_buffer, 10  );
                }
            }

        } while( ! quit );
        if( verbose ) {
            binary_semaphore_wait( semaphore_id );
            gettimeofday( & (telescope->gtime), & (telescope->tzone) );
            localtime_r( & (telescope->gtime.tv_sec), & LTime  );
            strftime( infoline, 24, "%Y-%m-%d %T", & LTime );
            if( verbose )
                printf( "[telescope_run] Main Loop ended at %s\n", infoline );
            binary_semaphore_post( semaphore_id );
        }
    } else {
        printf( "[telescope_run] Something wrong!\n" );
    }//END if( init_ok_flag )

    /**
     * INSTRUMENT SHARED MEMORY
     */
    /** Detach the shared memory segment */
    if( (void *) shared_memory != NULL ) {
        if( verbose )
            printf( "[telescope_run] Detaching Instrumen Shared Memory...\n");
        if( shmdt( shared_memory ) < 0 ) {
            perror( "[telescope_run] shmdt");
            if( verbose )
                printf( "[telescope_run] shmdt ERROR.\n");
        } else {
            if( verbose )
                printf( "[telescope_run] shmdt OK.\n");
        }
    }

    /** Deallocate the shared memory segment */
    if( segment_id >= 0 ) {
        if( verbose )
            printf( "[telescope_run] Deallocating Instrumen Shared Memory...\n");
        if( shmctl( segment_id, IPC_RMID, 0 ) < 0 ) {
            perror( "[telescope_run] shmctl");
            if( verbose )
                printf( "[telescope_run] shmctl ERROR.\n");
        } else {
            if( verbose )
                printf( "[telescope_run] shmctl OK.\n");
        }
    }

    /** Deallocate the semaphore */
    if( semaphore_id >= 0 ) {
        if( verbose )
            printf( "[telescope_run] Deallocating semaphore fo Instrumen Shared Memory (write)...\n");
        if( binary_semaphore_deallocate( semaphore_id ) < 0 ) {
            perror( "[telescope_run] semctl");
            if( verbose )
                printf( "[telescope_run] semctl ERROR.\n");
        } else {
            if( verbose )
                printf( "[telescope_run] semctl OK.\n");
        }
    }
    if( read_semaphore_id >= 0 ) {
        if( verbose )
            printf( "[telescope_run] Deallocating semaphore fo Instrumen Shared Memory (read)...\n");
        if( binary_semaphore_deallocate( read_semaphore_id ) < 0 ) {
            perror( "[telescope_run] semctl");
            if( verbose )
                printf( "[telescope_run] semctl ERROR.\n");
        } else {
            if( verbose )
                printf( "[telescope_run] semctl OK.\n");
        }
    }

    /**
     * USER SHARED MEMORY
     */
    /** Detach the shared memory segment */
    if( (void *) user_shared_memory != NULL ) {
        if( verbose )
            printf( "[telescope_run] Detaching User Shared Memory...\n");
        if( shmdt( user_shared_memory ) < 0 ) {
            perror( "[telescope_run] shmdt");
            if( verbose )
                printf( "[telescope_run] shmdt ERROR.\n");
        } else {
            if( verbose )
                printf( "[telescope_run] shmdt OK.\n");
        }
    }
    /** Deallocate the shared memory segment */
    if( user_segment_id >= 0 ) {
        if( verbose )
            printf( "[telescope_run] Deallocating Instrumen Shared Memory...\n");
        if( shmctl( user_segment_id, IPC_RMID, 0 ) < 0 ) {
            perror( "[telescope_run] shmctl");
            if( verbose )
                printf( "[telescope_run] shmctl ERROR.\n");
        } else {
            if( verbose )
                printf( "[telescope_run] shmctl OK.\n");
        }
    }
    /** Deallocate the semaphore */
    if( user_semaphore_id >= 0 ) {
        if( verbose )
            printf( "[telescope_run] Deallocating semaphore fo User Shared Memory...\n");
        if( binary_semaphore_deallocate( user_semaphore_id ) < 0 ) {
            perror( "[telescope_run] semctl");
            if( verbose )
                printf( "[telescope_run] semctl ERROR.\n");
        } else {
            if( verbose )
                printf( "[telescope_run] semctl OK.\n");
        }
    }
    if( verbose ) {
        printf( "[telescope_run] Good bye!\n" );
    }

}