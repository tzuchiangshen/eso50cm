#ifndef _MYTCP_H_
#define _MYTCP_H_


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>             //inet sockets
#include <arpa/inet.h>              //inet_ntoa
#include <string.h>
#include <errno.h>
#include <dlfcn.h>
#include <signal.h>                 //sigaction
#include <sys/wait.h>

#include <myLCU.h>

class myTCP {
    public:
        myTCP( struct in_addr local_address, uint16_t port );
        ~myTCP( void );

        int server_run( void );



    private:
        //void clean_up_child_process( int signal_number );
        void handle_connection( int connection_fd );
        void handle_get( int connection_fd, const char * page );
        void handle_txt( int connection_fd, const char * page );
        struct sockaddr_in m_socket_address;
        //struct hostent * m_hp, * gethostbymname();
        int m_server_socket_fd;

        int m_socket_is_connected;
        int m_socket_is_bind;
        int m_socket_is_listen;
        int m_socket_is_accept;
        int m_tcp_is_running;

        myLCU * m_lcu;
};

#endif  //_MYTCP_H_
