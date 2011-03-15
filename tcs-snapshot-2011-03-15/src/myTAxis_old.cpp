#include "myTAxis.h"

/**
 *  myTAxis::Class myTAxis 
 */
myTAxis::myTAxis( const char * fileName, char id, 
                 struct encoder_data_t * motor,
                 struct encoder_data_t * worm,
                 struct encoder_data_t * axis  ) {
   
    m_motor = motor;
    m_worm  = worm;
    m_axis  = axis;
    
    printf( "[myTAxis::myTAxis] HelloWorld!\n" );
    m_id = id;
    m_fd = 0;
    printf( "[myTAxis::myTAxis] Axis id %c\n", m_id );
    printf( "[myTAxis::myTAxis] Axis id %c motor 0x%02X %p\n", m_id, m_motor->i2c_address, m_motor );
    printf( "[myTAxis::myTAxis] Axis id %c worm  0x%02X %p\n", m_id,  m_worm->i2c_address, m_worm );
    printf( "[myTAxis::myTAxis] Axis id %c axis  0x%02X %p\n", m_id,  m_axis->i2c_address, m_axis );
    
    printf( "[myTAxis::myTAxis] Reading configuration file %s...\n", fileName );
    
    m_MotorEncoder_TicsPerSec = 0;
    m_MotorEncoder_Current_TicsPerSec = 0;
    m_MotorEncoder_Position = 0;
    m_MotorEncoder_HomePosition = 0;
    m_WormEncoder_Position  = 0;
    m_WormEncoder_Init = false;
    m_AxisEncoder_Position  = 0;

    readConfigFile( fileName );
    //m_MotorEncoder_Position = m_MotorEncoderToAxis_Reduction * m_MotorEncoder_TicsPerRev + 1;
    //m_WormEncoder_Position  = m_WormEncoderToAxis_Reduction * m_WormEncoder_TicsPerRev + 1;
    //m_AxisEncoder_Position  = m_AxisEncoderToAxis_Reduction * m_AxisEncoder_TicsPerRev + 1;
}
//myTAxis::myTAxis( const char * fileName, char id, const char * socket_name ) : myClientSocket( socket_name ) {
//    printf( "[myTAxis::myTAxis] HelloWorld!\n" );
//    m_id = id;
//    printf( "[myTAxis::myTAxis] Axis id %c\n", m_id );
//    printf( "[myTAxis::myTAxis] Reading configuration file %s...\n", fileName );
//    readConfigFile( fileName );
//}

/**
 *  myTAxis::~myTAxis
 */
myTAxis::~myTAxis() {
    printf( "[myTAxis::~myTAxis] %c Good bye!\n", m_id );
}


/**
 *  myTAxis::readConfigFile
 */
