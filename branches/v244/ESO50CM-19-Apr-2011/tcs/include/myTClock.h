#ifndef _MYTCLOCK_H_
#define _MYTCLOCK_H_

#include <stdio.h>
#include <sys/time.h>           //gettimeofday
#include <time.h>               //gmtime
#include <math.h>
#include <errno.h>              //error number


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

struct my_tClock_data_t
{
    double longitude;

    double LSTime;
    double JDate;
    double old_t;

    struct tm UTime;
    struct tm LTime;

    struct timeval  gtime;
    struct timezone tzone;
};

/**
  * @brief Provides a simple timer and LST calculator.
  */
class myTClock
{
    public:
        myTClock( struct my_tClock_data_t * clock_data );
        ~myTClock( void );

        void initializeClock( double longitude );
        void setClockLongitude( double longitude );
        double getClockLongitude( void );

        double currentTime( void );
        double currentTime( struct timeval * gtime );
        double getLST();
        double getJulianDate( void );
        double getDeltaT( void );
        struct tm * getLocalTime( void );
        struct tm * getUniversalTime( void );

    private:
        struct my_tClock_data_t * m_clock_data;

        double julianDate( void );
        double localSiderealTime( void );
};

#endif //* _MYTCLOCK_H_ *

