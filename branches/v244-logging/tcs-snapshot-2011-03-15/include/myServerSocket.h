#ifndef _MYSERVERSOCKET_H_
#define _MYSERVERSOCKET_H_

#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>

#include <time.h>

#include <pthread.h>

class myServerSocket {
    public:
        myServerSocket( const char * socket_name );
        virtual ~myServerSocket( void );
        int stop( void );
        int start( void );

        int socketStatus( int socket_fd );
        int serverSocketStatus( void );
        int clientSocketStatus( void );
        int acceptClient( void );
        
        int readSocket( char * buffer, int maxbytes );
        int writeSocket( char *, int );
        
        int getIsConnected( void );
        int getIsBind( void );
        int getIsListen( void );
        int getIsAccept( void );

        int setIsConnected( int value );
        int setIsBind( int value );
        int setIsListen( int value );
        int setIsAccept( int value );

    private:
        virtual void run();
        static void * gate(void *);
        pthread_t _tid;

        int m_socket_fd;
        struct sockaddr_un m_name;
        char m_buffer[128];
        int m_buffer_bytes;
        int m_socket_is_running;
        int m_socket_is_bind;
        int m_socket_is_listen;
        int m_socket_is_accept;
        
        int m_client_socket_fd;
        //char m_client_name[16];
        char m_client_name_length;
        int m_client_is_connected;
        
        
};

#endif  //_MYSERVERSOCKET_H_
