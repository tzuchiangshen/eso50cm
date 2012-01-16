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
         * Open rs232
         */
//        if( verbose ) {
//            printf( "[telescope_run] Opening rs232 device\n" );
//            printf( "[telescope_run] device = %s\n", device );
//            printf( "[telescope_run] baudrate = %d\n", (int) baudrate );
//        }
//        fd_rs232 = open_RS232( device, baudrate );
//        if( fd_rs232 == 0 ) {
//            init_ok_flag = 0;
//            if( verbose )
//                printf( "[telescope_run] fd_rs232 = 0\n" );
//        }
//
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


        do {
            /** This is the main loop */

            /** Check if there are availabe bytes in the /dev/ttySX */
            new_ttymssg_flag    = 0;
            wait_for_message    = 0;
            nums_of_time_outs   = 0;
            timeout_error_flag  = 0;
            rx_msg_length       = 0;
//            do {
//                if( (rx = status_RS232( fd_rs232, 1000 )) > 0 ) {
//                    /** There are bytes, then read them */
//                    if( (rx = read_RS232( fd_rs232, tty_buffer, 256 )) > 0 ) {
//                        for( k = 0; k < rx; k ++ ) {
//                            if( rx_msg_length == 0 ) {
//                                /** Check for a valid message starting character */
//                                if( tty_buffer[k] == ':' ) {
//                                    /** The starting character exists... */
//                                    msg_buffer[rx_msg_length] = tty_buffer[k];
//                                    rx_msg_length ++ ;
//                                    /** Then, there is an incomming message.
//                                        Wait until the message is completes or
//                                        max number of timeouts is reached */
//                                    wait_for_message = 1;
//                                }
//                            } else {
//                                /** The rest of the message */
//                                msg_buffer[rx_msg_length] = tty_buffer[k];
//                                rx_msg_length ++;
//                                /** Check if the message is complete... */
//                                if( msg_buffer[1] == 0xA0 && rx_msg_length == 10 ) {
//                                    /** A short message was received */
//                                    msg_buffer[rx_msg_length] = 0;
//                                    new_ttymssg_flag = 1;
//                                    wait_for_message = 0;
//                                } else if( msg_buffer[1] == 0xA1 && rx_msg_length == 42 ) {
//                                    /** A large message was received */
//                                    msg_buffer[rx_msg_length] = 0;
//                                    new_ttymssg_flag = 1;
//                                    wait_for_message = 0;
//                                }
//                            }// END else if( rx_msg_length > 0 )
//                        } // END for( k = 0; k < rx; k ++ )
//                    } else {
//                        if( verbose )
//                            printf( "[telescope_run] read ERROR\n" );
//                    } //END if( (rx = read_RS232( fd_rs232, tty_buffer, 256 )) > 0 )
//                } else {
//                    /** No bytes available. */
//                    nums_of_time_outs ++;
//                    if( wait_for_message && (nums_of_time_outs > 10 ) ) {
//                        /** Max number of timeouts has been reached while waiting
//                            for a message... */
//                        printf( "[telescope_run] timeout ERROR: bytes received = %d\n", rx_msg_length  );
//                        rx_msg_length = 0;
//                        wait_for_message = 0;
//                        new_ttymssg_flag = 0;
//                        /** The error flag is set true. */
//                        timeout_error_flag = 1;
//                    }
//                }// END if( (rx = status_RS232( fd_rs232, 250 )) > 0 )
//            } while( wait_for_message );
//            if( timeout_error_flag ) {
//                timeout_error_flag = 0;
//            }// END if( timeout_error_flag )
//
//            /**
//            * Process New RX Message
//            */
//            binary_semaphore_wait( semaphore_id );
//            if( new_ttymssg_flag ) {
//                if( (unsigned char) msg_buffer[1] == 0xA0 ) {
//                    if( verbose ) {
//                        printf( "[telescope_run] Short message received\n" );
//                        if( msg_buffer[9] == '#' ) {
//                            printf( "[telescope_run] Last char OK\n" );
//                        } else {
//                            printf( "[telescope_run] Last char Wrong\n" );
//                        }
//                        if( msg_buffer[2] == 'P' ) {
//                            new_timep_flag = 1;
//                            gettimeofday( & (telescope->gtime), & (telescope->tzone) );
//                            localtime_r( & (telescope->gtime.tv_sec), & LTime  );
//                            strftime( infoline, 24, "%Y-%m-%d %T", & LTime );
//                            printf( "[telescope_run] %s\n", infoline );
//                        }
//                    }
//                } else if( (unsigned char) msg_buffer[1] == 0xA1 ) {
//                    if( verbose ) {
//                        printf( "[telescope_run] Long message received\n" );
//                        if( msg_buffer[41] == '#' ) {
//                            printf( "[telescope_run] Last char OK\n" );
//                        } else {
//                            printf( "[telescope_run] Last char Wrong\n" );
//                        }
//                        if( msg_buffer[2] == 'N' ) {
//                            printf( "[telescope_run] Version number: %s\n", & msg_buffer[3] );
//                        } else if( msg_buffer[2] == 'D' ) {
//                            printf( "[telescope_run] Version date  : %s\n", & msg_buffer[3] );
//                        } else if( msg_buffer[2] == 'T' ) {
//                            printf( "[telescope_run] Version time  : %s\n", & msg_buffer[3] );
//                        } else if( msg_buffer[2] == 'P' ) {
//                            telescope->new_data = 1;
//                            //printf( "[telescope_run] " );
//                            //for( i = 0; i < 24; i ++ ) {
//                            //    printf( "[%02X]", (unsigned int) msg_buffer[3+i] );
//                            //}
//                            //printf( " (%d)\n", (unsigned int) msg_buffer[41] );
//                            char i2c;
//                            char mem;
//                            int numero;
//                            unsigned char chksum;
//
//                            chksum = 0;
//                            i = 3;
//
//                            for( j = 0; j < 4; j ++ ) {
//                                i2c     = msg_buffer[i];;
//                                chksum += (unsigned char) msg_buffer[i];
//                                mem     = msg_buffer[i+1];
//                                chksum += (unsigned char) msg_buffer[i+1];
//                                ((char *) & numero)[0] =  msg_buffer[i+2];
//                                chksum += (unsigned char) msg_buffer[i+2];
//                                ((char *) & numero)[1] =  msg_buffer[i+3];
//                                chksum += (unsigned char) msg_buffer[i+3];
//                                ((char *) & numero)[2] =  msg_buffer[i+4];
//                                chksum += (unsigned char) msg_buffer[i+4];
//                                ((char *) & numero)[3] =  msg_buffer[i+5];
//                                chksum += (unsigned char) msg_buffer[i+5];
//                                //printf( "[telescope_run] 0x%02X %d %d\n", (unsigned int) i2c, (int) mem, numero );
//
//                                for( k = 0; k < 6; k ++ ) {
//                                    if( telescope->encoder[k].i2c_address + 1 == i2c ) {
//                                        telescope->encoder[k].data[mem] = numero;
//                                        printf( "[telescope_run] 0x%02X %d %d\n",
//                                                (unsigned int) telescope->encoder[k].i2c_address,
//                                                (int) mem,
//                                                telescope->encoder[k].data[mem] );
//                                        break;
//                                    }
//                                }
//                                i += 6;
//                            }
//                            if( (unsigned char) msg_buffer[40] != chksum ) {
//                                printf( "[telescope_run] ChkSum Error(%d,%d)\n",
//                                        (unsigned char) msg_buffer[40], chksum );
//                            }
//                        }
//                    }
//                }
//                new_ttymssg_flag = 0;
//                rx_msg_length = 0;
//            }//END if( new_ttymssg_flag )
//
//            if( checksum_error_flag ) {
//            }//END if( checksum_error_flag )
//            binary_semaphore_post( semaphore_id );
//
//            /**
//            * Checks for a message in the instrument shared memory...
//            */
//            if( bin_message_len == 0 ) {
//                for( i = 0; i < 6; i ++ ) {
//                    binary_semaphore_wait( semaphore_id );
//                    if( telescope->encoder[i].message[0] == ':' ) {
//                        gettimeofday( & gtime, & tzone );
//                        startT  = ((double) gtime.tv_usec)/1000000.;
//                        startT += (double) gtime.tv_sec;
//                        if( verbose )
//                            printf( "[telescope_run] Sending new message for 0x%02X\n", telescope->encoder[i].i2c_address );
//                        retval = write_RS232( fd_rs232, telescope->encoder[i].message, 10  );
//                        memset( telescope->encoder[i].message, 0, 16 );
//                        length = 0;
//                        nums_of_time_outs = 0;
//                        timeout_error_flag = 0;
//                        do {
//                            if( (retval = status_RS232( fd_rs232, 1000 )) > 0 ) {
//                                //bytes_read = read_RS232( fd_rs232, & m_line[length], 128 - length );
//                                bytes_read = read_RS232( fd_rs232, & telescope->encoder[i].answer[length], 16 - length );
//                                if( bytes_read > 0 ) {
//                                    //printf( "[telescope_run] bytes_read = %d\n", bytes_read );
//                                    length += bytes_read;
//                                    telescope->encoder[i].answer[length] = '\0';
//                                }
//                            } else {
//                                nums_of_time_outs ++;
//                                if( nums_of_time_outs > 10 ) {
//                                    timeout_error_flag = 1;
//                                    break;
//                                }
//                                //printf( "[telescope_run] still waiting for answer...\n" );
//                            }
//                        } while( length < 10 );
//                        if( timeout_error_flag ) {
//                            printf( "[telescope_run] timeout ERROR\n" );
//                        } else {
//                            if( verbose )
//                                printf( "[telescope_run] Received the answer from 0x%02X\n", telescope->encoder[i].i2c_address );
//                        }
//                        binary_semaphore_post( read_semaphore_id );
//                        gettimeofday( & gtime, & tzone );
//                        endT  = ((double) gtime.tv_usec)/1000000.;
//                        endT += (double) gtime.tv_sec;
//                        if( verbose )
//                            printf( "[telescope_run] dT=%10.6lf[s]\n", endT - startT );
//                    }
//                    binary_semaphore_post( semaphore_id );
//
//                }
//            }

            /**
            * Checks for a message in the instrument shared memory... SIMULATION
            */
			int enc_count;
			enc_count = 0;
			char *ptr = (char*)&enc_count;
			char *ptr2;
            if( bin_message_len == 0 ) {
                for( i = 0; i < 6; i ++ ) {
                    binary_semaphore_wait( semaphore_id );
                    if( telescope->encoder[i].message[0] == ':' ) {
                        gettimeofday( & gtime, & tzone );
                        startT  = ((double) gtime.tv_usec)/1000000.;
                        startT += (double) gtime.tv_sec;

						int check_sum = 0;

						int rest = telescope->encoder[i].message[1] % 2;
						int mem_address  = telescope->encoder[i].message[2];
						printf("\n[telescope_run] -----------------------------------------\n");
						if (rest == 0)
						    printf("[telescope_run] New message received from LCUControl type=Set\n");
						else
						    printf("[telescope_run] New message received from LCUControl type=Get\n");

						printf( "[telescope_run] Sending new message to PIC for 0x%02X mem_address=%d\n", telescope->encoder[i].message[1], mem_address /*telescope->encoder[i].message[2]*/ );
						if(rest == 0) {
							//printf("es un write: \n");
							//even => comes from setDeviceMemory, save the encoder value for later the next readDeviceMemory()
				    		//Emulate sending data to the PIC (RS232)
				    		//it will save the target encoder value in a internal memory structure 
				    		//when the readMemory is received, it will return the target encoder value
                            length = 0;
                            nums_of_time_outs = 0;
                            timeout_error_flag = 0;
							//maybe its better to use memcopy
   							*ptr = telescope->encoder[i].message[3];
   							ptr++;
   							*ptr = telescope->encoder[i].message[4];
   							ptr++;
   							*ptr = telescope->encoder[i].message[5];
   							ptr++;
   							*ptr = telescope->encoder[i].message[6];
			                //memcpy(telescope_sim->encoder[i].data[mem_address], &telescope->encoder[i].message[3], 4);
							ptr = (char*)&telescope->encoder[i].data[mem_address];
							*ptr = telescope->encoder[i].message[3];
   							ptr++;
   							*ptr = telescope->encoder[i].message[4];
   							ptr++;
   							*ptr = telescope->encoder[i].message[5];
   							ptr++;
   							*ptr = telescope->encoder[i].message[6];
                            if( verbose ) {
                                printf( "[telescope_run] telescope->encoder[%d].message = 0x%X 0x%X 0x%X 0x%X\n", i, telescope->encoder[i].message[3], telescope->encoder[i].message[4], telescope->encoder[i].message[5], telescope->encoder[i].message[6]);
								printf( "[telescope_run] telescope->encoder[%d].data[%d] = %d\n", i, mem_address, telescope->encoder[i].data[mem_address]);
//                                printf( "[telescope_run] enc_count[%d].data[%d] = 0x%X 0x%X 0x%X 0x%X\n", i, mem_address, telescope->encoder[i].data[mem_address], telescope->encoder[i].data[4], telescope->encoder[i].data[5], telescope->encoder[i].data[6]);
   						    }

						}  else {
							//odd => read 
							//printf("es un read: \n");
							printf( "[telescope_run] telescope->encoder[%d].data[%d] = %d\n", i, mem_address, telescope->encoder[i].data[mem_address]);
							
							int val = telescope->encoder[i].data[mem_address];
							val = 40;
							printf( "[telescope_run] val = %d\n", val);

							char *ptr_val = (char*)&val;
                            telescope->encoder[i].answer[3] = *ptr_val;
							ptr_val ++;
							telescope->encoder[i].answer[4] = *ptr_val;
							ptr_val ++;
							telescope->encoder[i].answer[5] = *ptr_val;
							ptr_val ++;
							telescope->encoder[i].answer[6] = *ptr_val;

							printf( "[telescope_run] telescope->encoder[%d].answer = 0x%X 0x%X 0x%X 0x%X\n", i, telescope->encoder[i].answer[3], telescope->encoder[i].answer[4], telescope->encoder[i].answer[5], telescope->encoder[i].answer[6]);

							int z;
							for (z=0; z<7; z++) 
								check_sum += telescope->encoder[i].answer[z];

							telescope->encoder[i].answer[7] = check_sum;
							telescope->encoder[i].answer[9] = '#';
						}

						//Emulate the response of the PIC, which control the encoders and servos
						memset( telescope->encoder[i].message, 0, 16 );

						if( timeout_error_flag ) {
                            printf( "[telescope_run] timeout ERROR\n" );
                        } else {
                            if( verbose ) { 
								if (rest  == 0) 
                                	printf( "[telescope_run] Received the answer from PIC for 0x%02X enc_value= 0x%X 0x%X 0x%X 0x%X val=%d\n", telescope->encoder[i].i2c_address, telescope->encoder[i].answer[3], telescope->encoder[i].answer[4], telescope->encoder[i].answer[5], telescope->encoder[i].answer[6],telescope->encoder[i].data[mem_address]);
								else 
                                	printf( "[telescope_run] Received the answer from PIC for 0x%02X enc_value= 0x%X 0x%X 0x%X 0x%X val=%d\n", telescope->encoder[i].i2c_address+1, telescope->encoder[i].answer[3], telescope->encoder[i].answer[4], telescope->encoder[i].answer[5], telescope->encoder[i].answer[6],telescope->encoder[i].data[mem_address]);

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

//    /** Close RS232 device */
//    if( fd_rs232 > 0 ) {
//        if( verbose )
//            printf( "[telescope_run] Closing fd_rs232...\n");
//        if( close( fd_rs232 ) < 0 ) {
//            perror( "[telescope_run] close");
//            if( verbose )
//                printf( "[telescope_run] close ERROR.\n");
//        } else {
//            if( verbose )
//                printf( "[telescope_run] close OK.\n");
//        }
//    }

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
