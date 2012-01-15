#ifndef _MYLCU_H
#define _MYLCU_H

#define SHMKEY   0xFAFAFA01
#define SEMKEY   0xFAFAFA01

#include <stdio.h>
#include <unistd.h>     //sleep usleep

#include <sys/shm.h>    //shared memory
#include <sys/stat.h>   //shared memory

#include <string.h>
#include <errno.h>
#include <stdlib.h>


#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#include "myTelescope.h"
#include "myBSemaphore.h"
#include "LoggerHelper.h"

//#include "myTCP.h"
//#include <arpa/inet.h>              //inet_ntoa

//-------------------------------------------------------------
//  User Shared memory and semaphore
#define USRSHMKEY   0xFAFAFA01
#define USRSEMKEY   0xFAFAFA01

class myLCU
{
    public:
         myLCU(void );
        ~myLCU( void );

        int waitSemaphore( void );
        int postSemaphore( void );

        void createTelescope( void );

        int run( void );

        double getDeltaT( void );
        double getLCUMTime (void);
        struct tm * getLCU_Time( void );
        struct tm * getLCU_UniversalTime( void );
        void * get_lcu_data_address( void );

        myTelescope * telescope;
    private:
        struct my_lcu_data_t *  m_lcu_data;
        myBSemaphore *          m_lcu_semaphore;

        int                     m_quit;
        int                     m_semaphore_id;
        int                     m_segment_id;
        char *                  m_shared_memory;
        int                     m_segment_size;
        int                     m_shared_segment_size;
        double                  m_old_t;

        struct shmid_ds         m_shmbuffer;
        struct tm               m_LCU_Time;
        struct tm               m_LCU_UniversalTime;
	LoggerHelper            logger;
};

#endif //MYTCS_H
