#include <TelescopeImpl.h>

using namespace std;

TelescopeImpl::TelescopeImpl() {
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
