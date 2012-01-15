#include <myTelescope.h>


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


/**
  * myTelescope::myTelescope
  */
myTelescope::myTelescope( const char * fileName, const char * device, int baudrate ) : myRS232( device, baudrate ) {
    printf( "[myTelescope::myTelescope] Starting Telescope interface...\n" );
    //if( start() == 0 ) {
    //    printf( "[myTelescope::myTelescope] tty OK\n" );
    //} else {
    //    printf( "[myTelescope::myTelescope] tty ERROR\n" );
    //}
    //if( open_RS232() < 0  ) {
    //    printf( "[myTTY::run] Can't open  %s\n", getDeviceName() );
    //  
    //} else {
    //    printf( "[myTTY::run] %s is open\n", getDeviceName() );
    //    
    //}
    shared_segment_size = 1024;
    
    /** Allocate a semaphore */
    semaphore_id = binary_semaphore_allocate( SEMKEY,  S_IRUSR | S_IWUSR );

    /** Allocate a shared memory segment */
    segment_id = shmget( SHMKEY, shared_segment_size, S_IRUSR | S_IWUSR );
    if( semaphore_id > 0 &&  segment_id > 0 ) {
        /** Attach the shared memory segment */
        shared_memory = (char *) shmat( segment_id, 0, 0 );
        //printf( "sahred memory attached at adress %p\n", shared_memory );
        telescope = (struct telescope_data_t *) shared_memory;

        /** Determine the segment size */
        shmctl( segment_id, IPC_STAT, & shmbuffer );
        segment_size = shmbuffer.shm_segsz;
        printf( "[myTelescope::myTelescope] segment size: %d\n", segment_size );
    } else {
        printf( "[myTelescope::myTelescope] Can't access semaphore or shared mem segmane\n" );
    }
    
    for( int i = 0; i < 6; i ++ ){
        printf( "[myTelescope::myTelescope] encoder 0x%02X at address %p\n", 
                telescope->encoder[i].i2c_address,
                & telescope->encoder[i] );
    }
    
    printf( "[myTelescope::myTelescope] Reading configuration file %s...\n", fileName );    
    readConfigFile( fileName );
    
    printf( "[myTelescope::myTelescope] creating alpha...\n" );
    alpha = new myTAxis( fileName, 'A', & telescope->encoder[0], & telescope->encoder[2], & telescope->encoder[3] );
    //alpha = new myTAxis( fileName, 'A', "/tmp/alpha" );
    printf( "[myTelescope::myTelescope] creating delta...\n" );
    delta = new myTAxis( fileName, 'D', & telescope->encoder[1], & telescope->encoder[4], & telescope->encoder[5] );
    //delta = new myTAxis( fileName, 'D', "/tmp/delta" );
    
    //*************************************************************************
    //Timer cans start.
    printf( "[myTelescope::myTelescope] Timer...\n" );
    timer = new myTimer( m_Telescope_Longitude );
    if( timer->start() == 0 ) {
        printf( "[myTelescope::myTelescope] OK\n" );
    } else {
        printf( "[myTelescope::myTelescope] ERROR\n" );
    }
    
    tty_message_bytes = 0;
}


/**
 * myTelescope::~myTelescope
 */
myTelescope::~myTelescope( void ) {

    //if( stop() == 0 ) {
    //    printf( "[myTelescope::~myTelescope] tty stoped succesfully\n" );
    //} else {
    //    printf( "[myTelescope::~myTelescope] error when stoping tty\n" );
    //}
    
    printf( "[myTelescope::~myTelescope] deleting alpha...\n" );
    delete alpha;
    printf( "[myTelescope::~myTelescope] deleting delta...\n" );
    delete delta;
    
    //printf( "[myTelescope::~myTelescope] Exiting...\n" );
    //close_RS232();
    /** Detach the shared memory segment */
    shmdt( shared_memory );
    printf( "[myTelescope::myTelescope] shared_memory detached\n" );
    
    //*************************************************************************
    //Delete timer
    printf( "[myTelescope::~myTelescope] Deleting timer\n" );
    delete timer;
    printf( "[myTelescope::~myTelescope] Good bye!\n" );

}

/**
 *  Read Config File
 *  Path "./.telescope/"
 */
