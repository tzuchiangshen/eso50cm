#include "LCU.h"
#include <LCUImpl.h>


using namespace std;

LCUImpl::LCUImpl(): 
    logger("LCUControl"),
    telConfigFileName("/eso50cm/SWROOT/config/ESO50cm.conf")
{
    logger.logINFO("LCUImpl::LCUImpl LCUControl process started!!");
    m_lcu = new myLCU();
    rawEncoder_t = new OUC::RawEncoderData(); 
    encoder_t = new OUC::EncoderData();
    telescopeConfigData_t = new OUC::TelescopeConfigData();
    telescopeData_t = new OUC::TelescopeData();
    m_lcu->createTelescope();
    m_lcu->telescope->attachInstrumentMemory();

    // Configure Telescope
    string configPath =  getenv("SWROOT");
    configPath = configPath + "/config/" + telConfigFileName;
    try {
        setConfiguration(configPath);
    } catch (OUC::NotConfigurationFileEx& ex) {
        logger.logSEVERE("LCUImpl::LCUImpl Telescope could not be configured."); 
    } catch(const Ice::Exception& ex) {
        logger.logSEVERE("LCUImpl::LCUImpl Uncaught exception ."); 
        throw ex;
    }

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
    logger.logINFO("LCUImpl::sayHello Hello World!\n");
}

void
LCUImpl::shutdown(const Ice::Current& c)
{
    logger.logINFO("LCUImpl::shutdown Shutting down...\n");
    c.adapter->getCommunicator()->shutdown();
}


