#include <myTelescope.h>

/**
  * myTelescope::myTelescope
  */
myTelescope::myTelescope( struct my_lcu_data_t * lcu_data, LoggerHelper *logLCUImpl ) : myTClock( & lcu_data->clock_data, logLCUImpl )
{
    //int retval;
    logger = logLCUImpl;
    logger->logINFO( "[myTelescope::myTelescope] Hello World!" );

    m_telescope_data = & lcu_data->telescope_data;
    logger->logINFO( "[myTelescope::myTelescope] m_telescope_data at %p", (void *) m_telescope_data );
    logger->logINFO( "[myTelescope::myTelescope] m_clock_data at     %p", (void *) & lcu_data->clock_data );
    logger->logINFO( "[myTelescope::myTelescope] m_alpha_data at     %p", (void *) & lcu_data->alpha_data );
    logger->logINFO( "[myTelescope::myTelescope] m_delta_data at     %p", (void *) & lcu_data->delta_data );

    //clock = new myTClock( & lcu_data->clock_data );
    alpha = new myTAxis( 'A', & lcu_data->alpha_data );
    delta = new myTAxis( 'D', & lcu_data->delta_data );
}


/**
 * myTelescope::~myTelescope
 */
myTelescope::~myTelescope( void )
{
    delete alpha;
    delete delta;
    //delete clock;

    detachInstrumentMemory();
    logger->logINFO( "[myTelescope::~myTelescope] Good bye!" );

}

/**
 * myTelescope::attachInstrumentMemory
 */
int myTelescope::attachInstrumentMemory( void )
{
    int retval;

    m_shared_segment_size = 1024;

    /** Allocate the Instrument Shared Memory segment */
    m_segment_id = shmget( TELSHMKEY, m_shared_segment_size,
                           S_IRUSR | S_IRGRP | S_IROTH | S_IWUSR | S_IWGRP | S_IWOTH );
                           //IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR );
    if( m_segment_id < 0 ) {
        perror( "[myTelescope::attachTelescope] shmget" );
        return errno;
    }
    /** Attach the Instrument Shared Memory segment */
    m_shared_memory = (char *) shmat( m_segment_id, 0, 0 );
    if( m_shared_memory < 0 ) {
        perror( "[myTelescope::attachTelescope] shmat" );
        return errno;
    }

    /** Determine the Instrument Shared Memory segment size */
    shmctl( m_segment_id, IPC_STAT, & m_shmbuffer );
    m_segment_size = m_shmbuffer.shm_segsz;

    bin_telescope = (struct telescope_data_t *) m_shared_memory;
    logger->logINFO( "[myTelescope::attachTelescope] bin_telescope at %p", (void *) bin_telescope );
    for( int i = 0; i < 6; i ++ ) 
    {
        logger->logINFO( "[myTelescope::attachTelescope] encoder[%d] at %p",
			 i,
			 (void *) & bin_telescope->encoder[i] );
    }

    /** Create Semaphore to control the access to the Instrument Shared Memory */
    bin_telescope_semaphore = new myBSemaphore( TELSEMKEY, S_IRUSR | S_IRGRP | S_IROTH | S_IWUSR | S_IWGRP | S_IWOTH );
    if( (retval = bin_telescope_semaphore->allocate()) < 0 ) 
    {
        logger->logINFO( "[myTelescope::attachTelescope] Error allocating semaphore" );
        return retval;
    }

    bin_telescope_semaphore->wait();
    for( int i = 0; i < 6; i ++ ) 
    {
        logger->logINFO( "[myTelescope::initializeTelescope] encoder[0x%02X] at %p",
			 bin_telescope->encoder[i].i2c_address,
			 (void *) & bin_telescope->encoder[i] );
    }
    alpha->setInstrumentMemorySpace( & bin_telescope->encoder[0],
                                       & bin_telescope->encoder[2],
                                       & bin_telescope->encoder[3] );
    delta->setInstrumentMemorySpace( & bin_telescope->encoder[1],
                                       & bin_telescope->encoder[4],
                                       & bin_telescope->encoder[5] );
    bin_telescope_semaphore->post();

    return m_segment_size;
}

/**
 * detachInstrumentMemory
 */
int myTelescope::detachInstrumentMemory( void )
{
    /** Detach the shared memory segment */
    if( m_shared_memory > 0 ) {
        if( shmdt( m_shared_memory ) < 0 ) {
            perror( "[myTelescope::detachTelescope] shmdt" );
            return errno;
        }
        bin_telescope = NULL;
        logger->logINFO( "[myTelescope::detachTelescope] Done!" );
    } else {
        logger->logINFO( "[myTelescope::detachTelescope] Nothing to do" );
    }
    return 0;
}