#define LINEMAXC    256
int myTAxis::readConfigFile( const char * configFileName  ) {
    FILE * fp;
    int retval;
    size_t line_length = LINEMAXC;
    //char line[LINEMAXC];
    char * line;
    char * p;
    const char delim[] = ";#";

    line = (char *) malloc( line_length );

    if( (fp = fopen( configFileName, "r" )) == NULL ) {
        printf( "[myTAxis::readConfigFilefile] fopen" );
        printf( "[myTAxis::readConfigFilefile] fopen ERROR\n" );
    } else {
        printf( "[myTAxis::readConfigFilefile] fopen OK\n" );
        do {
            if( (retval = getline( & line, & line_length, fp )) > 0 ){
                if( !( line[0] == '#' || line[0] == '\n' || line[0] == '\r'  ) ){
                    p = strtok( line, delim );
                    if( p[0] == m_id ) {
                        switch( p[1] ) {
                            case 'M':
                                switch( p[2] ) {
                                    case '2':
                                        p = strtok( NULL, delim );
                                        m_Motor_I2CAdress = (unsigned char) atoi( p );                                      
                                        printf( "[myTAxis::readConfigFilefile] %c Motor drive i2c address (AMI): 0x%2X\n", m_id, m_Motor_I2CAdress );
                                        break;
                                    case 'R':
                                        p = strtok( NULL, delim );
                                        m_MotorEncoderToAxis_Reduction = atof( p );
                                        printf( "[myTAxis::readConfigFilefile] %c Motor(Encoder) axis/alpha axis reduction (AMR): %lf\n", m_id, m_MotorEncoderToAxis_Reduction );
                                        break;
                                    case 'T':
                                        p = strtok( NULL, delim );
                                        m_MotorEncoder_TicsPerRev = atof( p );
                                        printf( "[myTAxis::readConfigFilefile] %c Q. Encoder Motor tics/rev (AMT): %lf\n", m_id, m_MotorEncoder_TicsPerRev );
                                        break;
                                    case 'P':
                                        p = strtok( NULL, delim );
                                        m_Motor_Kp = atof( p );
                                        printf( "[myTAxis::readConfigFilefile] %c Motor Kp (AMP): %lf\n", m_id, m_Motor_Kp );
                                        break;
                                    case 'I':
                                        p = strtok( NULL, delim );
                                        m_Motor_Ki = atof( p );
                                        printf( "[myTAxis::readConfigFilefile] %c Motor Ki (AMI): %lf\n", m_id, m_Motor_Ki );
                                        break;
                                    case 'D':
                                        p = strtok( NULL, delim );
                                        m_Motor_Kd = atof( p );
                                        printf( "[myTAxis::readConfigFilefile] %c Motor Ki (AMI): %lf\n", m_id, m_Motor_Kd );
                                        break;
                                    default:
                                        printf( "[myTAxis::readConfigFilefile] %c Unknown parameter M%c\n", m_id, p[2] );
                                }
                                break;
                            case 'W':
                                switch( p[2] ) {
                                    case '2':
                                        p = strtok( NULL, delim );
                                        m_WormEncoder_I2CAdress = (unsigned char) atoi( p );
                                        printf( "[myTAxis::readConfigFilefile] %c Worm abs. encoder i2c address (AWI): 0x%2X\n", m_id, m_WormEncoder_I2CAdress );
                                        break;
                                    case 'R':
                                        p = strtok( NULL, delim );
                                        m_WormEncoderToAxis_Reduction = atof( p );
                                        printf( "[myTAxis::readConfigFilefile] %c Worm abs. enc. axis/alpha axis reduction (AWR): %lf\n", m_id, m_WormEncoderToAxis_Reduction );
                                        break;
                                    case 'T':
                                        p = strtok( NULL, delim );
                                        m_WormEncoder_TicsPerRev = atof( p );
                                        printf( "[myTAxis::readConfigFilefile] %c Worm Abs. Encoder tics/rev (AWT): %lf\n", m_id, m_WormEncoder_TicsPerRev );
                                        break;
                                    case 'H':
                                        p = strtok( NULL, delim );
                                        m_WormEncoder_HomePosition = atof( p );
                                        m_WormEncoder_Position = (int) m_WormEncoder_HomePosition;
                                        printf( "[myTAxis::readConfigFilefile] %c Worm Abs. Encoder Home position (AWH): %lf\n", m_id, m_WormEncoder_HomePosition );
                                        break;
                                    default:
                                        printf( "[myTAxis::readConfigFilefile] %c Unknown parameter W%c\n", m_id, p[2] );
                                }
                                break;
                            case 'A':
                                switch( p[2] ) {
                                    case '2':
                                        p = strtok( NULL, delim );
                                        m_AxisEncoder_I2CAdress = (unsigned char) atoi( p );
                                        printf( "[myTAxis::readConfigFilefile] %c Axis abs. encoder i2c address (AWI): 0x%2X\n", m_id, m_AxisEncoder_I2CAdress );
                                        break;
                                    case 'R':
                                        p = strtok( NULL, delim );
                                        m_AxisEncoderToAxis_Reduction = atof( p );
                                        printf( "[myTAxis::readConfigFilefile] %c Axis abs. enc. axis/alpha axis reduction (AWR): %lf\n", m_id, m_AxisEncoderToAxis_Reduction );
                                        break;
                                    case 'T':
                                        p = strtok( NULL, delim );
                                        m_AxisEncoder_TicsPerRev = atof( p );
                                        printf( "[myTAxis::readConfigFilefile] %c Axis Abs. Encoder tics/rev (AWT): %lf\n", m_id, m_AxisEncoder_TicsPerRev );
                                        break;
                                    case 'H':
                                        p = strtok( NULL, delim );
                                        m_AxisEncoder_HomePosition = atof( p );
                                        m_AxisEncoder_Position = (int) m_AxisEncoder_HomePosition;
                                        printf( "[myTAxis::readConfigFilefile] %c Axis Abs. Encoder Home position (AWH): %lf\n", m_id, m_AxisEncoder_HomePosition );
                                        break;
                                    default:
                                        printf( "[myTAxis::readConfigFilefile] %c Unknown parameter W%c\n", m_id, p[2] );
                                }
                                break;
                            default:
                                printf( "[myTAxis::readConfigFilefile] %c Unknown parameter %c\n", m_id, p[1] );
                        }
                    }
                }
            } else if( ! feof( fp )  ) {
                perror( "[myTAxis::readConfigFilefile] getline" );
                printf( "[myTAxis::readConfigFilefile] getline ERROR\n" );
            }
        } while( ! feof( fp ) );
        printf( "[myTCS::readConfigFilefile] closing %s...\n", configFileName );
        if( fclose( fp ) == 0 ) {
            printf( "[myTCS::readConfigFilefile] close OK\n" );
        } else {
            perror( "[myTCS::readConfigFilefile] close" );
            printf( "[myTCS::readConfigFilefile] close ERROR\n" );
        }
    } 
    if( line != NULL ){
        printf( "[myTCS::readConfigFilefile] freeing\n" );
        free( line );
    }
    return 0;
}



