#include "myTClock.h"

myTClock::myTClock( struct my_tClock_data_t * clock_data, LoggerHelper *logLCUImpl )
{
    logger = logLCUImpl;
    logger->logINFO( "[myTClock::myTClock] Hello World!" );
    m_clock_data = clock_data;
    logger->logINFO( "[myTClock::myTClock] aam_clock_data at %p", (void *) m_clock_data );
    m_clock_data->old_t = 0.;
    logger->logINFO( "[myTClock::myTClock] ready");
    
}


myTClock::~myTClock( void )
{
    logger->logINFO( "[myTClock::~myTClock] Good Bye!" );
}

/**
 * initializeClock
 */
void myTClock::initializeClock( double longitude )
{
    m_clock_data->longitude = longitude;      //Santiago = 70.53444
}

/**
 * setLongitude
 */
void myTClock::setClockLongitude( double longitude )
{
    m_clock_data->longitude = longitude;
}

/**
 * getLongitude
 */
double myTClock::getClockLongitude( void  )
{
    return m_clock_data->longitude;
}

/**
 * currentTime
 * @return LSTime (double) in degrees;
 */
double myTClock::currentTime( void )
{
    logger->logINFO("[myTClock::currentTime]"); 
    gettimeofday( & m_clock_data->gtime, & m_clock_data->tzone );
    /** UTC */
    gmtime_r( & m_clock_data->gtime.tv_sec, & m_clock_data->UTime  );
    logger->logINFO("[myTClock::currentTime] UTC = %s", asctime(&m_clock_data->UTime)); 
    /** LTime */
    localtime_r( & m_clock_data->gtime.tv_sec, & m_clock_data->LTime  );
    logger->logINFO("[myTClock::currentTime] LST = %s", asctime(&m_clock_data->LTime)); 
    /** Local Time in milliseconds **/
    m_clock_data->MlTime = (m_clock_data->gtime.tv_sec)+((m_clock_data->gtime.tv_usec/1000000.0));

    /** Julian Date */
    julianDate();
    /** Local Sideral Time */
    localSiderealTime();
    //strftime( infoline, 24, "LT %Y-%m-%d %T", & m_clock_data->LTime );
    //printf( "%s", infoline );
    //strftime( infoline, 24, "UT %Y-%m-%d %T", & m_clock_data->UTime );
    //printf( "%s", infoline );

    return m_clock_data->LSTime;
}
double myTClock::currentTime( struct timeval * gtime )
{
    //char infoline[32];
    /** UTC */
    gmtime_r( & (gtime->tv_sec), & m_clock_data->UTime  );
    /** Local Time in milliseconds **/
    m_clock_data->MlTime = (gtime->tv_sec)+((gtime->tv_usec/1000000.0));
    /** LTime */
    localtime_r( & (gtime->tv_sec), & m_clock_data->LTime  );
    /** Julian Date */
    julianDate();
    /** Local Sideral Time */
    localSiderealTime();
    //strftime( infoline, 24, "LT %Y-%m-%d %T", & m_clock_data->LTime );
    //printf( "%s", infoline );
    //strftime( infoline, 24, "UT %Y-%m-%d %T", & m_clock_data->UTime );
    //printf( "%s", infoline );

    return m_clock_data->LSTime;
}

/**
 * @return lst (double) in degrees;
 */
double myTClock::localSiderealTime( void )
{
    double T, Theta;
    int    dias;

    T= ( m_clock_data->JDate - JD2000 ) / 36525.0;
    Theta= ( (C0 + C1 * ( m_clock_data->JDate - JD2000 ) + C2 * T*T - T*T*T / C3) - m_clock_data->longitude );     // lst en grados
    dias = (int)(Theta / 360.0);

    m_clock_data->LSTime = Theta - (double)dias * 360.0;
    return m_clock_data->LSTime;
}

/**
 * @return Julian date (double) 
 */
double myTClock::julianDate( void )
{
    int A, B, AA, MM;
    double x1, x2, x3, x4;

    if( m_clock_data->UTime.tm_mon == 0 || m_clock_data->UTime.tm_mon == 1 )
    {
        AA = m_clock_data->UTime.tm_year + 1899;
        MM = m_clock_data->UTime.tm_mon + 14; //= mes+1+12=(date->tm_mon + 1) + 1 + 12
    } else 
    {
        AA = m_clock_data->UTime.tm_year + 1900;
        MM = m_clock_data->UTime.tm_mon + 2;  //= mes+1= (date->tm_mon + 1) + 1
    }

    A= (int)floor( (double)AA / 100.0 );
    B= 2 - A + (int)floor( A / 4.0 );

    x1  = ( 365.25 * ( AA + 4716.0 ) );
    x2  = ( 30.6001 * MM ); //= ( 30.6001 * mes + 1 )
    x3 =  ( (double)m_clock_data->UTime.tm_sec + (double)m_clock_data->UTime.tm_min * 60.0 +
            (double)m_clock_data->UTime.tm_hour * 3600.0 ) / 86400.0 +
            (int)m_clock_data->UTime.tm_mday;
    x4  = B - 1524.5;
    m_clock_data->JDate = floor(x1)+floor(x2)+x3+x4;

    return m_clock_data->JDate;
}

/**
 * getLocalTime
 */
struct tm * myTClock::getLocalTime( void )
{
    return & m_clock_data->LTime;
}


/**
 * getMLocalTime
 */
double myTClock::getMLocalTime( void )
{
    return m_clock_data->MlTime;
}

/**
 * getUniversalTime
 */
struct tm * myTClock::getUniversalTime( void )
{
    return & m_clock_data->UTime;
}



/**
 * getLST
 * @return LSTime (double) in degrees;
 */
double myTClock::getLST( void )
{
    return m_clock_data->LSTime;
}

/**
 * getJulianDate
 */
double myTClock::getJulianDate( void )
{
    return m_clock_data->JDate;
}

/**
 * getDeltaT
 */
double myTClock::getDeltaT( void )
{
    double new_t;
    double delta_t;

    struct timeval  gtime;
    struct timezone tzone;

    gettimeofday( & gtime, & tzone );

    new_t   = (double) (gtime.tv_usec)/1000000.;
    new_t  += (double) (gtime.tv_sec);
    delta_t = new_t - m_clock_data->old_t;
    m_clock_data->old_t = new_t;

    return delta_t;
}