#define LINEMAXC    256
int myTelescope::readConfigFile( const char * configFileName  ) {
    FILE * fp;
    int retval;
    size_t line_length = LINEMAXC;

    char * line;
    char * p;
    const char delim[] = ";#";
    int deg, min, sec;

    line = (char *) malloc( line_length );
    printf( "[myTelescope::readConfigFilefile] Configuration File %s\n", configFileName );

    if( (fp = fopen( configFileName, "r" )) == NULL ) {
        printf( "[myTelescope::readConfigFilefile] fopen" );
        printf( "[myTelescope::readConfigFilefile] fopen ERROR\n" );
    } else {
        printf( "[myTelescope::readConfigFilefile] fopen OK\n" );
        do {
            if( (retval = getline( & line, & line_length, fp ) ) > 0 ){
                if( !( line[0] == '#' || line[0] == '\n' || line[0] == '\r'  ) ){
                    p = strtok( line, delim );
                    switch( p[0] ) {
                        case 'V':
                            switch( p[1] ) {
                                case 'P':
                                    p = strtok( NULL, delim );
                                    m_Telescope_Name = new char[strlen(p) + 1];
                                    strcpy( m_Telescope_Name, p );
                                    printf( "[myTCS::readConfigFilefile] Telescope (product) name (VP): %s\n", m_Telescope_Name );
                                    break;
                                    /*
                                case 'D':
                                    p = strtok( NULL, delim );
                                    m_Telescope_FirmwareDate = new char[strlen(p) + 1];
                                    strcpy( m_Telescope_FirmwareDate, p );
                                    printf( "[myTCS::readConfigFilefile] Telescope firmware date (VD): %s\n", m_Telescope_FirmwareDate );
                                    break;
                                case 'T':
                                    p = strtok( NULL, delim );
                                    m_Telescope_FirmwareNumber = new char[strlen(p) + 1];
                                    strcpy( m_Telescope_FirmwareNumber, p );
                                    printf( "[myTCS::readConfigFilefile] Telescope firmware time (VT): %s\n", m_Telescope_FirmwareTime );
                                    break;
                                case 'N':
                                    p = strtok( NULL, delim );
                                    m_Telescope_FirmwareNumber = new char[strlen(p) + 1];
                                    strcpy( m_Telescope_FirmwareNumber, p );
                                    printf( "[myTCS::readConfigFilefile] Telescope firmware number (VN): %s\n", m_Telescope_FirmwareNumber );
                                    break;
                                default:
                                    printf( "[myTelescope::readConfigFilefile] Unknown parameter V%c\n", p[1] );
                                    */
                            }
                            break;
                        case 'g':
                            p = strtok( NULL, delim );
                            sscanf( p, "%d*%d:%d", & deg, & min, & sec );
                            m_Telescope_Longitude = fabs( (double) deg) + ((double) min)/60.0 + ((double) sec)/3600.0;
                            if( deg < 0 ) {
                                m_Telescope_Longitude *= -1.0;
                            }
                            printf( "[myTelescope::readConfigFilefile] Current site Longitude (g): %lf\n", m_Telescope_Longitude );       
                            break;
                        case 't':
                            p = strtok( NULL, delim );
                            sscanf( p, "%d*%d:%d", & deg, & min, & sec );
                            m_Telescope_Latitude = fabs( (double) deg) + ((double) min)/60.0 + ((double) sec)/3600.0;
                            if( deg < 0 ) {
                                m_Telescope_Latitude *= -1.0;
                            }
                            printf( "[myTelescope::readConfigFilefile] Current site Latitude (t): %lf\n", m_Telescope_Latitude );       
                            break;
                        case 'H':
                            p = strtok( NULL, delim );
                            m_Telescope_Altitude = atof( p );
                            printf( "[myTelescope::readConfigFilefile] Current site Altitude (H): %lf\n", m_Telescope_Altitude );       
                            break;
                        case 'o':
                            p = strtok( NULL, delim );
                            sscanf( p, "%lf*", & m_TelescopeSlew_LowElevation );
                            printf( "[myTelescope::readConfigFilefile] Low Elev above hrzn that tel will be allowed to slew (o): %lf\n", m_TelescopeSlew_LowElevation );
                            break;
                        case 'h':
                            p = strtok( NULL, delim );
                            sscanf( p, "%lf*", & m_TelescopeSlew_HighElevation );
                            printf( "[myTelescope::readConfigFilefile] High Elev above hrzn that tel will be allowed to slew (h): %lf\n", m_TelescopeSlew_HighElevation );
                            break;
                        case 'T':
                            p = strtok( NULL, delim );
                            m_Telescope_TrackingRate = atof( p );
                            printf( "[myTelescope::readConfigFilefile] Tracking rate (60[Hz]=>alpha at sidereal rate) (T): %lf\n", m_Telescope_TrackingRate );
                            break;
                        case 'w':
                            p = strtok( NULL, delim );
                            m_Telescope_SlewRate = atof( p );
                            printf( "[myTelescope::readConfigFilefile] Set maximum slew rate to W (2,...8) [deg/s] (w): %lf\n", m_Telescope_SlewRate );
                            break;
                        //default:
                        //    printf( "[myTelescope::readConfigFilefile] Unknown parameter %c\n", p[0] );
                          
                    }
                }
            } else if( ! feof( fp )  ) {
                perror( "[myTelescope::readConfigFilefile] getline" );
                printf( "[myTelescope::readConfigFilefile] getline ERROR\n" );
            } 
        } while( ! feof( fp ) );
        printf( "[myTelescope::readConfigFilefile] closing %s\n", configFileName );
        if( fclose( fp ) == 0 ) {
            printf( "[myTelescope::readConfigFilefile] close OK\n" );
        } else {
            perror( "[myTelescope::readConfigFilefile] close" );
            printf( "[myTelescope::readConfigFilefile] close ERROR\n" );
        }
    } 
    if( line != NULL ) {
        free( line );
    }
    return 0;
}

