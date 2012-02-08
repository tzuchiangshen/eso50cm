#include <myTEncoder.h>

/**
 * myTEncoder
 */
myTEncoder::myTEncoder( char ax, char id, struct my_tEncoder_data_t * encoder, LoggerHelper *logLCUImpl )
{
    logger = logLCUImpl;
    m_ax = ax;
    m_id = id;
    logger->logINFO( "[myTEncoder::myTEncoder] %c-%c Hello World!\n", m_ax, m_id );
    logger->logINFO( "[myTEncoder::myTEncoder] %c-%c encoder at %p\n", m_ax, m_id, (void *) encoder );
    m_encoder = encoder;
}


/**
 * myTEncoder
 */
myTEncoder::~myTEncoder( void )
{
    logger->logINFO( "[myTEncoder::~myTEncoder] %c-%c Good Bye!\n", m_ax, m_id );
    //bin_read_semaphore->post();
}

/**
 * initializeEncoder
 */
void myTEncoder::initializeEncoder( double encoder_to_axis_reduction,
                                    double tics_per_revs,
                                    double home_position )
{
    m_encoder->EncoderToAxis_Reduction = encoder_to_axis_reduction;
    m_encoder->TicsPerRev = tics_per_revs;
    m_encoder->HomePosition = home_position;
}
/**
 * setInstrumentMemorySpace
 */
void myTEncoder::setInstrumentMemorySpace( struct encoder_data_t * bin_encoder )
{
    int retval = 0;
    m_bin_encoder = bin_encoder;
    logger->logINFO( "[myTEncoder::setInstrumentMemorySpace] %c-%c 0x%02X\n", m_ax, m_id, m_bin_encoder->i2c_address );


    /** Create Semaphore to control the access to the Instrument Shared Memory */
    bin_write_semaphore = new myBSemaphore( WRSEMKEY,  S_IRUSR | S_IWUSR );
    if( (retval = bin_write_semaphore->allocate()) < 0 ) 
    {
        perror( "[myTelescope::attachTelescope] semaphore" );
        logger->logINFO( "[myTelescope::attachTelescope] semaphore ERROR\n" );
        //return retval;
    } else 
    {
        logger->logINFO( "[myTelescope::attachTelescope] semaphore OK\n" );
    }
    /** Create Semaphore to control the access to the Instrument Shared Memory */
    bin_read_semaphore = new myBSemaphore( RDSEMKEY,  S_IRUSR | S_IWUSR );
    if( (retval = bin_read_semaphore->allocate()) < 0 ) 
    {
        perror( "[myTelescope::attachTelescope] semaphore" );
        logger->logINFO( "[myTelescope::attachTelescope] semaphore ERROR\n" );
        //return retval;
    } else 
    {
        logger->logINFO( "[myTelescope::attachTelescope] semaphore OK\n" );
    }
}

/**
 * setEncoderToAxis_Reduction
 */
double myTEncoder::setEncoderToAxis_Reduction( double encoder_to_axis_reduction )
{
    m_encoder->EncoderToAxis_Reduction = encoder_to_axis_reduction;
    return m_encoder->EncoderToAxis_Reduction;
}

/**
 * setTicsPerRev
 */
double myTEncoder::setTicsPerRev( double tics_per_revs )
{
    m_encoder->TicsPerRev = tics_per_revs;
    return m_encoder->TicsPerRev;
}

/**
 * setHomePosition
 */
double myTEncoder::setHomePosition( double home_position )
{
    m_encoder->HomePosition = home_position;
    return m_encoder->HomePosition;
}

/**
 * getEncoderToAxis_Reduction
 */
double myTEncoder::getEncoderToAxis_Reduction( void )
{
    return m_encoder->EncoderToAxis_Reduction;
}

/**
 * getTicsPerRev
 */
double myTEncoder::getTicsPerRev( void )
{
    return m_encoder->TicsPerRev;
}

/**
 *
 */
double myTEncoder::getHomePosition( void )
{
    return m_encoder->HomePosition;
}

/**
 * getDegPosition
 */
double myTEncoder::getPosition( void )
{
    return (double) m_bin_encoder->data[4];
}

/**
 * getDegPosition
 */