/**
 * connectTelescope
 */
int myTelescope::connectTelescope( void )
{
    int retval;

    m_socket_fd = socket( PF_LOCAL, SOCK_STREAM, 0 );
    if( m_socket_fd < 0 ) {
        perror( "[myTelescope::myTelescope] socket" );
        logger->logINFO( "[myTelescope::myTelescope] socket ERROR" );
    } else {
        logger->logINFO( "[myTelescope::myTelescope] socket OK" );
    }
    m_server_name.sun_family= AF_LOCAL;
    strcpy( m_server_name.sun_path, "/tmp/mbux" );

    retval = connect( m_socket_fd, (struct sockaddr *) & m_server_name, SUN_LEN( & m_server_name ) );
    if( retval < 0 ) {
        perror( "[myTelescope::myTelescope] connect" );
        logger->logINFO( "[myTelescope::myTelescope] connect ERROR" );
    } else {
        logger->logINFO( "[myTelescope::myTelescope] connect OK" );
    }
    return retval;
}

/**
 * myTelescope::disconnectTelescope
 */
int myTelescope::disconnectTelescope( void )
{
    int retval;

    retval = close( m_socket_fd );
    if( retval < 0 ) {
        perror( "[myTelescope::myTelescope] close" );
        logger->logINFO( "[myTelescope::myTelescope] close ERROR" );
    } else {
        logger->logINFO( "[myTelescope::myTelescope] close OK" );
    }
    return retval;
}

/**
 * myTelescope::initialize
 */
void myTelescope::initialize( void )
{
}

/**
 * setLatitude
 */
void myTelescope::setLatitude( double lat )
{
    m_telescope_data->Latitude = lat;
}

/**
 * setLongitude
 */
void myTelescope::setLongitude( double lng )
{
    m_telescope_data->Longitude = lng;
}

/**
 * setAltitude
 */
void myTelescope::setAltitude( double alt )
{
    m_telescope_data->Altitude = alt;
}

/**
 * setLowElevation
 */
void myTelescope::setLowElevation( double l_elev )
{
    m_telescope_data->LowElevation = l_elev;
}

/**
 * setHighElevation
 */
void myTelescope::setHighElevation( double h_elev )
{
    m_telescope_data->HighElevation = h_elev;
}

/**
 * setIsConfigured
 */
void myTelescope::setIsConfigured( char init ){
    m_telescope_data->is_initilized = init;
}

/**
 * setIsInitilized
 */
char myTelescope::setIsInitilized( char value )
{
    return m_telescope_data->is_initilized = value;
}
/**
 * setIsRunningMonitor
 */
char myTelescope::setIsRunningMonitor( char value )
{
    return m_telescope_data->is_running_monitor = value;
}

/**
 * setIsRunningGoto
 */
char myTelescope::setIsRunningGoto( char value )
{
    return m_telescope_data->is_running_goto = value;
}


/**
 * setIsTracking
 */
char myTelescope::setIsTracking( char value )
{
    return m_telescope_data->is_tracking = value;
}



/**
 * getIsInitilized
 */
char myTelescope::getIsInitilized( void )
{
    return m_telescope_data->is_initilized;
}


/**
 * getIsRunningMonitor
 */
char myTelescope::getIsRunningMonitor( void )
{
    return m_telescope_data->is_running_monitor;
}


/**
 * getIsRunningGoto
 */
char myTelescope::getIsRunningGoto( void )
{
    return m_telescope_data->is_running_goto;
}


/**
 * getIsTracking
 */
char myTelescope::getIsTracking( void )
{
    return m_telescope_data->is_tracking;
}


/**
 * getLatitude
 */
double myTelescope::getLatitude( void )
{
    return m_telescope_data->Latitude;
}

/**
 * getLongitude
 */
double myTelescope::getLongitude( void )
{
    return m_telescope_data->Longitude;
}

/**
 * getAltitude
 */
double myTelescope::getAltitude( void )
{
    return m_telescope_data->Altitude;
}

/**
 * getLowElevation
 */
double myTelescope::getLowElevation( void )
{
    return m_telescope_data->LowElevation;
}


/**
 * getHighElevation
 */
double myTelescope::getHighElevation( void )
{
    return m_telescope_data->HighElevation;
}

/**
 * getHA
 */
double myTelescope::getHA( void )
{
    return m_telescope_data->currentHA;
}

/**
 * getRA
 */