/**
 * myTelescope::getFirmwareDate
 */
char *  myTelescope::getFirmwareDate( void) {
    return m_Telescope_FirmwareDate;
}
/**
 * myTelescope::getFirmwareTime
 */
char *  myTelescope::getFirmwareTime( void ) {
    return m_Telescope_FirmwareTime;
}
/**
 * myTelescope::getFirmwareNumber
 */
char *  myTelescope::getFirmwareNumber( void ) {
    return m_Telescope_FirmwareNumber;
}
/**
 * myTelescope::getName
 */
char *  myTelescope::getName( void ) {
    return m_Telescope_Name;
}

/**
 * myTelescope::startup( void )
 */
int myTelescope::startup( void ) {
    int retval;
    retval  = updateFirmwareDate();
    retval += updateFirmwareTime();
    retval += updateFirmwareNumber();
    return retval;
}
 
 



/**
 * myTelescope::getLongitude
 */
double myTelescope::getLongitude( void ) {
    return m_Telescope_Longitude;
}

/**
 * myTelescope::getLatitude
 */
double myTelescope::getLatitude( void ) {
    return m_Telescope_Latitude;
}

/**
 * myTelescope::getAltitudeAboveSeeLevel
 */
double myTelescope::getAltitudeAboveSeeLevel( void ) {
    return m_Telescope_Altitude;
}

/**
 * myTelescope::receiveRS232Data
 * @brief Updates Telscope Data with info received from hardware.
 */
