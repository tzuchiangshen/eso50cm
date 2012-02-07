#ifndef _MYCLIENSOCKET_H_
#define _MYCLIENSOCKET_H_

#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>

#include <time.h>
//#include <sys/time.h>           //gettimeofday

class myClientSocket {
    public:
        myClientSocket( const char * socket_name  );
        ~myClientSocket( void );
        
        int connectSocket( void );
        int closeSocket( void );
        int socketStatus( void );
        int readSocket( void );
        int readSocket( char * buffer, int max_bytes );
        int writeSocket(  char * line, int nbytes );
        int getIsConnected( void );
        char * getSocketName( void );
        
    private:
        int m_socket_fd;
        struct sockaddr_un m_name;
        int m_socket_is_connected;
        int m_client_sent_quite_message;
};

#endif //_MYCLIENSOCKET_H_


