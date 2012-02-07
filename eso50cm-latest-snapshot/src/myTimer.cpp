#include "myTimer.h"

#define  read_segment_key           0xFAFAFA01
#define  write_segment_key          0xFAFAFA02

#define  telescope_segment_key      0xFAFAFA04

#define  read_semaphore_key         0xFAFAFA01
#define  write_semaphore_key        0xFAFAFA02
#define  timer_semaphore_key        0xFAFAFA03          //myTimer semaphore
#define  telescope_semaphore_key    0xFAFAFA04

/**
 *  @brief  Constructor :-)
 * 0 Location       0
 * 1 Latitude       12
 * 2 Longitude      24  
 * 3 Altitude       36
 * 4 UDate          48
 * 5 UTC            60
 * 6 LDate          72
 * 7 LTime          84    
 * 8 LStime         96 
 * 9 JDate         108
 */
myTimer::myTimer( double Longitude ) {
    m_Telescope_Longitude = Longitude;//70.53444;//
    
    printf( "[myTimer::myTimer] Hello world!\n" );
        
    m_timer_mutex = new pthread_mutex_t;
    pthread_mutex_init( m_timer_mutex, NULL  );
    m_timer_tic = false;
    
    //-------------------------------------------------------------------
    //time initilization
    gettimeofday( &old_gtime, &tzone );
    iniT  = (double) (old_gtime.tv_usec)/1000000.;
    iniT += (double) (old_gtime.tv_sec);
    oldT  = iniT;
    
    _tid = 0;

}

/**
 *  @brief  Destructor calls stop.
 */
myTimer::~myTimer( void ) {
    printf( "[myTimer::~myTimer] Exiting...\n" );
    if( stop() == 0 ) {
        printf( "[myTimer::~myTimer] timer stoped succesfully\n" );
    } else {
        printf( "[myTimer::~myTimer] error when stoping timer\n" );
    }
    delete m_timer_mutex;
    printf( "[myTimer::~myTimer] Good bye!\n" );
}

/**
 *  @brief  stop stops timer
 *  @return pthread_join's return value: 0 = succesfull, otherwise error number.
 */
int myTimer::stop( void ) {
    int retval = 0;
    printf( "[myTimer::stop] ...\n" );
    m_end = false;
    m_running = false;
    if( pthread_mutex_unlock( m_timer_mutex ) == 0 ) {
        printf( "[myTimer::stop] unlocking timer\n" );
    } else {
        perror( "[myTimer::stop] pthread_mutex_unlock" );
        printf( "[myTimer::stop] pthread_mutex_unlock: unlocked\n" );
    }
    while( ! m_end ) {
        printf( "[myTimer::stop] thread still runing...\n" );
        sleep(1);
    }
    printf( "[myTimer::stop] Timer stoped\n" );
    
    if ( _tid ) {
        printf( "[myTimer::stop] Stoping timer (tid=%lu)...\n", _tid );
        retval = pthread_join( _tid, NULL );
        if( retval == 0 ) {
            printf( "[myTimer::stop] pthread joined succesfuly\n" );
        } else {
            perror( "[myTimer::stop] pthread_join" );
            printf( "[myTimer::stop] pthread_join ERROR\n" );
        }
    } else {
        printf( "[myTimer::stop] No Timer to stop\n" );
    }

    return retval;
}


/**
 *  @brief start starts child thread with timer loop.
 */
int myTimer::start( void ) {
    int rc;
    if( (rc = pthread_create( & _tid, NULL, gate, this ) ) == 0 ) {
        printf( "[myTimer::start] pthread create OK (return code = %d)\n", rc );
        printf( "[myTimer::start] thr_id = %lu\n",  _tid );
    } else {
        printf( "[myTimer::start] pthread create ERROR (return code = %d)\n", rc );
    }
    fflush( stdout );
    return rc;
}

/**
  * @brief gate
  */
void * myTimer::gate( void * p ) {
    ( (myTimer *)p )->run();
    return NULL;
}

