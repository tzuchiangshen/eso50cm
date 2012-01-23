#include "LCU.h"
#include <LCUImpl.h>


using namespace std;

LCUImpl::LCUImpl(): 
    telConfigFileName("ESO50cm.conf")    
{
    m_lcu = new myLCU();
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
        printf("LCUImpl::LCUImpl Telescope could not be configured."); 
    } catch(const Ice::Exception& ex) {
        printf("LCUImpl::LCUImpl Uncaught exception ."); 
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
    printf("\nHello World!\n");
    OUC::TelescopePosition offset;
	offset.RA = -30.0;
	offset.Dec = -10.0;
	offset = (const ::OUC::TelescopePosition&)offset;
	setOffset(offset);
    printf("Bye World!\n");
}

void
LCUImpl::shutdown(const Ice::Current& c)
{
    printf("Shutting down...\n");
    c.adapter->getCommunicator()->shutdown();
}


