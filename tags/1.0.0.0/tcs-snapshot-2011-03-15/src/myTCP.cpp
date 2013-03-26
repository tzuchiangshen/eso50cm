#include <myTCP.h>

extern int verbose;

char * bad_request_template =
    "HTTP/1.0 400 Bad Request\n"
    "Content-type: text/html\n"
    "\n"
    "<html>\n"
    " <body>\n"
    "  <h1>Bad Request</h1>\n"
    "  <h2>%s: [%s]!</h2>\n"
    " </body>\n"
    "</html>\n";

char * bad_reqtxt_template =
    "Bad Request\n"
    "%s: [%s]!\n";


/**
 * Handler for SIGCHLD, to clean up child processes that have
 * terminated.
 */
void clean_up_child_process( int signal_number ) {
    int status;
    wait( & status );
}

myTCP::myTCP( struct in_addr local_address, uint16_t port )
{
    m_server_socket_fd = 0;
    m_socket_is_bind = true;
    m_socket_is_listen = true;
    m_socket_is_accept = true;

    do {
        /** Create a TCP socket.  */
        if( (m_server_socket_fd = socket( PF_INET, SOCK_STREAM, 0 ) ) < 0 ) {
            perror( "[myTCP::myTCP] socket" );
            if( verbose ) printf( "[myTCP::myTCP] socket ERROR\n" );
            break;
        }
        if( verbose ) printf( "[myTCP::myTCP] socket OK.\n" );
        /** Construct a socket address structure for the local address on
            which we want to listen for connections.  */
        m_socket_address.sin_family = AF_INET;
        m_socket_address.sin_addr   = local_address;
        m_socket_address.sin_port   = port;

        /** Bind the socket to that address.  */
        if( bind( m_server_socket_fd, (struct sockaddr *) & m_socket_address, sizeof( m_socket_address ) ) < 0 ) {
            perror( "[myTCP::myTCP] bind" );
            if( verbose ) printf( "[myTCP::myTCP] bind ERROR\n" );
            m_socket_is_bind = false;
            break;
        }
        if( verbose ) printf( "[myTCP::myTCP] bind OK.\n" );

        /**  Instruct the socket to accept connections.  */
        if( listen( m_server_socket_fd, 5 ) < 0 ) {
            perror( "[myTCP::myTCP] listen");
            if( verbose ) printf( "[myTCP::myTCP] listen ERROR.\n");
            m_socket_is_listen = false;
            break;
        }
        if( verbose ) printf( "[myTCP::myTCP] listen OK.\n" );

    } while( 0 );

    if( ! m_server_socket_fd || ! m_socket_is_bind || ! m_socket_is_listen ) {
        if( verbose ) printf( "[myTCP::myTCP] Something wrong!\n");
        m_tcp_is_running = false;
    } else {
        if( verbose ) printf( "[myTCP::myTCP] Everything looks OK!\n");
        m_tcp_is_running = true;
    }
}

/**
 *  Class Destructor
 */
myTCP::~myTCP()
{
    printf( "[myTCP::~myTCP] Exiting...\n" );

    if( m_server_socket_fd > 0  ) {
        if( close( m_server_socket_fd ) < 0 ) {
            perror( "[myTCP::~myTCP] close" );
            if( verbose ) printf( "[myTCP::~myTCP] close ERROR\n" );
        }
    }
    printf( "[myTCP::~myTCP] Good bye!\n" );
}


/**
 *  server_run
 */
