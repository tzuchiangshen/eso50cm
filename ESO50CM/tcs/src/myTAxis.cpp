#include <myTAxis.h>

extern int verbose;

/**
 *  myTAxis
 */
myTAxis::myTAxis( char id, struct my_TAxis_data_t * axis )
{
    m_id = id;
    if( verbose ) printf( "[myTAxis::myTAxis] %c Hello World!\n", m_id );
    if( verbose ) printf( "[myTAxis::myTAxis] %c axis_data at %p\n", m_id, (void *) axis );
    if( verbose ) printf( "[myTAxis::myTAxis] %c motorE at    %p\n", m_id, (void *) & axis->motorE );
    if( verbose ) printf( "[myTAxis::myTAxis] %c wormE at     %p\n", m_id, (void *) & axis->wormE );
    if( verbose ) printf( "[myTAxis::myTAxis] %c axisE at     %p\n", m_id, (void *) & axis->axisE );

    Motor = new myTMotor( m_id, 'M',  & axis->motorE );
    WormE = new myTEncoder( m_id, 'W', & axis->wormE );
    AxisE = new myTEncoder( m_id, 'A', & axis->axisE );
	// Tzu: temporary hack, this should be done by reading the configuration files
	//Motor->setSimulationMode(true);
}

/**
 *  ~myTAxis
 */
myTAxis::~myTAxis() {
    delete Motor;
    delete WormE;
    delete AxisE;
    if( verbose ) printf( "[myTAxis::~myTAxis] %c Good Bye!\n", m_id );
}


/**
 *  setInstrumentMemorySpace
 */
void myTAxis::setInstrumentMemorySpace( struct encoder_data_t * bin_motor_e,
                                        struct encoder_data_t * bin_worm_e,
                                        struct encoder_data_t * bin_axis_e )
{
    Motor->setInstrumentMemorySpace( bin_motor_e );
    WormE->setInstrumentMemorySpace( bin_worm_e );
    AxisE->setInstrumentMemorySpace( bin_axis_e );
}

/**
 *  getPosition
 */
double myTAxis::getPosition( void )
{
    return WormE->getDegPosition();
}

/**
 *  offsetAxisInDeg
 */
int myTAxis::offsetAxisInDeg( double degs )
{
    int mtr_counts;
	double reduction;
	double tics;
	double tmp;

	reduction = Motor->getEncoderToAxis_Reduction();
	tics = Motor->getTicsPerRev();

    tmp = degs / 360.0; 
    tmp *= reduction;
	tmp *= tics;

    mtr_counts = (int) round( tmp );
	if(verbose)
	    printf("[myTAxis::offsetAxisInDeg] degs= %lf, reduction=%.lf, tics=%.lf, mtr_count=%d\n", degs, reduction, tics, mtr_counts);

    //TSH: should be abs(mtr_counts), deg can comes in negative
    if( mtr_counts > 0 ) {
        if( verbose ) printf( "[myTAxis::offsetAxisInDeg] Axis: %c  Running motor %d\n", m_id, mtr_counts );
        Motor->runEncSteps( mtr_counts );
    } else {
        if( verbose ) printf( "[myTAxis::offsetAxisInDeg] Axis: %c Nothing to do!\n", m_id );
    }

    return mtr_counts;
}


/**
 *  degToCount
 */
int myTAxis::degToCount( double degs )
{
    int mtr_counts;
	double reduction;
	double tics;
	double tmp;

	reduction = Motor->getEncoderToAxis_Reduction();
	tics = Motor->getTicsPerRev();

    tmp = degs / 360.0; 
    tmp *= reduction;
	tmp *= tics;

    mtr_counts = (int) round( tmp );
	if(verbose)
	    printf("[myTAxis::offsetAxisInDeg] degs= %lf, reduction=%.lf, tics=%.lf, mtr_count=%d\n", degs, reduction, tics, mtr_counts);

	return mtr_counts;
}
