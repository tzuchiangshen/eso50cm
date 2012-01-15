#include <myServerSocket.h>

myUxSckSrv::myUxSckSrv( const char * socket_name )
{
    m_server_socket_fd = 0;
    m_socket_is_bind = true;
    m_socket_is_listen = true;
    m_socket_is_accept = true;
    m_socket_is_connected = true;

    do {
        if( (m_server_socket_fd = socket( PF_LOCAL, SOCK_STREAM, 0 )) < 0 ){
            perror( "[myUxSckSrv::myUxSckSrv] socket" );
            if( verbose ) printf( "[myUxSckSrv::myUxSckSrv] socket ERROR\n" );
            break;
        }
        if( verbose ) printf( "[myUxSckSrv::myUxSckSrv] socket OK.\n" );
        
        /** Construct a socket address structure for the socket_name on
            which we want to listen for connections.  */
        m_socket_address.sun_family= AF_LOCAL;
        //strcpy( m_socket_address.sun_path, socket_name );
        m_socket_address.sun_path = socket_name;

        /** Bind the socket to that address.  */
        if( bind( m_server_socket_fd, (struct sockaddr *) & m_socket_address, SUN_LEN( & m_socket_address ) ) < 0  ) {
            perror( "[myUxSckSrv::myUxSckSrv] bind" );
            if( verbose ) printf( "[myUxSckSrv::myUxSckSrv] bind ERROR\n" );
            m_socket_is_bind = false;
        }
        if( verbose ) printf( "[myUxSckSrv::myUxSckSrv] bind OK.\n" );

        /**  Instruct the socket to accept connections.  */
        if( listen( m_server_socket_fd, 5 ) < 0 ) {
            perror( "[myUxSckSrv::myUxSckSrv] listen");
            if( verbose ) printf( "[myUxSckSrv::myUxSckSrv] listen ERROR.\n");
            m_socket_is_listen = false;
            break;
        }
        if( verbose ) printf( "[myUxSckSrv::myUxSckSrv] listen OK.\n" );

    } while( 0 );

    if( ! m_server_socket_fd || ! m_socket_is_bind || ! m_socket_is_listen ) {
        if( verbose ) printf( "[myUxSckSrv::myUxSckSrv] Something wrong!\n");
        m_uxsrv_is_running = false;
    } else {
        if( verbose ) printf( "[myUxSckSrv::myUxSckSrv] Everything looks OK!\n");
        m_uxsrv_is_running = true;
    }
}

/**
 *  Class Destructor
 */
myUxSckSrv::~myUxSckSrv( void )
{
    printf( "[myUxSckSrv::~myUxSckSrv] Exiting...\n" );

    if( m_server_socket_fd > 0  ) {
        if( close( m_server_socket_fd ) < 0 ) {
            perror( "[myUxSckSrv::~myUxSckSrv] close" );
            if( verbose ) printf( "[myUxSckSrv::~myUxSckSrv] close ERROR\n" );
        }
    }
    printf( "[myUxSckSrv::~myUxSckSrv] Good bye!\n" );
}

/**
 *  server_run
 */
int myUxSckSrv::server_run( void )
{
    int retval = 0;

    return retval;
}