int myTCP::server_run( void )
{
    int retval = 1;
    int k = 0;
    struct sigaction sigchld_action;

    /** Install a handler for SIGCHLD that cleans up child processes that
        have terminated.  */
    memset( & sigchld_action, 0, sizeof( sigchld_action ) );
    sigchld_action.sa_handler = & clean_up_child_process; 
    sigaction( SIGCHLD, & sigchld_action, NULL );

    if( verbose ) printf( "[myTCP::server_run] Hello world!\n" );

    m_lcu = new myLCU();
    m_lcu->createTelescope();
    m_lcu->telescope->attachInstrumentMemory();

    do {
        struct sockaddr_in remote_address; 
        socklen_t address_length;
        int connection;
        pid_t child_pid;

        /** Accept a connection.  This call blocks until a connection is 
            ready.  */
        address_length = sizeof( remote_address );
        connection = accept( m_server_socket_fd, (struct sockaddr *) & remote_address, & address_length );
        if( connection == -1 ) {
            /** The call to accept failed.  */
            if( errno == EINTR ) {
                /** The call was interrupted by a signal.  Try again.  */
                continue;
            } else {
                /** Something else went wrong.  */
                perror ("[myTCP::server_run] accept");
            }
        }

        /** We have a connection.  Print a message if were running in 
            verbose mode.  */
        if( verbose ) {
            socklen_t address_length;
            /** Get the remote address of the connection.  */
            address_length = sizeof( m_socket_address );
            if( getpeername( connection, (struct sockaddr *) & m_socket_address, &address_length ) < 0  ) {
                perror( "[myTCP::run] getpeername" );
                if( verbose ) printf( "[myTCP::server_run] getpeername ERROR\n" );
            }
            //assert (rval == 0);
            /** Print a message.  */
            if( verbose ) printf ("[myTCP::server_run] Connection accepted from %s\n",
                inet_ntoa( m_socket_address.sin_addr ) );
        }

        /** Fork a child process to handle the connection.  */
        child_pid = fork ();
        if( child_pid == 0 ) {
            //if( verbose ) printf("[myTCP::run] I am the Child!\n");
            /** This is the child process. */
            if( verbose ) {
                printf( "[myTCP::server_run] I am the Child!\n" );
                //printf( "[myTCP::server_run] m_lcu_data at address  %p\n", m_lcu->get_lcu_data_address() );
            }
            /** Handle connection. */
            handle_connection( connection );
            close( connection );
            m_tcp_is_running = false;
            retval =  0;
        } else if( child_pid > 0 ) {
            /** This is the parent process.  The child process handles the
                connection, so we dont need our copy of the connected socket
                descriptor.  Close it.  Then continue with the loop and
                accept another connection.  */
            if( verbose ) {
                printf( "[myTCP::server_run] I am the Parent!\n" );
                //printf( "[myTCP::server_run] m_lcu_data at address  %p\n", m_lcu->get_lcu_data_address() );
            }
            close( connection );
            k ++;
            if( k > 100000 ) {
                if( verbose ) {
                    printf("[myTCP::server_run] This was the %dth connection.\n", k );
                    printf("[myTCP::server_run] No more connections!\n" );
                }
                m_tcp_is_running = false;
                retval =  1;
            }
        } else {
            /** Call to fork failed.  */
            perror("[myTCP::server_run] fork");
            if( verbose ) printf("[myTCP::server_run] fork ERROR\n");
        }
    } while( m_tcp_is_running );
    if( verbose ) printf( "[myTCP::server_run] Good bye!\n" );
    delete m_lcu;
    return retval;
}


/**
 *  handle_connection
 */
