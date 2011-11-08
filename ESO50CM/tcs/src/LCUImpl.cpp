#include "LCU.h"
#include <LCUImpl.h>


using namespace std;

LCUImpl::LCUImpl(): 
    logger("LCUControl")
{
    logger.logINFO("LCUControl process started!!");
    m_lcu = new myLCU();
    rawEncoder_t = new OUC::RawEncoderData(); 
    encoder_t = new OUC::EncoderData();
    telescopeConfigData_t = new OUC::TelescopeConfigData();
    telescopeData_t = new OUC::TelescopeData();
    m_lcu->createTelescope();
    m_lcu->telescope->attachInstrumentMemory();

    const string fileName("/eso50cm/SWROOT/config/ESO50cm.conf");
    setConfiguration(fileName);

    //Get telescope configuration & tracking state
    getConfigState();
    getTrackingState();
}

void
LCUImpl::sayHello(int delay, const Ice::Current&)
{
  if(delay != 0)
    {
        IceUtil::ThreadControl::sleep(IceUtil::Time::milliSeconds(delay));
    }
    logger.logINFO("Hello World!\n");
}

void
LCUImpl::shutdown(const Ice::Current& c)
{
    logger.logINFO("Shutting down...\n");
    c.adapter->getCommunicator()->shutdown();
}


