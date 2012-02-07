#include <myServerSocket.h>

myServerSocket::myServerSocket( const char * socket_name ) {
    //strcpy( m_client_name, socket_name );

    m_socket_is_bind = false;
    m_socket_is_listen = false;
    m_socket_is_accept = false;
    m_client_is_connected = false;


    if( (m_socket_fd = socket( PF_LOCAL, SOCK_STREAM, 0 )) > 0 ){
        printf( "[myServerSocket::myServerSocket] %s socket OK.\n", socket_name );
        m_name.sun_family= AF_LOCAL;
        strcpy( m_name.sun_path, socket_name );
        if( bind( m_socket_fd, (struct sockaddr *) & m_name, SUN_LEN( & m_name ) ) == 0  ) {
            printf( "[myServerSocket::myServerSocket] %s bind OK\n", m_name.sun_path );
            m_socket_is_bind = true;
            if( listen( m_socket_fd, 5 ) == 0 ) {
                printf( "[myServerSocket::myServerSocket] %s listen OK\n", m_name.sun_path );
                m_socket_is_listen = true;
            } else {
                perror( "[myServerSocket::myServerSocket] listen" );
                printf( "[myServerSocket::myServerSocket] %s listen ERROR\n", m_name.sun_path );
            }
        } else {
            perror( "[myServerSocket::myServerSocket] bind" );
            printf( "[myServerSocket::myServerSocket] %s bind ERROR\n", m_name.sun_path );
        }
    } else {
        perror( "[myServerSocket::myServerSocket] socket" );
        printf( "[myServerSocket::myServerSocket] %s socket ERROR\n", m_name.sun_path );
    }
    
    memset( m_buffer, 0, 128 );
    m_buffer_bytes = 0;
    _tid = 0;
}

/**
 *  Class Destructor
 */
myServerSocket::~myServerSocket( void ) {
    printf( "[myServerSocket::~myServerSocket] Exiting...\n" );
    if( stop() == 0 ) {
        printf( "[myServerSocket::~myServerSocket] stop() returned succesfully.\n" );
    } else {
        printf( "[myServerSocket::~myServerSocket] stop() returned with error.\n" );
    }

    if( m_client_is_connected ) {
        if( close( m_client_socket_fd ) == 0 ) {
            printf( "[myServerSocket::~myServerSocket] close(%d) OK\n", m_client_socket_fd );
        } else {
            perror( "[myServerSocket::~myServerSocket] close" );
            printf( "[myServerSocket::~myServerSocket] close(%d) ERROR\n", m_client_socket_fd );
        }
    }
    if( close( m_socket_fd ) == 0 ) {
        printf( "[myServerSocket::~myServerSocket] close(%d) OK\n", m_socket_fd  );
    } else {
        perror( "[myServerSocket::~myServerSocket] close" );
        printf( "[myServerSocket::~myServerSocket] close(%d) ERROR\n", m_socket_fd  );
    }
    if( ( unlink( m_name.sun_path ) ) == 0 ) {
        printf( "[myServerSocket::~myServerSocket] %s unlink OK.\n", m_name.sun_path );
    } else {
        perror( "[myServerSocket::~myServerSocket] unlink" );
        printf( "[myServerSocket::~myServerSocket] %s unlink ERROR\n", m_name.sun_path );
    }
    printf( "[myServerSocket::~myServerSocket] Good bye!\n" );
}

/**
 *  stop thread
 */
int myServerSocket::stop( void ) {
    int retval = 0;
    
    if ( _tid ) {
        m_socket_is_running = false;
        while( ! m_socket_is_running ) {
            sleep(1);
        }
        m_socket_is_running = false;
        printf( "[myServerSocket::stop] Stoping thread (tid=%lu)...\n", _tid );
        if( ( retval = pthread_join( _tid, NULL ) )  == 0 ) {
            printf( "[myServerSocket::stop] pthread_join OK\n" );
        } else {
            perror( "[myServerSocket::stop] pthread_join" );
            printf( "[myServerSocket::stop] pthread_join ERROR\n" );
        }
    } else {
        printf( "[myServerSocket::stop] No thread to stop\n" );
    }

    return retval;
}

/**
 *  start thread
 */
int myServerSocket::start() {
    int retval;

    if( (retval = pthread_create( & _tid, NULL, gate, this ) ) == 0 ) {
        printf( "[myServerSocket::start] pthread_create OK\n" );
    } else {
        perror( "[myServerSocket::start] pthread_create" );
        printf( "[myServerSocket::start] pthread_create ERROR\n" );
    }
    return retval;
}

/**
 * gate
 * @brief used to start run through pthread_create
 */
