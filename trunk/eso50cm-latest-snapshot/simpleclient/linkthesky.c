#include <assert.h>
#include <getopt.h>

#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

#include <string.h>

#include <errno.h>

#include <math.h>

char * program_name;
int verbose;

/** Description of long options for getopt_long.  */
static const struct option long_options[] =
{
    { "address",        1, NULL, 'a' },
    { "port",           1, NULL, 'p' },
    { "input",          1, NULL, 'i' },
    { "output",         1, NULL, 'o' },
    { "help",           0, NULL, 'h' },
    { "verbose",        0, NULL, 'v' }
};

/** Description of short options for getopt_long.  */
static const char * const short_options = "a:p:i:o:hv";

/** Usage summary text.  */
static const char * const usage_template =
    "Usage: %s [ options ]\n"
    "  -a, --address SERVER      Server address.\n"
    "  -p, --port PORT           Port used by the server.\n"
    "  -i, --input FILE          Input FILE for command's parameters\n"
    "                            (not specified asumes stdin).\n"
    "  -o, --output FILE         Outout file (default is stdout).\n"
    "  -h, --help                Print this information.\n"
    "  -v, --verbose             Print verbose messages.\n";

/** Print usage information and exit.  If IS_ERROR is nonzero, write to
    stderr and use an error exit code.  Otherwise, write to stdout and
    use a non-error termination code.  Does not return.  */
static void print_usage( int is_error )
{
    fprintf( is_error ? stderr : stdout, usage_template, program_name );
    exit( is_error ? 1 : 0 );
}

/** get_info */
void get_info( int fd, char * url )
{
    char buffer[1024];
    ssize_t bytes_read;
    char no_quit;
    char monitor_flag;

    /** Send the HTTP GET command */
    sprintf( buffer, "TXT /%s HTTP/1.1\r\n\r\n", url );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "\r\n\r\n" );
    write( fd, buffer, strlen( buffer ) );
    /** Read from socket */
    //bytes_read = 0;

    if( strstr( url, "monitor" )  ) {
        no_quit = 20;
        monitor_flag = 1;
    } else {
        no_quit = 1;
        monitor_flag = 0;
    }
    do{
        while( 1 ) {
            bytes_read = read( fd, buffer, 1024 );
            fwrite( buffer, sizeof( char ), bytes_read, stdout );
            if( strstr( buffer, "\r\n\r\n" )  ) {
                if( verbose ) printf( "END of data!\n" );
                buffer[0] = 0;
                no_quit --;  //<<<---here
                break;
            }
        }
        //no_quit --;
        if( no_quit > 0 ) {
            sleep( 1 );
            if( verbose ) printf( "To quit = %d\n", no_quit );
            write( fd, "data", 4 );
        } else if( monitor_flag ) {
            //if( verbose ) printf( "[get_info] quit = %d\n", no_quit );
            write( fd, "quit", 4 );
        }
    } while( no_quit );

    while( 1 ) {
        bytes_read = read( fd, buffer, 1024 );
        if( bytes_read == 0 ) {
            if( verbose ) printf( "Connection closed!\n" );
            return;
        }
        fwrite( buffer, sizeof( char ), bytes_read, stdout );
    }
    if( verbose ) printf( "Good bye!\n" );
}

/** Main */
int main( int argc, char * const argv[] )
{
    int next_option;
    char * server_name;
    char * default_server = "192.168.0.10";
    char * input_file_name = NULL;
    char * output_file_name = NULL;
    FILE * fp_in = NULL;
    FILE * fp_out = NULL;
    int port = 4000;

    /** Store the program name, which well use in error messages.  */
    program_name = argv[0];

    /** Set defaults for options. */
    verbose = 0;
    server_name = default_server;


    /** Parse options.  */
    do {
        next_option = getopt_long( argc, argv, short_options, long_options, NULL );
        switch( next_option ) {
            case 'a':
                server_name = optarg;
                //printf( "server address: %s\n", server_name );
                break;
            case 'p':
                port = atoi( optarg );
                //printf( "server port: %d\n", port );
                break;
            case 'h':
                /** User specified -h or --help.  */
                print_usage( 0 );
                break;
            case 'v':
                /** User specified -v or --verbose.  */ 
                verbose = 1;
                break;
            case 'i':
                //if( optarg == NULL ) {
                //    fp_in = stdin;
                //} else {
                //    fp_in = NULL;
                //}
                input_file_name = optarg;

                break;
            //case '?':
            //    if( verbose ) printf( "Option ?: %s\n",  );
            //    /** User specified an unrecognized option.  */ 
            //    print_usage( 1 );
            //    break;
            //case -1:
            //    /** Done with options.  */
            //    break;
            //default:
            //    abort ();
        }
    } while( next_option != -1 );

    if( verbose ) printf( "Hello World!\n" );

    char command[128];
    int i, k;

    memset( command, 0, 128 );
    i = optind;
    if( i < argc ) {
        strcat( command, argv[i] );
        i ++;
        if( i < argc ) {
            strcat( command, "?" );
            strcat( command, argv[i] );
            i ++;
            //if( verbose ) printf( "command: [%s] (%d)\n", command, i );
            while( i < argc ) {
                strcat( command, "&" );
                strcat( command, argv[i] );
                i ++;
                //if( verbose ) printf( "command: [%s] (%d)\n", command, i );
            }
        }
        //if( verbose ) printf( "command: [%s]\n", command );
    } else {
        if( verbose ) printf( "no command, nothing to do.\n" );
        return 0;
    }

    if( input_file_name != NULL ) {
        if( strcmp( input_file_name, "0" ) == 0 ) {
            if( verbose ) printf( "reading arguments from standard input...\n" );
            fp_in = stdin;
            i = strlen( command );
            k = 0;
            while( ! feof( fp_in ) ){
                if( k == 0 ) {
                    command[i] = '?';
                } else {
                    command[i] = '&';
                }
                i++;
                command[i] = 0;
                if( fscanf( fp_in, "%s\n", & command[i] ) > 0 ) {
                    //if( verbose ) printf( "argument %d: %s\n", i+1, & command[i] );
                    i += strlen( & command[i] );
                    k ++;
                } else {
                    perror( "fscanf" );
                }
            }
        } else {
            if( verbose ) printf( "reading from %s...\n", input_file_name );
        }
    }
    if( verbose ) printf( "command: [%s]\n", command );


    int socket_fd;
    struct sockaddr_in server;
    struct hostent * hostinfo;

    /** Create the socket */
    socket_fd = socket( PF_INET, SOCK_STREAM, 0 );
    /** Store the server's name in the socket address */
    server.sin_family = AF_INET;
    /** Convert from string to numbers */
    hostinfo = gethostbyname( server_name );
    if( hostinfo == NULL ) {
        perror( "hostinfo" );
        return 1;
    } else {
        server.sin_addr = * ( (struct in_addr *) hostinfo->h_addr );
    }
    /** Server port */
    server.sin_port = htons( port );
    /** Connect to the server */
    if( verbose ) printf( "traying to connect to %s:%d...\n", server_name, port );
    if( connect( socket_fd, (struct sockaddr *) & server, sizeof( struct sockaddr_in ) ) < 0 ) {
        perror( "connect" );
        return 1;
    } else {
        if( verbose ) printf( "connected to server...\n" );
    }

    if( verbose ) printf( "executing command: [%s]...\n", command );
    if( verbose ) printf( "\n\n" );
    get_info( socket_fd, command );
    close( socket_fd );
    if( verbose ) printf( "Good bye!\n" );
    return 0;
}