double myTEncoder::getDegPosition( void )
{
    double degs;
    int value;
    readDeviceMemory( 4, & value, 0 );
	logger->logINFO("[myTEncoder::getDegPosition] readMemory(4)= %d \n", value);
    if( m_encoder->TicsPerRev != 0 ) {
        degs  = (double) value;                            //tics;
        degs -= m_encoder->HomePosition;
        //if( verbose ) printf( "[myTEncoder::getDegPosition] %c-%c P=%+9.0lf\n", m_ax, m_id,  degs );
        degs /= m_encoder->TicsPerRev;
        //if( verbose ) printf( "[myTEncoder::getDegPosition] %c-%c P=%+10.6lf\n", m_ax, m_id,  degs );
        degs *= 360.0 / m_encoder->EncoderToAxis_Reduction;
        //if( verbose ) printf( "[myTEncoder::getDegPosition] %c-%c P=%+10.6lf\n", m_ax, m_id,  degs );
    } else {
        degs = 0;
    }
    m_encoder->degs = degs;
    return degs;
}

/**
 * setPosition
 */
int myTEncoder::setPosition( double degs )
{
    int tics;
    degs /= 360.0 / m_encoder->EncoderToAxis_Reduction;
    degs *= m_encoder->TicsPerRev;
    degs += m_encoder->HomePosition;
    tics = (int) degs;
    return tics;
}

/**
 * readDeviceMemory
 */
int myTEncoder::readDeviceMemory( char mem_address, int * value )
{
    int retval = 0;
    char * ptr;
    int mem;
    char chk_sum;

    struct timeval  gtime;
    struct timezone tzone;
    double startT, endT;

    gettimeofday( & gtime, & tzone );
    startT  = ((double) gtime.tv_usec)/1000000.;
    startT += (double) gtime.tv_sec;

    bin_read_semaphore->wait();
    logger->logINFO( "[myTEncoder::readDeviceMemory] %c-%c read wait\n", m_ax, m_id );
    bin_write_semaphore->wait();
    logger->logINFO( "[myTEncoder::readDeviceMemory] %c-%c write wait\n", m_ax, m_id );
    memset( m_bin_encoder->message, 0, 16 );
    memset( m_bin_encoder->answer, 0, 16 );
    logger->logINFO( "[myTEncoder::readDeviceMemory] Writing message for 0x%02X...\n", m_bin_encoder->i2c_address );
    m_bin_encoder->message[0] = ':';
    m_bin_encoder->message[1] = m_bin_encoder->i2c_address + 1;
    m_bin_encoder->message[2] = mem_address;
    m_bin_encoder->message[3] = 0;
    m_bin_encoder->message[4] = 0;
    m_bin_encoder->message[5] = 0;
    m_bin_encoder->message[6] = 0;
    m_bin_encoder->message[7] = 0;
    m_bin_encoder->message[8] = 0;
    m_bin_encoder->message[9] = '#';
    bin_write_semaphore->post();
    bin_read_semaphore->wait();
    logger->logINFO( "[myTEncoder::readDeviceMemory] %c-%c read wait\n", m_ax, m_id );
    chk_sum  = m_bin_encoder->answer[1];
    chk_sum += m_bin_encoder->answer[2];
    ptr = (char *) value;
    ptr[0] = m_bin_encoder->answer[3];
    chk_sum += m_bin_encoder->answer[3];
    ptr[1] = m_bin_encoder->answer[4];
    chk_sum += m_bin_encoder->answer[4];
    ptr[2] = m_bin_encoder->answer[5];
    chk_sum += m_bin_encoder->answer[5];
    ptr[3] = m_bin_encoder->answer[6];
    chk_sum += m_bin_encoder->answer[6];
    logger->logINFO( "[myTEncoder::readDeviceMemory] %c 0x%02X %d %d (%d,%d) %c\n"
		     , m_bin_encoder->answer[0]
		     , (unsigned char) m_bin_encoder->answer[1]
		     , (int) m_bin_encoder->answer[2]
		     , * value
		     , (int) chk_sum
		     , (int) m_bin_encoder->answer[7]
		     , m_bin_encoder->answer[9] );
    if( chk_sum == m_bin_encoder->answer[7] ) 
    {
        mem = (int) mem_address;
        m_bin_encoder->data[mem] = * value;
    } else {
        logger->logINFO( "[myTEncoder::readDeviceMemory] Checksum ERROR!\n" );
    }
    bin_read_semaphore->post();

    gettimeofday( & gtime, & tzone );
    endT  = ((double) gtime.tv_usec)/1000000.;
    endT += (double) gtime.tv_sec;
    logger->logINFO( "[myTEncoder::readDeviceMemory] %c-%c dT=%10.6lf[s]\n", m_ax, m_id, endT - startT );

    return retval;
}