/**
  * @brief run starts timer loop
  * Waits for S_0, then S_1 = 1; 
  * sleep 250[ms] and sets S_1 = 0;
  */
void myTimer::run() {
    double t, dt;
    struct timespec req, rem;

    printf( "[myTimer::run] Hello world\n" );
    printf( "[myTimer::run] PID = %d (%u)\n", (int) getpid(), (int) pthread_self() );
    //printf( "[myTimer::run] ppid = %d\n",(int) getpid() );
    //printf( "[myTimer::run] pid = %d\n",(int) getpid() );
    //printf( "[myTimer::run] tpid = %lu\n",pthread_self() );
    int tmr = 0;
    m_running = true;
    while( m_running ) {
        req.tv_sec = 0;
        req.tv_nsec = 250000000;
        nanosleep( & req, & rem );
        t = getTime( & dt );
        tmr ++;
        m_timer_tic = true;
        //if( 1 & tmr ) {
        //    printf( "[myTimer::run] tic\n" );
        //} else {
        //    printf( "[myTimer::run] tac\n" );
        //}
        pthread_mutex_lock( m_timer_mutex );
    }
    m_end = true;
    printf( "[myTimer::run] The End (tid=%lu).\n", pthread_self() );
}


int myTimer::getIsTimerTic( void ) {
    int retval = false;
    if( m_timer_tic ) {
        m_timer_tic = false;
        pthread_mutex_unlock( m_timer_mutex );
        retval = true;
    }
    return retval;
}
/**
 * @brief gets local time/date
 * @param dt (double *) time elapsed since las call (in seconds)
 * @return currentT (double) Time elapsed since TCS started (in seconds).
 */
double myTimer::getTime( double * dt ) {
    double currentDT;

    gettimeofday( &gtime, &tzone );

    currentT  = (double) (gtime.tv_usec)/1000000.;
    currentT += (double) (gtime.tv_sec);
    currentDT = currentT - oldT;
    oldT      = currentT;

    old_gtime.tv_sec  = gtime.tv_sec;
    old_gtime.tv_usec = gtime.tv_usec;

    //UTC
    gmtime_r( &(gtime.tv_sec), & UTime  );
    //LTime
    localtime_r( &(gtime.tv_sec), & LTime  );
    //Julian Date
    JDate = julianDate();
    //Local Sideral Time
    LSTime = localSideralTime();

    //Time elapsed since TCS started (in seconds).
    * dt = currentDT;
    currentT -= iniT;

    return currentT;
}

/**
 * @return lst (double) in degrees;
 */
double myTimer::localSideralTime( void ) {
    double T, Theta;
    int    dias;

    T= ( JDate - JD2000 ) / 36525.0;
    Theta= ( (C0 + C1 * ( JDate - JD2000 ) + C2 * T*T - T*T*T / C3) - m_Telescope_Longitude );     // lst en grados
    dias = (int)(Theta / 360.0);

    return Theta - (double)dias * 360.0;
}

/**
 * @return Julian date (double) 
 */
double myTimer::julianDate( void ) {
    int A, B, AA, MM;
    double x1, x2, x3, x4, jd;

    if( UTime.tm_mon == 0 || UTime.tm_mon == 1 ){
        AA = UTime.tm_year + 1899;
        MM = UTime.tm_mon + 14; //= mes+1+12=(date->tm_mon + 1) + 1 + 12
    } else {
        AA = UTime.tm_year + 1900;
        MM = UTime.tm_mon + 2;  //= mes+1= (date->tm_mon + 1) + 1
    }

    A= (int)floor( (double)AA / 100.0 );
    B= 2 - A + (int)floor( A / 4.0 );

    x1  = ( 365.25 * ( AA + 4716.0 ) );
    x2  = ( 30.6001 * MM ); //= ( 30.6001 * mes + 1 )
    x3 =  ( (double)UTime.tm_sec + (double)UTime.tm_min * 60.0 +
            (double)UTime.tm_hour * 3600.0 ) / 86400.0 +
            (int)UTime.tm_mday;
    x4  = B - 1524.5;
    jd= floor(x1)+floor(x2)+x3+x4;

    return jd;
}

