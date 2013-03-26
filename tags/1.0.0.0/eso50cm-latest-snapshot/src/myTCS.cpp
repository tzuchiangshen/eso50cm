#include "myTCS.h"
 

myTCS::myTCS( const char * fileName  ) {
    readConfigFile( fileName );
    //*************************************************************************
    //telescope
    telescope = new myTelescope( fileName, m_Telescope_SerialCommDevice, m_Telescope_SerialCommBaudRate );

    
    //*************************************************************************
    //TCP server
    tcp = new myTCP( m_TCP_IP_Port );

    //*************************************************************************
    //Handset
    handset = new myTTY( "/dev/ttyUSB0", 57600 );
    
    //*************************************************************************
    //thesky
    //thesky = new myTTY( "/dev/ttts5", 9600 );
    

    printf( "[myTCS::myTCS] Starting TCS TCP/IP monitor...\n" );
    if( tcp->start()== 0 ) {
        printf( "[myTCS::myTCS] tcp OK\n" );
    } else {
        printf( "[myTCS::myTCS] tcp ERROR\n" );
    }
    m_FullStop           = false;
    m_SlewToTarget_AltAz = false;
    m_SlewToTarget       = false;
    m_SlewingToAlphaTarget = false;
    m_SlewingToDeltaTarget = false;
    m_SlewToTarget_AlphaReady = false;
    m_SlewToTarget_DeltaReady = false;

    m_MoveToNorth        = false;
    m_RunningDelta       = false;
    m_MoveToWest         = false;
    m_RunningAlpha       = false;
}

myTCS::~myTCS( void ) {
    printf( "[myTCS::~myTCS] Exiting...\n" );
    if( m_SiteName1 != NULL ) {
        delete m_SiteName1;
    }
    if( m_SiteName2 != NULL ) {
         delete m_SiteName2;
    }
    if( m_SiteName3 != NULL ) {
         delete m_SiteName3;
    }
    if( m_SiteName4 != NULL ) {
         delete m_SiteName4;
    }

    //*************************************************************************
    //Delete tcp
    printf( "[myTCS::stop] Deleting tcp\n" );
    delete tcp;

    //*************************************************************************
    //Delete telescope
    printf( "[myTCS::stop] Deleting telescope\n" );
    delete telescope;

    //*************************************************************************
    //Delete tty
    //printf( "[myTCS::stop] Deleting tty\n" );
    delete handset;
    
    //*************************************************************************
    //Delete thesky
    //printf( "[myTCS::stop] Deleting tty\n" );
    //delete thesky;

    //*************************************************************************
    printf( "[myTCS::stop] Good bye!\n" );
}


/**
 *  Read Config File
 *  Path ...
 */