int myTelescope::receiveRS232Data( void ) {
    char line[32];
    int  rx_bytes;

    if( getIsConnected() ) {
        memset( line, 0, 32 );
        if( (rx_bytes= status_RS232()) > 0 ) {
            rx_bytes = read_RS232( line, 32 );
            for( int i = 0; i < rx_bytes; i ++ ) {
                if( tty_message_bytes == 0 ) {
                    if( line[i] == ':' ) {
                        //printf( "[myTelescope::update] tty: Rx new message\n" );
                        tty_message[tty_message_bytes] = line[i];
                        tty_message_bytes ++;
                    }
                } else {
                    tty_message[tty_message_bytes] = line[i];
                    tty_message_bytes ++;
                    //unsigned int i2c;
                    //i2c = (unsigned int) tty_message[1];
                    //printf( "[myTCS::run] i2c=%d\n", (unsigned int) tty_message[1]);
                    if( (0xFE & tty_message[1]) < 160 ) {
                        if( line[i] == '#' ) {
                            //printf( "[myTTY::run] 0x%02X\n", (unsigned char) tty_message[1] );
                            tty_message[tty_message_bytes] = 0;
                            
                            printf( "[myTelescope::receiveRS232Data] %s\n", tty_message );
                            if( tty_message[1] == 'V' ) {
                                switch( tty_message[2] ) {
                                    case 'D':
                                        m_Telescope_FirmwareDate =  new char[tty_message_bytes-3];
                                        strcpy( m_Telescope_FirmwareDate, & tty_message[3] );
                                        printf( "[myTCS::receiveRS232Data] Telescope firmware date (VD): %s\n", m_Telescope_FirmwareDate );
                                        break;
                                    case 'T':
                                        m_Telescope_FirmwareTime = new char[tty_message_bytes-3];
                                        strcpy( m_Telescope_FirmwareTime, & tty_message[3] );
                                        printf( "[myTCS::receiveRS232Data] Telescope firmware time (VT): %s\n", m_Telescope_FirmwareTime );
                                        break;
                                    case 'N':
                                        m_Telescope_FirmwareNumber = new char[tty_message_bytes-3];
                                        strcpy( m_Telescope_FirmwareNumber, & tty_message[3] );
                                        printf( "[myTCS::receiveRS232Data] Telescope firmware number (VN): %s\n", m_Telescope_FirmwareNumber );
                                        break;
                                    default:
                                        printf( "[myTCS::receiveRS232Data] %s\n", tty_message );
                                }
                               
                            } else if( tty_message[1] == 'T' ) {
                                getPositionData();
                            }
                            tty_message_bytes = 0;
                        }
                    } else {   
                        if( tty_message[1] % 2 == 0 ) {
                            if( tty_message_bytes == 4 && line[i] == '#' ) {
                                tty_message[tty_message_bytes] = 0;
                                //printf( "[myTelescope::update] " );
                                if( alpha->receiveStatus( tty_message ) ) {
                                //    printf( "alpha: " );
                                } else if( delta->receiveStatus( tty_message ) ) {
                                //    printf( "delta:" );
                                } else {
                                //    printf( "wrong address:" );
                                }
                                //printf( " [0x%02X=%d]\n", (unsigned char) tty_message[1], (int) tty_message[2] );
                                tty_message_bytes = 0;
                           }  else if( tty_message_bytes > 5 ) {
                                tty_message_bytes = 0;
                                printf( "Something WRONG!\nMessage too long for address 0x%02X\n", tty_message[1] );
                            }
                        } else {                                    
                            if( tty_message_bytes == 9 && line[i] == '#' ) {
                                tty_message[tty_message_bytes] = 0;
                                //int numero;
                                //((char *) & numero)[0] = tty_message[3];
                                //((char *) & numero)[1] = tty_message[4];
                                //((char *) & numero)[2] = tty_message[5];
                                //((char *) & numero)[3] = tty_message[6];
                                //printf( "[myTelescope::update] " );
                                if( alpha->receiveData( tty_message ) ) {
                                //    printf( "alpha: " );
                                } else if( delta->receiveData( tty_message ) ) {
                                //    printf( "delta:" );
                                } else {
                                //    printf( "wrong address:" );
                                }
                                //printf( " 0x%02X[%d]=%d\n", (unsigned char) tty_message[1], (int) tty_message[2], numero );
                                tty_message_bytes = 0;
                            }  else if( tty_message_bytes > 10 ) {
                                tty_message_bytes = 0;
                                printf( "Something WRONG!\nMessage too long for address 0x%02X\n", tty_message[1] );
                            }
                        }
                    }
                }
            }
        }
        return rx_bytes;
    } else {
        return 0;
    }
}

/**
 * getHA
 * alpha = ha
 * delta 0 and alpha = 0 telescope pointing to zenith.
 */
double myTelescope::getHA( void ) {
    m_currentHA = alpha->getPosition();
    return m_currentHA;
}

/**
 * getRA
 * delta 0 and alpha = 0 telescope pointing to zenith.
 */
double myTelescope::getRA( void ) {
    m_currentHA = alpha->getPosition();
    m_currentRA = timer->getLST() - m_currentHA;
    if( m_currentRA < 0 ) {
        m_currentRA += 360.0;
    } else if( m_currentRA > 360.0 ) {
        m_currentRA -= 360.0;
    }
    return m_currentRA;
}

/**
 * getDec
 * dec = delta + Latitude
 * delta = 0 => dec = -33.15*
 * delta 0 and alpha = 0 telescope pointing to zenith.
 */
double myTelescope::getDec( void ) {
    m_currentDec  = m_Telescope_Latitude - delta->getPosition();
    return m_currentDec;
}

/**
 * getDifferenceHA
 */
double  myTelescope::getDifferenceHA( void ) {
    return m_differenceHA;
}
/**
 * getDifferenceRA
 */
