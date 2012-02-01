#ifndef _MYUXSRVSCK_H_
#define _MYUXSRVSCK_H_

#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>

#include <time.h>

#include <pthread.h>

class myUxSckSrv
{
    public:
        myUxSckSrv( const char * socket_name );
        ~myUxSckSrv( void );

        int server_run( void );


    private:
        void handle_connection( int connection_fd );

        struct sockaddr_un m_socket_address;
        //struct sockaddr_un m_name;
        //struct hostent * m_hp, * gethostbymname();
        int m_server_socket_fd;

        int m_socket_is_connected;
        int m_socket_is_bind;
        int m_socket_is_listen;
        int m_socket_is_accept;
        int m_uxsrv_is_running;
};

#endif  //_MYUXSRVSCK_H_
