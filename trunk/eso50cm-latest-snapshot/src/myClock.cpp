#include "myClock.h"

myClock::myClock( struct my_tClock_data_t * clock_data )
{
    m_clock_data = clock_data;
}


myClock::~myClock( void )
{
}

void myClock::initializeClock( double longitude )
{
    m_clock_data->m_longitude = longitude;      //Santiago = 70.53444
}

void myClock::setLongitude( double longitude )
{
    m_clock_data->m_longitude = longitude;
}

void myClock::setLongitude( void  )
{
    return m_clock_data->m_longitude;
}


void myClock::setTime( void )
{
    gettimeofday( & m_gtime, & m_tzone );

    //UTC
    gmtime_r( & (m_gtime.tv_sec), & m_clock_data->UTime  );
    //LTime
    localtime_r( & (m_gtime.tv_sec), & m_clock_data->LTime  );
    //Julian Date
    julianDate();
    //Local Sideral Time
    localSiderealTime();
}

void myClock::setTime( struct timeval * gtime )
{
    //UTC
    gmtime_r( & (gtime->tv_sec), & m_clock_data->UTime  );
    //LTime
    localtime_r( & (gtime->tv_sec), & m_clock_data->LTime  );
    //Julian Date
    julianDate();
    //Local Sideral Time
    localSiderealTime();
}


/**
 * @return lst (double) in degrees;
 */
double myClock::localSiderealTime( void ) {
    double T, Theta;
    int    dias;

    T= ( m_clock_data->JDate - JD2000 ) / 36525.0;
    Theta= ( (C0 + C1 * ( m_clock_data->JDate - JD2000 ) + C2 * T*T - T*T*T / C3) - m_clock_data->longitude );     // lst en grados
    dias = (int)(Theta / 360.0);

    m_clock_data->LSTime = Theta - (double)dias * 360.0;
    return m_clock_data->LSTime
}

/**
 * @return Julian date (double) 
 */
double myClock::julianDate( void ) {
    int A, B, AA, MM;
    double x1, x2, x3, x4, jd;

    if( m_clock_data->UTime.tm_mon == 0 || m_clock_data->UTime.tm_mon == 1 ){
        AA = m_clock_data->UTime.tm_year + 1899;
        MM = m_clock_data->UTime.tm_mon + 14; //= mes+1+12=(date->tm_mon + 1) + 1 + 12
    } else {
        AA = m_clock_data->UTime.tm_year + 1900;
        MM = m_clock_data->UTime.tm_mon + 2;  //= mes+1= (date->tm_mon + 1) + 1
    }

    A= (int)floor( (double)AA / 100.0 );
    B= 2 - A + (int)floor( A / 4.0 );

    x1  = ( 365.25 * ( AA + 4716.0 ) );
    x2  = ( 30.6001 * MM ); //= ( 30.6001 * mes + 1 )
    x3 =  ( (double)m_UTime.tm_sec + (double)m_UTime.tm_min * 60.0 +
            (double)m_UTime.tm_hour * 3600.0 ) / 86400.0 +
            (int)m_UTime.tm_mday;
    x4  = B - 1524.5;
    m_clock_data->JDate = floor(x1)+floor(x2)+x3+x4;

    return m_clock_data->JDate;
}

/**
 * @return LSTime (double) in degrees;
 */
double myClock::getLST( void ) {
    return m_clock_data->LSTime;
}


double myClock::getJDate( void ) {
    return m_clock_data->JDate;
}