double  myTelescope::getDifferenceRA( void ) {
    return m_differenceRA;
}
/**
 * getDifferenceDec
 */
double  myTelescope::getDifferenceDec( void ) {
    return m_differenceDec;
}


/**
 * updateFirmwareDate
 *  
 */
int myTelescope::updateFirmwareDate( void ){
    //sprintf( line, ":VD#" );
    return write_RS232( ":VD#", 4 );
}

/**
 * updateFirmwareTime
 */
int myTelescope::updateFirmwareTime( void ){
    //sprintf( line, ":VT#" );
    return write_RS232( ":VT#", 4 );
}

/**
 * updateFirmwareNumber
 * 
 */
int myTelescope::updateFirmwareNumber( void ){
    //sprintf( line, ":VN#" );
    return write_RS232( ":VN#", 4 );
}

/**
 * getTelescopeLatitude_MeadeFormat
 * @return pointer (char *) to string with Data
 * in Meade format 
 */
char * myTelescope::getTelescopeLatitude_MeadeFormat( char * line ) {
    Grados_A_Gr_Mi_Sg( m_Telescope_Latitude, line );
    return line;
}

/**
 * getTelescopeLongitude_MeadeFormat
 * @return pointer (char *) to string with Data
 * in Meade format 
 */
char * myTelescope::getTelescopeLongitude_MeadeFormat( char * line ) {
    Grados_A_Gr_Mi_Sg( m_Telescope_Longitude, line );
    return line;
}

/**
 * getHA_MeadeFormat( char * line )
 */
char * myTelescope::getHA_MeadeFormat( char * line ) {
    //m_currentHA = alpha->getPosition();
    Grados_A_Hr_Mi_Sg( m_currentHA, line );
    return line;
}

/**
 * getRA_MeadeFormat( char * line )
 */
char * myTelescope::getRA_MeadeFormat( char * line ) {
    //m_currentHA = alpha->getPosition();
    //m_currentRA = timer->getLST() - m_currentHA;
    Grados_A_Hr_Mi_Sg( getRA(), line );
    return line;
}

/**
 * getDec_MeadeFormat( char * line ) 
 */
char * myTelescope::getDec_MeadeFormat( char * line )  {
    //m_currentDec  = delta->getPosition() + m_Telescope_Latitude;
    Grados_A_Gr_Mi_Sg_MeadeFormat( getDec(), line );
    return line;
}

/**
 * updatePosition
 */
void myTelescope::updatePosition( void ) {
    binary_semaphore_wait( semaphore_id );
    alpha->updatePosition();
    delta->updatePosition();
    binary_semaphore_post( semaphore_id );
}

/**
 * checkAlphaPosition
 * delta 0 and alpha = 0 telescope pointing to zenith.
 */
int myTelescope::checkAlphaPosition( void ) {
    double ha = alpha->getPosition();
    if( m_TelescopeSlew_LowElevation < ha && ha < m_TelescopeSlew_HighElevation ) {
        return true;
    } else {
        printf( "[myTelescope::checkAlphaPosition] Warnig Telescope exceeding limits  (ha= %lf).\n", ha );
        return false;
    }
}


/**
 * checkDeltaPosition
 * delta 0 and alpha = 0 telescope pointing to zenith.
 */
int myTelescope::checkDeltaPosition( void ) {
    double dl = delta->getPosition();
    if( m_TelescopeSlew_LowElevation < dl && dl < m_TelescopeSlew_HighElevation ) {
        return true;
    } else {
        printf( "[myTelescope::setTargetDec] Warnig Telescope exceeding limits  (dl= %lf).\n", dl );
        return false;
    }
}

/**
 * setTargetRA
 * delta 0 and alpha = 0 telescope pointing to zenith.
 */
int myTelescope::setTargetRA( double t_ra ) {
    double t_ha;
    printf( "[myTelescope::setTargetRA] T.RA = %lf\n", t_ra );
    t_ha = timer->getLST();
    printf( "[myTelescope::setTargetRA] T.LS = %lf\n", t_ha );
    t_ha -= t_ra;
    printf( "[myTelescope::setTargetRA] T.HA = %lf\n", t_ha );
    
    if( t_ha < -180.0  ) {
        t_ha += 360.0;
    } else if( t_ha > 180.0 ) {
        t_ha -= 360.0;
    }
    
    if( m_TelescopeSlew_LowElevation < t_ha && t_ha < m_TelescopeSlew_HighElevation ) {
        m_targetRA = t_ra;
        m_targetHA = t_ha;
        printf( "[myTelescope::setTargetRA] t_ha = %lf\n", t_ha );
        return true;
    } else {
        printf( "[myTelescope::setTargetRA] Error, t_ha = %lf\n", t_ha );
        return false;
    }
}

