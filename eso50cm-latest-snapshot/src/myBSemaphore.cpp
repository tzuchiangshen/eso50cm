#include <myBSemaphore.h>
extern int verbose;

/**
 *
 */
myBSemaphore::myBSemaphore( key_t key, int sem_flags )
{
    m_key = key;
    m_sem_flags = sem_flags;
}

/**
 *
 */
myBSemaphore::~myBSemaphore( void )
{
}

/**
 *
 */
void myBSemaphore::setKey( key_t key )
{
    m_key = key;
}


/**
 *
 */
void myBSemaphore::setFlags( int sem_flags )
{
    m_sem_flags = sem_flags;
}


/**
 *
 */
int myBSemaphore::allocate( void )
{
    return ( m_sem_id = semget( m_key, 1, m_sem_flags ) );
}

/**
 *
 */
int myBSemaphore::deallocate( void )
{
    union semun ignored_argument;
    return semctl( m_sem_id, 1, IPC_RMID, ignored_argument );
}

/**
 *
 */
int myBSemaphore::initialize( void )
{
    unsigned short value[1];
    union semun argument;

    value[0] = 1;
    argument.array = value;
    return semctl( m_sem_id, 0, SETALL, argument );
}

/**
 *
 */
int myBSemaphore::wait( void )
{
    struct sembuf operations[1];

    operations[0].sem_num = 0;
    operations[0].sem_op  = -1;
    operations[0].sem_flg = 0;//SEM_UNDO;

    return semop( m_sem_id, operations, 1 );
}

/**
 *
 */
int myBSemaphore::post( void )
{
    struct sembuf operations[1];    //one operation

    operations[0].sem_num = 0;
    operations[0].sem_op  = 1;
    operations[0].sem_flg = 0;//SEM_UNDO;

    return semop( m_sem_id, operations, 1 );
}