int myTEncoder::readDeviceMemory( char mem_address, int * value, int m_verbose )
{
    int retval = 0;
    char * ptr;
    int mem;
    char chk_sum;

    struct timeval  gtime;
    struct timezone tzone;
    double startT, endT;

    gettimeofday( & gtime, & tzone );
    startT  = ((double) gtime.tv_usec)/1000000.;
    startT += (double) gtime.tv_sec;

    bin_read_semaphore->wait();
    if( m_verbose )
        logger->logINFO( "[myTEncoder::readDeviceMemory] %c-%c read wait\n", m_ax, m_id );
    bin_write_semaphore->wait();
    if( m_verbose )
        logger->logINFO( "[myTEncoder::readDeviceMemory] %c-%c write wait\n", m_ax, m_id );
    memset( m_bin_encoder->message, 0, 16 );
    memset( m_bin_encoder->answer, 0, 16 );
    if( m_verbose )
        logger->logINFO( "[myTEncoder::readDeviceMemory] Writing message for 0x%02X...\n", m_bin_encoder->i2c_address );
    m_bin_encoder->message[0] = ':';
    m_bin_encoder->message[1] = m_bin_encoder->i2c_address + 1;
    m_bin_encoder->message[2] = mem_address;
    m_bin_encoder->message[3] = 0;
    m_bin_encoder->message[4] = 0;
    m_bin_encoder->message[5] = 0;
    m_bin_encoder->message[6] = 0;
    m_bin_encoder->message[7] = 0;
    m_bin_encoder->message[8] = 0;
    m_bin_encoder->message[9] = '#';
    bin_write_semaphore->post();
    bin_read_semaphore->wait();
    if( m_verbose )
        logger->logINFO( "[myTEncoder::readDeviceMemory] %c-%c read wait\n", m_ax, m_id );
    chk_sum  = m_bin_encoder->answer[1];
    chk_sum += m_bin_encoder->answer[2];
    ptr = (char *) value;
    ptr[0] = m_bin_encoder->answer[3];
    chk_sum += m_bin_encoder->answer[3];
    ptr[1] = m_bin_encoder->answer[4];
    chk_sum += m_bin_encoder->answer[4];
    ptr[2] = m_bin_encoder->answer[5];
    chk_sum += m_bin_encoder->answer[5];
    ptr[3] = m_bin_encoder->answer[6];
    chk_sum += m_bin_encoder->answer[6];
    if( m_verbose )
        logger->logINFO( "[myTEncoder::readDeviceMemory] %c 0x%02X %d %d (%d,%d) %c\n"
                , m_bin_encoder->answer[0]
                , (unsigned char) m_bin_encoder->answer[1]
                , (int) m_bin_encoder->answer[2]
                , * value
                , (int) chk_sum
                , (int) m_bin_encoder->answer[7]
                , m_bin_encoder->answer[9] );
    if( chk_sum == m_bin_encoder->answer[7] ) 
    {
        mem = (int) mem_address;
        m_bin_encoder->data[mem] = * value;
    } else {
        if( m_verbose )
            logger->logINFO( "[myTEncoder::readDeviceMemory] Checksum ERROR!\n" );
    }
    bin_read_semaphore->post();

    gettimeofday( & gtime, & tzone );
    endT  = ((double) gtime.tv_usec)/1000000.;
    endT += (double) gtime.tv_sec;
    if( m_verbose )
        logger->logINFO( "[myTEncoder::readDeviceMemory] %c-%c dT=%10.6lf[s]\n", m_ax, m_id, endT - startT );

    return retval;
}

/**
 * setDeviceMemory
 */