/**
 * myTAxis::setAxisDegPerSec
 *
 * o Alpha case:
 *   * 1 revs Alpha Axis => 48 * 360 rev Alpha Motor Axis
 *     m_MotorToAxis_Reduction = 48 * 360 = 17280
 *
 *     f^{\alpha} = \frac{deg_per_sec[^o/s]}{360[^o]}
 *     f^{\alpha}_{w} = 360\times\frac{deg_per_sec}{360} [Hz]
 *     f^{\alpha}_{m} = 48\times 360\times \frac{deg_per_sec}{360} [Hz]
 *                    = 17280\times \frac{deg_per_sec}{360} [Hz]
 *
 *     f^{\alpha}_{QE} = 3000[tics/rev] (\times 17280\times  \frac{deg_per_sec}{360} [rev/s])
 *          (m_MotorQEncoder_TicsPerRev=3000 [tics/rev])
 *
 *     deg_per_sec[^o/s] = 360[^o]/(24\times 3600 [s])
 *
 *     f^{\alpha}_{QE} = 3000[t/rev] \times (48\times 360) 
 *                       \times \frac{360[^o]/(24\times 3660 [s])}{360}[rev/s]
 *                     = 600[tics/s]
 *
 *     deg_per_sec[^o/s] = 1[^o/s])
 *     f^{\alpha}_{QE} = 3000[t] \times (48\times 360)
 *                       \times  \frac{1}{360} [rev/s])
 *                     = 144000
 *
 */
int  myTAxis::setAxisDegPerSec( double deg_per_sec ) {
    double tmp;
    m_AxisSpeed_DegPerSec = deg_per_sec;
    tmp = m_MotorEncoder_TicsPerRev * m_MotorEncoderToAxis_Reduction * m_AxisSpeed_DegPerSec / 360.0;
    m_MotorEncoder_TicsPerSec = (int) tmp;
    
    return m_MotorEncoder_TicsPerSec;
}


/**
 * myTAxis::setMaxSlewRate
 */
int  myTAxis::setMaxSlewRate( int n ) {
    double DegPerSec;
    switch( n ) {
        case 0:
            DegPerSec = 0;              // 1x   0.25['/s]
            break;
        case 1:
            DegPerSec = 1./240.;        // 1x   0.25['/s]
            break;
        case 2:
            DegPerSec = 1./120.;        // 2x   0.5['/s]
            break;
        case 3:
            DegPerSec = 1./30.;         // 8x   2['/s]
            break;
        case 4:
            DegPerSec = 1./15.;         // 16x  4['/s]
            break;
        case 5:
            DegPerSec = 4./15.;         // 64x  16['/s]
            break;
        case 6:
            DegPerSec = 8./15.;         //128x  32['/s]
            break;           
        case 7:
            DegPerSec = 3/4.;           //      45['/s]
            break; 
        case 8:
            DegPerSec = 3/4.;           //      45['/s]
            break;
        case 9:
            DegPerSec = 3/4.;           //      45['/s]
            break;
        default:
            DegPerSec = 1./240.;        // 1x   0.25['/s]
    }
    
    return setAxisDegPerSec( DegPerSec );
}

/**
 * myTAxis::incrementTrackingRate
 */
int myTAxis::incrementTrackingRate( void ){
    if( 0 < m_MotorEncoder_TicsPerSec && m_MotorEncoder_TicsPerSec < 795 ) {
        m_MotorEncoder_TicsPerSec += 5;
    }
    return m_MotorEncoder_TicsPerSec;
}

/**
 * myTAxis::decrementTrackingRate
 */
int myTAxis::decrementTrackingRate( void ){
    if( 60 < m_MotorEncoder_TicsPerSec && m_MotorEncoder_TicsPerSec < 800 ) {
        m_MotorEncoder_TicsPerSec -= 5;
    }
    return m_MotorEncoder_TicsPerSec;
}

/**
 * myTAxis::setMotorInputVoltage
 */
int  myTAxis::setMotorInputVoltage( int V ) {
    m_Motor_InputVoltage= V;
    
    return m_Motor_InputVoltage;
}

/**
 * myTAxis::setAxisDir
 */
int  myTAxis::setAxisDir( int dir ) {
    m_Motor_AxisDir = dir;
    return m_Motor_AxisDir;
}



/**
 * myTAxis::setMotorEncoderHomePositionToCurrentPosition
 */
int myTAxis::setMotorEncoderHomePositionToCurrentPosition( void ) {
    if( m_MotorEncoder_Position < m_MotorEncoderToAxis_Reduction * m_MotorEncoder_TicsPerRev  ) {
        m_MotorEncoder_HomePosition = m_MotorEncoder_Position;
        return true;
    } else {
        return false;
    }
}

/**
 * myTAxis::setMotorEncoderHomePositionTo
 */
double myTAxis::setMotorEncoderHomePositionTo( double deg ) {
    m_MotorEncoder_HomePosition  = deg / 360.0;
    m_MotorEncoder_HomePosition *= m_MotorEncoderToAxis_Reduction;
    m_MotorEncoder_HomePosition *= m_MotorEncoder_TicsPerRev;
    return m_MotorEncoder_HomePosition;
}

