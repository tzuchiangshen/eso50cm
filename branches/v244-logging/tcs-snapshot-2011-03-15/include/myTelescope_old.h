#ifndef _MYTELSECOPE_H_
#define _MYTELSECOPE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>

#include <errno.h>

#include <myRS232.h>
#include <myTAxis.h>
#include <myTimer.h>
#include "degUtil.h"

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/stat.h>


#define SHMKEY   0xFAFAFA00
#define SEMKEY   0xFAFAFA00



struct telescope_data_t {
    struct timeval  gtime;
    struct timezone tzone;

    struct encoder_data_t encoder[6];

    char last[128];
    char msg[128];
    char quit;
};

/**
  * class myTelescope
  */
class myTelescope : public myRS232 {
    public:
        myTelescope( const char * fileName, const char * device, int baudrate );
        virtual ~myTelescope( void );

        myTimer * timer;
        
        int     readConfigFile( const char * );
        int     startup( void );
        
        char *  getFirmwareDate( void );
        char *  getFirmwareTime( void );
        char *  getFirmwareNumber( void );
        char *  getName( void );
        
        
        int     receiveRS232Data( void );
        
        int     updateFirmwareDate( void );
        int     updateFirmwareTime( void );
        int     updateFirmwareNumber( void );

        void    updatePosition( void );
        void    updateTelescope( void );
        char *  updatePositionData( char * pData );
        char *  updateEncodersData( char * pData );
        char *  updateSiteData( char * pData );
        
        double  getHA( void );
        double  getRA( void );
        double  getDec( void );
        double  getLatitude( void );
        double  getLongitude( void );

        double  getDifferenceHA( void );
        double  getDifferenceRA( void );
        double  getDifferenceDec( void );

        //char *  getTelescopeFirmwareDate( char * line );
        //char *  getTelescopeFirmwareTime( char * line );
        //char *  getTelescopeFirmwareNumber( char * line );
        char *  getHA_MeadeFormat( char * line );
        char *  getRA_MeadeFormat( char * line );
        char *  getDec_MeadeFormat( char * line );
        char *  getTelescopeLatitude_MeadeFormat( char * line );
        char *  getTelescopeLongitude_MeadeFormat( char * line );
        double  getAltitudeAboveSeeLevel( void );
        
        int     checkAlphaPosition( void );
        int     checkDeltaPosition( void );
        
        int     setTargetRA( double t_ra );
        int     setTargetDec( double t_dec );
        double  getTargetRA( void );
        double  getTargetHA( void );
        double  getTargetDec( void );

        int     setSlewHighElevation( double t_he );
        int     setSlewLowElevation( double t_le );
        
        int setAxesComm( void );
        
        int getPositionData( void );

        myTAxis * alpha;
        myTAxis * delta;
        
        int segment_id;
        char * shared_memory;
        struct shmid_ds shmbuffer;
        int segment_size;
        int shared_segment_size;
        int semaphore_id;
        
        struct telescope_data_t * telescope;
        
    private:
        int m_telescope_is_running;
        
        char timer_status[128];
        char alpha_status[128];
        char delta_status[128];

        int run_flag;     
        int first_time_flag;
        
        char *          m_Telescope_Name;                        //VP
        char *          m_Telescope_FirmwareDate;               //VD
        char *          m_Telescope_FirmwareTime;               //VT
        char *          m_Telescope_FirmwareNumber;             //VN
        double          m_Telescope_Longitude;                //g
        double          m_Telescope_Latitude;                 //t
        double          m_Telescope_Altitude;                 //H
        double          m_Telescope_TrackingRate;
        double          m_Telescope_SlewRate;
        double          m_TelescopeSlew_HighElevation;                 //h
        double          m_TelescopeSlew_LowElevation;                  //o

        char            m_targetName[16];
        double          m_currentRA;
        double          m_currentHA;
        double          m_currentDec;

        double          m_targetRA;
        double          m_targetHA;
        double          m_targetDec;

        double          m_differenceRA;
        double          m_differenceHA;
        double          m_differenceDec;

        
        int             tty_message_bytes;
        char            tty_message[32];
};

#endif // * _MYTELSECOPE_H_ *