void myTCP::handle_connection( int connection_fd )
{
    char buffer[256];
    ssize_t bytes_read;

    if( verbose ) printf( "[myTCP::handle_connection] Hello World!\n" );
    /** Read some data from the client.  */
    bytes_read = read( connection_fd, buffer, sizeof( buffer ) - 1 );
    if( bytes_read > 0 ) {
        char method[ sizeof( buffer ) ];
        char url[ sizeof( buffer ) ];
        char protocol[ sizeof( buffer ) ];

        /** Some data was read successfully.
            NUL-terminate the buffer so
            we can use string operations on it.  */
        buffer[bytes_read] = '\0';
        /** The first line the client sends is the HTTP request, which is
            composed of a method, the requested page, and the protocol
            version. */
        sscanf( buffer, "%s %s %s", method, url, protocol );
        if( verbose ) {
            printf( "[myTCP::handle_connection] %s %s %s\n", method, protocol, url );
        }
        /** The client may send various header information following the
            request.  For this HTTP implementation, we dont care about it.
            However, we need to read any data the client tries to send.  Keep
            on reading data until we get to the end of the header, which is
            delimited by a blank line.  HTTP specifies CR/LF as the line
            delimiter.  */
        while( strstr( buffer, "\r\n\r\n" ) == NULL ) {
            bytes_read = read( connection_fd, buffer, sizeof (buffer) );
        }

        /** Make sure the last read didnt fail.  If it did, theres a
            problem with the connection, so give up.  */
        if( bytes_read == -1 ) {
            if( verbose ) printf( "[myTCP::handle_connection] Connection closed by client.\n" );
            close( connection_fd );
            return;
        }

        /** Check the protocol field.  We understand HTTP versions 1.0 and 
            1.1.  */
        if( strcmp( protocol, "HTTP/1.0" ) != 0 && strcmp( protocol, "HTTP/1.1" ) != 0 ) {
            /** We dont understand this protocol.  Report a bad response.  */
            char * response = NULL;
            response = (char *) malloc( strlen( bad_request_template ) + strlen( protocol ) + 16 );
            sprintf( response, bad_request_template, "Wrong Protocol:", protocol );
            write( connection_fd, response, sizeof( response ) );
            free( response );
        } else if( strcmp( method, "GET") != 0 ) {
            /** This server only implements the GET method.  The client 
                specified some other method, so report the failure.  */ 
            char * response = NULL;
            response = (char *) malloc( strlen( bad_request_template ) + strlen( method ) + 16 );
            sprintf( response, bad_request_template, "Wrong Method:", method );
            write( connection_fd, response, sizeof( response ) );
            free( response );
        }  else {
            /** A valid request.  Process it.  */
            handle_get( connection_fd, url );
        }

    } else if( bytes_read == 0 ) {
        /** The client closed the connection before sending any data. 
            Nothing to do.  */
        ;
    } else {
        /** The call to read failed.  */ 
        perror( "[myTCP::handle_connection] read" );
        if( verbose ) printf( "[myTCP::handle_connection] read ERROR\n" );
    }
    //printf( "[myTCP::handle_connection] Good bye!\n" );

}

/**
 *  handle_get
 */
void myTCP::handle_get( int connection_fd, const char * page )
{
    char module_file_name[64];
    char module_file_path[64];

    char arguments[256];

    char * ptr;
    void * handle = NULL;

    if( verbose ) printf( "[myTCP::handle_get] Hello World!\n" );
    /** Make sure the requested page begins with a slash and does not
        contain any additional slashes -- we dont support any
        subdirectories. */
    if( * page == '/' && strchr( page + 1, '/' ) == NULL) {
        /** Check page for arguments */
        * arguments = 0;
        if( (ptr = strchr( page, (char) '?' )) != NULL) {
            /** The requested page has arguments, then extracts them */
            snprintf( arguments, sizeof( arguments ), "%s", ptr + 1 );
            /** Separate the page from arguments */
            * ptr = 0;
        }
        /** The page name looks OK.  Construct the module name by appending
            so to the page name. */
        snprintf( module_file_name, sizeof (module_file_name), "%s.so", page + 1 );
        snprintf( module_file_path, sizeof (module_file_path), "./%s.so", page + 1 );
        if( verbose ) {
            printf( "[myTCP::handle_get] %s", module_file_name );
            if( * arguments != 0 ) {
                printf( " %s", arguments );
            }
            printf( "\n" );
        }

        /** Try to open the module. */
        //module = module_open( module_file_name );
        handle = dlopen( module_file_path, RTLD_NOW );
    }
    if( handle == NULL ) {
        if( verbose ) printf( "[myTCP::handle_get] Bad Request\n" );
        if( strstr( arguments, "txt" ) != NULL ) {
            /** Client has requested non HTML output */
            char * response = NULL;
            response = (char *) malloc( strlen( bad_reqtxt_template ) + strlen( page + 1 ) + 16  );
            if( response == NULL ) {
                perror( "[myTCP::handle_text] malloc" );
                printf( "[myTCP::handle_text] malloc ERROR\n" );
            } else {
                sprintf( response,  bad_reqtxt_template, "Wrong module:", page + 1 );
                write( connection_fd, response, strlen( response ) );
                free( response );
            }
        } else {
            /** HTML output */
            char * response = NULL;
            response = (char *) malloc( strlen( bad_request_template ) + strlen( page + 1 ) + 16  );
            if( response == NULL ) {
                perror( "[myTCP::handle_get] malloc" );
                printf( "[myTCP::handle_get] malloc ERROR\n" );
            } else {
                sprintf( response,  bad_request_template, "Wrong module:", page + 1 );
                write( connection_fd, response, strlen( response ) );
                free( response );
            }
        }
    } else {
        typedef void (* module_generate_t)( int, const char *, myLCU * );
        /** Reset errors */
        dlerror();
        module_generate_t module_generate = (module_generate_t) dlsym( handle, "module_generate" );
        const char * dlsym_error = dlerror();
        if( dlsym_error ) {
            //cerr << "[Main] dlsym error!\n[Main] " << dlsym_error << '\n';
            dlclose(handle);
        } else {
            module_generate( connection_fd, arguments, m_lcu );
            dlclose(handle);
        }
    }
}