/**
 * myTAxis::getPosition returns the position of the axis (in degrees).
 * The precision is given through the updatePosition.
 */
double myTAxis::getPosition( void ) {
   return getPosition_WormEncoderPrecision();
}

/**
 * myTAxis::getPosition_MotorEncoderPrecision
 */
double myTAxis::getPosition_MotorEncoderPrecision( void ) {
    double tmp;
    //m_MotorEncoder_Position  = (double) m_MotorData[4];
    tmp  = m_MotorEncoder_Position;
    tmp -= m_MotorEncoder_HomePosition;
    tmp /= (double) m_MotorEncoder_TicsPerRev;
    tmp /= m_MotorEncoderToAxis_Reduction;
    tmp *= 360.0;
    return tmp;
}

/**
 * myTAxis::getPosition_WormEncoderPrecision
 */
double myTAxis::getPosition_WormEncoderPrecision( void ) {
    double tmp;
    //m_WormEncoder_Position  = (double) m_WormData[4];
    //m_AxisEncoder_Position  = (m_WormEncoder_Position-m_WormEncoder_HomePosition) * (m_AxisEncoderToAxis_Reduction/m_WormEncoderToAxis_Reduction)  + m_AxisEncoder_HomePosition;
    tmp  = m_WormEncoder_Position;
    //printf( "%c P=%lf degs\n", m_id, tmp );
    tmp -= m_WormEncoder_HomePosition;
    //printf( "%c P=%lf degs\n", m_id, tmp );
    tmp /= (double) m_WormEncoder_TicsPerRev;
    //printf( "%c P=%lf degs\n", m_id, tmp );
    tmp /= m_WormEncoderToAxis_Reduction;
    //printf( "%c P=%lf degs\n", m_id, tmp );
    tmp *= 360.0;
    //printf( "%c P=%lf degs\n", m_id, tmp );
    return tmp;
}

/**
 * myTAxis::getPosition_AxisEncoderPrecision
 */
double myTAxis::getPosition_AxisEncoderPrecision( void ) {
    double tmp;
    //m_AxisEncoder_Position  = (double) m_AxisData[4];
    //m_WormEncoder_Position  = (m_AxisEncoder_Position - m_AxisEncoder_HomePosition) * (m_WormEncoderToAxis_Reduction/m_AxisEncoderToAxis_Reduction) + m_WormEncoder_HomePosition;
    tmp  = m_AxisEncoder_Position;
    tmp -= m_AxisEncoder_HomePosition;
    tmp /= (double) m_AxisEncoder_TicsPerRev;
    tmp /= m_AxisEncoderToAxis_Reduction;
    tmp *= 360.0;
    return tmp;
}

/**
 * myTAxis::resetPosition
 */
void myTAxis::resetPosition( double degrees ) {
    double tmp;
    
    tmp  = degrees / 360.0;
    tmp *= m_AxisEncoderToAxis_Reduction;
    tmp *= (double) m_AxisEncoder_TicsPerRev;
    tmp -= m_AxisEncoder_Position;
    tmp *= -1.0;
    m_AxisEncoder_HomePosition = tmp;
    printf( "[myTAxis::resetPosition] %c m_AxisEncoder_HomePosition  = %lf\n", m_id, m_AxisEncoder_HomePosition );
    
    tmp  = degrees / 360.0;
    tmp *= m_WormEncoderToAxis_Reduction;
    tmp *= (double) m_WormEncoder_TicsPerRev;
    tmp -= m_WormEncoder_Position;
    tmp *= -1.0;
    m_WormEncoder_HomePosition = tmp;
    printf( "[myTAxis::resetPosition] %c m_WormEncoder_HomePosition  = %lf\n", m_id, m_WormEncoder_HomePosition );

    tmp  = degrees / 360.0;
    tmp *= m_MotorEncoderToAxis_Reduction;
    tmp *= (double) m_MotorEncoder_TicsPerRev;
    tmp -= m_MotorEncoder_Position;
    tmp *= -1.0;
    m_MotorEncoder_HomePosition = tmp;
    printf( "[myTAxis::resetPosition] %c m_MotorEncoder_HomePosition = %lf\n", m_id, m_MotorEncoder_HomePosition );
}

/**
 * myTAxis::printHomePosition
 */
void myTAxis::printHomePosition( void ) {

    printf( "[myTAxis::resetPosition] %c m_AxisEncoder_HomePosition  = %lf\n", m_id, m_AxisEncoder_HomePosition );
    printf( "[myTAxis::resetPosition] %c m_WormEncoder_HomePosition  = %lf\n", m_id, m_WormEncoder_HomePosition );
    printf( "[myTAxis::resetPosition] %c m_MotorEncoder_HomePosition = %lf\n", m_id, m_MotorEncoder_HomePosition );
}

/**
 * myTAxis::setTargetPosition_MotorEncoderPrecision
 */
