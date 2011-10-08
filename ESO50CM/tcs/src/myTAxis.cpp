#include <myTAxis.h>


/**
 *  myTAxis
 */
myTAxis::myTAxis( char id, struct my_TAxis_data_t * axis ):
    logger("myTAxis")
{
    m_id = id;
    //logger.logFINE("myTAxis::myTAxis %c Hello World!", m_id );
    //logger.logFINE("myTAxis::myTAxis %c axis_data at %p", m_id, (void *) axis );
    //logger.logFINE("myTAxis::myTAxis %c motorE at    %p", m_id, (void *) & axis->motorE );
    //logger.logFINE("myTAxis::myTAxis %c wormE at     %p", m_id, (void *) & axis->wormE );
    //logger.logFINE("myTAxis::myTAxis %c axisE at     %p", m_id, (void *) & axis->axisE );

    Motor = new myTMotor( m_id, 'M',  & axis->motorE );
    WormE = new myTEncoder( m_id, 'W', & axis->wormE );
    AxisE = new myTEncoder( m_id, 'A', & axis->axisE );
}

/**
 *  ~myTAxis
 */
myTAxis::~myTAxis() {
    delete Motor;
    delete WormE;
    delete AxisE;
    //logger.logFINE("myTAxis::~myTAxis %c Good Bye!", m_id );
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

    degs /= 360.0;
    degs *= Motor->getEncoderToAxis_Reduction();
    degs *= Motor->getTicsPerRev();

    mtr_counts = (int) round( degs );
    if( mtr_counts > 0 ) {
        //logger.logFINE("myTAxis::offsetAxisInDeg Running motor %d", mtr_counts);
        Motor->runEncSteps( mtr_counts );
    } else {
        logger.logFINE("myTAxis::offsetAxisInDeg Nothing to do!");
    }
    return mtr_counts;
}



