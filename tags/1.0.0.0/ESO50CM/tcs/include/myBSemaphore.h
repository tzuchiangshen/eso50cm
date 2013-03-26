#ifndef _MYSEMAPHORE_H_
#define _MYSEMAPHORE_H_

#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <unistd.h>
//#include <dirent.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <sys/ipc.h>        /** semaphores */
#include <sys/sem.h>        /** semaphores */

#include <errno.h>


class myBSemaphore
{
public:
    myBSemaphore( key_t key, int sem_flags );
    ~myBSemaphore( void );

    union semun
    {
        int                 val;
        struct semid_ds     * buf;
        unsigned short int  * array;
        struct seminfo      * __buf;
    };

    void setKey( key_t key );
    void setFlags( int sem_flags );
    int allocate( void );
    int deallocate( void );
    int initialize( void );
    int wait( void );
    int post( void );

private:
    int     m_sem_id;
    key_t   m_key;
    int     m_sem_flags;

};

#endif //_MYSEPAPHORE_H_