double myTAxis::setTargetPosition_MotorEncoderPrecision( double deg ) {
    m_MotorEncoder_TargetPosition  = deg / 360.0;
    m_MotorEncoder_TargetPosition *= m_MotorEncoderToAxis_Reduction;
    m_MotorEncoder_TargetPosition *= (double) m_MotorEncoder_TicsPerRev;
    m_MotorEncoder_TargetPosition += m_MotorEncoder_HomePosition;
    
    //m_MotorEncoder_Position  = (double) m_MotorData[4];
    //tmp  = m_MotorEncoder_Position;
    //tmp -= m_MotorEncoder_HomePosition;
    //tmp /= (double) m_MotorEncoder_TicsPerRev;
    //tmp /= m_MotorEncoderToAxis_Reduction;
    //tmp *= 360.0;
    return m_MotorEncoder_TargetPosition;
}

/**
 * myTAxis::setTargetPosition_WormEncoderPrecision
 */
double myTAxis::setTargetPosition_WormEncoderPrecision( double deg ) {
    m_WormEncoder_TargetPosition  = deg /360.0;
    m_WormEncoder_TargetPosition *= m_WormEncoderToAxis_Reduction;
    m_WormEncoder_TargetPosition *= (double) m_WormEncoder_TicsPerRev;
    m_WormEncoder_TargetPosition += m_WormEncoder_HomePosition;
    //m_WormEncoder_Position  = (double) m_WormData[4];
    //m_AxisEncoder_Position  = (m_WormEncoder_Position-m_WormEncoder_HomePosition) * (m_AxisEncoderToAxis_Reduction/m_WormEncoderToAxis_Reduction)  + m_AxisEncoder_HomePosition;
    //tmp  = (double) m_WormData[4];
    //tmp -= m_WormEncoder_HomePosition;
    //tmp /= (double) m_WormEncoder_TicsPerRev;
    //tmp /= m_WormEncoderToAxis_Reduction;
    //tmp *= 360.0;
    return m_WormEncoder_TargetPosition;
}

/**
 * myTAxis::setTargetPosition_AxisEncoderPrecision
 */
double myTAxis::setTargetPosition_AxisEncoderPrecision( double deg ) {
    m_AxisEncoder_TargetPosition  = deg / 360.0;                         //Fraction of 1 turn of the axis
    m_AxisEncoder_TargetPosition *= m_AxisEncoderToAxis_Reduction;       //Number of turns of the Axis Encoder
    m_AxisEncoder_TargetPosition *= (double) m_AxisEncoder_TicsPerRev;   //Total number of the Axis Encoder tics
    m_AxisEncoder_TargetPosition += m_AxisEncoder_HomePosition;          //Position in tics relative to home position
    
    
    //tmp  = (double) m_AxisData[4];
    //tmp -= m_AxisEncoder_HomePosition;
    //tmp /= (double) m_AxisEncoder_TicsPerRev;
    //tmp /= m_AxisEncoderToAxis_Reduction;
    //tmp *= 360.0;
    return m_AxisEncoder_TargetPosition;
}

/**
 * myTAxis::getIntegerPosition
 */
int myTAxis:: getIntegerPosition( int precision ) {
    if( precision == 0 ) {
        return  (int) m_MotorEncoder_Position;
    } else if( precision == 1 ) {
        return  (int) m_WormEncoder_Position;
    } else {
        return  (int) m_AxisEncoder_Position;
    }
}

/**
 * myTAxis::getMotorIntegerPosition
 */
int myTAxis:: getMotorIntegerPosition( void ) {
    return  (int) m_MotorEncoder_Position;//mMotorData[4];
}

/**
 * myTAxis::getWormIntegerPosition
 */
int myTAxis:: getWormIntegerPosition( void ) {
    return  (int) m_WormEncoder_Position;//m_WormData[4];
}

/**
 * myTAxis::getAxisIntegerPosition
 */
int myTAxis:: getAxisIntegerPosition( void ) {
    return  (int) m_AxisEncoder_Position;//m_AxisData[4];
}

/** ***************************************************************************
 * PIC's Commands
 */

/**
 * myTAxis::readDeviceMemory
 */
int myTAxis::readDeviceMemory( unsigned char i2c_address, char mem_address ) {
    char buffer[8];
    buffer[0] = ':';
    buffer[1] = i2c_address + 1;
    buffer[2] = mem_address;
    buffer[3] = '#';
    //return writeSocket( buffer, 4 );
    return write( m_fd, buffer, 4 );
}

/**
 * myTAxis::setDeviceMemory
 */
int myTAxis::setDeviceMemory( unsigned char i2c_address, char mem_address, int data ) {
    char buffer[8];
    char * p;
    buffer[0] = ':';
    buffer[1] = i2c_address;
    buffer[2] = mem_address;
    p = (char *) & data;
    buffer[3] = p[0];
    buffer[4] = p[1];
    buffer[5] = p[2];
    buffer[6] = p[3];
    buffer[7] = '#';
    return write( m_fd, buffer, 8 );
}
 
/**
 * myTAxis::stopMotor()
 * @param 
 */