int myTEncoder::setDeviceMemory( char mem_address, int * value )
{
    int retval = 0;
    char * ptr;

    struct timeval  gtime;
    struct timezone tzone;
    double startT, endT;

    gettimeofday( & gtime, & tzone );
    startT  = ((double) gtime.tv_usec)/1000000.;
    startT += (double) gtime.tv_sec;

    bin_read_semaphore->wait();
    logger->logINFO( "[myTEncoder::setDeviceMemory] %c-%c read wait\n", m_ax, m_id );
    bin_write_semaphore->wait();
    logger->logINFO( "[myTEncoder::setDeviceMemory] %c-%c write wait\n", m_ax, m_id );
    memset( m_bin_encoder->message, 0, 16 );
    memset( m_bin_encoder->answer, 0, 16 );
    
    logger->logINFO( "[myTEncoder::setDeviceMemory] Writing message for 0x%02X...\n", m_bin_encoder->i2c_address );
    m_bin_encoder->message[0] = ':';
    m_bin_encoder->message[1] = m_bin_encoder->i2c_address;
    m_bin_encoder->message[2] = mem_address;
    ptr = (char *) value;
    m_bin_encoder->message[3] = ptr[0];
    m_bin_encoder->message[4] = ptr[1];
    m_bin_encoder->message[5] = ptr[2];
    m_bin_encoder->message[6] = ptr[3];
    m_bin_encoder->message[7] = 0;
    m_bin_encoder->message[8] = 0;
    m_bin_encoder->message[9] = '#';
    bin_write_semaphore->post();
    bin_read_semaphore->wait();
    logger->logINFO( "[myTEncoder::setDeviceMemory] %c-%c read wait\n", m_ax, m_id );
    logger->logINFO( "[myTEncoder::setDeviceMemory] %c 0x%02X %d %d %c\n"
		     , m_bin_encoder->answer[0]
		     , (unsigned char) m_bin_encoder->answer[1]
		     , (int) m_bin_encoder->answer[2]
		     , * (int *) & m_bin_encoder->answer[3]
		     , m_bin_encoder->answer[9] );
    retval = (int) m_bin_encoder->answer[2];
    bin_read_semaphore->post();

    gettimeofday( & gtime, & tzone );
    endT  = ((double) gtime.tv_usec)/1000000.;
    endT += (double) gtime.tv_sec;
    logger->logINFO( "[myTEncoder::setDeviceMemory] %c-%c dT=%10.6lf[s]\n", m_ax, m_id, endT - startT );

    return retval;
}

int myTEncoder::setDeviceMemory( char mem_address, int * value, int m_verbose )
{
    int retval = 0;
    char * ptr;

    struct timeval  gtime;
    struct timezone tzone;
    double startT, endT;

    gettimeofday( & gtime, & tzone );
    startT  = ((double) gtime.tv_usec)/1000000.;
    startT += (double) gtime.tv_sec;

    bin_read_semaphore->wait();
    if( m_verbose )
        logger->logINFO( "[myTEncoder::setDeviceMemory] %c-%c read wait\n", m_ax, m_id );
    bin_write_semaphore->wait();
    if( m_verbose )
        logger->logINFO( "[myTEncoder::setDeviceMemory] %c-%c write wait\n", m_ax, m_id );
    memset( m_bin_encoder->message, 0, 16 );
    memset( m_bin_encoder->answer, 0, 16 );

    if( m_verbose )
        logger->logINFO( "[myTEncoder::setDeviceMemory] Writing message for 0x%02X...\n", m_bin_encoder->i2c_address );
    m_bin_encoder->message[0] = ':';
    m_bin_encoder->message[1] = m_bin_encoder->i2c_address;
    m_bin_encoder->message[2] = mem_address;
    ptr = (char *) value;
    m_bin_encoder->message[3] = ptr[0];
    m_bin_encoder->message[4] = ptr[1];
    m_bin_encoder->message[5] = ptr[2];
    m_bin_encoder->message[6] = ptr[3];
    m_bin_encoder->message[7] = 0;
    m_bin_encoder->message[8] = 0;
    m_bin_encoder->message[9] = '#';
    bin_write_semaphore->post();
    bin_read_semaphore->wait();
    if( m_verbose )
        logger->logINFO( "[myTEncoder::setDeviceMemory] %c-%c read wait\n", m_ax, m_id );
    if( m_verbose )
        logger->logINFO( "[myTEncoder::setDeviceMemory] %c 0x%02X %d %d %c\n"
                    , m_bin_encoder->answer[0]
                    , (unsigned char) m_bin_encoder->answer[1]
                    , (int) m_bin_encoder->answer[2]
                    , * (int *) & m_bin_encoder->answer[3]
                    , m_bin_encoder->answer[9] );
    retval = (int) m_bin_encoder->answer[2];
    bin_read_semaphore->post();

    gettimeofday( & gtime, & tzone );
    endT  = ((double) gtime.tv_usec)/1000000.;
    endT += (double) gtime.tv_sec;
    if( m_verbose )
        logger->logINFO( "[myTEncoder::setDeviceMemory] %c-%c dT=%10.6lf[s]\n", m_ax, m_id, endT - startT );

    return retval;
}




