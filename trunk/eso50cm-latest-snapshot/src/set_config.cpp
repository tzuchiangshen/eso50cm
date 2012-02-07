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
static char* page_start = 
    "<html>\n" 
    "<head>\n"
    "<title>myTelescope</title>"
    "</head>\n"
    " <body>\n"
    "  <h2 align=\"left\">Set Configuration</h2>\n"
    "  <pre>\n";


/** HTML source for the end of the process listing page.  */
static char* page_end = 
    "  </pre>\n"
    " </body>\n"
    "</html>\n";


extern "C" void module_generate ( int fd, const char * arguments, myLCU * lcu )
{
    extern int verbose;
    char string_data[32];
    char buffer[128];

    if( verbose ) {
        printf( "[set_config] Hello world!\n" );
        if( * arguments != 0 ) {
            printf( "[set_config] %s\n", arguments );
        }
    }

    if( verbose) printf( "[set_config] My file descriptor is %d\n", fd );
    /** Create a stream corresponding to the client socket file
        descriptor.  */

    write( fd, page_start, strlen( page_start ) );

    lcu->waitSemaphore();

    if( lcu->telescope->getIsConfigured() == 0 ) {
        sprintf( buffer, "Loading configuration\n\n" );
        write( fd, buffer, strlen( buffer ) );

        {
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
                config_file.close();
            } else {
                if( verbose ) printf( "[set_config] Unable to open Configuration File!\n" );
            }


        }

        //lcu->configureTelescope();
    } else {
        sprintf( buffer, "Configuration is loaded\n\n" );
        write( fd, buffer, strlen( buffer ) );
    }

    sprintf( buffer, "Is Config.=      %d\n", (int) lcu->telescope->getIsConfigured() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "Lat       = %+11.4lf \n", lcu->telescope->getLatitude() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "Lng       = %+11.4lf \n", lcu->telescope->getLongitude() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "Alt       = %+11.4lf \n", lcu->telescope->getAltitude() );
    write( fd, buffer, strlen( buffer ) );

    sprintf( buffer, "AMT       = %+11.4lf \n", lcu->telescope->alpha->Motor->getTicsPerRev() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "AMH       = %+11.4lf \n", lcu->telescope->alpha->Motor->getHomePosition() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "AMR       = %+11.4lf \n", lcu->telescope->alpha->Motor->getEncoderToAxis_Reduction() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "AWT       = %+11.4lf \n", lcu->telescope->alpha->WormE->getTicsPerRev() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "AWH       = %+11.4lf \n", lcu->telescope->alpha->WormE->getHomePosition() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "AWR       = %+11.4lf \n", lcu->telescope->alpha->WormE->getEncoderToAxis_Reduction() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "AAT       = %+11.4lf \n", lcu->telescope->alpha->AxisE->getTicsPerRev() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "AAH       = %+11.4lf \n", lcu->telescope->alpha->AxisE->getHomePosition() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "AAR       = %+11.4lf \n", lcu->telescope->alpha->AxisE->getEncoderToAxis_Reduction() );
    write( fd, buffer, strlen( buffer ) );

    sprintf( buffer, "DMT       = %+11.4lf \n", lcu->telescope->delta->Motor->getTicsPerRev() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "DMH       = %+11.4lf \n", lcu->telescope->delta->Motor->getHomePosition() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "DMR       = %+11.4lf \n", lcu->telescope->delta->Motor->getEncoderToAxis_Reduction() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "DWT       = %+11.4lf \n", lcu->telescope->delta->WormE->getTicsPerRev() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "DWH       = %+11.4lf \n", lcu->telescope->delta->WormE->getHomePosition() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "DWR       = %+11.4lf \n", lcu->telescope->delta->WormE->getEncoderToAxis_Reduction() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "DAT       = %+11.4lf \n", lcu->telescope->delta->AxisE->getTicsPerRev() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "DAH       = %+11.4lf \n", lcu->telescope->delta->AxisE->getHomePosition() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "DAR       = %+11.4lf \n", lcu->telescope->delta->AxisE->getEncoderToAxis_Reduction() );
    write( fd, buffer, strlen( buffer ) );
    lcu->postSemaphore();

    /** LCU Local Time */
    sprintf( buffer, "--------------------------------------------------\n" );
    write( fd, buffer, strlen( buffer ) );
    strftime( string_data, 32, "%Y-%m-%d, %T", lcu->getLCU_Time() );
    sprintf( buffer, "Information generated at: [%s]\n", string_data );
    write( fd, buffer, strlen( buffer ) );

    write( fd, page_end, strlen( page_end ) );

    if( verbose) printf( "[set_config] Good bye!\n" );
}

