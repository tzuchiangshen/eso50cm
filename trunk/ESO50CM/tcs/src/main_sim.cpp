#include <assert.h>
#include <getopt.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include <termios.h>
//#include <fcntl.h>
//#include <sys/times.h>
//#include <sys/ioctl.h>

#include <errno.h>

#include "./telescope.h"

/** Description of long options for getopt_long.  */
static const struct option long_options[] = {
    { "baudrate",       1, NULL, 'b' },
    { "device",         1, NULL, 'd' },
    { "help",           0, NULL, 'h' },
    { "module-dir",     1, NULL, 'm' },
    { "sck_path",       1, NULL, 's' },
    { "verbose",        0, NULL, 'v' }
};

/** Description of short options for getopt_long.  */
static const char* const short_options = "b:d:hm:s:v";

/* Usage summary text.  */
static const char * const usage_template =
    "Usage: %s [ options ]\n"
    "  -b, --baudrate BAUDS      BAUDS is an integer.\n"
    "                               (by default baudrate 57600.)\n"
    "  -d, --device DEV          Default device /dev/ttyS0.\n"
    "  -h, --help                Print this information.\n"
    "  -m, --module-dir DIR      Load modules from specified directory\n"
    "                               (by default, use executable directory).\n"
    "  -s, --sck_path PATH       Ux Socket  path to e used.\n"
    "  -v, --verbose             Print verbose messages.\n";

/** Print usage information and exit.  If IS_ERROR is nonzero, write to
    stderr and use an error exit code.  Otherwise, write to stdout and
    use a non-error termination code.  Does not return.  */
static void print_usage( int is_error ) {
    fprintf (is_error ? stderr : stdout, usage_template, program_name);
    exit (is_error ? 1 : 0); 
}

void test_device( char * device ) {
    struct stat dir_info;

    /** Check that it exists.  */
    if( access( device, F_OK ) != 0 ) {
        error( device, "device does not exist");
    } else {
        if( verbose )
            printf( "device %s exists\n", device );
    }
    /** Check that it is accessible.  */
    if( access( device, R_OK ) != 0) {
        error (device, "device is not readable");
    } else {
        if( verbose )
            printf( "device %s is readable\n", device );
    }
    if( access( device, W_OK) != 0) {
        error (device, "device is not writable");
    } else {
        if( verbose )
            printf( "device %s is writable\n", device );
    }
}


int main( int argc, char* const argv[] ) {
    int next_option;
    char * device;
    speed_t baudrate;
    char * module_dir;
    char * socket_path;

    /** Store the program name, which well use in error messages.  */
    program_name = argv[0];

    /** Set defaults for optionsfor device and baudrate.  */
    device = NULL;
    baudrate = B0;

    /** Load modules from the directory containing this executable.  */
    module_dir = get_self_executable_directory ();
    assert( module_dir != NULL );

    /** Parse options.  */
    do {
        next_option =
        getopt_long (argc, argv, short_options, long_options, NULL);
        switch (next_option) {
            case 'b':
               {
                    long value;
                    char * end;
                    value = strtol (optarg, & end, 10);
                    if( * end != '\0' )
                        /** The user specified nondigits in the port number.  */ 
                        print_usage (1);
                    /** The baudrate.  */
                    //printf( "baudrate = %ld\n", value );
                    switch( value ) {
                        case 0:
                            baudrate = B0;
                            break;
                        case 9600:
                            baudrate = B9600;
                            break;
                        case 19200:
                            baudrate = B19200;
                            break;
                        case 38400:
                            baudrate = B38400;
                            break;
                        case 57600:
                            baudrate = B57600;
                            break;
                        case 115200:
                            baudrate = B115200;
                            break;
                        case 230400:
                            baudrate = B230400;
                            break;
                        default:
                            baudrate = B57600;
                    }
                }
                break;
            case 'd':
                /** User specified -d or --device.  */
                device = strdup( optarg );
                break;
            case 'h':
                /** User specified -h or --help.  */
                print_usage( 0 );
            case 'm':
                /** User specified -m or --module-dir.  */
                {
                    struct stat dir_info;
                    /** Check that it exists.  */
                    if (access (optarg, F_OK) != 0)
                        error( optarg, "module directory does not exist");
                    /** Check that it is accessible.  */
                    if (access (optarg, R_OK |X_OK) != 0)
                        error( optarg, "module directory is not accessible");
                    /** Make sure that it is a directory.  */
                    if (stat (optarg, &dir_info) != 0 ||!S_ISDIR (dir_info.st_mode))
                        error( optarg, "not a directory" );
                    /** It looks OK, so use it.  */
                    module_dir = strdup (optarg);
                }
                break;
            case 's':
                socket_path = optarg;
                break;
            case 'v':
                /** User specified -v or --verbose.  */ 
                verbose = 1;
                break; 
            case '?':
                /** User specified an unrecognized option.  */ 
                print_usage( 1 );
            case -1:
                /** Done with options.  */
                break;
            default:
                abort ();
        }
    } while( next_option != -1 );
    /** This program takes no additional arguments.  Issue an error if the 
        user specified any.  */
    if( optind != argc )
        print_usage( 1 );

    if( device == NULL ) {
      device = (char *) malloc( sizeof( "/dev/ttyS0" ) );
        strcpy( device, "/dev/ttyS0" );
        if( verbose ){
            printf( "No device was secified, using default %s\n", device );
        }
    }

    if( baudrate == 0 ) {
        baudrate = B57600;
        if( verbose ){
            printf( "No baudrate was secified, using default %d\n", (int) baudrate );
        }
    }


//    if( strcmp( device, "debug" ) != 0 ) {
//        //struct stat dir_info;
//        /** Check that it exists.  */
//        if( access( device, F_OK ) != 0 ) {
//            error( device, "device does not exist");
//        } else {
//            if( verbose )
//                printf( "device %s exists\n", device );
//        }
//        /** Check that it is accessible.  */
//        if( access( device, R_OK ) != 0) {
//            error (device, "device is not readable");
//        } else {
//            if( verbose )
//                printf( "device %s is readable\n", device );
//        }
//        if( access( device, W_OK) != 0) {
//            error (device, "device is not writable");
//        } else {
//            if( verbose )
//                printf( "device %s is writable\n", device );
//        }
//    }


    /** Print  module directory if were running verbose.  */
    if( verbose ) {
        printf( "modules will be loaded from %s\n", module_dir );
        printf( "...\n" );
    }
    /** Run the server.  */
    telescope_run( device, baudrate, socket_path );
    return 0;
}
