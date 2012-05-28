#ifndef _MYTIMER_H_
#define _MYTIMER_H_

#include <sys/time.h>           //gettimeofday
#include <time.h>               //gmtime
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>              //error number
#include <sys/types.h>          //getpid()
#include <unistd.h>             //getpid()

//#include <mySemaphore.h>
#include <degUtil.h>

//-------------------------------------------------------------
//      Julian Date
#define C0                  280.46061837
#define C1                  360.98564736629
#define C2                    0.000387933
#define C3             38710000.0
#define JD2000          2451545.0

//-------------------------------------------------------------
//      Site
#define LNGSCL              -70.55
#define LATSCL              -33.26
#define ALTOUC             1450.0

/**
  * @brief Provides a simple timer and LST calculator.
  */
class myTimer {
    public:
        myTimer( double Longitude );
        virtual ~myTimer( void );
        int start( void );
        int stop( void );
        int getIsTimerTic( void );

        double getTime( double * );
        double updateTime( void );
        double getLST( void );
        
        //char * getSiteName( void );
        //char * getSiteLatitude( void );
        //char * getSiteLongitude( void );
        //char * getSiteAltitude( void );

        char * getUTime( char * line );
        char * getUDate( char * line );
        char * getLTime( char * line );
        char * getLTime_MeadeFormat( char * line );
        char * getLDate( char * line );
        char * getLDate_MeadeFormat( char * line );
        char * getLSTime( char * line );
        char * getJDate( char * line );
        char * updateTimeData( char * tData );
    private:
        virtual void run();
        static void * gate(void *);
        pthread_t _tid;
        int m_end;
        int m_running;

        double julianDate( void );
        double localSideralTime( void );

        //char   Location[24];
        //double Latitude;
        //double Longitude;
        //double Altitude;

        double          m_Telescope_Longitude;
        //double          m_CurrentSite_Latitude;
        //double          m_CurrentSite_Altitude;

        double LSTime;
        double JDate;
        struct tm UTime;
        struct tm LTime;

        struct timeval  gtime;
        struct timezone tzone;
        struct timeval  old_gtime;
        double oldT;
        double iniT;
        double currentT;

        //mySemaphore semaph;
        //mySemaphore * timer_semaphore;
        //int timer_semaphore_id;
        
        pthread_mutex_t * m_timer_mutex;
        int m_timer_tic;
        
        char * m_timer_data;
};

#endif //* _MYTIMER_H_ *

