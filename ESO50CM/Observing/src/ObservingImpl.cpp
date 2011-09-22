
#include <ObservingImpl.h>
#include <TelescopeImpl.h>
#include <Ice/Ice.h>

using namespace std;

ObservingImpl::ObservingImpl(const Ice::ObjectAdapterPtr& adapter) : logger("ObservingControl"),
    _telescope(new TelescopeImpl),
    _telescopePrx(OUC::TelescopePrx::uncheckedCast(adapter->addWithUUID(_telescope)))
{
}

void
ObservingImpl::sayHello(int delay, const Ice::Current& c)
{
    if(delay != 0)
    {
        IceUtil::ThreadControl::sleep(IceUtil::Time::milliSeconds(delay));
    }
    logger.logINFO("Hello World!");
}

void
ObservingImpl::shutdown(const Ice::Current& c)
{
    logger.logINFO("Shutting down...");
    c.adapter->getCommunicator()->shutdown();
}


OUC::TelescopePrx
ObservingImpl::getTelescope(const Ice::Current& c)
{
    logger.logINFO("Returning Telescope Reference");
    return _telescopePrx;
}
