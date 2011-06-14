#include <TelescopeImpl.h>
#include <Ice/Ice.h>

using namespace std;

TelescopeImpl::TelescopeImpl() {
  int status;
  Ice::CommunicatorPtr communicator;

  try
  {
    /* Reading configuration info */
    Ice::InitializationData initData;
    initData.properties = Ice::createProperties();
    initData.properties->load("config");
    communicator = Ice::initialize(initData);

    /* Create proxy */
    Ice::PropertiesPtr properties = communicator->getProperties();
    const char* proxyProperty = "LCUAdapter.Proxy";
    string proxy = properties->getProperty(proxyProperty);
    Ice::ObjectPrx base = communicator->stringToProxy(proxy);
    lcuPrx = OUC::LCUPrx::checkedCast(base->ice_twoway()->ice_timeout(-1));

    if(!lcuPrx)
    {
       cerr << "Can not create reference to LCU Proxy!!" << endl;
       status = EXIT_FAILURE;
    }

  }catch(const Ice::Exception& ex) {
    cerr << ex << endl;
    status = EXIT_FAILURE;
  }
}

void
TelescopeImpl::sayHelloTelescope(int delay, const Ice::Current&)
{
    if(delay != 0)
    {
        IceUtil::ThreadControl::sleep(IceUtil::Time::milliSeconds(delay));
    }
    printf("Hello World Telescope!\n");
}


bool TelescopeImpl::isConfigured(const Ice::Current&)
{
}
 
bool TelescopeImpl::isTracking(const Ice::Current&)
{
}

OUC::RawEncoderData TelescopeImpl::getRawEncodersPosition(const Ice::Current&)
{
}
 
OUC::EncoderData TelescopeImpl::getEncodersPosition(const Ice::Current&)
{
}
 
OUC::TelescopeData TelescopeImpl::getPosition(const Ice::Current&)
{
}

OUC::TelescopeConfigData TelescopeImpl::getConfiguration(const Ice::Current&)
{
}

void TelescopeImpl::setConfiguration(const string& fileName, const Ice::Current&)
{
}

void TelescopeImpl::setTarget(const ::OUC::TelescopePosition& targetPos, const Ice::Current&)
{
}

void TelescopeImpl::setOffset(const ::OUC::TelescopePosition& offsetPos, const Ice::Current&)
{
}

void TelescopeImpl::setTracking(const ::OUC::TrackingInfo& trkInfo, const Ice::Current&)
{
}

void TelescopeImpl::parkTelescope(const Ice::Current&)
{
}
