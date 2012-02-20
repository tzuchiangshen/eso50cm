#include <TelescopeImpl.h>
#include <Ice/Ice.h>

using namespace std;

TelescopeImpl::TelescopeImpl(): logger("TelescopeImpl") {
    Ice::CommunicatorPtr communicator;

    try
    {
        /* Reading configuration info */
        string configPath =  getenv("SWROOT");
        configPath = configPath + "/config/LCU-config";
        Ice::InitializationData initData;
        initData.properties = Ice::createProperties();
        initData.properties->load(configPath);
        communicator = Ice::initialize(initData);

        /* Create proxy */
        Ice::PropertiesPtr properties = communicator->getProperties();
        const char* proxyProperty = "LCUAdapter.Proxy";
        string proxy = properties->getProperty(proxyProperty);
        Ice::ObjectPrx base = communicator->stringToProxy(proxy);
        lcuPrx = OUC::LCUPrx::checkedCast(base->ice_twoway()->ice_timeout(-1));
        logger.logFINE("TelescopeImpl::TelescopeImp: Got reference to LCUControl at " + proxy);

        if(!lcuPrx)
        {
            OUC::NotLCUReferenceAvailableEx ex;
            ex.reason = "Not reference to LCU Proxy available!";
            logger.logSEVERE("TelescopeImpl::TelescopeImp: Not reference to LCU Proxy available!");
            throw ex;
        }
    } catch(const Ice::Exception& ex) {
        logger.logSEVERE("TelescopeImpl::TelescopeImp: Unexpected run-time error");
        throw ex;
    }
}

void
TelescopeImpl::sayHelloTelescope(int delay, const Ice::Current&)
{
    if(delay != 0)
    {
        IceUtil::ThreadControl::sleep(IceUtil::Time::milliSeconds(delay));
    }
    logger.logFINE("TelescopeImpl::sayHelloTelescope: Hello World Telescope!\n");
}


bool TelescopeImpl::isConfigured(const Ice::Current&)
{
    logger.logFINEST( "TelescopeImpl::isConfigured" ); 

    try
    {
        if(!lcuPrx)
        {
            OUC::NotLCUReferenceAvailableEx ex;
            ex.reason = "Not reference to LCU Proxy available!";
            throw ex;	
        }

        return lcuPrx->isConfigured();
    } catch (Ice::Exception& ex) {
        logger.logSEVERE("TelescopeImpl::isConfigured: Unexpected run-time error");
        throw ex;
    }
}
 
bool TelescopeImpl::isTracking(const Ice::Current&)
{
    logger.logFINEST( "TelescopeImpl::isTracking" ); 

    try
    {
        if(!lcuPrx)
        {
            OUC::NotLCUReferenceAvailableEx ex;
            ex.reason = "Not reference to LCU Proxy available!";
            throw ex;	
        }

        return lcuPrx->isTracking();
    } catch (Ice::Exception& ex) {
        logger.logSEVERE("TelescopeImpl::isTracking: Unexpected run-time error");
        throw ex;
    }
}

OUC::RawEncoderData TelescopeImpl::getRawEncodersPosition(const Ice::Current&)
{
    logger.logFINEST( "TelescopeImpl::getRawEncodersPosition" ); 

    try
    {
        if(!lcuPrx)
        {
            OUC::NotLCUReferenceAvailableEx ex;
            ex.reason = "Not reference to LCU Proxy available!";
            throw ex;	
        }

        return  lcuPrx->getRawEncodersPosition();
    } catch (OUC::TelescopeNotConfiguredEx& ex) {
      logger.logSEVERE("TelescopeImpl::getRawEncodersPosition: LCU returned: %s. Execute setConfiguration method first", &ex.reason);  
        throw ex;
    } catch (Ice::Exception& ex) {
        logger.logSEVERE("TelescopeImpl::getRawEncodersPosition: Unexpected run-time error");
        throw ex;
    }
}
 
OUC::EncoderData TelescopeImpl::getEncodersPosition(const Ice::Current&)
{
    logger.logFINEST( "TelescopeImpl::getEncodersPosition" ); 

    try
    {
        if(!lcuPrx)
        {
            OUC::NotLCUReferenceAvailableEx ex;
            ex.reason = "Not reference to LCU Proxy available!";
            throw ex;	
        }

        return  lcuPrx->getEncodersPosition();
    } catch (OUC::TelescopeNotConfiguredEx& ex) {
      logger.logSEVERE("TelescopeImpl::getEncodersPosition: LCU returned: %s. Execute setConfiguration method first",&ex.reason);
        throw ex;
    } catch (Ice::Exception& ex) {
        logger.logSEVERE("TelescopeImpl::getEncodersPosition: Unexpected run-time error: %s");
        throw ex;
    }  
}
 
