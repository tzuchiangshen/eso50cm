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
        logger.logINFO("Got reference to LCUControl at " + proxy);

        if(!lcuPrx)
        {
            OUC::NotLCUReferenceAvailableEx ex;
            ex.reason = "Not reference to LCU Proxy available!";
            throw ex;
        }
    }catch(const Ice::Exception& ex) {
        cerr << ex << endl;
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
    logger.logFINE("Hello World Telescope!\n");
}


bool TelescopeImpl::isConfigured(const Ice::Current&)
{
    extern int verbose;

    if( verbose ) 
        logger.logFINE( "TelescopeImpl::isConfigured" ); 

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
        cerr << "Unexpected run-time error: " << ex << endl;
        throw ex;
    }
}
 
bool TelescopeImpl::isTracking(const Ice::Current&)
{
    extern int verbose;

    if( verbose ) 
        logger.logFINE( "TelescopeImpl::isTracking" ); 

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
        cerr << "Unexpected run-time error: " << ex << endl;
        throw ex;
    }
}

OUC::RawEncoderData TelescopeImpl::getRawEncodersPosition(const Ice::Current&)
{
    extern int verbose;

    if( verbose ) 
        logger.logFINE( "TelescopeImpl::getRawEncodersPosition" ); 

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
        cout << "LCU returned: " << ex.reason << "Execute setConfiguration method first !!"<< endl;  
        throw ex;
    } catch (Ice::Exception& ex) {
        cerr << "Unexpected run-time error: " << ex << endl;
        throw ex;
    }
}
 
OUC::EncoderData TelescopeImpl::getEncodersPosition(const Ice::Current&)
{
    extern int verbose;

    if( verbose ) 
        logger.logFINE( "TelescopeImpl::getEncodersPosition" ); 

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
        cout << "LCU returned: " << ex.reason << "Execute setConfiguration method first !!"<< endl;  
        throw ex;
    } catch (Ice::Exception& ex) {
        cerr << "Unexpected run-time error: " << ex << endl;
        throw ex;
    }  
}
 
OUC::TelescopeData TelescopeImpl::getPosition(const Ice::Current&)
{
    extern int verbose;

    if( verbose ) 
        logger.logFINE( "TelescopeImpl::getPosition" ); 

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
        cout << "LCU returned: " << ex.reason << "Execute setConfiguration method first !!"<< endl;  
        throw ex;
    } catch (Ice::Exception& ex) {
        cerr << "Unexpected run-time error: " << ex << endl;
        throw ex;
    }
}

OUC::TelescopeConfigData TelescopeImpl::getConfiguration(const Ice::Current&)
{
    extern int verbose;

    if( verbose ) 
        logger.logFINE( "TelescopeImpl::getConfiguration" ); 

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
        cout << "LCU returned: " << ex.reason << "Execute setConfiguration method first !!"<< endl;  
        throw ex;
    } catch (Ice::Exception& ex) {
        cerr << "Unexpected run-time error: " << ex << endl;
        throw ex;
    }
}

void TelescopeImpl::setConfiguration(const string& fileName, const Ice::Current&)
{
    extern int verbose;

    if( verbose ) 
        logger.logFINE( "TelescopeImpl::setConfiguration" ); 

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
        cerr << "Unexpected run-time error: " << ex << endl;
        throw ex;
    }
}

void TelescopeImpl::setTarget(const ::OUC::TelescopePosition& targetPos, const Ice::Current&)
{
    extern int verbose;

    if( verbose ) 
        logger.logFINE( "TelescopeImpl::setTarget" ); 

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
        cout << "LCU returned: " << ex.reason << "Execute setConfiguration method first !!"<< endl;  
        throw ex;
    } catch (OUC::TargetOutOfLimitsEx& ex) {
        cout << "LCU returned: " << ex.reason << "Try another target!!"<< endl;  
        throw ex;
    } catch (Ice::Exception& ex) {
        cerr << "Unexpected run-time error: " << ex << endl;
        throw ex;
    }
}

void TelescopeImpl::handsetSlew(const ::OUC::SlewInfo& slewInfo, const Ice::Current&)
{
    extern int verbose;

    if( verbose ) 
        logger.logFINE( "TelescopeImpl::handsetSlew" ); 

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
        cout << "LCU returned: " << ex.reason << "Execute setConfiguration method first !!"<< endl;  
        throw ex;
    } catch (OUC::TargetOutOfLimitsEx& ex) {
        cout << "LCU returned: " << ex.reason << "Try another target!!"<< endl;  
        throw ex;
    } catch (Ice::Exception& ex) {
        cerr << "Unexpected run-time error: " << ex << endl;
        throw ex;
    }
}

void TelescopeImpl::setOffset(const ::OUC::TelescopePosition& offsetPos, const Ice::Current&)
{
    extern int verbose;

    if( verbose ) 
        logger.logFINE( "TelescopeImpl::setOffset" ); 

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
        cout << "LCU returned: " << ex.reason << "Execute setConfiguration method first !!"<< endl;  
        throw ex;
    } catch (OUC::TargetOutOfLimitsEx& ex) {
        cout << "LCU returned: " << ex.reason << "Try another target!!"<< endl;  
        throw ex;
    } catch (Ice::Exception& ex) {
        cerr << "Unexpected run-time error: " << ex << endl;
        throw ex;
    }
}

void TelescopeImpl::setTracking(const OUC::TrackingInfo& trkInfo, const Ice::Current&)
{
    extern int verbose;

    if( verbose ) 
        logger.logFINE( "TelescopeImpl::setTracking" ); 

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
        cout << "LCU returned: " << ex.reason << "Execute setConfiguration method first !!"<< endl;  
        throw ex;
    } catch (Ice::Exception& ex) {
        cerr << "Unexpected run-time error: " << ex << endl;
        throw ex;
    }
}

void TelescopeImpl::parkTelescope(const Ice::Current&)
{
    extern int verbose;

    if( verbose ) 
        logger.logFINE( "TelescopeImpl::parkTelescope" ); 

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
        cout << "LCU returned: " << ex.reason << "Execute setConfiguration method first !!"<< endl;  
        throw ex;
    } catch (Ice::Exception& ex) {
        cerr << "Unexpected run-time error: " << ex << endl;
        throw ex;
    }
}

void TelescopeImpl::stopTelescope(const ::OUC::TelescopeDirection dir, const Ice::Current&)
{
    extern int verbose;

    if( verbose ) 
        logger.logFINE( "TelescopeImpl::stopTelescope" ); 

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
        cout << "LCU returned: " << ex.reason << "Execute setConfiguration method first !!"<< endl;  
        throw ex;
    } catch (Ice::Exception& ex) {
        cerr << "Unexpected run-time error: " << ex << endl;
        throw ex;
    }
}

void TelescopeImpl::moveToTarget(const Ice::Current&)
{
    extern int verbose;

    if( verbose ) 
        logger.logFINE( "TelescopeImpl::moveToTarget" ); 

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
        cout << "LCU returned: " << ex.reason << "Execute setConfiguration method first !!"<< endl;  
        throw ex;
    } catch (Ice::Exception& ex) {
        cerr << "Unexpected run-time error: " << ex << endl;
        throw ex;
    }
}
