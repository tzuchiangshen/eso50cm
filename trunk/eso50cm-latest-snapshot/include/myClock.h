#ifndef _MYCLOCK_H_
#define _MYCLOCK_H_

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
    double m_longitude;

    double LSTime;
    double JDate;

    struct tm Utime;
    struct tm Ltime;

    struct timeval  gtime;
    struct timezone tzone;
};

/**
  * @brief Provides a simple timer and LST calculator.
  */
class myClock
{
    public:
        myClock( struct my_tClock_data_t * clock_data );
        ~myClock( void );

        void initializeClock( double longitude );
        void setLongitude( double longitude );
        double getLongitude( void );
        void setTime( void );
        void setTime( struct timeval gtime )
        double getLST( void );
        double getJDate( void );

    private:
        struct my_tClock_data_t * m_clock_data;

        double julianDate( void );
        double localSiderealTime( void );
};

#endif //* _MYCLOCK_H_ *

