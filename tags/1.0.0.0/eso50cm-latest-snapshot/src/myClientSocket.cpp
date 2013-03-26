#include <myClientSocket.h>

myClientSocket::myClientSocket( const char * socket_name ) {
    printf( "[myClientSocket::myClientSocket] Hello World!\n" );
    printf( "[myClientSocket::myClientSocket] Socket name: %s\n", socket_name );
    
    m_socket_fd = socket( PF_LOCAL, SOCK_STREAM, 0 );
    m_name.sun_family= AF_LOCAL;
    strcpy( m_name.sun_path, socket_name );
    m_socket_is_connected = false;
    
    //if( connectSocket( socket_name ) == 0 ) {
    //    printf( "[myClientSocket::myClientSocket] m_socket_fd = %d\n", m_socket_fd );
    //} else {
    //    printf( "[myClientSocket::myClientSocket] Error no connection to socket\n" );
    //}
}

/**
 *  Class Destructor
 */
myClientSocket::~myClientSocket() {
    printf( "[myClientSocket::~myClientSocket] Exiting...\n" );
    closeSocket();
}

/**
 *  @brief open socket.
 *  @param isConnected (int *)
 *  @return retval (int) of connect.
 */
int myClientSocket::connectSocket( void ) {
    int retval;
    
    //printf( "[myClientSocket::connectSocket] m_name.sun_path =%s\n", m_name.sun_path );
    
    if( (retval= connect( m_socket_fd, (struct sockaddr *) & m_name, SUN_LEN( & m_name ) )) == 0 ) {
        m_socket_is_connected = true;
        //printf( "[myClientSocket::openConnection] connected\n" );
        printf( "[myClientSocket::connectSocket] %s (%d) connected\n", m_name.sun_path, m_socket_fd );
    } else {
        perror( "[myClientSocket::connectSocket] connect" );
        printf( "[myClientSocket::connectSocket] connect error\n" );
    } 
    
    return retval;
}

/**
 *  @brief close connection.
 *  @param socket_fd (int)
 *  @return close return value (int).
 */
int myClientSocket::closeSocket( void ) {
    return close( m_socket_fd );
}

/**
 *  @brief socketSatus monitors socket with file descriptor msgsck.
 *  Waits until timeout of 500[us] or bytes available in socket.
 *  @return select's return value.
 */
int myClientSocket::socketStatus( void ) {
    fd_set rfds;
    struct timeval tv;
    int retval;

    FD_ZERO( &rfds );
    FD_SET( m_socket_fd, &rfds );

    tv.tv_sec = 0;
    tv.tv_usec= 500;

    retval = select( m_socket_fd + 1, &rfds, NULL, NULL, &tv );
    if( retval < 0 ) {
        perror( "[myClientSocket::socketSatus] select" );
        printf( "[myClientSocket::socketSatus] select error.\n" );
    }
    return retval;
}

/**
 *  readSocket 
 *  @return read's return value.
 */
int myClientSocket::readSocket( void ) {
    char buffer[128];
    int retval;

    retval = read( m_socket_fd, buffer, 128 );
    if( retval < 0 ) {
        perror( "[myClientSocket::readSocket] read" );
        printf( "[myClientSocket::readSocket] read ERROR\n" );
    } else if( retval == 0 ) {
        printf( "[myClientTCP::readSocket] connection closed by client\n" );
        m_socket_is_connected = false;
        //close( m_socket_fd );    
    } else if( retval > 0 ) {
        printf( "[myClientSocket::readSocket] %s\n", buffer );
    }
    return retval;
}

/**
 *  readSocket 
 *  @return read's return value.
 */
int myClientSocket::readSocket( char * buffer, int max_bytes ) {
    int retval;

    retval = read( m_socket_fd, buffer, max_bytes );
    if( retval < 0 ) {
        perror( "[myClientSocket::readSocket] read" );
        printf( "[myClientSocket::readSocket] read ERROR\n" );
    } else if( retval == 0 ) {
        m_socket_is_connected = false;
        //close( m_socket_fd );
    }
    return retval;
}

/**
 *  Sends data packed with header and trailer.
 *  @param type (unsigned char): tyoe of data
 *  @param data (char *).
 *  @param datasize (int): size in bytes of data.
 *  @return write's return value (int).
 */
int myClientSocket::writeSocket(  char * line, int nbytes ) {
    return write( m_socket_fd, line, nbytes );
}

/**
 * getIsConnected
 * @return true if a client has connected (accept) successfully to the server.
 * @return false if there no are clients connected to the server
 *        
 */
int myClientSocket::getIsConnected( void ) {
    return m_socket_is_connected;
}

/**
 * getSocketName
 */
 char * myClientSocket::getSocketName( void ) {
    return m_name.sun_path;
}