/**
 *  handle_txt
 */
//void myTCP::handle_txt( int connection_fd, const char * page )
//{
    //char module_file_name[64];
    //char module_file_path[64];
    //char arguments[256];
    //char * ptr;
    //void * handle = NULL;

    //if( verbose ) printf( "[myTCP::handle_text] Hello World!\n" );
    /** Make sure the requested page begins with a slash and does not
        contain any additional slashes -- we dont support any
        subdirectories. */
    //if( * page == '/' && strchr( page + 1, '/' ) == NULL) {
        /** Check page for arguments */
    //     * arguments = 0;
    //    if( (ptr = strchr( page, '?' )) != NULL) {
            /** The requested page has arguments, then extracts them */
    //        snprintf( arguments, sizeof( arguments ), "%s", ptr + 1 );
            /** Separate the page from arguments */
    //        * ptr = 0;
    //    }
        /** The page name looks OK.  Construct the module name by appending
            so to the page name. */
    //    snprintf( module_file_name, sizeof (module_file_name), "%s.so", page + 1 );
    //    snprintf( module_file_path, sizeof (module_file_path), "./%s.so", page + 1 );
    //    if( verbose ) {
    //        printf( "[myTCP::handle_text] %s", module_file_name );
    //        if( * arguments != 0 ) {
    //            printf( " %s", arguments );
    //        }
    //        printf( "\n" );
    //    }

        /** Try to open the module. */
        //module = module_open( module_file_name );
    //    handle = dlopen( module_file_path, RTLD_NOW );
    //}
    //if( handle == NULL ) {
    //    printf( "[myTCP::handle_text] Bad Request\n" );
    //    char * bad_req_template =
    //        "Bad Request\n"
    //        "Module [%s] not found!\n";
    //    char * response = NULL;
    //    response = (char *) malloc( strlen( bad_req_template ) + strlen( page + 1 )  );
    //    if( response == NULL ) {
    //        perror( "[myTCP::handle_text] malloc" );
    //        printf( "[myTCP::handle_text] malloc ERROR\n" );
    //    } else {
    //        sprintf( response,  bad_req_template, page + 1 );
            //fwrite( response,  sizeof( char ),  strlen( response ), stdout );
    //        write( connection_fd, response, strlen( response ) );
    //        free( response );
    //    }
    //} else {
    //    typedef void (* module_generate_txt_t)( int, const char *, myLCU * );
        /** Reset errors */
    //    dlerror();
    //    module_generate_txt_t module_generate_txt = (module_generate_txt_t) dlsym( handle, "module_generate_txt" );
    //    const char * dlsym_error = dlerror();
    //    if( dlsym_error ) {
            //cerr << "[Main] dlsym error!\n[Main] " << dlsym_error << '\n';
    //        dlclose(handle);
    //    } else {
    //        module_generate_txt( connection_fd, arguments, m_lcu );
    //        dlclose(handle);
    //    }
    //}
//}



