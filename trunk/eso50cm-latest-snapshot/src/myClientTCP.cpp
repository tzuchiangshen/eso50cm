#include <myClientTCP.h>

myClientTCP::myClientTCP( char * server_name, char * server_port ) {
    strcpy( m_server_name, server_name );
    m_server_port = atoi( server_port );
    printf( "[myClientTCP::myClientTCP] %s:%d\n", m_server_name, m_server_port );
    m_socket_is_connected = false;


}

/**
 *  Class Destructor
 */
myClientTCP::~myClientTCP( void ) {
    printf( "[myClientTCP::~myClientTCP] Exiting\n" );
    if( m_socket_is_connected ) {
        closeSocket();
    }
    printf( "[myClientTCP::~myClientTCP] Good bye!\n" );
}


/**
 *  @brief open connection.
 *  @param server_name (char *)
 *  @return retval (int) = return value from connect;
 */
int myClientTCP::connectSocket( void ) {
    int retval = -1;
    //struct sockaddr_in name;
    //struct hostent* hostinfo;
    //unsigned char ChkSum;
    m_socket_is_connected = false;
    
    m_socket_fd = socket( PF_INET, SOCK_STREAM, 0 );
    m_name.sin_family = AF_INET;
    
    if( ( m_hostinfo = gethostbyname( m_server_name ) ) != NULL ) {
        m_name.sin_addr = * ( (struct in_addr *) m_hostinfo->h_addr );
        m_name.sin_port = htons( m_server_port );   /* Web servers use port 80. */
        if( ( retval = connect( m_socket_fd, (struct sockaddr *) & m_name, sizeof( struct sockaddr_in ) ) ) == 0 ) {
            printf( "[myClientTCP::openConnection] connected\n" );
            m_socket_is_connected = true;
        } else {
            perror( "[myClientTCP::openConnection] connect" );
            printf( "[myClientTCP::openConnection] connect ERROR\n" );
        }
    } else {
        perror( "[myClientTCP::openConnection] gethostbyname" );
        printf( "[myClientTCP::openConnection] gethostbyname ERROR\n" );
    }
    return retval;
}

/**
 *  @brief close connection.
 *  @return retval (int) = return value from close;
 */
int myClientTCP::closeSocket( void  ) {
    int retval;
    if( (retval = close( m_socket_fd )) == 0 ) {
        printf( "[myClientTCP::closeConnection] close ok\n" );
        m_socket_is_connected = false;
    } else {
        perror( "[myClientTCP::closeConnection] close" );
        printf( "[myClientTCP::closeConnection] close ERROR\n" );
    }
    return retval;
}

/**
 *  @brief socketSatus monitors socket with file descriptor msgsck.
 *  Waits until timeout of 500[us] or bytes available in socket.
 *  @return retval (int) = return value from select.
 */
int myClientTCP::socketStatus( void ) {
    fd_set rfds;
    struct timeval tv;
    int retval;

    FD_ZERO( & rfds );
    FD_SET( m_socket_fd, & rfds );

    tv.tv_sec = 0;
    tv.tv_usec= 500;

    if( ( retval = select( m_socket_fd + 1, &rfds, NULL, NULL, &tv ) ) < 0 ) {
        perror( "[myClientTCP::socketSatus] select < 0" );
        printf( "[myClientTCP::socketSatus] Error, select(%d) < 0\n", socket );
    }
    return retval;
}

/**
 *  @brief Reads bytes from socket with file descriptor msgsck.
 *  @return read's return value.
 */
int myClientTCP::readSocket( void ) {
    char buffer[1024];
    int retval;

    if( ( retval = read( m_socket_fd, buffer, 1024 ) ) < 0 ) {
        perror( "[myClientTCP::readSocket] read" );
        printf( "[myClientTCP::readSocket] read ERROR\n" );
    } else if( retval == 0 ) {
        printf( "[myClientTCP::readSocket] connection closed by client\n" );
        m_socket_is_connected = false;
        close( m_socket_fd );
    } else if( retval > 0 ) {
        buffer[retval] = 0;
        printf( "[myClientTCP::readSocket] %s\n", buffer );
    }
    return retval;
}

/**
 *  @brief Reads bytes from socket with file descriptor msgsck.
 *  @return read's return value.
 */
int myClientTCP::readSocket( char * buffer, int max_bytes ) {
    int retval;

    if( ( retval = read( m_socket_fd, buffer, max_bytes ) ) < 0 ) {
        perror( "[myClientTCP::readSocket] read" );
        printf( "[myClientTCP::readSocket] read ERROR\n" );
    } else if( retval == 0 ) {
        //printf( "[myClientTCP::readSocket] connection closed by client\n" );
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
int myClientTCP::writeSocket( char * line, int nbytes ) {
    return write( m_socket_fd, line, nbytes );
}

/**
 * getIsConnected
 * @return true if a client has connected (accept) successfully to the server.
 * @return false if there no are clients connected to the server
 *        
 */
int myClientTCP::getIsConnected( void ) {
    return m_socket_is_connected;
}




