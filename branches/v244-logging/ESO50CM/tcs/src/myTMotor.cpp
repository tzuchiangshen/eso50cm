#include <myTMotor.h>

extern int verbose;

/**
 * myTMotor
 */
myTMotor::myTMotor( char ax, char id, struct my_tEncoder_data_t * encoder )
    : myTEncoder( ax, id, encoder ), m_simulated(false)
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
	printf("[myTMotor::runEncSteps]\n");
	if(m_simulated) {
		/**
		Also set the values in the encoders in the Worm in order to simulate the movement
		**/
		printf("-----------------------------------> En Simulation -----------------------\n");

	}
	printf("[myTMotor::runEncSteps] moving the motor to %d\n", enc_steps);
    return setDeviceMemory( 7, & enc_steps, 0 );
}

/**
 * setSimulationMode
 */
void myTMotor::setSimulationMode( bool mode)
{
	m_simulated = mode;
}

/**
 * isSimulated
 */
bool myTMotor::isSimulated( void )
{
	return m_simulated;
}