#define LINEMAXC    256
int myTCS::readConfigFile( const char * configFileName  ) {
    FILE * fp;
    int retval;
    size_t line_length = LINEMAXC;

    char * line;
    char * p;
    const char delim[] = ";#";
    //int deg, min, sec;
    //double tmp;

    line = (char *) malloc( line_length );
    printf( "[myTCS::readConfigFilefile] Configuration File %s\n", configFileName );

    if( (fp = fopen( configFileName, "r" )) == NULL ) {
        printf( "[myTCS::readConfigFilefile] fopen" );
        printf( "[myTCS::readConfigFilefile] fopen ERROR\n" );
    } else {
        printf( "[myTCS::readConfigFilefile] fopen OK\n" );
        do {
            if( (retval = getline( & line, & line_length, fp ) ) > 0 ){
                if( !( line[0] == '#' || line[0] == '\n' || line[0] == '\r'  ) ){
                    p = strtok( line, delim );
                    switch( p[0] ) {
                        case 'M':
                            if( (p = strtok( NULL, delim )) != NULL ) {
                                m_SiteName1 = new char[strlen(p) + 1];
                                strcpy( m_SiteName1, p );
                                printf( "[myTCS::readConfigFilefile] Site Name 1 (M): %s\n", m_SiteName1 );
                            }else {
                                printf( "[myTCS::readConfigFilefile] Site Name 1 (M): void\n" );
                            }
                            break;
                        case 'N':
                            if( (p = strtok( NULL, delim )) != NULL ) {
                                m_SiteName2 = new char[strlen(p) + 1];
                                strcpy( m_SiteName2, p );
                                printf( "[myTCS::readConfigFilefile] Site Name 2 (N): %s\n", m_SiteName2 );
                            }else {
                                printf( "[myTCS::readConfigFilefile] Site Name 2 (N): void\n" );
                            }
                            break;
                        case 'O':
                            if( (p = strtok( NULL, delim )) != NULL ) {
                                m_SiteName3 = new char[strlen(p) + 1];
                                strcpy( m_SiteName3, p );
                                printf( "[myTCS::readConfigFilefile] Site Name 3 (O): %s\n", m_SiteName3 );
                            }else {
                                printf( "[myTCS::readConfigFilefile] Site Name 3 (O): void\n" );
                            }
                            break;
                        case 'P':
                            if( (p = strtok( NULL, delim )) != NULL ) {
                                m_SiteName4 = new char[strlen(p) + 1];
                                strcpy( m_SiteName4, p );
                                printf( "[myTCS::readConfigFilefile] Site Name 4 (O): %s\n", m_SiteName4 );
                            }else {
                                printf( "[myTCS::readConfigFilefile] Site Name 4 (O): void\n" );
                            }
                             break;
                        case 'G':
                            p = strtok( NULL, delim );
                            m_CurrentSite_UTCOffset = atoi( p );
                            printf( "[myTCS::readConfigFilefile] Current site UTC offset time (G): %d\n", m_CurrentSite_UTCOffset );       
                            break;
                        case 'c':
                            p = strtok( NULL, delim );
                            m_CurrenCalendarFormat = atoi( p );
                            printf( "[myTCS::readConfigFilefile] Current calendar format (c): %d\n", m_CurrenCalendarFormat );       
                            break;
                        case 'B':
                            p = strtok( NULL, delim );
                            m_TCS_SerialCommBaudRate = atoi( p );
                            printf( "[myTCS::readConfigFilefile] TCS comm port baud rate (B): %d\n", m_TCS_SerialCommBaudRate );       
                            break;
                        case 'p':
                            switch( p[1] ) {
                                case 'T':
                                    switch( p[2] ) {
                                        case 'P':
                                            p = strtok( NULL, delim );
                                            m_TCS_SerialCommDevice = new char[strlen(p) + 1];
                                            strcpy( m_TCS_SerialCommDevice, p );
                                            printf( "[myTCS::readConfigFilefile] TCS comm port device (pPP): %s\n", m_TCS_SerialCommDevice );
                                            break;
                                        case 'B':
                                            p = strtok( NULL, delim );
                                            m_TCS_SerialCommBaudRate = atoi( p );
                                            printf( "[myTCS::readConfigFilefile] TCS comm port device (pPB): %d\n", m_TCS_SerialCommBaudRate );
                                            break;
                                        default:
                                            printf( "[myTCS::readConfigFilefile] Unknown parameter pP%c\n", p[2] );
                                            break;
                                    }
                                    break;
                                case 'I':
                                    p = strtok( NULL, delim );
                                    m_TCP_IP_Port = atoi( p );
                                    printf( "[myTCS::readConfigFilefile] TCS TCP/IP port  (pI): %d\n", m_TCP_IP_Port );
                                    break;
                                case 'G':
                                    switch( p[2] ) {
                                        case 'P':
                                            p = strtok( NULL, delim );
                                            m_GPS_SerialCommDevice = new char[strlen(p) + 1];
                                            strcpy( m_GPS_SerialCommDevice, p );
                                            printf( "[myTCS::readConfigFilefile] GPS comm port device (pGP): %s\n", m_GPS_SerialCommDevice );
                                            break;
                                        case 'B':
                                            p = strtok( NULL, delim );
                                            m_GPS_SerialCommBaudRate = atoi( p );
                                            printf( "[myTCS::readConfigFilefile] GPS comm port device (pGB): %d\n", m_GPS_SerialCommBaudRate );
                                            break;
                                        default:
                                            printf( "[myTCS::readConfigFilefile] Unknown parameter pG%c\n", p[2] );
                                            break;
                                    }
                                    break;
                                case 't':
                                    switch( p[2] ) {
                                        case 'P':
                                            p = strtok( NULL, delim );
                                            m_Telescope_SerialCommDevice = new char[strlen(p) + 1];
                                            strcpy( m_Telescope_SerialCommDevice, p );
                                            printf( "[myTCS::readConfigFilefile] Telescope comm port device (ptP): %s\n", m_Telescope_SerialCommDevice );
                                            break;
                                        case 'B':
                                            p = strtok( NULL, delim );
                                            m_Telescope_SerialCommBaudRate = atoi( p );
                                            printf( "[myTCS::readConfigFilefile] Telescope comm port device (ptB): %d\n", m_Telescope_SerialCommBaudRate );
                                            break;
                                        default:
                                            printf( "[myTCS::readConfigFilefile] Unknown parameter pt%c\n", p[2] );
                                            break;
                                    }
                                    break;
                            }
                            break;
                        //default:
                        //    printf( "[myTCS::readConfigFilefile] Unknown parameter %c\n", p[0] );
                            
                    }
                }
            } else if( ! feof( fp )  ) {
                perror( "[myTAxis::readConfigFilefile] getline" );
                printf( "[myTAxis::readConfigFilefile] getline ERROR\n" );
            } 
        } while( ! feof( fp ) );
        printf( "[myTCS::readConfigFilefile] closing %s\n", configFileName );
        if( fclose( fp ) == 0 ) {
            printf( "[myTCS::readConfigFilefile] close OK\n" );
        } else {
            perror( "[myTCS::readConfigFilefile] close" );
            printf( "[myTCS::readConfigFilefile] close ERROR\n" );
        }
    } 
    if( line != NULL ) {
        free( line );
    }
    return 0;
}


