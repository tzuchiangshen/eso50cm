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
        logger.logSEVERE("LCUImpl::LCUImpl Telescope could not be configured."); 
    } catch(const Ice::Exception& ex) {
        logger.logSEVERE("LCUImpl::LCUImpl Uncaught exception ."); 
        throw ex;
    } 

    logger.logINFO("LCUImpl::LCUImpl Telescope ready."); 

    //Get telescope configuration & tracking state
    getConfigState();
    getTrackingState();
}

void
LCUImpl::sayHello(int delay, const Ice::Current& c)
{
    if(delay != 0)
    {
        for (int i = 0; i < 100; i++)
        {
            logger.logINFO("LCUImpl::sayHello I'm on the loop!!!!."); 
            IceUtil::ThreadControl::sleep(IceUtil::Time::milliSeconds(delay));
        }
    }
    logger.logINFO("LCUImpl::sayHello Hello World!");
}

int
LCUImpl::readDeviceMemory(int deviceId, int address, int value, const Ice::Current& c)
{
    logger.logFINER("LCUImpl::readDeviceMemory: device id=0x%02X, address=%d, value=%d", (char)deviceId, address, value);

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
        logger.logWARNING("LCUImpl::readDeviceMemory: Invalid device Id. Received=%d", deviceId);

    }
	return value;
}

int
LCUImpl::setDeviceMemory(int deviceId, int address, int value, const Ice::Current& c)
{
    logger.logFINER("LCUImpl::setDeviceMemory: device id=0x%02X, address=%d, value=%d", (char)deviceId, address, value);

    char *p = (char*)&value;
    logger.logFINER("LCUImpl::setDeviceMemory: value[0]=0x%02X", *p);
    p++;
    logger.logFINER("LCUImpl::setDeviceMemory: value[1]=0x%02X", *p);
    p++;
    logger.logFINER("LCUImpl::setDeviceMemory: value[2]=0x%02X", *p);
    p++;
    logger.logFINER("LCUImpl::setDeviceMemory: value[3]=0x%02X", *p);
    p++;

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
        logger.logWARNING("LCUImpl::readDeviceMemory: Invalid device Id. Received=%d", deviceId);
    }
	return value;
}


void
LCUImpl::shutdown(const Ice::Current& c)
{
    logger.logINFO("LCUImpl::shutdown  Shutting down...");
    c.adapter->getCommunicator()->shutdown();
}