double myTelescope::getRA( void )
{
    return m_telescope_data->currentRA;
}

/**
 * getDec
 */
double myTelescope::getDec( void )
{
    return m_telescope_data->currentDec;
}

/**
 * getAlt
 */
double myTelescope::getAlt( void )
{
    return m_telescope_data->currentAlt;
}

/**
 * getAz
 */
double myTelescope::getAz( void )
{
    return m_telescope_data->currentAz;
}

/**
 * getDifferenceHA
 */
double myTelescope::getDifferenceHA( void )
{
    return m_telescope_data->differenceHA;
}

/**
 * getDifferenceRA
 */
double myTelescope::getDifferenceRA( void )
{
    return m_telescope_data->differenceRA;
}

/**
 * getDifferenceDec
 */
double myTelescope::getDifferenceDec( void )
{
    return m_telescope_data->differenceDec;
}


/**
 * getTargetRA
 */
double myTelescope::getTargetRA( void )
{
    return m_telescope_data->targetRA;
}

/**
 * getTargetHA
 */
double myTelescope::getTargetHA( void )
{
    return m_telescope_data->targetHA;
}

/**
 * getTargetDec
 */
double myTelescope::getTargetDec( void )
{
    return m_telescope_data->targetDec;
}

/**
 * getTargetAlt
 */
double myTelescope::getTargetAlt( void )
{
    return m_telescope_data->targetAlt;
}

/**
 * getTargetAz
 */
double myTelescope::getTargetAz( void )
{
    return m_telescope_data->targetAz;
}

/**
 * getIsConfigured
 */
char myTelescope::getIsConfigured( void ){
    return m_telescope_data->is_initilized;
}

/**
 * getIfNewData
 */
int myTelescope::getIfNewData( void )
{
    char new_data;

    bin_telescope_semaphore->wait();
    new_data = bin_telescope->new_data;
    if( new_data ) {
        bin_telescope->new_data = 0;
        //computeTimeData( & bin_telescope->gtime );
    }
    bin_telescope_semaphore->post();
    return new_data;
}



/** * targetPosition
 */
int myTelescope::targetPosition( double * lst, double * ra, double * dec, double * alt, double * az, double * ha  )
{
   * lst = currentTime();
   * ra  = m_telescope_data->targetRA;
   * dec = m_telescope_data->targetDec;
   * ha = m_telescope_data->targetHA;
   * alt = m_telescope_data->targetAlt;
   * az = m_telescope_data->targetAz;
}

/** * currentPosition
 */
int myTelescope::currentPosition( double * lst, double * ra, double * dec, double * alt, double * az, double * ha  )
{
    double degs;
    //char new_data;

    //if( m_shared_memory == NULL ) {
    //    if( verbose ) printf( "[myTelescope::currentPosition] Error: m_shared_memory %p",
    //            m_shared_memory );
    //   return -1;
    //}
    //bin_telescope_semaphore->wait();
    //new_data = bin_telescope->new_data;
    //if( new_data ) {
    //    bin_telescope->new_data = 0;
    //    if( verbose ) printf( "[myTelescope::currentPosition] New data = TRUE" );
    //}
    /** current LST */
    //* lst = currentTime( & bin_telescope->gtime );
    * lst = currentTime();
    /** current HA */
    m_telescope_data->currentHA = alpha->getPosition();
    /** currend Dec */
    m_telescope_data->currentDec = m_telescope_data->Latitude - delta->getPosition();

    //bin_telescope_semaphore->post();
    /** current RA */
    degs = * lst - m_telescope_data->currentHA;
    if( degs < 0 ) {
        degs += 360.;
    } else if( degs > 360. )  {
        degs -= 360.;
    }
    m_telescope_data->currentRA = degs;
    /** current Target HA */
    degs = * lst - m_telescope_data->targetRA;
    if( degs < -180.0  ) {
        degs += 360.0;
    } else if( degs > 180.0 ) {
        degs -= 360.0;
    }
    m_telescope_data->targetHA = degs;
    /** current degs to Target RA */
    degs = m_telescope_data->targetRA - m_telescope_data->currentRA;
    if( degs < -180. ) {
        degs += 360.;
    } else if( degs > 180. ) {
        degs -= 360.;
    }
    m_telescope_data->differenceRA = degs;
    /** current degs to Target HA
    differenceRA = - differenceHA */
    m_telescope_data->differenceHA = - m_telescope_data->differenceRA;
    /** current degs to Target Dec */
    m_telescope_data->differenceDec = m_telescope_data->targetDec - m_telescope_data->currentDec;

    * ra  = m_telescope_data->currentRA;
    * dec = m_telescope_data->currentDec;
    * ha = m_telescope_data->currentHA;

    equatorialToHorizontal( m_telescope_data->currentHA, m_telescope_data->currentDec, alt, az  );
    //if( m_telescope_data->HighElevation >= * alt  && * alt >= m_telescope_data->LowElevation ) {
        //printf( "[myTelescope::setTarget] Te [alt = %lf] [az = %lf]", * alt, * az );
    //} else {
    //    if( verbose ) {
    //        printf( "[myTelescope::currentPosition] Telescope [lst = %lf] [ra = %lf] [dec = %lf]", * lst, * ra, * dec );
    //        printf( "[myTelescope::currentPosition] Telescope [alt = %lf] [az = %lf] bellow horizon (%lf).", alt, az, m_telescope_data->LowElevation );
    //   }
    //    return 0;
    //}

    return 1;
}




