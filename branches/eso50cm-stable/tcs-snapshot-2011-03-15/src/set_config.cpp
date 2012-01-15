/**
 * Compile using:
 * g++ -Wall -g -fPIC -shared -I../include -lm
 *      ../obj/myLCU.o
 *      ../obj/myTelescope.o
 *      ../obj/myTClock.o
 *      ../obj/myTAxis.o
 *      ../obj/myTMotor.o
 *      ../obj/myTEncoder.o
 *      ../obj/myBSemaphore.o
 *      -o example1/lcu_test.so lcu_test.cpp
 *
 */
#include <stdio.h>

#include "myLCU.h"
#include <sys/uio.h>


/** HTML source for the start of the process listing page.  */
static char * page_start = 
    "<html>\n" 
    "<head>\n"
    "<title>myTelescope</title>"
    "</head>\n"
    " <body>\n"
    "  <h2 align=\"left\">%s</h2>\n"
    "  <pre>\n";


/** HTML source for the end of the process listing page.  */
static char * page_end = 
    "  </pre>\n"
    " </body>\n"
    "</html>\n";



extern "C" void module_generate( int fd, const char * arguments, myLCU * lcu )
{
    extern int verbose;
    int  retval;
    char string_data[32];
    char html_output_flag = 1;

    if( verbose ) printf( "[set_config] Hello world!\n" );

    /** Checks for arguments */
    if( * arguments != 0 ) {
        if( verbose ) printf( "[set_config] Has arguments: %s\n", arguments );
        char * ptr = NULL;

        if( (ptr = strstr( arguments, "txt" )) != NULL ) {
            html_output_flag = 0;
            if( verbose ) printf( "[set_config] No HTML output [%s]\n", ptr );
        }
    }

    /** Create a stream corresponding to the client socket file
        descriptor.  */
    if( verbose) printf( "[set_config] My file descriptor is %d\n", fd );

    char buffer[1024];
    int  buf_len = 0;

    if( html_output_flag ) {
        buf_len += sprintf( buffer, page_start, "Set Configuration (set_config)" );
    }

    lcu->waitSemaphore();
    if( lcu->telescope->getIsConfigured() == 0 ) {
        buf_len += sprintf( & buffer[buf_len], "Loading configuration...\n\n" );
    } else {
        buf_len += sprintf( & buffer[buf_len], "Reloading configuration...\n\n" );
    }

    string line;
    string keyword;
    string variable;
    string comment;
    int i, j;

    ifstream config_file( "ESO50cm.conf" );
    if( config_file.is_open() ) {
        while( ! config_file.eof() ) {
            getline( config_file, line );
            if( ! line.empty() ) {
        //cout << line << "\n";
                if( line[0] != '#' ) {
                    i = line.find_first_of( " ", 0 );
                    keyword = line.substr( 0, i );
                    j = line.find_first_not_of( " ", i );
                    i = line.find_first_of( " ", j );
                    variable = line.substr( j, i-j );
                    j = line.find_first_not_of( " ", i );
                    i = line.length();
                    comment = line.substr( j, j-i );

            //cout << "[myLCU::initializeTelescope] " << keyword << " ";
            //cout << variable << "    ";
            //cout << comment << "\n";
                    switch( keyword[0] ) {
                        int deg, min, sec;
                        double tmp;
                        case 'g':
                            sscanf( variable.c_str(), "%d*%d:%d", & deg, & min, & sec );
                            tmp = fabs( (double) deg) + ((double) min)/60.0 + ((double) sec)/3600.0;
                            if( deg < 0 ) {
                                tmp *= -1.0;
                            }
                    //m_lcu_data->telescope_data.Longitude = tmp;
                            lcu->telescope->setLongitude( tmp );
                            lcu->telescope->setClockLongitude( tmp );
                            break;
                        case 't':
                            sscanf( variable.c_str(), "%d*%d:%d", & deg, & min, & sec );
                            tmp = fabs( (double) deg) + ((double) min)/60.0 + ((double) sec)/3600.0;
                            if( deg < 0 ) {
                                tmp *= -1.0;
                            }
                            lcu->telescope->setLatitude( tmp );
                            break;
                        case 'H':
                            tmp = atof( variable.c_str() );
                            lcu->telescope->setAltitude( tmp );
                            break;
                        case 'o':
                            tmp = atof( variable.c_str() );
                            lcu->telescope->setLowElevation( tmp );
                            break;
                        case 'h':
                            tmp = atof( variable.c_str() );
                            lcu->telescope->setHighElevation( tmp );
                            break;
                        case 'A':
                            switch( keyword[1] ) {
                                case 'M':
                                    switch( keyword[2] ) {
                                        case 'R':
                                            tmp = atof( variable.c_str() );
                                            lcu->telescope->alpha->Motor->setEncoderToAxis_Reduction( tmp );
                                            break;
                                        case 'T':
                                            tmp = atof( variable.c_str() );
                                            lcu->telescope->alpha->Motor->setTicsPerRev( tmp );
                                            break;
                                        case 'H':
                                            tmp = atof( variable.c_str() );
                                            lcu->telescope->alpha->Motor->setHomePosition( tmp );
                                            break;
                                        default:
                                            cout << variable << "   " << comment << "\n";
                                    }
                                    break;
                                case 'W':
                                    switch( keyword[2] ) {
                                        case 'R':
                                            tmp = atof( variable.c_str() );
                                            lcu->telescope->alpha->WormE->setEncoderToAxis_Reduction( tmp );
                                            break;
                                        case 'T':
                                            tmp = atof( variable.c_str() );
                                            lcu->telescope->alpha->WormE->setTicsPerRev( tmp );
                                            break;
                                        case 'H':
                                            tmp = atof( variable.c_str() );
                                            lcu->telescope->alpha->WormE->setHomePosition( tmp );
                                            break;
                                        default:
                                            cout << variable << "   " << comment << "\n";
                                    }
                                    break;
                                case 'A':
                                    switch( keyword[2] ) {
                                        case 'R':
                                            tmp = atof( variable.c_str() );
                                            lcu->telescope->alpha->AxisE->setEncoderToAxis_Reduction( tmp );
                                            break;
                                        case 'T':
                                            tmp = atof( variable.c_str() );
                                            lcu->telescope->alpha->AxisE->setTicsPerRev( tmp );
                                            break;
                                        case 'H':
                                            tmp = atof( variable.c_str() );
                                            lcu->telescope->alpha->AxisE->setHomePosition( tmp );
                                            break;
                                        default:
                                            cout << variable << "   " << comment << "\n";
                                    }
                                    break;
                            }
                            break;
                        case 'D':
                            switch( keyword[1] ) {
                                case 'M':
                                    switch( keyword[2] ) {
                                        case 'R':
                                            tmp = atof( variable.c_str() );
                                            lcu->telescope->delta->Motor->setEncoderToAxis_Reduction( tmp );
                                            break;
                                        case 'T':
                                            tmp = atof( variable.c_str() );
                                            lcu->telescope->delta->Motor->setTicsPerRev( tmp );
                                            break;
                                        case 'H':
                                            tmp = atof( variable.c_str() );
                                            lcu->telescope->delta->Motor->setHomePosition( tmp );
                                            break;
                                        default:
                                            cout <<  variable << "   " << comment << "\n";
                                    }
                                    break;
                                case 'W':
                                    switch( keyword[2] ) {
                                        case 'R':
                                            tmp = atof( variable.c_str() );
                                            lcu->telescope->delta->WormE->setEncoderToAxis_Reduction( tmp );
                                            break;
                                        case 'T':
                                            tmp = atof( variable.c_str() );
                                            lcu->telescope->delta->WormE->setTicsPerRev( tmp );
                                            break;
                                        case 'H':
                                            tmp = atof( variable.c_str() );
                                            lcu->telescope->delta->WormE->setHomePosition( tmp );
                                            break;
                                        default:
                                            cout <<  variable << "   " << comment << "\n";
                                    }
                                    break;
                                case 'A':
                                    switch( keyword[2] ) {
                                        case 'R':
                                            tmp = atof( variable.c_str() );
                                            lcu->telescope->delta->AxisE->setEncoderToAxis_Reduction( tmp );
                                            break;
                                        case 'T':
                                            tmp = atof( variable.c_str() );
                                            lcu->telescope->delta->AxisE->setTicsPerRev( tmp );
                                            break;
                                        case 'H':
                                            tmp = atof( variable.c_str() );
                                            lcu->telescope->delta->AxisE->setHomePosition( tmp );
                                            break;
                                        default:
                                            cout << "[set_config] " << variable << "   " << comment << "\n";
                                    }
                                    break;
                            }
                            break;
                        default:
                            cout << "[set_config] " << line << "\n";
                    }
                } else {
                    cout << "[set_config] " << line << "\n";
                }
            }
        }
        lcu->telescope->setIsConfigured( true );
        lcu->telescope->setIsRunningMonitor( 0 );
        lcu->telescope->setIsRunningGoto( 0 );
        lcu->telescope->setIsTracking( 0 );
        config_file.close();
    } else {
        if( verbose ) printf( "[set_config] Unable to open Configuration File!\n" );
    }

    //buf_len += sprintf( & buffer[buf_len], "Configuration loaded.\n\n" );

    //buf_len += sprintf( & buffer[buf_len], "Is Config.=      %d\n", (int) lcu->telescope->getIsConfigured() );

    buf_len += sprintf( & buffer[buf_len], "Lat       = %+11.4lf \n", lcu->telescope->getLatitude() );

    buf_len += sprintf( & buffer[buf_len], "Lng       = %+11.4lf \n", lcu->telescope->getLongitude() );

    buf_len += sprintf( & buffer[buf_len], "Alt       = %+11.4lf \n", lcu->telescope->getAltitude() );


    buf_len += sprintf( & buffer[buf_len], "AMT       = %+11.4lf \n", lcu->telescope->alpha->Motor->getTicsPerRev() );

    buf_len += sprintf( & buffer[buf_len], "AMH       = %+11.4lf \n", lcu->telescope->alpha->Motor->getHomePosition() );

    buf_len += sprintf( & buffer[buf_len], "AMR       = %+11.4lf \n", lcu->telescope->alpha->Motor->getEncoderToAxis_Reduction() );

    buf_len += sprintf( & buffer[buf_len], "AWT       = %+11.4lf \n", lcu->telescope->alpha->WormE->getTicsPerRev() );

    buf_len += sprintf( & buffer[buf_len], "AWH       = %+11.4lf \n", lcu->telescope->alpha->WormE->getHomePosition() );

    buf_len += sprintf( & buffer[buf_len], "AWR       = %+11.4lf \n", lcu->telescope->alpha->WormE->getEncoderToAxis_Reduction() );

    buf_len += sprintf( & buffer[buf_len], "AAT       = %+11.4lf \n", lcu->telescope->alpha->AxisE->getTicsPerRev() );

    buf_len += sprintf( & buffer[buf_len], "AAH       = %+11.4lf \n", lcu->telescope->alpha->AxisE->getHomePosition() );

    buf_len += sprintf( & buffer[buf_len], "AAR       = %+11.4lf \n", lcu->telescope->alpha->AxisE->getEncoderToAxis_Reduction() );


    buf_len += sprintf( & buffer[buf_len], "DMT       = %+11.4lf \n", lcu->telescope->delta->Motor->getTicsPerRev() );

    buf_len += sprintf( & buffer[buf_len], "DMH       = %+11.4lf \n", lcu->telescope->delta->Motor->getHomePosition() );

    buf_len += sprintf( & buffer[buf_len], "DMR       = %+11.4lf \n", lcu->telescope->delta->Motor->getEncoderToAxis_Reduction() );

    buf_len += sprintf( & buffer[buf_len], "DWT       = %+11.4lf \n", lcu->telescope->delta->WormE->getTicsPerRev() );

    buf_len += sprintf( & buffer[buf_len], "DWH       = %+11.4lf \n", lcu->telescope->delta->WormE->getHomePosition() );

    buf_len += sprintf( & buffer[buf_len], "DWR       = %+11.4lf \n", lcu->telescope->delta->WormE->getEncoderToAxis_Reduction() );

    buf_len += sprintf( & buffer[buf_len], "DAT       = %+11.4lf \n", lcu->telescope->delta->AxisE->getTicsPerRev() );

    buf_len += sprintf( & buffer[buf_len], "DAH       = %+11.4lf \n", lcu->telescope->delta->AxisE->getHomePosition() );

    buf_len += sprintf( & buffer[buf_len], "DAR       = %+11.4lf \n", lcu->telescope->delta->AxisE->getEncoderToAxis_Reduction() );

    lcu->postSemaphore();

    /** LCU Local Time */
    buf_len += sprintf( & buffer[buf_len], "--------------------------------------------------\n" );

    strftime( string_data, 32, "%Y-%m-%d, %T", lcu->getLCU_Time() );
    buf_len += sprintf( & buffer[buf_len], "Page generated at: [%s]\n", string_data );


    if( html_output_flag ) {
        buf_len += sprintf( & buffer[buf_len], page_end );
    } else {
        buf_len += sprintf( & buffer[buf_len], "\r\n\r\n" );
    }

    retval = write( fd, buffer, buf_len );
    if( retval > 0 ) {
        if( verbose ) printf( "[set_config] %d Bytes written in fd=%d \n", retval, fd );
    } else {
        perror( "[set_config] write" );
        if( verbose ) printf( "[set_config] write ERROR\n" );
    }

    if( verbose ) printf( "[set_config] Good bye!\n" );
}