int myTAxis::resetMotor( void ) {
    /*
    char buffer[8];

    buffer[0] = ':';
    buffer[1] = m_Motor_I2CAdress;
    buffer[2] = 0;
    buffer[3] = 2;
    buffer[4] = 0;
    buffer[5] = 0;
    buffer[6] = 0;
    buffer[7] = '#';
    
    return write( m_fd, buffer, 8 );
    */
    printf( "[myTAxis::resetMotor] Reseting %c-Motor\n", m_id );
    return setDeviceMemory( m_Motor_I2CAdress, 0, 2 );
}

/**
 * myTAxis::stopMotor()
 * @param 
 */
int myTAxis::stopMotor( void ) {
    /*
    char buffer[8];

    buffer[0] = ':';
    buffer[1] = m_Motor_I2CAdress;
    buffer[2] = 6;
    buffer[3] = 0;
    buffer[4] = 0;
    buffer[5] = 0;
    buffer[6] = 0;
    buffer[7] = '#';
    
    return write( m_fd, buffer, 8 );
    */
    printf( "[myTAxis::stopMotor] %c-Motor speed = %d[tics/s]\n", m_id, 0 );
    return setDeviceMemory( m_Motor_I2CAdress, 6, 0 );
}

/**
 * myTAxis::runMotor
 * @param 
 */
int myTAxis::runMotor( void ) {
    /*
    char buffer[8];
    char * p;
    int tmp_ticsPerSec;
    
    tmp_ticsPerSec = m_Motor_AxisDir * m_MotorEncoder_TicsPerSec;
    buffer[0] = ':';
    buffer[1] = m_Motor_I2CAdress;
    buffer[2] = 6;
    p = (char *) & tmp_ticsPerSec;
    buffer[3] = p[0];
    buffer[4] = p[1];
    buffer[5] = p[2];
    buffer[6] = p[3];
    buffer[7] = '#';
    
    return write( m_fd, buffer, 8 );
    */
    printf( "[myTAxis::runMotor] %c-Motor speed = %d[tics/s] dir = %d\n", m_id, m_MotorEncoder_TicsPerSec, m_Motor_AxisDir );
    return setDeviceMemory( m_Motor_I2CAdress, 6, m_Motor_AxisDir * m_MotorEncoder_TicsPerSec );
}

/**
 * myTAxis::runMotorToPosition
 * @param 
 */
int myTAxis::runMotorToPosition( double degs ) {
    int diffPos;
    
    degs /= 360.0;
    
    degs  *= m_MotorEncoderToAxis_Reduction;
    degs  *= m_MotorEncoder_TicsPerRev;
    diffPos = (int) degs;
    
    printf( "[myTAxis::runMotorToPosition] %c-Motor increment position in = %d[tics]\n", m_id, diffPos );
    return setDeviceMemory( m_Motor_I2CAdress, 7, diffPos);
    //return 0;
}

/**
 * myTAxis::startTracking
 * @param 
 */
int myTAxis::startTracking( int rate ) {
    /*
    char buffer[8];
    
    buffer[0] = ':';
    buffer[1] = m_Motor_I2CAdress;
    buffer[2] = 3;
    buffer[3] = 1;
    buffer[4] = 0;
    buffer[5] = 0;
    buffer[6] = 0;
    buffer[7] = '#';   
    
    return write( m_fd, buffer, 8 );
    */
    printf( "[myTAxis::runMotor] %c-Motor tracking ON\n", m_id );
    return setDeviceMemory( m_Motor_I2CAdress, 3, rate );
}

/**
 * myTAxis::stopTracking
 * @param 
 */
int myTAxis::stopTracking( void ) {
    /*
    char buffer[8];

    buffer[0] = ':';
    buffer[1] = m_Motor_I2CAdress;
    buffer[2] = 3;
    buffer[3] = 0;
    buffer[4] = 0;
    buffer[5] = 0;
    buffer[6] = 0;
    buffer[7] = '#';
    
    return write( m_fd, buffer, 8 );
    */
    printf( "[myTAxis::runMotor] %c-Motor tracking OFF\n", m_id );
    return setDeviceMemory( m_Motor_I2CAdress, 3, 0 );
}

/**
 * myTAxis::stopMotorWithInputVoltage
 * @param 
 */
int myTAxis::stopMotorWithInputVoltage( void ) {
    /*
    char buffer[8];

    buffer[0] = ':';
    buffer[1] = m_Motor_I2CAdress;
    buffer[2] = 1;
    buffer[3] = 0;
    buffer[4] = 0;
    buffer[5] = 0;
    buffer[6] = 0;
    buffer[7] = '#';
    
    return write( m_fd, buffer, 8 );
    */
    printf( "[myTAxis::runMotor] %c-Motor voltage = %d[1..1000]\n", m_id, m_Motor_InputVoltage );
    return setDeviceMemory( m_Motor_I2CAdress, 1, 0 );
}
/**
 * myTAxis::runMotorWithInputVoltage
 * @param 
 */