/**
 * myTelescope::setTarget
 */
int myTelescope::setTarget( double trg_ra, double trg_dec, double * trgAlt, double * trgAz  )
{
    double lst;
    double degs;
    //char new_data;

    //bin_telescope_semaphore->wait();
    //new_data = bin_telescope->new_data;
    //if( new_data ) {
    //    bin_telescope->new_data = 0;
    //    /** current LST */
    //    lst = currentTime( & bin_telescope->gtime );
    //    /** current HA */
    //    m_telescope_data->currentHA = alpha->getPosition();
    //    /** currend Dec */
    //    m_telescope_data->currentDec = m_telescope_data->Latitude - delta->getPosition();
    //} else {
    //    lst = getLST();
    //}
    //bin_telescope_semaphore->post();
    /** current LST */
    //* lst = currentTime( & bin_telescope->gtime );
    lst = currentTime();
    /** current HA */
    m_telescope_data->currentHA = alpha->getPosition();
    /** currend Dec */
    m_telescope_data->currentDec = m_telescope_data->Latitude - delta->getPosition();


    /** current RA */
    degs = lst - m_telescope_data->currentHA;
    if( degs < 0 ) {
        degs += 360.;
    } else if( degs > 360. )  {
        degs -= 360.;
    }
    m_telescope_data->currentRA = degs;


    logger->logINFO( "[myTelescope::setTarget] Target [lst = %lf] [ra = %lf] [dec = %lf]", lst, trg_ra, trg_dec );
    equatorialToHorizontal( lst - trg_ra, trg_dec, trgAlt, trgAz  );
    if( m_telescope_data->HighElevation >= * trgAlt  && * trgAlt >= m_telescope_data->LowElevation ) {
        m_telescope_data->targetRA  = trg_ra;
        m_telescope_data->targetDec = trg_dec;
        logger->logINFO( "[myTelescope::setTarget] Target [alt = %lf] [az = %lf]", * trgAlt, * trgAz );
    } else 
    {  
        logger->logINFO( "[myTelescope::setTarget] Target [lst = %lf] [ra = %lf] [dec = %lf]", lst, trg_ra, trg_dec );
        logger->logINFO( "[myTelescope::setTarget] Target [alt = %lf] [az = %lf] bellow horizon (%lf).", * trgAlt, * trgAz, m_telescope_data->LowElevation );
        return 0;
    }

    /** current Target HA */
    degs = lst - m_telescope_data->targetRA;
    if( degs < -180.0  ) 
    {
        degs += 360.0;
    } else if( degs > 180.0 ) 
    {
        degs -= 360.0;
    }
    m_telescope_data->targetHA = degs;
    /** current degs to Target RA */
    degs = m_telescope_data->targetRA - m_telescope_data->currentRA;
    if( degs < -180. ) {
        degs += 360.;
    } else if( degs > 180. ) {
        degs -= 360.;
    }
    m_telescope_data->differenceRA = degs;
    /** current degs to Target HA
    differenceRA = - differenceHA */
    m_telescope_data->differenceHA = - m_telescope_data->differenceRA;
    /** current degs to Target Dec */
    m_telescope_data->differenceDec = m_telescope_data->targetDec - m_telescope_data->currentDec;

    return 1;
}

/**
 * myTelescope::equatorialToHorizontal
 */
