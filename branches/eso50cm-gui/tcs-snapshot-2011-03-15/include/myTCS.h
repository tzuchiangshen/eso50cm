#ifndef MYTCS_H
#define MYTCS_H

#include <stdio.h>
#include <unistd.h>     //sleep

#include <sys/shm.h>    //shared memory
#include <sys/stat.h>   //shared memory

#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#include "myTCP.h"
//#include "myServerSocket.h"
#include "myTTY.h"
#include "myTelescope.h"

class myTCS {
    public:
        myTCS( const char * fileName );
        ~myTCS( void );
        
        int run( void );
        int myLX200CommandInterpreter( char * command, char * retstr );
        int readConfigFile( const char * configFileName  );
    private:
        myTCP * tcp;
        //myServerSocket *tcp;
        
        //myTTY * tty;
        char tty_name[128];
        int b_from, b_to, rx_bytes;
        char c;
        int tty_open;
        
        myTelescope     * telescope;
        myTTY           * handset;
        //myTTY           * thesky;
        
        int             m_tcs_is_runnig;
        
        char *          m_SiteName1;                            //M
        char *          m_SiteName2;                            //N
        char *          m_SiteName3;                            //O
        char *          m_SiteName4;                            //P
        int             m_CurrentSite_UTCOffset;                //G
        int             m_CurrenCalendarFormat;                 //c
        char *          m_TCS_SerialCommDevice;                 //pTP
        int             m_TCS_SerialCommBaudRate;               //pTB
        int             m_TCP_IP_Port;                          //pI
        char *          m_GPS_SerialCommDevice;                 //pGP
        int             m_GPS_SerialCommBaudRate;               //pGB
        char *          m_Telescope_SerialCommDevice;           //ptP
        int             m_Telescope_SerialCommBaudRate;         //ptB
        
        int             m_SlewToTarget_AltAz;
        int             m_MoveToNorth;
        int             m_RunningDelta;
        int             m_MoveToWest;
        int             m_RunningAlpha;
        int             m_HaltTelescope;
        int             m_SlewToTarget;
        int             m_FineSlewToTarget;
        int             m_FullStop;
        int             m_SlewingToAlphaTarget;
        int             m_SlewingToDeltaTarget;
        int             m_SlewToTarget_AlphaReady;
        int             m_SlewToTarget_DeltaReady;
        int             m_Tracking;
        int             m_FindAlphaHome;

};

#endif //MYTCS_H
