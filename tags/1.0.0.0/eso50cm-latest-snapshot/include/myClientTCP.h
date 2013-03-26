#ifndef _CLIENTTCP_H_
#define _CLIENTTCP_H_

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <netdb.h>
#include <netinet/in.h>


class myClientTCP {
    public:
        myClientTCP( char * server_name, char * server_port  );
        ~myClientTCP( void );

        int connectSocket( void );
        int closeSocket( void );
        int socketStatus( void  );
        int readSocket( void );
        int readSocket( char * buffer, int max_bytes );
        int writeSocket( char * line, int nbytes );
        int getIsConnected( void );
        
    private:
        char    m_server_name[128];
        int     m_server_port;
        
        int     m_socket_fd;
        struct sockaddr_in m_name;
        struct hostent* m_hostinfo;        
        
        int     m_socket_is_connected;
        int     m_client_sent_quite_message;

};





#endif //_CLIENTTCP_H_


