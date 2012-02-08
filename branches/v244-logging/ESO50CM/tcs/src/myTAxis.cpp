#include <myTAxis.h>


/**
 *  myTAxis
 */
myTAxis::myTAxis( char id, struct my_TAxis_data_t * axis, LoggerHelper *logLCUImpl )
{
    logger = logLCUImpl;
    m_id = id;
    logger->logINFO( "[myTAxis::myTAxis] %c Hello World!\n", m_id );
    logger->logINFO( "[myTAxis::myTAxis] %c axis_data at %p\n", m_id, (void *) axis );
    logger->logINFO( "[myTAxis::myTAxis] %c motorE at    %p\n", m_id, (void *) & axis->motorE );
    logger->logINFO( "[myTAxis::myTAxis] %c wormE at     %p\n", m_id, (void *) & axis->wormE );
    logger->logINFO( "[myTAxis::myTAxis] %c axisE at     %p\n", m_id, (void *) & axis->axisE );

    Motor = new myTMotor( m_id, 'M',  & axis->motorE, logger );
    WormE = new myTEncoder( m_id, 'W', & axis->wormE, logger );
    AxisE = new myTEncoder( m_id, 'A', & axis->axisE, logger );
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
    logger->logINFO( "[myTAxis::~myTAxis] %c Good Bye!\n", m_id );
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
    logger->logINFO("[myTAxis::offsetAxisInDeg] degs= %lf, reduction=%.lf, tics=%.lf, mtr_count=%d\n", degs, reduction, tics, mtr_counts);

    //TSH: should be abs(mtr_counts), deg can comes in negative
    if( mtr_counts > 0 ) {
        logger->logINFO( "[myTAxis::offsetAxisInDeg] Axis: %c  Running motor %d\n", m_id, mtr_counts );
        Motor->runEncSteps( mtr_counts );
    } else {
        logger->logINFO( "[myTAxis::offsetAxisInDeg] Axis: %c Nothing to do!\n", m_id );
    }

    return mtr_counts;
}


/**
 *  degToCountMotorEnc
 */
int myTAxis::degToCountMotorEnc( double degs )
{
    int mtr_counts;
	double reduction;
	double tics;
	double home;
	double tmp;

	reduction = Motor->getEncoderToAxis_Reduction();
	tics = Motor->getTicsPerRev();
	home = Motor->getHomePosition();

    tmp = degs / 360.0; 
    tmp *= reduction;
	tmp *= tics;

    mtr_counts = (int) round( tmp );
    logger->logINFO("[myTAxis::degToCountMotorEnc] axis=%c,  degs= %lf, reduction=%.lf, tics=%.lf, home=%.lf, mtr_count=%d\n", m_id, degs, reduction, tics, home, mtr_counts);

    return mtr_counts;
}


/**
 *  degToCountAxisEnc
 */
int myTAxis::degToCountAxisEnc( double degs )
{
    int mtr_counts;
	double reduction;
	double tics;
	double home;
	double tmp;

	reduction = AxisE->getEncoderToAxis_Reduction();
	tics = AxisE->getTicsPerRev();
	home = AxisE->getHomePosition();

    tmp = degs / (360.0/reduction); 
	tmp *= tics;
	tmp += home;

    mtr_counts = (int) round( tmp );
    logger->logINFO("[myTAxis::degToCountAxisEnc] axis=%c,  degs= %lf, reduction=%.lf, tics=%.lf, home=%.lf, mtr_count=%d\n", m_id, degs, reduction, tics, home, mtr_counts);
    
    return mtr_counts;
}


/**
 *  degToCountWormEnc
 */
int myTAxis::degToCountWormEnc( double degs )
{
    int mtr_counts;
	double reduction;
	double tics;
	double home;
	double tmp;

	reduction = WormE->getEncoderToAxis_Reduction();
	tics = WormE->getTicsPerRev();
	home = WormE->getHomePosition();

    tmp = degs / (360.0/reduction); 
	tmp *= tics;
	tmp += home;

    mtr_counts = (int) round( tmp );
    logger->logINFO("[myTAxis::degToCountWormEnc] axis=%c,  degs= %lf, reduction=%.lf, tics=%.lf, home=%.lf, mtr_count=%d\n", m_id, degs, reduction, tics, home, mtr_counts);

    return mtr_counts;
}


