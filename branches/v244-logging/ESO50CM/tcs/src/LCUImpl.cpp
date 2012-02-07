#include "LCU.h"
#include <LCUImpl.h>


using namespace std;

LCUImpl::LCUImpl(): 
  telConfigFileName("ESO50cm.conf"),
  logger("LCUControl")
{
    m_lcu = new myLCU(&logger);
    rawEncoder_t = new OUC::RawEncoderData(); 
    encoder_t = new OUC::EncoderData();
    telescopeConfigData_t = new OUC::TelescopeConfigData();
    telescopeData_t = new OUC::TelescopeData();
    m_lcu->createTelescope();
    m_lcu->telescope->attachInstrumentMemory();

    //Configure Telescope
    string configPath =  getenv("SWROOT");
    configPath = configPath + "/config/" + telConfigFileName;
    try {
        setConfiguration(configPath);
    } catch (OUC::NotConfigurationFileEx& ex) {
        logger.logINFO("LCUImpl::LCUImpl Telescope could not be configured."); 
    } catch(const Ice::Exception& ex) {
        logger.logINFO("LCUImpl::LCUImpl Uncaught exception ."); 
        throw ex;
    } 

    //Get telescope configuration & tracking state
    getConfigState();
    getTrackingState();
}

void
LCUImpl::sayHello(int delay, const Ice::Current& c)
{
    if(delay != 0)
    {
        IceUtil::ThreadControl::sleep(IceUtil::Time::milliSeconds(delay));
    }
	int alpha_mtr_counts = delay * 3000;
    logger.logINFO("Hello World!");
    OUC::TelescopePosition offset;
	offset.RA = -30.0;
	offset.Dec = -10.0;
	offset = (const ::OUC::TelescopePosition&)offset;
	setOffset(offset);
    logger.logINFO("Bye World!\n");
}

void
LCUImpl::shutdown(const Ice::Current& c)
{
    logger.logINFO("Shutting down...");
    c.adapter->getCommunicator()->shutdown();
}