void * myServerSocket::gate( void * p ) {
    ( (myServerSocket *)p )->run();
    return NULL;
}

/**
 *  Thread loop
 */
void myServerSocket::run( void ) {
    int retval;

    printf( "[myServerSocket::run] Hello world\n" );
    printf( "[myServerSocket::run] PID = %d (%u)\n", (int) getpid(), (int) pthread_self() );
    m_socket_is_running = true;
    while( m_socket_is_running ) {
        if( m_client_is_connected ) {
            sleep( 1 );
        } else if( m_socket_is_bind ) {
            if( m_socket_is_listen ) {
                if( (retval = socketStatus( m_socket_fd )) > 0 ) {
                    if( (m_client_socket_fd = accept( m_socket_fd, 0, 0 ) )  > 0 ) {
                        m_socket_is_accept = true;
                        m_client_is_connected = true;
                        printf( "[myServerSocket::run] %s (%d) connected\n", m_name.sun_path, m_client_socket_fd );
                    } else {
                        perror( "[myServerSocket::run] accept" );
                        printf( "[myServerSocket::run] %s accept ERROR\n", m_name.sun_path );
                    }
                }
            } else {          
                if( listen( m_socket_fd, 5 ) == 0 ) {
                    printf( "[myServerSocket::run] %s listen OK\n", m_name.sun_path );
                    m_socket_is_listen = true;
                } else {
                    perror( "[myServerSocket::run] listen");
                    printf( "[myServerSocket::run] %s listen ERROR.\n", m_name.sun_path );
                    m_socket_is_listen = false;
                }
            }
        } else {
            if(  bind( m_socket_fd, (struct sockaddr *) & m_name, SUN_LEN( & m_name ) )  == 0 ) { 
                printf( "[myServerSocket::run] %s bind OK\n", m_name.sun_path );
                m_socket_is_bind = true;
            } else {
                perror( "[myServerSocket::run] bind" );
                printf( "[myServerSocket::run] %s bind ERROR\n", m_name.sun_path );
                m_socket_is_bind = false;
                sleep( 10 );
            }
        }
    }
    m_socket_is_running = true;
    printf( "[myServerSocket::run] The End (tid=%lu).\n", pthread_self() );
}


/**
 * socketStatus
 * @brief monitors socket with file descriptor socket_fd.
 * Waits until timeout of 500[us] or bytes available in socket.
 * @param socket_fd (int) file descriptor of a socket.
 * @return select's return value.
 */
int myServerSocket::socketStatus( int socket_fd ) {
    fd_set rfds;
    struct timeval tv;
    int retval;

    FD_ZERO( &rfds );
    FD_SET( socket_fd, &rfds );

    tv.tv_sec = 0;
    tv.tv_usec= 500;

    if( ( retval = select( socket_fd + 1, &rfds, NULL, NULL, &tv ) ) < 0 ) {
        perror( "[myServerSocket::socketSatus] select " );
        printf( "[myServerSocket::socketSatus] select error\n" );
    }
    return retval;
}

/**
 *  serverSocketStatus
 *  @brief monitors m_socket_fd (server socket)
 *  @return socketSatus's return value.
 */
int myServerSocket::serverSocketStatus( void ) {
    return socketStatus( m_socket_fd );
}

/**
 *  clientSocketStatus
 *  @brief monitors m_client_socket_fd (a client must be connected)
 *  @return socketSatus's return value.
 */
int myServerSocket::clientSocketStatus( void ) {
    return socketStatus( m_client_socket_fd );
}

/**
 * acceptClient
 * @return m_client_socket_fd != 0 if a client has connected (accept) 
 *         successfully to the server.
 *         0 if a client disconnect drom serverSocketStatus
 *         < 0 on error in accept. 
 *         
 */
int myServerSocket::acceptClient( void ) { 
    if( (m_client_socket_fd = accept( m_socket_fd, 0, 0 ) )  < 0 ) {
        perror( "[myServerSocket::acceptClient] accept " );
        printf( "[myServerSocket::acceptClient] accept error\n" );
    } else if( m_client_socket_fd == 0 ) {
        m_socket_is_accept = false;
        m_client_is_connected = false;
        printf( "[myServerSocket::acceptClient] connection closed by client\n" );
    } else {
        m_socket_is_accept = true;
        m_client_is_connected = true;
        printf( "[myServerSocket::acceptClient] m_client_socket_fd = %d\n", m_client_socket_fd );
    }
    return m_client_socket_fd;
}

/**
 *  @brief Reads bytes from socket with file descriptor msgsck.
 *  @return read's return value.
 */