OUC::TelescopeData TelescopeImpl::getPosition(const Ice::Current&)
{

    logger.logFINEST( "TelescopeImpl::getPosition" ); 

    try
    {
        if(!lcuPrx)
        {
            OUC::NotLCUReferenceAvailableEx ex;
            ex.reason = "Not reference to LCU Proxy available!";
            throw ex;	
        }

        return  lcuPrx->getPosition();
    } catch (OUC::TelescopeNotConfiguredEx& ex) {
        logger.logSEVERE("TelescopeImpl::getPosition: LCU returned: %s. Execute setConfiguration method first", &ex.reason);
        throw ex;
    } catch (Ice::Exception& ex) {
        logger.logSEVERE("TelescopeImpl::getPosition: Unexpected run-time error");
        throw ex;
    }
}

OUC::TelescopeConfigData TelescopeImpl::getConfiguration(const Ice::Current&)
{
    logger.logFINEST( "TelescopeImpl::getConfiguration" ); 

    try
    {
        if(!lcuPrx)
        {
            OUC::NotLCUReferenceAvailableEx ex;
            ex.reason = "Not reference to LCU Proxy available!";
            throw ex;	
        }

        return  lcuPrx->getConfiguration();
    } catch (OUC::TelescopeNotConfiguredEx& ex) {
        logger.logSEVERE("TelescopeImpl::getConfiguration: LCU returned: %s. Execute setConfiguration method first", &ex.reason);
        throw ex;
    } catch (Ice::Exception& ex) {
        logger.logSEVERE("TelescopeImpl::getConfiguration: Unexpected run-time error");
        throw ex;
    }
}

void TelescopeImpl::setConfiguration(const string& fileName, const Ice::Current&)
{
    logger.logFINEST( "TelescopeImpl::setConfiguration" ); 

    try
    {
        if(!lcuPrx)
        {
            OUC::NotLCUReferenceAvailableEx ex;
            ex.reason = "Not reference to LCU Proxy available!";
            throw ex;	
        }

        lcuPrx->setConfiguration(fileName); 
    } catch (Ice::Exception& ex) {
        logger.logSEVERE("TelescopeImpl::setConfiguration: Unexpected run-time error.");
        throw ex;
    }
}

void TelescopeImpl::setTarget(const ::OUC::TelescopePosition& targetPos, const Ice::Current&)
{
    logger.logFINEST( "TelescopeImpl::setTarget" ); 

    try
    {
        if(!lcuPrx)
        {
            OUC::NotLCUReferenceAvailableEx ex;
            ex.reason = "Not reference to LCU Proxy available!";
            throw ex;	
        }

        return  lcuPrx->setTarget(targetPos);
    } catch (OUC::TelescopeNotConfiguredEx& ex) {
        logger.logSEVERE("TelescopeImpl::setTarget: LCU returned: %s. Execute setConfiguration method first", ex.reason.c_str());
        throw ex;
    } catch (OUC::TargetOutOfLimitsEx& ex) {
        logger.logSEVERE("TelescopeImpl::setTarget: LCU returned: %s. Try another target!!", ex.reason.c_str());
        throw ex;
    } catch (Ice::Exception& ex) {
        logger.logSEVERE("TelescopeImpl::setTarget: Unexpected run-time error");
        throw ex;
    }
}

void TelescopeImpl::handsetSlew(const ::OUC::SlewInfo& slewInfo, const Ice::Current&)
{
    logger.logFINEST( "TelescopeImpl::handsetSlew" ); 

    try
    {
        if(!lcuPrx)
        {
            OUC::NotLCUReferenceAvailableEx ex;
            ex.reason = "Not reference to LCU Proxy available!";
            throw ex;	
        }

        return  lcuPrx->handsetSlew(slewInfo);
    } catch (OUC::TelescopeNotConfiguredEx& ex) {
        logger.logSEVERE("TelescopeImpl::handsetSlew: LCU returned: %s. Execute setConfiguration method first", &ex.reason);
        throw ex;
    } catch (OUC::TargetOutOfLimitsEx& ex) {
        logger.logSEVERE("TelescopeImpl::handsetSlew: LCU returned: %s. Try another target!!", &ex.reason);
        throw ex;
    } catch (Ice::Exception& ex) {
        logger.logSEVERE("TelescopeImpl::handsetSlew: Unexpected run-time error");
        throw ex;
    }
}