/**
 * setTargetDec
 * dec = delta + Latitude
 * delta = 0 => dec = -33.15*
 * delta 0 and alpha = 0 telescope pointing to zenith.
 */
int myTelescope::setTargetDec( double t_dec ) {
    double dl;
    printf( "[myTelescope::setTargetDec] T.Dec = %lf\n", t_dec );
    printf( "[myTelescope::setTargetDec] T.Lat = %lf\n", m_Telescope_Latitude );
    dl = t_dec - m_Telescope_Latitude;
    printf( "[myTelescope::setTargetDec] dl = %lf\n", dl );
    if( m_TelescopeSlew_LowElevation < dl && dl < m_TelescopeSlew_HighElevation ) {
        m_targetDec = t_dec;
        return true;
    } else {
        printf( "[myTelescope::setTargetDec] Error, t_dec = %lf\n", t_dec );
        return false;
    }
}



/**
 * getTargetHA
 */
double myTelescope::getTargetHA( void ) {
    return timer->getLST() - m_targetRA;
}

/**
 * getTargetRA
 */
double myTelescope::getTargetRA( void ) {
    return m_targetRA;
}

/**
 * getTargetDec
 */
double myTelescope::getTargetDec( void ) {
    return m_targetDec;
}


/**
 * setSlewLowElevation
 */
int myTelescope::setSlewLowElevation( double t_le ) {
    int retval = 0;
    if( -90.0 <= t_le  && t_le < 0  ) {
        m_TelescopeSlew_LowElevation = t_le;
        retval = 1;
    } else {
        m_TelescopeSlew_LowElevation = -89.0;
        retval = 0;
    }
    printf( "[myTelescope::setSlewLowElevation] m_TelescopeSlew_LowElevation = %lf\n", t_le );
    return retval;   
    
}

/**
 * setSlewHighElevation
 */
int myTelescope::setSlewHighElevation( double t_he ) {
    int retval = 0;
    if( 0 < t_he && t_he <= 90.0 ) {
        m_TelescopeSlew_HighElevation = t_he;
        retval = 1;
    } else {
        m_TelescopeSlew_HighElevation = 89.0;
        retval = 0;
    }
    printf( "[myTelescope::setSlewHighElevation] m_TelescopeSlew_HighElevation = %lf\n", t_he );
    return retval;
}

/**
 *  setAxesComm
 */
int myTelescope::setAxesComm( void ) {
    int retval;
    
    retval = getPort();
    alpha->set_fd( retval );
    delta->set_fd( retval ); 
    
    return retval;
}


/**
 *  updatePositionData
 */