/**
 *
 */
int myTCS::myLX200CommandInterpreter( char * command, char * retline ) {
    int retval;
    char line[16];
    //char * p;
    //char mem_address;
    //int x;
    double deg_per_sec;
    double degs;
    //const char delim[] = ";#";
    int deg, hrs, min, sec, int_tmp;
    double tmp;

    switch( command[1] ) {
//*****************************************************************************
// Direct text commands to i2c devices
        case '_':
            //printf( "[myTCS::myLX200CommandInterpreter] Text command " );
            printf( "%s\n", & command[2] );
            retval = telescope->write_RS232( & command[2], strlen( & command[2] ) );
            sprintf( retline, "Text command returned with %d", retval );
            break;
//*****************************************************************************
// A (My Way) Commands
        case 'A':
            double tmp_degrees;
            sprintf( retline, "One Star Alignment Command" );

            tmp_degrees = telescope->getTargetHA();
            telescope->alpha->resetPosition( tmp_degrees );
            tmp_degrees = telescope->getLatitude() - telescope->getTargetDec();
            telescope->delta->resetPosition( tmp_degrees );
            break;
//*****************************************************************************
// G Commands
        case 'G':
            switch( command[2] ) {
                case 'M':   //Site Name
                    sprintf( retline, "%s", m_SiteName1 );
                    break;
                case 'N':   //Site Name
                    sprintf( retline, "%s", m_SiteName2 );
                    break;
                case 'O':   //Site Name
                    sprintf( retline, "%s", m_SiteName3 );
                    break;
                case 'P':   //Site Name
                    sprintf( retline, "%s", m_SiteName4 );
                    break;
                case 'A':   //Site Altitude
                    if( command[3] == '#' ) {
                        sprintf( retline, "%7.1lf", telescope->getAltitudeAboveSeeLevel() );
                    }
                    break;
                case 't':   //Site Latitude
                    sprintf( retline, "%s", telescope->getTelescopeLatitude_MeadeFormat( line ) );
                    break;
                case 'g':   //Site Longitude
                    sprintf( retline, "%s", telescope->getTelescopeLongitude_MeadeFormat( line ) );
                    break;
                case 'C':
                    sprintf( retline, "%s", telescope->timer->getLDate_MeadeFormat( line ) );
                    break;
                case 'L':
                    sprintf( retline, "%s", telescope->timer->getLTime( line ) );
                    break;
                case 'S':
                    sprintf( retline, "%s", telescope->timer->getLSTime( line ) );
                    break;
                case 'R':   //Get telescope RA
                    telescope->getRA_MeadeFormat( line );
                    sprintf( retline, "%s", line );
                    //sprintf( retline, "T.RA =%lf %s", 
                    //            telescope->alpha->getPosition( command ),
                    //            telescope->timer->getLTime() );
                    break;
                case 'D':   //Get telescope Dec 
                    if( command[3] == '#' ) {
                        telescope->getDec_MeadeFormat( line );
                        sprintf( retline, "%s", line );
                    } 
                    break;
                case 'V':
                    switch( command[3] ) {
                        case 'D': //Telescope firmware date
                            
                            sprintf( retline, "%s", telescope->getFirmwareDate() );
                            break;
                        case 'T': //Telescope firmware time
                            
                            sprintf( retline, "%s", telescope->getFirmwareTime() );
                            break;
                        case 'N': //Telescope firmware number
                            
                            sprintf( retline, "%s", telescope->getFirmwareNumber() );
                            break;
                        case 'P': //Telescope product name
                            sprintf( retline, "%s", telescope->getName() );
                            break;
                        default:
                            sprintf( retline, "Unknown command GV%c", command[3] );
                    }
                    break;
                default:
                    sprintf( retline, "Unknown command G%c", command[2] );
            }
            break;
//*****************************************************************************
// M Commands
        case 'M':
            switch( command[2] ) {
                case 'A':
                    //m_SlewToTarget_AltAz = true;
                    degs = telescope->getDifferenceHA();
                    telescope->alpha->runMotorToPosition( degs );
                    sprintf( retline, "MA: Slew to To alpha + %lf [degs]", degs );
                    break;
                case 'D':
                    //m_SlewToTarget_AltAz = true;
                    degs = telescope->getDifferenceDec();
                    telescope->delta->runMotorToPosition( degs );
                    sprintf( retline, "MA: Slew to To delta + %lf [degs]", degs );
                    break;
                case 'e':
                    m_MoveToWest = -1;
                    telescope->alpha->setAxisDir( m_MoveToWest );
                    telescope->alpha->runMotor();
                    //printf( "[myTCS::run] Moving to East\n" );
                    sprintf( retline, "Me: Move telescope EAST at current slew rate" );
                    break;
                case 'n':
                    m_MoveToNorth = -1;
                    telescope->delta->setAxisDir( m_MoveToNorth );
                    telescope->delta->runMotor();
                    sprintf( retline, "Mn: Move telescope NORTH at current slew rate" );
                    break;
                case 's':
                    m_MoveToNorth = 1;
                    telescope->delta->setAxisDir( m_MoveToNorth );
                    telescope->delta->runMotor();
                    sprintf( retline, "Ms: Move telescope SOUTH at current slew rate" );
                    break;
                case 'w':
                    m_MoveToWest = 1;
                    telescope->alpha->setAxisDir( m_MoveToWest );
                    telescope->alpha->runMotor();
                    sprintf( retline, "Mw: Move telescope WEST at current slew rate" );
                    break;
                case 'S':
                    m_SlewToTarget = true;
                    m_FineSlewToTarget = false;
                    m_SlewToTarget_AlphaReady = false;
                    m_SlewToTarget_DeltaReady = false;
                    sprintf( retline, "MS: Slew to target" );
                    break;
                case 'F':
                    m_SlewToTarget = false;
                    m_FineSlewToTarget = true;
                    m_SlewToTarget_AlphaReady = false;
                    m_SlewToTarget_DeltaReady = false;
                    sprintf( retline, "MS: Slew to target" );
                    break;
                default:
                    sprintf( retline, "Unknown command M%c", command[2] );
            }
            break;
//*****************************************************************************
// Q Commands
        case 'Q':
            switch( command[2] ) {
                case '#':
                    m_FullStop           = true;
                    m_SlewToTarget_AltAz = false;
                    m_MoveToNorth        = 0;
                    m_MoveToWest         = 0;
                    m_SlewToTarget       = false;
                    m_FineSlewToTarget   = false;
                    telescope->alpha->stopMotor();
                    telescope->delta->stopMotor();
                    sprintf( retline, "Full Stop!" );
                    break;
                case 'e':
                    m_MoveToWest = 0;
                    telescope->alpha->stopMotor();
                    sprintf( retline, "Qe: Halt EASTward slewing!" );
                    break;
                case 'n':
                    m_MoveToNorth = 0;
                    telescope->delta->stopMotor();
                    sprintf( retline, "Qn: Halt NORTHward slewing!" );
                    break;
                case 's':
                    m_MoveToNorth = 0;
                    telescope->delta->stopMotor();
                    sprintf( retline, "Qs: Halt SOUTHward slewing!" );
                    break;
                case 'w':
                    m_MoveToWest = 0;
                    telescope->alpha->stopMotor();
                    sprintf( retline, "Qw: Halt WESTward slewing!" );
                    break;
                case 'S':
                    m_SlewToTarget = false;
                    m_FineSlewToTarget = false;
                    sprintf( retline, "QS: Stop slew to target" );
                    break;
                default:
                    sprintf( retline, "Unknown command Q%c", command[2] );
            }
            break;
//*****************************************************************************
// S Commands
        case 'S':
            switch( command[2] ) {
                case 'A':
                    switch( command[3] ) {
                        case 'S':
                            if( command[4] == 'T' ) {
                                deg_per_sec = 1.0/240.0;
                            } else {
                                //p = strtok( & command[4], "#" );
                                sscanf( & command[4], "%lf#", & deg_per_sec );
                            }
                            retval = telescope->alpha->setAxisDegPerSec( deg_per_sec );
                            sprintf( retline, "SAS: %d", retval );
                            break;
                        default:
                            printf( "Unknown parameter %s\n", & command[2] );
                    }
                    break;
                case 'D':
                    switch( command[3] ) {
                        case 'S':
                            if( command[4] == 'T' ) {
                                deg_per_sec = 1.0/240.0;
                            } else {
                                //p = strtok( & command[4], "#" );
                                sscanf( & command[4], "%lf#", & deg_per_sec );
                            }
                            retval = telescope->delta->setAxisDegPerSec( deg_per_sec );
                            sprintf( retline, "SDS: %d", retval );
                            break;
                        default:
                            printf( "Unknown parameter %s\n", & command[2] );
                    }
                    break;
                case 'd':
                    //p = strtok( & command[3], "#" );
                    sscanf( & command[3], "%d*%d:%d#", & deg, & min, & sec );
                    tmp = fabs( (double) deg ) + ((double) min)/60.0 + ((double) sec)/3600.0;
                    if( deg < 0 ) {
                        tmp *= -1.0;
                    }
                    sprintf( retline, "Sd: %d", telescope->setTargetDec( tmp ) );
                    break;
                case 'r':
                    //p = strtok( & command[3], "#" );
                    sscanf( & command[3], "%d:%d:%d#", & hrs, & min, & sec );
                    tmp = 15.0 * ( fabs( (double) hrs) + ((double) min)/60.0 + ((double) sec)/3600.0 );
                    if( hrs < 0 ) {
                        tmp *= -1.0;
                    }
                    sprintf( retline, "Sr: %d", telescope->setTargetRA( tmp ) );
                    break;
                case 'w':
                    //*********************************************************
                    //Set max slew rate
                    
                    switch( command[3] ) {
                        case 'S':
                            int_tmp = 6;
                            break;
                        case 's':
                            int_tmp = 4;
                            break;
                        case 'G':
                            int_tmp = 2;
                            break;
                        case 'O':
                            int_tmp = 1;
                            break;
                        case '0':
                            int_tmp = 0;
                            break;
                        case '1':
                            int_tmp = 1;
                            break;
                        case '2':
                            int_tmp = 2;
                            break;
                        case '3':
                            int_tmp = 3;
                            break;
                        case '4':
                            int_tmp = 4;
                            break;
                        case '5':
                            int_tmp = 5;
                            break;
                        case '6':
                            int_tmp = 6;
                            break;
                        default:
                            sscanf( & command[3], "%d#", & int_tmp );   
                    }
                    if( int_tmp < 0 || int_tmp > 9 ) {
                        sprintf( retline, "Wrong Max Slew Rate%d", int_tmp );
                    } else {
                        sprintf( retline, "Set Max Slew Rate to %d", int_tmp );
                        telescope->alpha->setMaxSlewRate( int_tmp );
                        telescope->delta->setMaxSlewRate( int_tmp );
                    }
                    break;
                default:
                    sprintf( retline, "Unknown command S%c", command[2] );
             }
            break;
//*****************************************************************************
// T Commands
        case 'T':
            if( command[3] == '#' ) {
                switch( command[2] ) {
                    case '1':
                        telescope->alpha->startTracking( 600 );
                        sprintf( retline, "Starting Tracking at fixed rate" );
                        m_Tracking = true;
                        break;
                    case '0':
                        telescope->alpha->stopTracking( );
                        sprintf( retline, "Stop Tracking" );
                        m_Tracking = false;
                        break;
                    case '+':
                        int_tmp = telescope->alpha->incrementTrackingRate();
                        //telescope->alpha->runMotor();
                        sprintf( retline, "Set Tracking Rate to %d[tics/s]", int_tmp );
                        break;
                    case '-':
                        int_tmp = telescope->alpha->decrementTrackingRate();
                        //telescope->alpha->runMotor();
                        sprintf( retline, "Set Tracking Rate to %d[tics/s]", int_tmp );
                        break;
                }
            } else {
                sscanf( & command[2], "%d#", & int_tmp ); 
                if( 300 < int_tmp && int_tmp < 900 ) {
                    telescope->alpha->startTracking( int_tmp );
                    sprintf( retline, "Set Tracking Rate to %d[tics/s]", int_tmp );
                } else {
                    sprintf( retline, "Invalid Tracking Rate %d[tics/s]", int_tmp );
                }
    
            }
            break;
//*****************************************************************************
// Z (My) Commands
        case 'Z':   //TCS commands
            switch( command[2] ) {
                case 'X':   //Exit TCS
                    m_tcs_is_runnig = false;
                    //sprintf( retline, "T.PrN=ts7800" );
                    //printf( "[myTCS::run] Exiting...\n" );                                        
                    sprintf( retline, "Exiting..." );
                    break;
                default:
                    sprintf( retline, "Unknown command Z%c", command[2] );
            }
            break;
        default:
            sprintf( retline, "Unknown command %c", command[1] );
    }
    //printf( "[myTCS::myLX200CommandInterpreter] %s\n", retline );
    return strlen( retline );
}

