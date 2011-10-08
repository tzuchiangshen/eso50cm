#ifndef _MYTELSECOPE_H_
#define _MYTELSECOPE_H_

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/stat.h>

#include <stdio.h>
#include <sys/socket.h>         //socket
#include <sys/un.h>             //ux-socket
#include <unistd.h>             //close
#include <stdlib.h>

#include <myTAxis.h>
#include <myTClock.h>
#include <myBSemaphore.h>
#include "LoggerHelperDummy.h"

#define TELSHMKEY   0xFAFAFA00
#define TELSEMKEY   0xFAFAFA00

struct telescope_data_t {
    struct timeval  gtime;
    struct timezone tzone;

    struct encoder_data_t encoder[6];

    char last[128];
    char msg[128];
    char new_data;
    char quit;
};

struct my_telescope_data_t
{
    char is_initilized;
    char is_running_monitor;
    char is_running_goto;
    char is_tracking;

    double Longitude;                   //g
    double Latitude;                    //t
    double Altitude;                    //H

    double LowElevation;                //o
    double HighElevation;               //h

    double currentRA;
    double currentHA;
    double currentDec;
    double currentAlt;
    double currentAz;

    char   targetName[16];
    double targetRA;
    double targetHA;
    double targetDec;
    double targetAlt;
    double targetAz;

    double differenceRA;
    double differenceHA;
    double differenceDec;
    double differenceAlt;
    double differenceAz;
};

struct my_lcu_data_t
{
    struct my_tClock_data_t     clock_data;
    struct my_telescope_data_t  telescope_data;

    struct my_TAxis_data_t      alpha_data;
    struct my_TAxis_data_t      delta_data;
};

class myTelescope : public myTClock
{
    public:
        myTelescope( struct my_lcu_data_t * lcu_data );
        ~myTelescope( void );

        int attachInstrumentMemory( void );
        int detachInstrumentMemory( void );
        int connectTelescope( void );
        int disconnectTelescope( void );
        void initialize( void );

        void setLongitude( double );
        void setLatitude( double );
        void setAltitude( double );
        void setLowElevation( double );
        void setHighElevation( double );
        void setIsConfigured( char );

        char setIsInitilized( char );
        char setIsRunningMonitor( char );
        char setIsRunningGoto( char );
        char setIsTracking( char );

        char getIsInitilized( void );
        char getIsRunningMonitor( void );
        char getIsRunningGoto( void );
        char getIsTracking( void );

        double getLongitude( void );
        double getLatitude( void );
        double getAltitude( void );
        double getLowElevation( void );
        double getHighElevation( void );
        double getHA( void );
        double getRA( void );
        double getDec( void );
        double getAlt( void );
        double getAz( void );
        double getDifferenceHA( void );
        double getDifferenceRA( void );
        double getDifferenceDec( void );
        double getTargetRA( void );
        double getTargetHA( void );
        double getTargetDec( void );
        double getTargetAlt( void );
        double getTargetAz( void );

        char   getIsConfigured( void );
        int    getIfNewData( void );

        //int    currentPosition( double * lst, double * ra, double * dec  );
        int    currentPosition( double * lst, double * ra, double * dec, double * alt, double * az, double * ha );
	int    targetPosition( double * lst, double * ra, double * dec, double * alt, double * az, double * ha );
        int    setTarget( double trg_ra, double trg_dec, double * trgAlt, double * trgAz );
        double equatorialToHorizontal( double ha, double dec, double * alt, double * az );


        myTAxis  * alpha;
        myTAxis  * delta;

    private:
        struct my_telescope_data_t * m_telescope_data;

        struct shmid_ds m_shmbuffer;
        int     m_segment_id;
        char *  m_shared_memory;
        int     m_segment_size;
        int     m_shared_segment_size;
        int     m_semaphore_id;

        struct telescope_data_t * bin_telescope;
        myBSemaphore * bin_telescope_semaphore;

        int m_socket_fd;
        struct sockaddr_un m_server_name;
	LoggerHelper logger;
};

#endif // * _MYTELSECOPE_H_ *
