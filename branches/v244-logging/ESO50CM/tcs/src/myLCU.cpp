#include "myLCU.h"

/**
 *  @brief myLCU class provides necesary functions to access the telescope shared memory segments.
 *  Telescope's (myTelescope) shared memory segments are two: one for the instrument (low level
 *  instructions and data) and a second for the users (high level instruction and data).
 */
myLCU::myLCU(LoggerHelper *logLCUImpl)
{
    logger = logLCUImpl; 
    logger->logINFO( "[myLCU::myLCU] Hello World!" );
    m_segment_size = 0;
    m_shared_segment_size = 1024;           //sizeof( struct my_lcu_data_t );

    /** Allocate the User Shared Memory segment */
    m_segment_id = shmget( USRSHMKEY, m_shared_segment_size,
                           S_IRUSR | S_IWUSR );
                           //IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR );

    /** Attach the User Shared Memory segment */
    m_shared_memory = (char *) shmat( m_segment_id, 0, 0 );
    if( (int) m_shared_memory < 0) {
        perror( "[myLCU::myLCU] shmat" );
        logger->logINFO("[myLCU::myLCU] Can't attach User Shared Memory!, id=%d", m_segment_id );
        m_shared_memory = NULL;
    } else {
        logger->logINFO( "[myLCU::myLCU] User Shared Memory attached at address %p", (void *) m_shared_memory );

        /** Determine the User Shared Memory segment size */
        shmctl( m_segment_id, IPC_STAT, & m_shmbuffer );
        m_segment_size = m_shmbuffer.shm_segsz;
        logger->logINFO( "[myLCU::myLCU] segment size:                   %d", m_segment_size );
        logger->logINFO( "[myLCU::myLCU] sizeof( struct my_lcu_data_t ): %d", (int) sizeof( struct my_lcu_data_t ) );

        m_lcu_data = (struct my_lcu_data_t *) m_shared_memory;
        logger->logINFO( "[myLCU::myLCU] m_lcu_data at address                   %p", (void *) m_lcu_data );
        logger->logINFO( "[myLCU::myLCU] m_lcu_data->m_telescope_data at address %p", (void *) & m_lcu_data->telescope_data );
    }
    /** Create Semaphore to control the access to the User Shared Memory */
    m_lcu_semaphore = new myBSemaphore( USRSEMKEY,  S_IRUSR | S_IWUSR );
    if( m_lcu_semaphore->allocate() < 0 ) {
        perror( "[myLCU::myLCU] semget" );
        logger->logINFO( "[myLCU::myLCU] Can't allocate User Shared Memory's Semaphore" );
        //return retval;
    }
    /** */
    telescope = NULL;

    /** */
    m_old_t = 0.;
}

/**
 *
 */
myLCU::~myLCU( void )
{
    if( telescope != NULL ) 
    {
        logger->logINFO( "[myLCU::~myLCU] Deleting telescope" );
        delete telescope;
    } else {
        logger->logINFO( "[myLCU::~myLCU] No telescope!" );
    }

    /** Detach the shared memory segment */
    if( m_shared_memory != NULL ) 
    {
        logger->logINFO( "[myLCU::~myLCU] Detaching User Shared Memory" );
        shmdt( m_shared_memory );
    } else 
    {
        logger->logINFO( "[myLCU::~myLCU] No User Shared Memory attached!" );
    }

    logger->logINFO( "[myLCU::~myLCU] Good Bye!" );
}

/**
 *
 */
int myLCU::run( void )
{
    int retval = 0;
    logger->logINFO( "[myLCU::run] Hello World!" );
    logger->logINFO( "[myLCU::run] Good bye!" );
    return retval;
}

/**
 * waitSemaphore
 */
int myLCU::waitSemaphore( void )
{
    return m_lcu_semaphore->wait();
}

/**
 * postSemaphore
 */
int myLCU::postSemaphore( void )
{
    return m_lcu_semaphore->post();
}


/**
 * createTelescope
 */
void myLCU::createTelescope( void )
{
    telescope = new myTelescope( m_lcu_data, logger );
    logger->logINFO("[myLCU::initializeTelescope] telescope %p", (void *) telescope );
}


/**
 * getLCU_Time
 */
struct tm * myLCU::getLCU_Time( void )
{
    struct timeval  gtime;
    struct timezone tzone;

    gettimeofday( & gtime, & tzone );
    localtime_r( & (gtime.tv_sec), & m_LCU_Time  );
    return & m_LCU_Time;
}

/**
 * getLCUMTime (time in milliseconds)
 */
double myLCU::getLCUMTime( void )
{
    struct timeval gtime;
    struct timezone tzone;

    gettimeofday( & gtime, & tzone);
    return (gtime.tv_sec+(gtime.tv_usec/1000000.0));
}

/**
 * getDeltaT
 */
double myLCU::getDeltaT( void )
{
    double new_t;
    double delta_t;

    struct timeval  gtime;
    struct timezone tzone;

    gettimeofday( & gtime, & tzone );

    new_t   = (double) (gtime.tv_usec)/1000000.;
    new_t  += (double) (gtime.tv_sec);
    delta_t = new_t - m_old_t;
    m_old_t = new_t;

    return delta_t;
}

/**
 * getLCU_UniversalTime
 */
struct tm * myLCU::getLCU_UniversalTime( void )
{
    struct timeval  gtime;
    struct timezone tzone;

    gettimeofday( & gtime, & tzone );
    gmtime_r( & (gtime.tv_sec), & m_LCU_UniversalTime  );
    return & m_LCU_UniversalTime;
}

/**
 * getLCU_Time
 */
void * myLCU::get_lcu_data_address( void )
{
    return (void *) m_lcu_data;
}

