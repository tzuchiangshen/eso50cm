#ifndef _MYTTY_H_
#define _MYTTY_H_

#include <cstring>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <time.h>
#include <sys/time.h>           //gettimeofday
#include <errno.h>              //error number

#include <sys/shm.h>    //shared memory
#include <sys/stat.h>   //shared memory

#include "myRS232.h"
//#include "myServerSocket.h"


using namespace std;

class myTTY : public myRS232 {
    public:
        myTTY( const char * device, int baudrate );
        virtual ~myTTY( void );
        int start( void );
        int stop( void );
        virtual void run( void );
        static void * gate(void *);
        pthread_t _tid;

        int getIsConnected( void );

    private:
        int m_tty_end;
        int m_tty_is_running;
        int m_tty_is_connected;
 
        char m_buffer[128];
        int  m_buffer_length;
        int  m_isConnected;
        int  m_incomingMessage;
        int  m_newMessage;
        int  m_read;
        
        //myServerSocket * alpha;
        //myServerSocket * delta;


};

#endif // * _MYTTY_H_ *