int myServerSocket::readSocket( char * buffer, int maxbytes ) {
    //char    buffer[128];
    int     retval;

    retval = read( m_client_socket_fd, buffer, maxbytes );
    if( retval < 0 ) {
        perror( "[myServerSocket::readSocket] read" );
        printf( "[myServerSocket::readSocket] read ERROR\n" );
    } else if( retval == 0 ) {
        printf( "[myServerSocket::readSocket] connection closed by client\n" );
        m_socket_is_accept = false;
        m_client_is_connected = false;
        close( m_client_socket_fd );
    } else if( retval > 0 ) {
        //printf( "[myServerSocket::readSocket] %s\n", buffer );
        //writeSocket( buffer, retval );
    }
    return retval;
}

/**
 * writeSocket
 * @return write returned value
 */
int myServerSocket::writeSocket( char * line, int nbytes ) {
    return write( m_client_socket_fd, line, nbytes );
}


/**
 * getIsConnected
 * @return true if a client has connected (accept) successfully to the server.
 * @return false if there no are clients connected to the server
 *        
 */
int myServerSocket::getIsConnected( void ) {
    int retval;

    if( ! m_client_is_connected ) {
        if( m_socket_is_bind ) {
            if( m_socket_is_listen ) {
                if( (retval = socketStatus( m_socket_fd )) > 0 ) {
                    if( (m_client_socket_fd = accept( m_socket_fd, 0, 0 ) )  > 0 ) {
                        m_socket_is_accept = true;
                        m_client_is_connected = true;
                        printf( "[myServerSocket::getIsConnected] %s (%d) connected\n", m_name.sun_path, m_client_socket_fd );
                    } else {
                        perror( "[myServerSocket::getIsConnected] accept" );
                        printf( "[myServerSocket::getIsConnected] %s accept ERROR\n", m_name.sun_path );
                    }
                }
            } else {          
                if( listen( m_socket_fd, 5 ) == 0 ) {
                    printf( "[myServerSocket::getIsConnected] %s listen OK\n", m_name.sun_path );
                    m_socket_is_listen = true;
                } else {
                    perror( "[myServerSocket::getIsConnected] listen");
                    printf( "[myServerSocket::getIsConnected] %s listen ERROR\n", m_name.sun_path );
                    m_socket_is_listen = false;
                }
            }
        } else {
            if(  bind( m_socket_fd, (struct sockaddr *) & m_name, SUN_LEN( & m_name ) )  == 0 ) { 
                printf( "[myServerSocket::getIsConnected] %s bind OK\n", m_name.sun_path );
                m_socket_is_bind = true;
            } else {
                perror( "[myServerSocket::getIsConnected] bind" );
                printf( "[myServerSocket::getIsConnected] %s bind ERROR\n", m_name.sun_path );
                m_socket_is_bind = false;
                //sleep( 10 );
            }
        }
    }
    return m_client_is_connected;
}

/**
 * getIsBind
 * @return true if a client has connected (accept) successfully to the server.
 * @return false if there no are clients connected to the server
 *        
 */
int myServerSocket::getIsBind( void ) {
    return m_socket_is_bind;
}

/**
 * getIsListen
 * @return true if a client has connected (accept) successfully to the server.
 * @return false if there no are clients connected to the server
 *        
 */
int myServerSocket::getIsListen( void ) {
    return m_socket_is_listen;
}

/**
 * getIsAccept
 * @return true if a client has connected (accept) successfully to the server.
 * @return false if there no are clients connected to the server
 *        
 */
int myServerSocket::getIsAccept( void ) {
    return m_socket_is_accept;
}

/**
 * setIsConnected
 * @return true if a client has connected (accept) successfully to the server.
 * @return false if there no are clients connected to the server
 *        
 */
int myServerSocket::setIsConnected( int value ) {
    m_client_is_connected = value;
    return m_client_is_connected;
}

/**
 * setIsBind
 * @return true if a client has connected (accept) successfully to the server.
 * @return false if there no are clients connected to the server
 *        
 */
int myServerSocket::setIsBind( int value ) {
    m_socket_is_bind = value;
    return m_socket_is_bind;
}

/**
 * setIsListen
 * @return true if a client has connected (accept) successfully to the server.
 * @return false if there no are clients connected to the server
 *        
 */
int myServerSocket::setIsListen( int value ) {
    m_socket_is_listen = value;
    return m_socket_is_listen;
}

/**
 * setIsAccept
 * @return true if a client has connected (accept) successfully to the server.
 * @return false if there no are clients connected to the server
 *        
 */
int myServerSocket::setIsAccept( int value ) {
    m_socket_is_accept = value;
    return m_socket_is_accept;
}


