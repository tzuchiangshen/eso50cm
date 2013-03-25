#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/stat.h>

/**
 * Binary Semaphore
 */
union semun {
    int                 val;
    struct semid_ds     * buf;
    unsigned short int  * array;
    struct seminfo      * __buf;
};


int binary_semaphore_allocate( key_t key, int sem_flags ) {
    return semget( key, 1, sem_flags );
}

int binary_semaphore_deallocate( int sem_id ) {
    union semun ignored_argument;
    return semctl( sem_id, 1, IPC_RMID, ignored_argument );
}

int binary_semaphore_initialize( int sem_id ) {
    unsigned short value[1];
    union semun argument;

    value[0] = 1;
    argument.array = value;
    return semctl( sem_id, 0, SETALL, argument );
}

int binary_semaphore_wait( int sem_id ) {
    struct sembuf operations[1];

    operations[0].sem_num = 0;
    operations[0].sem_op  = -1;
    operations[0].sem_flg = SEM_UNDO;

    return semop( sem_id, operations, 1 );
}

int binary_semaphore_post( int sem_id ) {
    struct sembuf operations[1];    //one operation

    operations[0].sem_num = 0;
    operations[0].sem_op  = 1;
    operations[0].sem_flg = SEM_UNDO;

    return semop( sem_id, operations, 1 );
}

int binary_semaphore_check(int sem_id) {
    union semun  {
        int val;
        struct semid_ds *buf;
        ushort *array;
    } arg;

    return semctl( sem_id, 0, GETVAL, arg );
}