void TelescopeImpl::setOffset(const ::OUC::TelescopePosition& offsetPos, const Ice::Current&)
{
    logger.logFINEST( "TelescopeImpl::setOffset" ); 

    try
    {
        if(!lcuPrx)
        {
            OUC::NotLCUReferenceAvailableEx ex;
            ex.reason = "Not reference to LCU Proxy available!";
            throw ex;	
        }

        return  lcuPrx->setOffset(offsetPos);
    } catch (OUC::TelescopeNotConfiguredEx& ex) {
        logger.logSEVERE("TelescopeImpl::setOffset: LCU returned: %s. Execute setConfiguration method first", &ex.reason);
        throw ex;
    } catch (OUC::TargetOutOfLimitsEx& ex) {
        logger.logSEVERE("TelescopeImpl::setOffset: LCU returned: %s. Try another target!!", &ex.reason);
        throw ex;
    } catch (Ice::Exception& ex) {
        logger.logSEVERE("TelescopeImpl::setOffset: Unexpected run-time error");
        throw ex;
    }
}

void TelescopeImpl::setTracking(const OUC::TrackingInfo& trkInfo, const Ice::Current&)
{
    logger.logFINEST( "TelescopeImpl::setTracking" ); 

    try
    {
        if(!lcuPrx)
        {
            OUC::NotLCUReferenceAvailableEx ex;
            ex.reason = "Not reference to LCU Proxy available!";
            throw ex;	
        }

        return  lcuPrx->setTracking(trkInfo);
    } catch (OUC::TelescopeNotConfiguredEx& ex) {
        logger.logSEVERE("TelescopeImpl::setTracking: LCU returned: %s. Execute setConfiguration method first", &ex.reason);
        throw ex;
    } catch (Ice::Exception& ex) {
        logger.logSEVERE("TelescopeImpl::setTracking: Unexpected run-time error");
        throw ex;
    }
}

void TelescopeImpl::parkTelescope(const Ice::Current&)
{
    logger.logFINEST( "TelescopeImpl::parkTelescope" ); 

    try
    {
        if(!lcuPrx)
        {
            OUC::NotLCUReferenceAvailableEx ex;
            ex.reason = "Not reference to LCU Proxy available!";
            throw ex;	
        }

        return  lcuPrx->parkTelescope();
    } catch (OUC::TelescopeNotConfiguredEx& ex) {
        logger.logSEVERE("TelescopeImpl::parkTelescope: LCU returned: %s. Execute setConfiguration method first", &ex.reason);
        throw ex;
    } catch (Ice::Exception& ex) {
        logger.logSEVERE("TelescopeImpl::parkTelescope: Unexpected run-time error");
        throw ex;
    }
}

void TelescopeImpl::stopTelescope(const ::OUC::TelescopeDirection dir, const Ice::Current&)
{
    logger.logFINEST( "TelescopeImpl::stopTelescope" ); 

    try
    {
        if(!lcuPrx)
        {
            OUC::NotLCUReferenceAvailableEx ex;
            ex.reason = "Not reference to LCU Proxy available!";
            throw ex;	
        }

        return  lcuPrx->stopTelescope(dir);
    } catch (OUC::TelescopeNotConfiguredEx& ex) {
        logger.logSEVERE("TelescopeImpl::stopTelescope: LCU returned: %s. Execute setConfiguration method first", &ex.reason);
        throw ex;
    } catch (Ice::Exception& ex) {
        logger.logSEVERE("TelescopeImpl::stopTelescope: Unexpected run-time error");
        throw ex;
    }
}

void TelescopeImpl::moveToTarget(const Ice::Current&)
{
    logger.logFINEST( "TelescopeImpl::moveToTarget" ); 

    try
    {
        if(!lcuPrx)
        {
            OUC::NotLCUReferenceAvailableEx ex;
            ex.reason = "Not reference to LCU Proxy available!";
            throw ex;	
        }

        return  lcuPrx->moveToTarget();
    } catch (OUC::TelescopeNotConfiguredEx& ex) {
        logger.logSEVERE("TelescopeImpl::moveToTarget: LCU returned: %s. Execute setConfiguration method first", &ex.reason);
        throw ex;
    } catch (Ice::Exception& ex) {
        logger.logSEVERE("TelescopeImpl::moveToTarget: Unexpected run-time error");
        throw ex;
    }
}