int myTAxis::runMotorWithInputVoltage( void ) {
    /*
    char buffer[8];
    char * p;

    buffer[0] = ':';
    buffer[1] = m_Motor_I2CAdress;
    buffer[2] = 1;
    p = (char *) & m_Motor_InputVoltage;
    buffer[3] = p[0];
    buffer[4] = p[1];
    buffer[5] = p[2];
    buffer[6] = p[3];
    buffer[7] = '#';
    
    return write( m_fd, buffer, 8 );
    */
    printf( "[myTAxis::runMotor] %c-Motor voltage = %d[1..1000]\n", m_id, m_Motor_InputVoltage );
    return setDeviceMemory( m_Motor_I2CAdress, 2, m_Motor_InputVoltage );
}

/**
 * myTAxis::updatePosition
 * @param resolution (int) = 0 Motor Quadrature Encoder resolution, 
 *                         = 1 Worm Absolute Encoder resolution
 *                         = 2 Axis Absolute Encoder resolution
 * @return retval from updateXXXXPosition()
 */
int myTAxis::updatePosition( int resolution ) {
   int retval = 0;
   if( resolution == 2 ) {
       retval = updateAxisPosition();
   } else if( resolution == 1 ) {
       retval = updateWormPosition();
   } else if( resolution == 0 ) {
       retval = updateMotorPosition();
   }
   return retval;
}

int myTAxis::updatePosition( void ) {
    int retval = true;

    m_MotorEncoder_Position = (double) m_motor->data[4];
    m_WormEncoder_Position  = (double)  m_worm->data[4];
    m_AxisEncoder_Position  = (double)  m_axis->data[4];
    printf( "[myTAxis::updatePosition] %c %lf %lf %lf\n", m_id, 
            m_MotorEncoder_Position, m_WormEncoder_Position, m_AxisEncoder_Position );
    
   return retval;
}

/**
 * myTAxis::updateMotorPosition
 */
int myTAxis::updateMotorPosition( void ) {
    return readDeviceMemory( m_Motor_I2CAdress, 4 );
}

/**
 * myTAxis::updateWormPosition
 */
int myTAxis::updateWormPosition( void ) {
    return readDeviceMemory( m_WormEncoder_I2CAdress, 4 );
}

/**
 * myTAxis::updateAxisPosition
 */
int myTAxis::updateAxisPosition( void ) {
    return readDeviceMemory( m_AxisEncoder_I2CAdress, 4 );
}

/**
 * myTAxis::receiveData receives data from devices
 *
 * If data corresponds to total position of an encoder, then calculates
 * the calculates the other encoders positions. For example, if data received
 * comes from Worm Encoder, then it calculates the Motor encoder position and
 * the axis encoder position. 
 */
