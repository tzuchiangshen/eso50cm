#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/stat.h>

#include <sys/time.h>   //gettimeofday
#include <time.h>       //gmtime

#include <math.h>
#include <string.h>
#include <signal.h>     //sigaction
#include <sys/wait.h>   //wait
//#include <fcntl.h>
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


/** A template for the HTML page this module generates.  */ 
static char* page_template =
"%s\n"
"  0x%02X = %d\n"
"  0x%02X = %d\n"
"  0x%02X = %d\n"
"  0x%02X = %d\n"
"  0x%02X = %d\n"
"  0x%02X = %d\n"
"  %s\n\n";



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

    int i;
    int j;
    int k;
    char * line;

    char quit;

    int fd_stdin;
    int fd_rs232;
    char infoline[64];
    char buffer[128];
    char tty_buffer[256];
    char msg_buffer[256];
    int rx_msg_length;
    char * shm_message;

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

    int wait_for_answer;
    int nums_of_time_outs;

    int * ptr_to_int;
    int reply_flag;
    int sec;

    struct sigaction sigchld_action;

    /** Install a handler for SIGCHLD that cleans up child processes that
    have terminated.  */
    memset( & sigchld_action, 0, sizeof( sigchld_action ) );
    sigchld_action.sa_handler = & clean_up_child_process;
    sigaction( SIGCHLD, & sigchld_action, NULL );

    /**
     * INSTRUMENT SHARED MEMORY
     */
    /** Allocate a semaphore */
    semaphore_id = binary_semaphore_allocate( SEMKEY, 
                        IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR );
    retval = binary_semaphore_initialize( semaphore_id );

    read_semaphore_id = binary_semaphore_allocate( RDSEMKEY,
                            IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR );
    retval = binary_semaphore_initialize( read_semaphore_id );

    /** Allocate a shared memory segment */
    segment_id = shmget( SHMKEY, shared_segment_size, 
                         IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR );

    /** Attach the shared memory segment */
    shared_memory = (char *) shmat( segment_id, 0, 0 );
    if( verbose )
        printf( "[telescope_run] instrument sahred memory attached at adress %p\n", (void *) shared_memory );

    /** Determine the segment size */
    shmctl( segment_id, IPC_STAT, & shmbuffer );
    segment_size = shmbuffer.shm_segsz;
    if( verbose ) {
        printf( "[telescope_run] instrument segment size:           %d\n", segment_size );
        printf( "[telescope_run] sizeof( struct telescope_data_t ): %d\n", sizeof( struct telescope_data_t ) );
    }
    telescope = (struct telescope_data_t *) shared_memory;

    /**
     * USER SHARED MEMORY
     */
    /** Allocate a semaphore */
    user_semaphore_id = binary_semaphore_allocate( USRSEMKEY,
                                              IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR );
    retval = binary_semaphore_initialize( user_semaphore_id );

    /** Allocate a shared memory segment */
    user_segment_id = shmget( USRSHMKEY, shared_segment_size,
                         IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR );

    /** Attach the shared memory segment */
    user_shared_memory = (char *) shmat( user_segment_id, 0, 0 );
    if( verbose )
        printf( "[telescope_run] user sahred memory attached at adress %p\n", (void *) user_shared_memory );

    /** Determine the segment size */
    shmctl( user_segment_id, IPC_STAT, & shmbuffer );
    user_segment_size = shmbuffer.shm_segsz;
    if( verbose ) {
        printf( "[telescope_run] user segment size: %d\n", segment_size );
    }


    /**
     * Open rs232
     */
    if( verbose ) {
        printf( "[telescope_run] Opening rs232 device\n" );
        printf( "[telescope_run] device = %s\n", device );
        printf( "[telescope_run] baudrate = %d\n", (int) baudrate );
    }
    fd_rs232 = 0;
    fd_rs232 = open_RS232( device, baudrate );
    new_message_flag = 0;
    new_ttymssg_flag = 0;

    /**
     * File descriptor for standard in
     */
    fd_stdin = fileno( stdin );

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
    if( verbose ) printf( "[telescope_run] Hello World!\n%s\n", infoline );

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

    /*
    int m_socket_fd;
    struct sockaddr_un m_server_name;

    m_socket_fd = socket( PF_LOCAL, SOCK_STREAM, 0 );
    if( m_socket_fd < 0 ) {
        perror( "[telescope_run] socket" );
        printf( "[telescope_run] socket ERROR\n" );
    } else {
        printf( "[telescope_run] socket OK\n" );
    }
    m_server_name.sun_family= AF_LOCAL;
    strcpy( m_server_name.sun_path, "/tmp/mbux" );

    retval = bind( m_socket_fd, (struct sockaddr *) & m_server_name, SUN_LEN( & m_server_name ) );
    if( retval < 0 ) {
        perror( "[telescope_run] bind" );
        printf( "[telescope_run] bind ERROR\n" );
    } else {
        printf( "[telescope_run] bind OK\n" );
    }

    retval = listen( m_socket_fd, 6 );
    if( retval < 0 ) {
        perror( "[telescope_run] listen" );
        printf( "[telescope_run] listen ERROR\n" );
    } else {
        printf( "[telescope_run] listen OK\n" );
    }
    */
    /** MAIN LOOP */
    if( verbose ) printf( "[telescope_run] Everything looks OK!\n[telescope_run] Starting main loop\n" );
    reply_flag = 0;
    checksum_error_flag = 0;
    rx_msg_length = 0;
    message_length = 0;
    bin_message_len = 0;
    j = 0;
    sec = 120;
    wait_for_answer = 0;
    //wait_for_bin_message = 0;

    int usr2bin_fd;
    int bin2usr_fd;
    char m_line[128];
    int bytes_read;
    int length;
    unsigned int uint_tmp;


    do {
        /*
        int m_client_socket_fd;
        struct sockaddr_un m_client_name;
        socklen_t m_client_name_len;

        printf( "[telescope_run] waiting for a new client...\n" );
        m_client_socket_fd = accept( m_socket_fd, (struct sockaddr *) & m_client_name, & m_client_name_len );
        if( m_client_socket_fd < 0 ) {
            perror( "[telescope_run] accept" );
            printf( "[telescope_run] accept ERROR\n" );
        } else {
            printf( "[telescope_run] accept OK\n" );
            printf( "[telescope_run] accept %s %d\n", m_client_name.sun_path, m_client_socket_fd );
            // Read message
            printf( "[telescope_run] waiting for a ux-sck data...\n" );
            memset( m_line, 0, 128 );
            length = 0;
            do {
                bytes_read = read( m_client_socket_fd, & m_line[length], 128 - length );
                if( bytes_read > 0 ) {
                    printf( "[telescope_run] bytes_read = %d\n", bytes_read );
                    length += bytes_read;
                    m_line[length] = '\0';
                }
            } while( length < 32 );
            printf( "[telescope_run] %s\n", m_line );
            sscanf( m_line, ":0x%X Hola!#", & uint_tmp );

            // Send answer
            memset( m_line, 0, 32 );
            sprintf( m_line, ":0x%02X Chao!#", uint_tmp );
            printf( "[telescope_run] %s\n", m_line );
            write( m_client_socket_fd, m_line, 32 );
            retval = close( m_client_socket_fd );
            if( retval < 0 ) {
                perror( "[telescope_run] close" );
                printf( "[telescope_run] close ERROR\n" );
            } else {
                printf( "[telescope_run] close OK\n" );
            }
        }
        */

        /*
         * RX Data
         * o Binary Messge:
         *   - OK.
         * o Text Message:
         *   - OX.
         * o Binary + Text:
         *   -Message Unknown.
         *                                                        msg_len
         *  :                                                    =  1
         *  [0xAX][ mem][num0][num2][num2][num3][chks][0x00]     =  9
         *  [0xAX][ mem][num0][num2][num2][num3][chks][0x00]     =  17
         *  [0xAX][ mem][num0][num2][num2][num3][chks][0x00]   = 8
         *  [0xAX][ mem][num0][num2][num2][num3][chks][0x00]   = 8
         *  #                                                  =
         *
         */
        //printf( "[telescope_run] checking for an rs232 answer...\n" );
        wait_for_answer = 0;
        nums_of_time_outs = 0;
        timeout_error_flag = 0;
        do {
            if( (rx = status_RS232( fd_rs232, 1000 )) > 0 ) {
                if( (rx = read_RS232( fd_rs232, tty_buffer, 256 )) > 0 ) {
                    for( k = 0; k < rx; k ++ ) {
                        if( rx_msg_length == 0 ) {
                            if( tty_buffer[k] == ':' ) {
                                msg_buffer[rx_msg_length] = tty_buffer[k];
                                rx_msg_length ++ ;
                                next_bin_msg = 1;
                                new_ttymssg_flag = 0;
                                wait_for_answer = 1;
                            }
                        } else if( rx_msg_length > 0  && rx_msg_length < 255 ) {
                            msg_buffer[rx_msg_length] = tty_buffer[k];
                            rx_msg_length ++ ;
                            if( (unsigned char) msg_buffer[1] < 160 ) {
                                if( tty_buffer[k] == '#' ) {
                                    msg_buffer[rx_msg_length] =  0;
                                    tty_msg_len = rx_msg_length;
                                    rx_msg_length = 0;
                                    new_ttymssg_flag = 1;
                                    wait_for_answer = 0;
                                }
                            } else if( rx_msg_length == next_bin_msg + 1 ) {
                                if( msg_buffer[next_bin_msg] == '#' ) {
                                    tty_msg_len = rx_msg_length;
                                    rx_msg_length = 0;
                                    new_ttymssg_flag = 1;
                                    wait_for_answer = 0;
                                } else {
                                    next_bin_msg += 8;
                                }
                            }
                        } else {
                            printf( "[telescope_run] rx_msg_length ERROR\n" );
                            rx_msg_length = 0;
                        }// END else if( rx_msg_length > 0 )
                    } // END for( k = 0; k < rx; k ++ )
                } else {
                    if( verbose )
                        printf( "[telescope_run] read ERROR\n" );
                } //END if( (rx = read_RS232( fd_rs232, tty_buffer, 256 )) > 0 )
            } else {
                nums_of_time_outs ++;
                if( wait_for_answer && (nums_of_time_outs > 10 ) ) {
                    printf( "[telescope_run] timeout ERROR\n" );
                    tty_msg_len = rx_msg_length;
                    rx_msg_length = 0;
                    new_ttymssg_flag = 1;
                    wait_for_answer = 0;
                    timeout_error_flag = 1;
                }
            }// END if( (rx = status_RS232( fd_rs232, 250 )) > 0 )
        } while( wait_for_answer );

        /*
         * Process New RX Message
         */
        //printf( "[telescope_run] processing rs232 answer...\n" );
        binary_semaphore_wait( semaphore_id );
        if( new_ttymssg_flag ) {
            if( (unsigned char) msg_buffer[1] < 0xA0 ) {
                printf( "%s\n", msg_buffer );
            } else {
                //printf( "%c\n", msg_buffer[0] );
                i = 1;
                do{
                    if( msg_buffer[i] % 2 == 0 ){
                        if( (unsigned char) msg_buffer[i] == 0xA0 ){
                            //binary_semaphore_wait( semaphore_id );
                            telescope->new_data = 1;
                            gettimeofday( & (telescope->gtime), & (telescope->tzone) );    

                            if( verbose ){
                                if( ! reply_flag ) {
                                    gettimeofday( & (telescope->gtime), & (telescope->tzone) );
                                    localtime_r( & (telescope->gtime.tv_sec), & LTime  );
                                    strftime( infoline, 24, "%Y-%m-%d %T", & LTime );
                                    printf( "[telescope_run] %s\n", infoline ); 
                                }
                            }
                            //binary_semaphore_post( semaphore_id );
                            if( reply_flag ) {
                                if( bin_message_len == 0 ) {
                                    bin_message[bin_message_len] = ':';
                                    bin_message_len ++;
                                }
                                bin_message[bin_message_len] = 0xA1;
                                bin_message_len ++;
                                bin_message[bin_message_len] = 0;
                                bin_message_len ++;
                                bin_message[bin_message_len] = '#';
                                bin_message_len ++;
                            }
                        } else {
                            //printf( "0x%02X ", msg_buffer[i] );     //i2c_address
                            //printf( "0x%02X\n", msg_buffer[i+1] );  //i2c_status
                            //binary_semaphore_wait( semaphore_id );
                            for( k = 0; k < 6; k++ ) {
                                if( telescope->encoder[k].i2c_address == (0xFE & msg_buffer[i]) ){
                                    telescope->encoder[k].i2c_status = msg_buffer[i+1];
                                    printf( "[0x%02X=%d]\n", msg_buffer[i], (int) msg_buffer[i+1] );
                                    break;
                                }
                            }
                            //binary_semaphore_post( semaphore_id );
                        }
                        i += 8;
                    } else {
                        int mem;
                        int numero;
                        unsigned char chksum;

                        chksum  = (unsigned char) msg_buffer[i];
                        chksum += (unsigned char) msg_buffer[i+1];
                        ((char *) & numero)[0] =  msg_buffer[i+2];
                        chksum += (unsigned char) msg_buffer[i+2];
                        ((char *) & numero)[1] =  msg_buffer[i+3];
                        chksum += (unsigned char) msg_buffer[i+3];
                        ((char *) & numero)[2] =  msg_buffer[i+4];
                        chksum += (unsigned char) msg_buffer[i+4];
                        ((char *) & numero)[3] =  msg_buffer[i+5];
                        chksum += (unsigned char) msg_buffer[i+5]; 
                        if( chksum == msg_buffer[i+6] ) {
                            //binary_semaphore_wait( semaphore_id );
                            for( k = 0; k < 6; k ++ ) {
                                if( telescope->encoder[k].i2c_address == (0xFE & msg_buffer[i]) ){
                                    telescope->encoder[k].i2c_status = 0;
                                    mem = (int) msg_buffer[i+1];
                                    telescope->encoder[k].data[mem] = numero;
                                    if( ! reply_flag || (unsigned char) msg_buffer[i] < 0xA6 ) {
                                        printf( "0x%02X[%d]=%d\n", msg_buffer[i], mem, numero );
                                    }
                                    break;
                                }
                            }
                            //binary_semaphore_post( semaphore_id );
                            //if( ! reply_flag || msg_buffer[i] < 0xA6 ) {
                                //printf( "0x%02X[%d]=%d\n", msg_buffer[i], mem, numero );
                                //printf( "0x%02X ", msg_buffer[i] );     //i2c_address
                                //printf( "0x%02X ", msg_buffer[i+1] );   //i2c_memory
                                //printf( "0x%02X ", msg_buffer[i+2] );   //numero 
                                //printf( "0x%02X ", msg_buffer[i+3] );
                                //printf( "0x%02X ", msg_buffer[i+4] );
                                //printf( "0x%02X ", msg_buffer[i+5] );
                                //printf( "0x%02X\n", msg_buffer[i+6] );  //chksum
                            //} 
                        } else {
                            checksum_error_flag = 1;
                            printf( "[telescope_run] Checksum ERROR for 0x%02X\n", msg_buffer[i] );
                        }
                        i += 8;
                    }  
                } while ( i < tty_msg_len-1 );
                //printf( "%c\n", msg_buffer[tty_msg_len-1] );
            }
            new_ttymssg_flag = 0;
        }//END if( new_ttymssg_flag )
        //binary_semaphore_post( semaphore_id );
        if( checksum_error_flag || timeout_error_flag ) {
            //If there was some error, prints a message...
            checksum_error_flag = 0;
            timeout_error_flag = 0;
            i = 1;
            do{
                unsigned char chksum;
                if( msg_buffer[i] % 2 == 0 ){
                    printf( "0x%02X ", msg_buffer[i] );     //i2c_address
                    printf( "0x%02X\n", msg_buffer[i+1] );   //i2c_memory
                    i += 2;
                } else {
                    printf( "0x%02X ", msg_buffer[i] );     //i2c_address
                    chksum = msg_buffer[i];
                    printf( "0x%02X ", msg_buffer[i+1] );   //i2c_memory
                    chksum += msg_buffer[i+1];
                    printf( "0x%02X ", msg_buffer[i+2] );   //numero 
                    chksum += msg_buffer[i+2];
                    printf( "0x%02X ", msg_buffer[i+3] );
                    chksum += msg_buffer[i+3];
                    printf( "0x%02X ", msg_buffer[i+4] );
                    chksum += msg_buffer[i+4];
                    printf( "0x%02X ", msg_buffer[i+5] );
                    chksum += msg_buffer[i+5];
                    printf( "0x%02X = 0x%02X\n", msg_buffer[i+6], chksum );  //chksum  
                    i += 7;
                } 
            } while ( i < tty_msg_len-1 );
            localtime_r( & (telescope->gtime.tv_sec), & LTime  );
            strftime( infoline, 24, "%Y-%m-%d %T", & LTime );
            printf( "%s\n\n", infoline );
        }//END if( checksum_error_flag || timeout_error_flag )
        binary_semaphore_post( semaphore_id );
        /*
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

        /*
         * Process standr in message
         */
        //printf( "[telescope_run] processing  a stdio message...\n" );
        if( new_message_flag ) {
            new_message_flag = 0;
            if( strcmp( buffer, "exit" ) == 0 ) {
                message_length = 0;
                if( verbose )
                    printf( "[tty] %s\n", buffer );
                quit = 1;
            } else if( strcmp( buffer, "status" ) == 0 ) {
                message_length = 0;
                if( verbose ) {
                    binary_semaphore_wait( semaphore_id );
                    localtime_r( & (telescope->gtime.tv_sec), & LTime  );
                    strftime( infoline, 24, "%Y-%m-%d %T", & LTime );
                    printf( page_template,
                                "I2C Devices status:",
                                (unsigned int) telescope->encoder[0].i2c_address, telescope->encoder[0].i2c_status,
                                (unsigned int) telescope->encoder[1].i2c_address, telescope->encoder[1].i2c_status,
                                (unsigned int) telescope->encoder[2].i2c_address, telescope->encoder[2].i2c_status,
                                (unsigned int) telescope->encoder[3].i2c_address, telescope->encoder[3].i2c_status,
                                (unsigned int) telescope->encoder[4].i2c_address, telescope->encoder[4].i2c_status,
                                (unsigned int) telescope->encoder[5].i2c_address, telescope->encoder[5].i2c_status,
                                infoline );
                    binary_semaphore_post( semaphore_id );    
                }
            } else if( strcmp( buffer, "data" ) == 0 ) {
                message_length = 0;
                if( verbose ) {
                    binary_semaphore_wait( semaphore_id );
                    localtime_r( & (telescope->gtime.tv_sec), & LTime  );
                    strftime( infoline, 24, "%Y-%m-%d %T", & LTime );
                    printf( page_template,
                                "Encoders position:",
                                (unsigned int) telescope->encoder[0].i2c_address, telescope->encoder[0].data[4],
                                (unsigned int) telescope->encoder[1].i2c_address, telescope->encoder[1].data[4],
                                (unsigned int) telescope->encoder[2].i2c_address, telescope->encoder[2].data[4],
                                (unsigned int) telescope->encoder[3].i2c_address, telescope->encoder[3].data[4],
                                (unsigned int) telescope->encoder[4].i2c_address, telescope->encoder[4].data[4],
                                (unsigned int) telescope->encoder[5].i2c_address, telescope->encoder[5].data[4],
                                infoline );
                    binary_semaphore_post( semaphore_id );    
                }
            } else if( strcmp( buffer, "reply" ) == 0 ) {
                binary_semaphore_wait( semaphore_id );
                localtime_r( & (telescope->gtime.tv_sec), & LTime  );
                binary_semaphore_post( semaphore_id );     
                strftime( infoline, 24, "%Y-%m-%d %T", & LTime );
                printf( "Reply:\n  %s\n\n", infoline );
                message_length = 0;
                reply_flag = 1;
            } else if( strcmp( buffer, "noreply" ) == 0 ) {
                binary_semaphore_wait( semaphore_id );
                localtime_r( & (telescope->gtime.tv_sec), & LTime  );
                binary_semaphore_post( semaphore_id );     
                strftime( infoline, 24, "%Y-%m-%d %T", & LTime );
                printf( "No Reply:\n  %s\n\n", infoline );
                message_length = 0;
                reply_flag = 0;
            } else if( strcmp( buffer, "date" ) == 0 ) {
                message_length = 0;
                binary_semaphore_wait( semaphore_id );
                //gettimeofday( & (telescope->gtime), & (telescope->tzone) );
                localtime_r( & (telescope->gtime.tv_sec), & LTime  );
                strftime( infoline, 24, "%Y-%m-%d %T", & LTime );
                printf( "Date:\n  %s\n\n", infoline );
                binary_semaphore_post( semaphore_id );
            } else if( buffer[0] == 'b' || buffer[0] == 'B' ) {
                const char * delim = ":";
                char * ptr;
                char function;
                int address;
                int memory;
                int value;

                char i2c_address;
                char i2c_memory;


                int ix;

                //printf( "Convertng to binary message...\n" );
                ptr = strtok( buffer, delim );
                //printf( "%s\n", ptr );
                if( ptr[0] == 'b' || ptr[0] == 'B' ) {
                    if( bin_message_len == 0 ) {
                        bin_message[bin_message_len] = ':';
                        bin_message_len ++;
                    } else {
                        // remove '#'
                        bin_message_len --;
                    }
                    ptr = strtok( 0, delim );
                    do{ 
                        //printf( "%s\n", ptr );
                        function = 0xFF;
                        address  = 0xFFFFFFFF;
                        memory   = 0xFFFFFFFF;
                        value    = 0xFFFFFFFF;
                        if( ptr[0] == 'r' || ptr[0] == 'R' ) {
                            sscanf( ptr, "%c 0x%X %d", & function, & address, & memory );
                            if( (unsigned char) function != 0xFF && 
                                (unsigned int) address != 0xFFFFFFFF &&
                                (unsigned int) memory != 0xFFFFFFFF )
                            {
                                if( address % 2 == 0  ) {
                                    address ++;
                                    printf( "Warning: i2c_address EVEN!\n" );
                                }
                                if( 160 < address && address < 256 ) {
                                    i2c_address = (char) address;
                                    if( -1 < memory && memory < 256 ) {
                                        if( bin_message_len + 3 < 32 ) {
                                            i2c_memory = (char) memory;
                                            //printf( ":0x%02X[%d]#\n", (unsigned int) i2c_address, (unsigned int) i2c_memory );
                                            bin_message[bin_message_len] = i2c_address;
                                            bin_message_len ++;
                                            bin_message[bin_message_len] = i2c_memory;
                                            bin_message_len ++;
                                        } else {
                                            printf( "Not enough memory in the device!\n" );
                                        }
                                    } else {
                                        printf( "WRONG i2c_memory!\n" );
                                    }
                                } else {
                                    printf( "WRONG i2c_address\n" );
                                }
                            } else {
                                printf( "WRONG message!\n" );
                            }
                        } else if( ptr[0] == 'w' || ptr[0] == 'W' ) {
                            sscanf( ptr, "%c 0x%X %d %d", & function, & address, & memory, & value );

                            if( (unsigned char) function != 0xFF && (unsigned int) address != 0xFFFFFFFF &&
                                (unsigned int) memory != 0xFFFFFFFF && (unsigned int) value != 0xFFFFFFFF ) {
                                if( address % 2 == 0  && 160 < address && address < 256 ) {
                                    i2c_address = (char) address;
                                    if( -1 < memory && memory < 256 ) {
                                        if( bin_message_len + 7 < 32 ) {
                                            i2c_memory = (char) memory;
                                            //printf( ":0x%02X[%d]=%d#\n", (unsigned int) i2c_address, (unsigned int) i2c_memory, value );
                                            bin_message[bin_message_len] = i2c_address;
                                            bin_message_len ++;
                                            bin_message[bin_message_len] = i2c_memory;
                                            bin_message_len ++;
                                            bin_message[bin_message_len] = * ((char *) & value);
                                            bin_message_len ++;
                                            bin_message[bin_message_len] = * (((char *) & value)+1);
                                            bin_message_len ++;
                                            bin_message[bin_message_len] = * (((char *) & value)+2);
                                            bin_message_len ++;
                                            bin_message[bin_message_len] = * (((char *) & value)+3);
                                            bin_message_len ++;
                                        } else {
                                            printf( "Not enough memory in the device!\n" );
                                        }

                                    } else {
                                        printf( "WRONG i2c_memory!\n" );
                                    }
                                } else {
                                    printf( "WRONG i2c_address\n" );
                                }
                            } else {
                                printf( "WRONG message!\n" );
                                printf( "%c %d %d %d\n", function, address, memory, value );
                            }
                        }
                        ptr = strtok( 0, delim );
                    } while( ptr != 0 );
                    bin_message[bin_message_len] = '#';
                    bin_message_len ++;
                }
                message_length = 0;
            } // END else if( buffer[0] == 'b' || buffer[0] == 'B' )
        } // END if( new_message_flag )

        /*
         * Checks for a message in the instrument shared memory
         */
        if( bin_message_len == 0 ) {
            for( i = 0; i < 6; i ++ ) {
                binary_semaphore_wait( semaphore_id );
                if( telescope->encoder[i].message[0] == ':' ) {
                    printf( "[telescope_run] Sending new message for 0x%02X\n", telescope->encoder[i].i2c_address );
                    retval = write_RS232( fd_rs232, telescope->encoder[i].message, 8  );
                    memset( telescope->encoder[i].message, 0, 16 );
                    length = 0;
                    nums_of_time_outs = 0;
                    timeout_error_flag = 0;
                    do {
                        if( (retval = status_RS232( fd_rs232, 1000 )) > 0 ) {
                            //bytes_read = read_RS232( fd_rs232, & m_line[length], 128 - length );
                            bytes_read = read_RS232( fd_rs232, & telescope->encoder[i].answer[length], 16 - length );
                            if( bytes_read > 0 ) {
                                //printf( "[telescope_run] bytes_read = %d\n", bytes_read );
                                length += bytes_read;
                                telescope->encoder[i].answer[length] = '\0';
                            }
                        } else {
                            nums_of_time_outs ++;
                            if( nums_of_time_outs > 10 ) {
                                timeout_error_flag = 1;
                                break;
                            }
                            //printf( "[telescope_run] still waiting for answer...\n" );
                        }
                    } while( length < 10 );
                    if( timeout_error_flag ) {
                        printf( "[telescope_run] timeout ERROR\n" );
                    } else {
                        printf( "[telescope_run] Received the answer from 0x%02X\n", telescope->encoder[i].i2c_address );
                    }
                    //sprintf( telescope->encoder[i].answer, ":0x%02X Chao!#", telescope->encoder[i].i2c_address );
                    //sleep( 1 );
                    //printf( "i2c_device[%d] = %s\n", i, telescope->encoder[i].answer );
                    binary_semaphore_post( read_semaphore_id );
                }
                binary_semaphore_post( semaphore_id );
            }
        }
        /*
         * Transmit new message
         */
        if( bin_message_len != 0 ) {
            retval = write_RS232( fd_rs232, bin_message, bin_message_len );
            if( bin_message_len == retval ) {
                if( verbose && ! reply_flag ) {
                    int ix;
                    int value;

                    printf( "[telescope_run] Writing binary message...\n" );
                    ix = 1;
                    while( bin_message[ix] != '#' && bin_message_len > ix  ) {
                        if( (unsigned char) bin_message[ix] != 0xA1 )
                            printf( "%c", bin_message[0] );
                        if( bin_message[ix] % 2 == 0 ) {
                            ((char *) & value)[0] = bin_message[ix+2];
                            ((char *) & value)[1] = bin_message[ix+3];
                            ((char *) & value)[2] = bin_message[ix+4];
                            ((char *) & value)[3] = bin_message[ix+5];
                            printf( "0x%02X[%d]=%d", (unsigned int) bin_message[ix],
                                    (unsigned int) bin_message[ix+1], value );
                            ix += 6;
                        } else  {
                            printf( "0x%02X[%d]", (unsigned int) bin_message[ix],
                                (unsigned int) bin_message[ix+1] );
                            ix += 2;
                        }
                    }
                    printf( "%c\n", bin_message[ix] );
                }
            }
            bin_message_len = 0;
        }  //END if( bin_message_len != 0 )

        if( message_length != 0 ) {
            if( buffer[0] == ':' &&  buffer[message_length-1] == '#' ) {
                retval = write_RS232( fd_rs232, buffer, message_length  );
                if( message_length == retval ) {
                    if( verbose ) {
                        printf( "[telescope_run] Writing text message...\n" );
                        printf( "%s\n", buffer );
                    }
                }
            } else {
                if( verbose ) {
                    printf( "[telescope_run] Wrong text message: %s\n", buffer );
                }
            }
            message_length = 0;
        } //END if( message_length != 0 )

        //req.tv_sec = 0;
        //req.tv_nsec = 250000;
        //nanosleep( & req, & rem );

        //j ++;
    } while( ! quit );

    if( verbose ) {
        binary_semaphore_wait( semaphore_id );
        gettimeofday( & (telescope->gtime), & (telescope->tzone) );
        localtime_r( & (telescope->gtime.tv_sec), & LTime  );
        strftime( infoline, 24, "%Y-%m-%d %T", & LTime );
        printf( "[telescope_run] Good bye!\n%s\n", infoline );
        binary_semaphore_post( semaphore_id );
    }

    /** Close RS232 device */
    if( fd_rs232 > 0 ) {
        if( verbose ) printf( "[telescope_run] closing fd_rs232...\n");
        if( close( fd_rs232 ) < 0 ) {
            perror( "[telescope_run] close");
            if( verbose ) printf( "[telescope_run] close ERROR.\n");
        } else {
            if( verbose ) printf( "[telescope_run] close OK.\n");
        }
    }

    /**
     * INSTRUMENT SHARED MEMORY
     */
    /** Detach the shared memory segment */
    shmdt( shared_memory );

    /** Deallocate the shared memory segment */
    shmctl( segment_id, IPC_RMID, 0 );

    /** Deallocate the semaphore */
    binary_semaphore_deallocate( semaphore_id );
    binary_semaphore_deallocate( read_semaphore_id );


    /**
     * USER SHARED MEMORY
     */
    /** Detach the shared memory segment */
    shmdt( user_shared_memory );

    /** Deallocate the shared memory segment */
    shmctl( user_segment_id, IPC_RMID, 0 );

    /** Deallocate the semaphore */
    binary_semaphore_deallocate( user_semaphore_id );
    //return 0;

    /*
    close( m_socket_fd );
    unlink( m_server_name.sun_path );
    */
}