/**
 * @return LSTime (double) in degrees;
 */
double myTimer::getLST( void ) {
    return LSTime;
}

/**
 * getUDate
 * @return pointer (char *) to string with Universal Date
 */
char * myTimer::getUDate( char * line ) {
    strftime( line, 24, " %Y-%m-%d", & UTime );
    return line;
}

/**
 * getUTime
 * @return pointer (char *) to string with Universal Time
 */
char * myTimer::getUTime( char * line ) {
    strftime( line, 24, " %T", & UTime );
    return line;
}

/**
 * getLTime
 * @return pointer (char *) to string with Local Time
 */
char * myTimer::getLTime( char * line ) {
    strftime( line, 24, " %T", & LTime );
    return line;
}

/**
 * getLDate
 * @return pointer (char *) to string with Local Date
 */
char * myTimer::getLDate( char * line ) {
    strftime( line, 24, " %Y-%m-%d", & LTime );
    return line;
}

/**
 * getLDate
 * @return pointer (char *) to string with Local Date
 * in Meade format (06/04/10)
 */
char * myTimer::getLDate_MeadeFormat( char * line ) {
    strftime( line, 24, " %m/%d/%y", & LTime );
    return line;
}

/**
 * getLTime
 * @return pointer (char *) to string with Local Time
 */
char * myTimer::getLTime_MeadeFormat( char * line ) {
    strftime( line, 24, " %T", & LTime );
    return line;
}
/**
 * getLSTime
 * @return pointer (char *) to string with Local Sidereal Time
 */
char * myTimer::getLSTime( char * line ) {
    Grados_A_Hr_Mi_Sg( LSTime, line );
    return line;
}
/**
 * getJDate
 * @return pointer (char *) to string with JulianDate
 */
char * myTimer::getJDate( char * line ) {
    sprintf( line, "%15.6lf", JDate );
    return line;
}



/**
 * updateTimeData
 * @return pointer (char *) to string with Time Data
 * 0 Location       0
 * 1 Latitude       12
 * 2 Longitude      24  
 * 3 Altitude       36
 * 4 UDate          48
 * 5 UTC            60
 * 6 LDate          72
 * 7 LTime          84    
 * 8 LStime         96 
 * 9 JDate         108
 */
char *  myTimer::updateTimeData( char * tData ) {
    double T;
    double DT;
    char * ptr;
    int l;

    gettimeofday( &gtime, &tzone );

    T  = (double) (gtime.tv_usec)/1000000.;
    T += (double) (gtime.tv_sec);
    DT = T - oldT;

    tData[0] = '?';
    tData[1] = 'T';
    //UDate 
    ptr = & tData[2];
    strftime( ptr, 24, " %Y-%m-%d;", & UTime );
    
    //UTC
    l = strlen( tData );
    ptr = & tData[l];
    strftime( ptr, 24, " %T;", & UTime );
    //sprintf( ptr, "%02d:%02d:%02d", UTime.tm_hour, UTime.tm_min, UTime.tm_sec );

    //LDate
    l = strlen( tData );
    ptr = & tData[l];
    strftime( ptr, 24, " %Y-%m-%d;", & LTime );

    //LTime.tm_mday,
    l = strlen( tData );
    ptr = & tData[l];
    strftime( ptr, 24, " %T;", & LTime );
    //sprintf( ptr, "%02d:%02d:%02d", LTime.tm_hour, LTime.tm_min, LTime.tm_sec );

    //LSTime
    l = strlen( tData );
    ptr = & tData[l];
    Grados_A_Hr_Mi_Sg( LSTime, ptr );
    l = strlen( tData );
    tData[l] = ';';
    tData[l+1] = 0;

    //JDate
    l = strlen( tData );
    ptr = & tData[l];
    sprintf( ptr, "%15.6lf#", JDate );

    return tData;
}

