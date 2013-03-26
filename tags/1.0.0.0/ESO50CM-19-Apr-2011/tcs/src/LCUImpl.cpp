
#include <LCUImpl.h>
#include <IceE/IceE.h>

using namespace std;

LCUImpl::LCUImpl() {
    m_lcu = new myLCU();
    m_lcu->createTelescope();
    m_lcu->telescope->attachInstrumentMemory();
}

void
LCUImpl::sayHello(int delay, const Ice::Current&) const
{
    if(delay != 0)
    {
        IceUtil::ThreadControl::sleep(IceUtil::Time::milliSeconds(delay));
    }
    printf("Hello World!\n");
}

void
LCUImpl::shutdown(const Ice::Current& c)
{
    printf("Shutting down...\n");
    c.adapter->getCommunicator()->shutdown();
}

void LCUImpl::getEncodersPosition (const Ice::Current& c)
{
    extern int verbose;
    char string_data[32];

    if( verbose ) {
        printf( "[LCUImpl::get_encoder_position] Hello world!\n" );
        //if( * arguments != 0 ) {
        //    printf( "[LCUImpl::get_encoder_position] %s\n", arguments );
        //}
    }
    //myLCU * lcu;
    //lcu = new myLCU();
    //m_lcu->createTelescope();
    double lst, ra, dec, alt, az;

    /** We will wait untill we get the access to the
        User Shared Memory segment. */
    m_lcu->waitSemaphore();
    /** We have gor access to the User Shared Memory segment
        then we can translate low level telescope data
        to user's format.
        The corresponding telescope routine waits until it
        have access to Instrument Shared memory
        segment, and after read it will free the acces. */
    m_lcu->telescope->currentPosition( & lst, & ra, & dec, &alt, &az );
    printf( "[LCUImpl::get_encoder_position] No HTML output!\n" );
    /** Local Time */
    strftime( string_data, 32, "%Y-%m-%d, %T", m_lcu->telescope->getLocalTime() );
    printf( "[LCUImpl::get_encoder_position] LT  = [%s]\n", string_data );
    printf( "[LCUImpl::get_encoder_position] AW Position  = [%+10.0lf]\n", m_lcu->telescope->alpha->WormE->getPosition() );
    printf( "[LCUImpl::get_encoder_position] AA Position  = [%+10.0lf]\n", m_lcu->telescope->alpha->AxisE->getPosition() );
    printf( "[LCUImpl::get_encoder_position] DW Position  = [%+10.0lf]\n", m_lcu->telescope->delta->WormE->getPosition() );
    printf( "[LCUImpl::get_encoder_position] DA Position  = [%+10.0lf]\n", m_lcu->telescope->delta->AxisE->getPosition() );

    /** LCU Local Time */
    printf( "[LCUImpl::get_encoder_position] --------------------------------\n" );
    strftime( string_data, 32, "%Y-%m-%d, %T", m_lcu->getLCU_Time() );
    printf( "[LCUImpl::get_encoder_position] Page generated at: [%s]\n", string_data );
    m_lcu->postSemaphore();
    //delete lcu;

    if( verbose) printf( "[LCUImpl::get_encoder_position] Good bye!\n" );
}
