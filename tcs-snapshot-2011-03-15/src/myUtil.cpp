#include "myUtil.h"

char * strfdegs( char * string, size_t max_len, const char * format, double degs )
{
    int gg, mm, ss, cc;
    double m_secs;
    double m_degs;
    double tmp;

    m_degs = fabs( degs );
    gg   = (int) floor( m_degs );
    tmp  = ((double) gg );
    mm   = (int) floor( ( m_degs - tmp ) * 60. );
    tmp += ((double) mm) / 60.;
    ss   = (int) floor( ( m_degs - tmp ) * 3600. );
    tmp += ((double) ss) / 3600.;
    cc   = (int) round( ( m_degs - tmp ) * 360000. );

    //printf( "[myUtil] strfdegs: degs=%lf -> gg=%d mm=%d ss=%d cc=%d\n", degs, gg, mm, ss, cc );
    if( cc > 99 ) {
        cc = 0;
        ss ++;
    }
    if( ss > 59  ) {
        ss = 0;
        mm ++;
    }
    if( mm > 59 ) {
        mm = 0;
        gg ++;
    }
    if( degs  < 0 ){
        gg = -gg;
    }

    m_secs = (double) ss + ((double) cc)/100.;
    //snprintf( string, max_len, format, gg, mm, m_secs );
    snprintf( string, max_len, format, gg, mm, (double) ss );
    return string;
}
