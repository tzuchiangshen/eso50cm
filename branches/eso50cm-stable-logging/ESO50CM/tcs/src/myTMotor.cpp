#include <myTMotor.h>

extern int verbose;

/**
 * myTMotor
 */
myTMotor::myTMotor( char ax, char id, struct my_tEncoder_data_t * encoder )
    : myTEncoder( ax, id, encoder )
{
}


/**
 * ~myTMotor
 */
myTMotor::~myTMotor( void )
{
}

/**
 * setSpeed
 */
int myTMotor::setSpeed( double degs_per_sec )
{
    int tics_per_sec;
    if( getTicsPerRev() > 0 ) {
        degs_per_sec /= 360.0 * getEncoderToAxis_Reduction() / getTicsPerRev();
        tics_per_sec = (int) degs_per_sec;
    } else {
        tics_per_sec = 0;
    }
    return tics_per_sec;
}

/**
 * runEncSteps
 */
int myTMotor::runEncSteps( int enc_steps )
{
    return setDeviceMemory( 7, & enc_steps, 0 );
}