int stream_status( int m_port ) {
    fd_set fds;
    struct timeval tv;
    int retval;

    FD_ZERO( & fds );
    FD_SET( m_port, & fds );
    tv.tv_sec = 0;
    tv.tv_usec = 100;

    if( ( retval = select( m_port + 1, & fds, NULL, NULL, & tv ) ) < 0  ) {
        printf( "Error on select()\n" );
    }
    return retval;
}

/**
 * @brief Telescope Control System min loop.
 *
 */
int myTCS::run( void ) {
    int tmr = 0;
    int retval;
    
    int rx_bytes;   
    char line[128];
    char retline[256];
    
    int std_command_bytes = 0;
    char std_command[32];
    int tcp_command_bytes = 0;
    char tcp_command[32];
    char tcp_retline[32];

    int handset_is_connected = false;
    int tty_command_bytes = 0;
    char tty_command[32];
    
    
    
    double d_alpha, d_delta, tmp_degs;
    double degs_per_s = 0;
    double old_degs_per_s = 0;
    

    int seconds = 60;
    
    int fd_stdin = fileno( stdin );
    
    int telescope_startup   = false;
    //int telescope_firmware  = false;
    int tcp_startup_flag = false;
    
    printf( "[myTCS::run] Hello world\n" );
    printf( "[myTCS::run] PID = %d (%u)\n", (int) getpid(), (int) pthread_self() );

    if( handset->open_RS232() > 0 ) {
        handset_is_connected = true;
        printf( "[myTCS::run] Handset (%s) is open\n", handset->getDeviceName() );
    } else {
        handset_is_connected = false;
        printf( "[myTCS::run] Handset Can't open %s\n", handset->getDeviceName() );
    }
    
    //if( thesky->open_RS232() > 0 ) {
    //    thesky_is_connected = true;
    //    printf( "[myTCS::run] TheSky %s is open\n", thesky->getDeviceName() );
    //} else {
    //    thesky_is_connected = false;
    //    printf( "[myTCS::run] TheSky Can't open %s\n", thesky->getDeviceName() );
    //}
        
    m_tcs_is_runnig = true;
    while( m_tcs_is_runnig ) {

        
        //*********************************************************************
        // Standard input
        if( ( retval = stream_status( fd_stdin ) ) > 0 ) {
            memset( line, 0 , 128 );
            rx_bytes = read( fd_stdin, line, 128 );
            rx_bytes --;
            line[rx_bytes] = 0;  //replace enter -> 0
            fflush( stdin );
            //printf( "[myTCS::run] new line from stdin: %s\n", line );
            for( int i = 0; i < rx_bytes; i ++ ) {
                if( std_command_bytes == 0 ) {
                    if( line[i] == ':' ) {
                        //printf( "[myTCS::run] starting new command from stdin: %c\n", line[i] );
                        std_command[std_command_bytes] = line[i];
                        std_command_bytes ++;
                    }
                } else {
                    //printf( "[myTCS::run] adding to new command from stdin: %c\n", line[i] );
                    std_command[std_command_bytes] = line[i];
                    std_command_bytes ++;
                    if( line[i] == '#' ) {
                        std_command[std_command_bytes] = 0;
                        //for( int j = 0; j < std_command_bytes; j ++ ){
                        //    printf( "%c", std_command[j] );
                        //}
                        //printf( "\n" );
                        std_command_bytes = 0;
                        //printf( "[myTCS::run] new command from stdin: %s\n", std_command );
                        
                        if( myLX200CommandInterpreter( std_command, retline ) > 0 ) {
                            printf( "[myTCS::run] command from stdin returned with: %s\n", retline );
                        }
                    }
                }
            }
            //new_message_flag = true;
        }
        //*********************************************************************
        // TCP/IP
        if( tcp->getIsConnected() ) {
            if( ( retval = tcp->clientSocketStatus() ) > 0 ) {
                memset( line, 0, 128 );
                rx_bytes = tcp->readSocket( line, 128 );
                for( int i = 0; i < rx_bytes; i ++ ) {
                    if( tcp_command_bytes == 0 ) {
                        if( line[i] == ':' ) {
                            tcp_command[tcp_command_bytes] = line[i];
                            tcp_command_bytes ++;
                        }
                    } else {
                        tcp_command[tcp_command_bytes] = line[i];
                        tcp_command_bytes ++;
                        if( line[i] == '#' ) {
                            tcp_command[tcp_command_bytes] = 0;
                            tcp_command_bytes = 0;
                            //printf( "[myTCS::run] new tcp command %s\n", tcp_command );
                            
                            if( myLX200CommandInterpreter( tcp_command, retline ) > 0 ) {
                                //printf( "[myTCS::run] writing to socket: %s\n", retline );
                                sprintf( tcp_retline, "! %s#", retline );
                                tcp->writeSocket( tcp_retline, strlen( tcp_retline ) );
                            }
                        }
                    }
                }
            }
        }
        //*********************************************************************
        // Handset
        if( handset_is_connected ) {
            if( ( retval = handset->status_RS232() ) > 0 ) {
                memset( line, 0, 128 );
                rx_bytes = handset->read_RS232( line, 128 );
                for( int i = 0; i < rx_bytes; i ++ ) {
                    if( tty_command_bytes == 0 ) {
                        if( line[i] == ':' ) {
                            tty_command[tty_command_bytes] = line[i];
                            tty_command_bytes ++;
                        }
                    } else {
                        tty_command[tty_command_bytes] = line[i];
                        tty_command_bytes ++;
                        if( line[i] == '#' ) {
                            tty_command[tty_command_bytes] = 0;
                            tty_command_bytes = 0;
                            printf( "[myTCS::run] new tty command %s\n", tty_command );
                            
                            if( myLX200CommandInterpreter( tty_command, retline ) > 0 ) {
                                //tty->writeSocket( retline, strlen( retline ) );
                            }
                        }
                    }
                }
            }
        }        


        //*********************************************************************
        // Telescope
        if( (retval = telescope->receiveRS232Data()) > 0 ) {
            //printf( "[myTCS::run] telescope updated\n" );
        }
        
        
        //*****************************************************************
        // TIMER
        if( telescope->timer->getIsTimerTic() ) {
            if( tmr % 4 == 0 ) {      
                if( seconds == 60 ) {
                    printf( "[myTCS::run] LT= %s\n", telescope->timer->getLTime( retline ) );
                    seconds = 0;
                } else {
                    seconds ++;
                }
                if( ! telescope_startup ) {
                    /*
                    if( telescope->alpha->get_fd() == 0 && telescope->delta->get_fd() == 0  ) {
                        if( telescope->getIsConnected() ) {
                            telescope->setAxesComm();
                        }
                    } else  if( ! telescope_firmware ) {
                        if( telescope->getFirmwareDate() == NULL ) {
                            telescope->updateFirmwareDate();
                        } else if( telescope->getFirmwareTime() == NULL ) {
                            telescope->updateFirmwareTime();
                        } else if( telescope->getFirmwareNumber() == NULL ) {
                            telescope->updateFirmwareNumber();
                        } else {
                            telescope_firmware = true;
                        }
                    } else {
                        telescope->setTargetRA( telescope->timer->getLST() );
                        telescope->setTargetDec( telescope->getLatitude() );

                        printf( "[myTCS::run] telescope startup OK\n" );
                        printf( "[myTCS::run] LT= %s\n", telescope->timer->getLTime( retline ) );
                        telescope_startup = true;    
                    }
                    */
                    telescope_startup = true;
                } else {
                    telescope->updatePosition();
                    
                    //*************************************************************
                    //  Check Alpha
                    //telescope->alpha->updatePosition( 0 );
                    if( ! telescope->checkAlphaPosition() ) {
                        printf( "[myTCS::run] Warning Alpha exceeding limits!\n" );
                    }
                  
                    //*************************************************************
                    //  Check delta
                    //telescope->delta->updatePosition( 0 );
                    if( ! telescope->checkDeltaPosition() ) {
                        printf( "[myTCS::run] Warning Delta exceeding limits!\n" );
                    }

                    //*************************************************************
                    //  Full Stop
                    if( m_FullStop ) {
                        m_FullStop = false;
                        telescope->alpha->stopMotor();
                        telescope->delta->stopMotor();
                    }
                    
                    //*************************************************************
                    //  Slew to target
                    if( m_SlewToTarget ) {
                        
                        if( ! m_SlewToTarget_AlphaReady  ) {
                            d_alpha = telescope->getDifferenceRA();
                            if( d_alpha  < -1.0 ) {
                                if( ! m_SlewingToAlphaTarget ) {
                                    telescope->alpha->setMaxSlewRate( 6 );
                                    telescope->alpha->setAxisDir( 1 );
                                    telescope->alpha->runMotor();
                                    m_SlewingToAlphaTarget = true;
                                    printf( "[myTCS::run] SlewingToAlphaTarget Dir +1, d_alpha = %lf\n", d_alpha );
                                }
                            } else if( d_alpha < 1.0 ) {
                                if( m_SlewingToAlphaTarget ) {
                                    telescope->alpha->stopMotor();
                                    m_SlewingToAlphaTarget = false;
                                    printf( "[myTCS::run] Stop SlewingToAlphaTarget d_alpha = %lf\n", d_alpha );
                                } else {
                                    m_SlewToTarget_AlphaReady = true;
                                    printf( "[myTCS::run] SlewToTarget_AlphaReady\n" );
                                }
                                
                            } else {
                                if( ! m_SlewingToAlphaTarget ) {
                                    telescope->alpha->setMaxSlewRate( 6 );
                                    telescope->alpha->setAxisDir( -1 );
                                    telescope->alpha->runMotor();
                                    m_SlewingToAlphaTarget = true;
                                    printf( "[myTCS::run] SlewingToAlphaTarget Dir -1, d_alpha = %lf\n", d_alpha );
                                }
                            }
                        } else if( ! m_SlewToTarget_DeltaReady ) {
                            d_delta = telescope->getDifferenceDec();
                            if( d_delta  < -1.0 ) {
                                if( ! m_SlewingToDeltaTarget ) {
                                    telescope->delta->setMaxSlewRate( 6 );
                                    telescope->delta->setAxisDir( 1 );
                                    telescope->delta->runMotor();
                                    m_SlewingToDeltaTarget = true;
                                    printf( "[myTCS::run] SlewingToDeltaTarget Dir -1, d_delta = %lf\n", d_delta );
                                }
                            } else if( d_delta < 1.0 ) {
                                if( m_SlewingToDeltaTarget ) {
                                    telescope->delta->stopMotor();
                                    m_SlewingToDeltaTarget = false;
                                    printf( "[myTCS::run] Stop SlewingToDeltaTarget d_delta = %lf\n", d_delta );
                                } else {
                                    m_SlewToTarget_DeltaReady = true;
                                    printf( "[myTCS::run] SlewToTarget_DeltaRead\n" );
                                }
                            } else {
                                if( ! m_SlewingToDeltaTarget ) {
                                    telescope->delta->setMaxSlewRate( 6 );
                                    telescope->delta->setAxisDir( -1 );
                                    telescope->delta->runMotor();
                                    m_SlewingToDeltaTarget = true;
                                    printf( "[myTCS::run] SlewingToDeltaTarget Dir +1, d_delta = %lf\n", d_delta );
                                }
                            }                        
                        } else {
                            printf( "[myTCS::run] End Slewing To Target\n" );
                            m_SlewToTarget = false;
                            m_FineSlewToTarget = true;
                            degs_per_s = 0.0333;
                            m_SlewToTarget_AlphaReady = false;
                            m_SlewToTarget_DeltaReady = false;
                            
                        }
                    } else if( m_FineSlewToTarget ) { // else of if( m_SlewToTarget )  
                        int my_dir = 0;
                        
                        if( ! m_SlewToTarget_AlphaReady ) {
                            d_alpha = telescope->getDifferenceRA();
                            if( degs_per_s > 0.0043 ) {
                                tmp_degs = d_alpha / degs_per_s;
                                //printf( "[myTCS::run] FineSlewingToAlphaTarget d_alpha = %lf\n", d_alpha );
                                printf( "[myTCS::run] FineSlewingToAlphaTarget sec to target = %lf[s]\n", tmp_degs );
                                if( (tmp_degs < -2)  ) {
                                    my_dir = 1;
                                } else if( (tmp_degs < -1) && (-1 < tmp_degs / 2.0)  ) {
                                    degs_per_s /= 2.0;
                                    my_dir = 1;
                                } else if( (tmp_degs > 1) && (1>tmp_degs / 2.0)  ) {
                                    degs_per_s /= 2.0;
                                    my_dir = -1;                                    
                                } else if ( (tmp_degs > 2)   ){
                                    my_dir = -1;                               
                                }
                                if( degs_per_s * my_dir != old_degs_per_s ) {
                                    printf( "[myTCS::run] FineSlewingToAlphaTarget New deg_per_s = %lf (%lf)\n", degs_per_s * my_dir, old_degs_per_s );
                                    old_degs_per_s = degs_per_s * my_dir;
                                    telescope->alpha->setAxisDegPerSec( degs_per_s );
                                    telescope->alpha->setAxisDir( my_dir );
                                    telescope->alpha->runMotor();                               
                                }
                            } else {
                                telescope->alpha->stopMotor();
                                m_SlewToTarget_AlphaReady = true;
                                degs_per_s = 1;
                                old_degs_per_s = 0;
                                my_dir = 0;
                            }
                        } else if( ! m_SlewToTarget_DeltaReady ) {
                            d_delta = telescope->getDifferenceDec();
                            if( degs_per_s > 0.03125 ) {
                                tmp_degs = d_delta / degs_per_s;
                                //printf( "[myTCS::run] FineSlewingToDeltaTarget d_delta = %lf\n", d_delta );
                                printf( "[myTCS::run] FineSlewingToDeltaTarget sec to target = %lf[s]\n", tmp_degs );
                                if( (tmp_degs < -2) ){
                                    my_dir = 1;
                                } else if( (tmp_degs < -1) && (-1 < tmp_degs / 2.0) ) {
                                    degs_per_s /= 2.0;
                                    my_dir = 1;
                                } else if( (tmp_degs > 1) && ( 1>tmp_degs / 2.0) ) {
                                    degs_per_s /= 2.0;
                                    my_dir = -1;
                                } else if ( (tmp_degs > 2)  ){
                                    my_dir = -1;
                                }
                                if( degs_per_s * my_dir != old_degs_per_s ) {
                                    printf( "[myTCS::run] FineSlewingToDeltaTarget New deg_per_s = %lf (%lf)\n", degs_per_s * my_dir, old_degs_per_s );
                                    old_degs_per_s = degs_per_s * my_dir;
                                    telescope->delta->setAxisDegPerSec( degs_per_s );
                                    telescope->delta->setAxisDir( my_dir );
                                    telescope->delta->runMotor();     
                                }
                            } else {
                                telescope->delta->stopMotor();
                                m_SlewToTarget_DeltaReady = true;
                            }
                        } else {
                            m_SlewToTarget = false;
                            m_FineSlewToTarget = false;

                            m_SlewToTarget_AlphaReady = false;
                            m_SlewToTarget_DeltaReady = false;        
                            printf( "[myTCS::run] End Fine Slewing To Target\n" );
                        }
                        //d_delta = telescope->getDifferenceDec();
                    }  // end of if( m_SlewToTarget )     
                } //end of (if( ! telescope_startup )
                
                if( tcp->getIsConnected() ) {
                    if( ! tcp_startup_flag ) {
                        tcp_startup_flag = true;
                        telescope->updateSiteData( retline );
                        tcp->writeSocket( retline, strlen(retline) );
                    } else {
                        telescope->timer->updateTimeData( retline );
                        tcp->writeSocket( retline, strlen(retline) );

                        telescope->updatePositionData( retline );
                        tcp->writeSocket( retline, strlen(retline) );

                        telescope->updateEncodersData( retline );
                        tcp->writeSocket( retline, strlen(retline) );
                    }
                } else if( tcp_startup_flag ) {
                    tcp_startup_flag = false;
                }
            } //end of if( tmr % 4 == 0 ) 
            
            

                            
            tmr ++;
        } // end of if( telescope->timer->getIsTimerTic() ) 
       
    }
    
    if( handset_is_connected ) {
        handset->close_RS232();
    }

    //if( thesky_is_connected ) {
    //    thesky->close_RS232();
    //}

    
    printf( "[myTCS::run] Good bye!\n" );

    return 0;
}