char * myTelescope::updatePositionData( char * pData ) {
    char * ptr;
    int l;

    getRA();
    if( m_targetRA == 0 ) {
        m_targetRA = m_currentRA;
    }
    m_targetHA = timer->getLST() - m_targetRA;
    m_differenceRA= m_targetRA - m_currentRA;
    m_differenceHA= m_targetHA - m_currentHA;
    if(  m_differenceRA < -180.0  ) {
        m_differenceRA += 360.0;
    } else if( 180.0 < m_differenceRA ) {
        m_differenceRA -= 360.0;
    }
    if(  m_differenceHA < -180.0  ) {
        m_differenceHA += 360.0;
    } else if( 180.0 < m_differenceHA ) {
        m_differenceHA -= 360.0;
    }
    getDec();
    if( m_targetDec == 0 ) {
        m_targetDec = m_currentDec;
    }
    m_differenceDec= m_targetDec - m_currentDec;
        
    pData[0] = '?';
    pData[1] = 'P';
    l = 2;
    //  (1) m_currentRA
    ptr = & pData[l];   
    Grados_A_Hr_Mi_Sg( m_currentRA, ptr );
    l = strlen( pData );
    pData[l] = ';';
    l ++;
    pData[l] = 0;
    
    //  (2) m_currentHA
    ptr = & pData[l];   
    Grados_A_Hr_Mi_Sg( m_currentHA, ptr );
    l = strlen( pData );
    pData[l] = ';';
    l ++;
    pData[l] = 0;

    //  (3) m_differenceRA
    ptr = & pData[l];   
    Grados_A_Hr_Mi_Sg( m_differenceRA, ptr );
    l = strlen( pData );
    pData[l] = ';';
    l ++;
    pData[l] = 0;

    //  (4) m_differenceHA
    ptr = & pData[l];   
    Grados_A_Hr_Mi_Sg( m_differenceHA, ptr );
    l = strlen( pData );
    pData[l] = ';';
    l ++;
    pData[l] = 0;
    
    //  (5) m_targetRA
    ptr = & pData[l];   
    Grados_A_Hr_Mi_Sg( m_targetRA, ptr );
    l = strlen( pData );
    pData[l] = ';';
    l ++;
    pData[l] = 0;
    
    //  (6) m_targetHA
    ptr = & pData[l];   
    Grados_A_Hr_Mi_Sg( m_targetHA, ptr );
    l = strlen( pData );
    pData[l] = ';';
    l ++;
    pData[l] = 0;
    
    // (7)  m_currentDec
    ptr = & pData[l];   
    Grados_A_Gr_Mi_Sg_MeadeFormat( m_currentDec, ptr );
    l = strlen( pData );
    pData[l] = ';';
    l ++;
    pData[l] = 0;
    
    //  (8) m_differenceDec
    ptr = & pData[l];   
    Grados_A_Gr_Mi_Sg_MeadeFormat( m_differenceDec, ptr );
    l = strlen( pData );
    pData[l] = ';';
    l ++;
    pData[l] = 0;
    
    //  (9) m_targetDec
    ptr = & pData[l];   
    Grados_A_Gr_Mi_Sg_MeadeFormat( m_targetDec, ptr );
    l = strlen( pData );
    pData[l] = '#';
    l ++;
    pData[l] = 0;

    return pData;
}

/**
 *  updateSiteData
 */
char * myTelescope::updateSiteData( char * pData ) {
    char * ptr;
    int l;

        
    pData[0] = '?';
    pData[1] = 'L';
    l = 2;
    //  (1) Name
    ptr = & pData[l];   
    sprintf( ptr, "%s", m_Telescope_Name );
    l = strlen( pData );
    pData[l] = ';';
    l ++;
    pData[l] = 0;
    
    //  (2) Latitude
    ptr = & pData[l];   
    Grados_A_Gr_Mi_Sg_MeadeFormat( m_Telescope_Latitude, ptr );
    l = strlen( pData );
    pData[l] = ';';
    l ++;
    pData[l] = 0;
    //  (3) Longitude
    ptr = & pData[l];   
    Grados_A_Gr_Mi_Sg_MeadeFormat( m_Telescope_Longitude, ptr );
    l = strlen( pData );
    pData[l] = ';';
    l ++;
    pData[l] = 0;
    //  (4) Altitude
    ptr = & pData[l];   
    sprintf( ptr, "%8.1lf#", m_Telescope_Altitude );
    //l = strlen( pData );


    return pData;
}



/**
 *  updateEncodersData
 */
char * myTelescope::updateEncodersData( char * pData ) {
    char * ptr;
    int l;

        
    pData[0] = '?';
    pData[1] = 'E';
    l = 2;
    //  (1) alpha motor
    ptr = & pData[l];   
    sprintf( ptr, "% 8d;", alpha->getMotorIntegerPosition() );
    l = strlen( pData );

    //  (2) alpha worm
    ptr = & pData[l];   
    sprintf( ptr, "% 8d;", alpha->getWormIntegerPosition() );
    l = strlen( pData );

    //  (3) alpha axis
    ptr = & pData[l];   
    sprintf( ptr, "% 8d;", alpha->getAxisIntegerPosition() );
    l = strlen( pData );

    //  (4) delta motor
    ptr = & pData[l];   
    sprintf( ptr, "% 8d;", delta->getMotorIntegerPosition() );
    l = strlen( pData );

    //  (5) delta worm
    ptr = & pData[l];   
    sprintf( ptr, "% 8d;", delta->getWormIntegerPosition() );
    l = strlen( pData );
    
    //  (6) delta axis
    ptr = & pData[l];   
    sprintf( ptr, "% 8d#", delta->getAxisIntegerPosition() );

    return pData;
}

/**
 * getPositionData
 */
int myTelescope::getPositionData( void ) {
    return write_RS232( ":P#", 3 );
}