char myTAxis::receiveData( char * buffer  ) {
    char retval = false;
    int idx = (int) buffer[2]; 
    //double tmp;
    unsigned char chksum;
    int number;
    if( buffer[1] % 2 != 0) {
        chksum  = (unsigned char) buffer[1];
        chksum += (unsigned char) buffer[2];
        
        chksum += (unsigned char) buffer[3];
        ((char *) & number)[0] = buffer[3];
        
        chksum += (unsigned char) buffer[4];
        ((char *) & number)[1] = buffer[4];
        
        chksum += (unsigned char) buffer[5];
        ((char *) & number)[2] = buffer[5];
        
        chksum += (unsigned char) buffer[6];
        ((char *) & number)[4] = buffer[6];
            
        printf( "[myTAxis::receiveData] 0x%02X[%d] = %d\n", (unsigned int) buffer[1], idx, number );
        if( chksum == (unsigned char) buffer[7] ) {
            if( m_Motor_I2CAdress == ( 0xFE & buffer[1] ) ) {
                m_MotorData[idx] = number;
                if( idx == 4 ) {
                    m_MotorEncoder_Position = (double) m_MotorData[idx];
                }
                retval = true;
            } else if( m_WormEncoder_I2CAdress == ( 0xFE & buffer[1] ) ) {
                m_WormData[idx] = number;
                if( idx == 4 ) {
                    m_WormEncoder_Position = (double) m_WormData[idx];
                }
                retval = true;
            } else if( m_AxisEncoder_I2CAdress == ( 0xFE & buffer[1] ) ) {
                m_AxisData[idx] = number;
                if( idx == 4 ) {
                    m_AxisEncoder_Position  = (double) m_AxisData[idx];
                }
                retval = true;
            }
        } else {
            retval = false;
        }
                
        /*     
        if( m_Motor_I2CAdress == ( 0xFE & buffer[1] ) ) {
            ((char *) & m_MotorData[idx])[0] = buffer[3];
            ((char *) & m_MotorData[idx])[1] = buffer[4];
            ((char *) & m_MotorData[idx])[2] = buffer[5];
            ((char *) & m_MotorData[idx])[3] = buffer[6];
            
            if( idx == 4 ) {
                m_MotorEncoder_Position = (double) m_MotorData[idx];
                m_WormEncoder_Position  = (m_MotorEncoder_Position-m_MotorEncoder_HomePosition) * (m_WormEncoderToAxis_Reduction/m_MotorEncoderToAxis_Reduction) + m_WormEncoder_HomePosition;
                m_AxisEncoder_Position  = (m_WormEncoder_Position-m_WormEncoder_HomePosition) * (m_AxisEncoderToAxis_Reduction/m_WormEncoderToAxis_Reduction)  + m_AxisEncoder_HomePosition;
                //printf( "[myTAxis::updateData] 0x%02X= %8lf %8lf\n", (unsigned char) buffer[1], m_WormEncoder_Position, m_AxisEncoder_Position );
            }
            
             retval = true;
        } else if( m_WormEncoder_I2CAdress == ( 0xFE & buffer[1] ) ) {

            ((char *) & m_WormData[idx])[0] = buffer[3];
            ((char *) & m_WormData[idx])[1] = buffer[4];
            ((char *) & m_WormData[idx])[2] = buffer[5];
            ((char *) & m_WormData[idx])[3] = buffer[6];
            
            if( idx == 4 ) {
                tmp = (double) m_WormData[idx];
  
                if( ! m_WormEncoder_Init  ) {
                    m_WormEncoder_Init = true;
                    m_WormEncoder_Position = tmp;
                } else if( fabs( m_WormEncoder_Position - tmp ) < 1024.0  ) {
                    m_WormEncoder_Position  = tmp;
                    m_MotorEncoder_Position = (m_WormEncoder_Position-m_WormEncoder_HomePosition) * (m_MotorEncoderToAxis_Reduction/m_WormEncoderToAxis_Reduction) + m_MotorEncoder_HomePosition;
                    m_AxisEncoder_Position  = (m_WormEncoder_Position-m_WormEncoder_HomePosition) * (m_AxisEncoderToAxis_Reduction/m_WormEncoderToAxis_Reduction)  + m_AxisEncoder_HomePosition;
                } else {
                    printf( "[myTAxis::updateData] ERROR 0x%02X= %d(0x%08X)\n", (unsigned char) buffer[1], m_WormData[idx], m_WormData[idx] );
                }

            }
            
            retval = true;
        } else if( m_AxisEncoder_I2CAdress == ( 0xFE & buffer[1] ) ) {
            ((char *) & m_AxisData[idx])[0] = buffer[3];
            ((char *) & m_AxisData[idx])[1] = buffer[4];
            ((char *) & m_AxisData[idx])[2] = buffer[5];
            ((char *) & m_AxisData[idx])[3] = buffer[6];
            
            if( idx == 4 ) {
                if( 0 < m_AxisData[idx] && m_AxisData[idx]  < 1024.0  ) {
                    m_AxisEncoder_Position  = (double) m_AxisData[idx];
                    m_WormEncoder_Position  = (m_AxisEncoder_Position - m_AxisEncoder_HomePosition) * (m_WormEncoderToAxis_Reduction/m_AxisEncoderToAxis_Reduction) + m_WormEncoder_HomePosition;
                    m_MotorEncoder_Position = (m_WormEncoder_Position-m_WormEncoder_HomePosition) * (m_MotorEncoderToAxis_Reduction/m_WormEncoderToAxis_Reduction) + m_MotorEncoder_HomePosition;
                    //printf( "[myTAxis::updateData] 0x%02X= %8lf %8lf\n", (unsigned char) buffer[1], m_WormEncoder_Position, m_AxisEncoder_Position );
                } else {
                    printf( "[myTAxis::updateData] ERROR 0x%02X= %d(0x%08X)\n", (unsigned char) buffer[1], m_AxisData[idx], m_AxisData[idx] );
                }
            }
            retval = true;
        } 
        */
    } 
    return retval;
}
    
/**
 * myTAxis::receiveStatus receives the status of devices
 */
char myTAxis::receiveStatus( char * buffer  ) {
    char retval = false;
    if( buffer[1] % 2 == 0) {
        if( m_Motor_I2CAdress == ( 0xFE & buffer[1] ) ) {
            m_MotorStatus = buffer[2];
            //printf( "[myTAxis::updateStatus] 0x%02X, ack=%d\n", (unsigned char) buffer[1], (int) buffer[2] );
            retval = true;
        } else if( m_WormEncoder_I2CAdress == ( 0xFE & buffer[1] ) ) {
            m_WormStatus = buffer[2];
            //printf( "[myTAxis::updateStatus] 0x%02X, ack=%d\n", (unsigned char) buffer[1], (int) buffer[2] );
            retval = true;
        } else if( m_AxisEncoder_I2CAdress == ( 0xFE & buffer[1] ) ) {
            m_AxisStatus = buffer[2];
            //printf( "[myTAxis::updateStatus] 0x%02X, ack=%d\n", (unsigned char) buffer[1], (int) buffer[2] );
            retval = true;
        } 
    } 
    return retval;
}
    
    
/**
 * myTAxis::set_fd
 */
int  myTAxis::set_fd( int fd ) {
    return ( m_fd = fd );
}
    

/**
 * myTAxis::get_fd
 */
int  myTAxis::get_fd( void ) {
    return m_fd;
}