extern "C" void module_generate_txt( int fd, const char * arguments, myLCU * lcu )
{
    extern int verbose;
    char string_data[32];
    char buffer[128];

    if( verbose ) {
        printf( "[set_config] Hello world!\n" );
        if( * arguments != 0 ) {
            printf( "[set_config] %s\n", arguments );
        }
    }

    if( verbose) printf( "[set_config] My file descriptor is %d\n", fd );

    /** Create a stream corresponding to the client socket file
        descriptor.  */
    //sprintf( buffer, "\r\n\r\n" );
    //write( fd, buffer, strlen( buffer ) );

    lcu->waitSemaphore();
    if( lcu->telescope->getIsConfigured() == 0 ) {
        sprintf( buffer, "Loading configuration...\n\n" );
        write( fd, buffer, strlen( buffer ) );
    } else {
        sprintf( buffer, "Reloading configuration...\n\n" );
        write( fd, buffer, strlen( buffer ) );
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




    sprintf( buffer, "Configuration loaded.\n\n" );
    write( fd, buffer, strlen( buffer ) );

    sprintf( buffer, "Is Config.=      %d\n", (int) lcu->telescope->getIsConfigured() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "Lat       = %+11.4lf \n", lcu->telescope->getLatitude() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "Lng       = %+11.4lf \n", lcu->telescope->getLongitude() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "Alt       = %+11.4lf \n", lcu->telescope->getAltitude() );
    write( fd, buffer, strlen( buffer ) );

    sprintf( buffer, "AMT       = %+11.4lf \n", lcu->telescope->alpha->Motor->getTicsPerRev() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "AMH       = %+11.4lf \n", lcu->telescope->alpha->Motor->getHomePosition() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "AMR       = %+11.4lf \n", lcu->telescope->alpha->Motor->getEncoderToAxis_Reduction() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "AWT       = %+11.4lf \n", lcu->telescope->alpha->WormE->getTicsPerRev() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "AWH       = %+11.4lf \n", lcu->telescope->alpha->WormE->getHomePosition() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "AWR       = %+11.4lf \n", lcu->telescope->alpha->WormE->getEncoderToAxis_Reduction() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "AAT       = %+11.4lf \n", lcu->telescope->alpha->AxisE->getTicsPerRev() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "AAH       = %+11.4lf \n", lcu->telescope->alpha->AxisE->getHomePosition() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "AAR       = %+11.4lf \n", lcu->telescope->alpha->AxisE->getEncoderToAxis_Reduction() );
    write( fd, buffer, strlen( buffer ) );

    sprintf( buffer, "DMT       = %+11.4lf \n", lcu->telescope->delta->Motor->getTicsPerRev() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "DMH       = %+11.4lf \n", lcu->telescope->delta->Motor->getHomePosition() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "DMR       = %+11.4lf \n", lcu->telescope->delta->Motor->getEncoderToAxis_Reduction() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "DWT       = %+11.4lf \n", lcu->telescope->delta->WormE->getTicsPerRev() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "DWH       = %+11.4lf \n", lcu->telescope->delta->WormE->getHomePosition() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "DWR       = %+11.4lf \n", lcu->telescope->delta->WormE->getEncoderToAxis_Reduction() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "DAT       = %+11.4lf \n", lcu->telescope->delta->AxisE->getTicsPerRev() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "DAH       = %+11.4lf \n", lcu->telescope->delta->AxisE->getHomePosition() );
    write( fd, buffer, strlen( buffer ) );
    sprintf( buffer, "DAR       = %+11.4lf \n", lcu->telescope->delta->AxisE->getEncoderToAxis_Reduction() );
    write( fd, buffer, strlen( buffer ) );
    lcu->postSemaphore();

    /** LCU Local Time */
    sprintf( buffer, "--------------------------------------------------\n" );
    write( fd, buffer, strlen( buffer ) );
    strftime( string_data, 32, "%Y-%m-%d, %T", lcu->getLCU_Time() );
    sprintf( buffer, "Page generated at: [%s]\n", string_data );
    write( fd, buffer, strlen( buffer ) );

    sprintf( buffer, "\r\n\r\n" );
    write( fd, buffer, strlen( buffer ) );

    if( verbose) printf( "[set_config] Good bye!\n" );
}

