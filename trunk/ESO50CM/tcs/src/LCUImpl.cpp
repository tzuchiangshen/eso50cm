
#include <LCUImpl.h>
#include <IceE/IceE.h>

using namespace std;

LCUImpl::LCUImpl() {
    m_lcu = new myLCU();
    encoder_t = new OUC::EncoderData();
    m_lcu->createTelescope();
    m_lcu->telescope->attachInstrumentMemory();

    //Get telescope configuration
    getConfigState();
}

void
LCUImpl::sayHello(int delay, const Ice::Current&)
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