double myTelescope::equatorialToHorizontal( double ha, double dec, double * alt, double * az )
{
    //if( verbose ) {
    //    printf( "[myTelescope::equatorialToHorizontal] [ha  = %+11.6lf]", ha );
    //    printf( "[myTelescope::equatorialToHorizontal] [dec = %+11.6lf]", dec );
    //}
    double cosAlt;

    double sinAlt       = sin( m_telescope_data->Latitude * M_PI / 180. ) * sin( dec * M_PI / 180. ) +
                cos( m_telescope_data->Latitude * M_PI / 180. ) * cos( dec * M_PI / 180. ) * cos( ha * M_PI / 180. );
    double cosAz_cosAlt = cos( m_telescope_data->Latitude * M_PI / 180. ) * sin( dec * M_PI / 180. ) -
                sin( m_telescope_data->Latitude * M_PI / 180. ) * cos( dec * M_PI / 180. ) * cos( ha * M_PI / 180. );
    double sinAz_cosAlt = - cos( dec * M_PI / 180. ) * sin( ha * M_PI / 180. );

    //if( verbose ) {
    //    printf( "[myTelescope::equatorialToHorizontal] [cosAz_cosAlt = %+11.6lf]", cosAz_cosAlt );
    //    printf( "[myTelescope::equatorialToHorizontal] [sinAz_cosAlt = %+11.6lf]", sinAz_cosAlt );
    //}
    if( cosAz_cosAlt == 0 ) {
        if( sinAz_cosAlt < 0 ) {
            * az = 270.0;
            cosAlt = - sinAz_cosAlt;
        } else {
            * az = 90.0;
            cosAlt = sinAz_cosAlt;
        }
        //if( verbose )
        //    printf( "[myTelescope::equatorialToHorizontal] [cosAlt = %+11.6lf]", cosAlt );
    } else {
        if( cosAz_cosAlt < 0 ) {
            if( sinAz_cosAlt < 0 ) {
                * az   = M_PI + atan( fabs( sinAz_cosAlt / cosAz_cosAlt ) );
            } else {
                * az   = M_PI - atan( fabs( sinAz_cosAlt / cosAz_cosAlt ) );
            }
        } else {
            if( sinAz_cosAlt < 0 ) {
                * az   = M_PI + M_PI - atan( fabs( sinAz_cosAlt / cosAz_cosAlt ) );
            } else {
                * az   = atan( fabs( sinAz_cosAlt / cosAz_cosAlt ) );
            }
        }
        cosAlt = cosAz_cosAlt / cos( * az );
        //if( verbose )
        //    printf( "[myTelescope::equatorialToHorizontal] [cosAlt = %+11.6lf]", cosAlt );
        //cosAlt = sinAz_cosAlt / sin( * az );
        //if( verbose )
        //    printf( "[myTelescope::equatorialToHorizontal] [cosAlt = %+11.6lf]", cosAlt );
        //* az *= 180./ M_PI;
        //if( verbose ) {
        //    printf( "[myTelescope::equatorialToHorizontal] [Az = %+11.6lf]", * az );
        //    printf( "[myTelescope::equatorialToHorizontal] [Az = %+11.6lf]", 180./ M_PI * atan( fabs( sinAz_cosAlt / cosAz_cosAlt ) ) );
        //}
        * az *= 180. / M_PI;
    }

    double cos2Alt = cosAz_cosAlt * cosAz_cosAlt + sinAz_cosAlt * sinAz_cosAlt;
    double rad2    = cos2Alt + sinAlt * sinAlt;

    //if( verbose )
    //    printf( "[myTelescope::equatorialToHorizontal] [sinAlt = %+11.6lf cosAlt = %+11.6lf]",
    //            sinAlt, cosAlt );

    if( cosAlt == 0 ) {
        if( sinAlt < 0 ) {
            * alt = -90.0;
        } else {
            * alt = 90.0;
        }
    } else if( cosAlt < 0 ) {
        if( sinAlt < 0 ) {
            //* alt = 180. + 180. / M_PI * atan( fabs( sinAlt / cosAlt ) );   //180..270
            * alt = - 180. / M_PI * atan( fabs( sinAlt / cosAlt ) );
        } else {
            * alt = 180. / M_PI * atan( fabs( sinAlt / cosAlt ) );   //90...180
        }
    } else {
        if( sinAlt < 0 ) {
            * alt = - 180. / M_PI * atan( fabs( sinAlt / cosAlt ) );   //270...390
        } else {
            * alt = 180. / M_PI * atan( fabs( sinAlt / cosAlt ) );          //0...90
        }
    }

    //if( verbose ) {
    //    printf( "[myTelescope::equatorialToHorizontal] [r2 = %+11.6lf alt = %+11.6lf az = %+11.6lf]",
    //            rad2, * alt, * az );
    //}
    return rad2;
}



