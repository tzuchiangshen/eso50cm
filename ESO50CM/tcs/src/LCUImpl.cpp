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
	//int alpha_mtr_counts = delay * 3000;
    //printf("\nHello World!\n");
    //OUC::TelescopePosition offset;
	//offset.RA = -30.0;
	//offset.Dec = -10.0;
	//offset = (const ::OUC::TelescopePosition&)offset;
	//setOffset(offset);
	int value = 0;
	printf("value=%d len=%d\n", value, sizeof(value));
	readDeviceMemory(1, 7, value, c);
	printf("value=%d\n", value);
    printf("Bye World!\n");
}

int
LCUImpl::readDeviceMemory(int deviceId, int address, int value, const Ice::Current& c)
{
	printf("[LCUImpl::readDeviceMemory]: device id=0x%02X, address=%d, value=%d \n", (char)deviceId, address, value);

    if (deviceId == 0xA2) {
        //alpha-motor
        m_lcu->telescope->alpha->Motor->readDeviceMemory(address, &value, 0);
    } else if (deviceId == 0xA4) {
		//delta-motor
        m_lcu->telescope->delta->Motor->readDeviceMemory(address, &value, 0);
    } else if (deviceId == 0xA6) {
		//alpha-worm
        m_lcu->telescope->alpha->WormE->readDeviceMemory(address, &value, 0);
    } else if (deviceId == 0xA8) {
		//alpha-axis
        m_lcu->telescope->alpha->AxisE->readDeviceMemory(address, &value, 0);
    } else if (deviceId == 0xAA) {
		//delta-worm
        m_lcu->telescope->delta->WormE->readDeviceMemory(address, &value, 0);
    } else if (deviceId == 0xAC) {
		//delta-axis
        m_lcu->telescope->delta->AxisE->readDeviceMemory(address, &value, 0);
    } else {
        printf(" invalid device Id. Received=%d\n", deviceId);
    }
	return value;
}

int
LCUImpl::setDeviceMemory(int deviceId, int address, int value, const Ice::Current& c)
{
	printf("[LCUImpl::readDeviceMemory]: device id=0x%02X, address=%d, value=%d \n", (char)deviceId, address, value);

    if (deviceId == 0xA2) {
        //alpha-motor
        m_lcu->telescope->alpha->Motor->setDeviceMemory(address, &value, 0);
    } else if (deviceId == 0xA4) {
		//delta-motor
        m_lcu->telescope->delta->Motor->setDeviceMemory(address, &value, 0);
    } else if (deviceId == 0xA6) {
		//alpha-worm
        m_lcu->telescope->alpha->WormE->setDeviceMemory(address, &value, 0);
    } else if (deviceId == 0xA8) {
		//alpha-axis
        m_lcu->telescope->alpha->AxisE->setDeviceMemory(address, &value, 0);
    } else if (deviceId == 0xAA) {
		//delta-worm
        m_lcu->telescope->delta->WormE->setDeviceMemory(address, &value, 0);
    } else if (deviceId == 0xAC) {
		//delta-axis
        m_lcu->telescope->delta->AxisE->setDeviceMemory(address, &value, 0);
    } else {
        printf(" invalid device Id. Received=%d\n", deviceId);
    }
	return value;
}


void
LCUImpl::shutdown(const Ice::Current& c)
{
    logger.logINFO("Shutting down...");
    c.adapter->getCommunicator()->shutdown();
}


