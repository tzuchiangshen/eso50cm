#include <ObservingImpl.h>
#include <TelescopeImpl.h>
#include <Ice/Ice.h>

using namespace std;

ObservingImpl::ObservingImpl(const Ice::ObjectAdapterPtr& adapter) : logger("ObservingControl"),
    _adapter(adapter),
    _telescope(new TelescopeImpl),
    _telescopePrx(OUC::TelescopePrx::uncheckedCast(adapter->addWithUUID(_telescope)))
{
  logger.logINFO("ObservingImpl::ObservingImp");
}

void
ObservingImpl::sayHello(int delay, const Ice::Current& c)
{
    logger.logINFO("ObservingImpl::sayHello");
  
    if(delay != 0)
    {
        IceUtil::ThreadControl::sleep(IceUtil::Time::milliSeconds(delay));
    }
    logger.logINFO("ObservingImpl::sayHello: Hello World!");
}

void
ObservingImpl::shutdown(const Ice::Current& c)
{
    logger.logINFO("ObservingImpl::shutdown: Shutting down...");
    c.adapter->getCommunicator()->shutdown();
}


OUC::TelescopePrx
ObservingImpl::getTelescope(const Ice::Current& c)
{
    logger.logINFO("ObservingImpl::getTelescope: Getting telescope new reference");
    //return always a new instance of proxy to avoid blocking subsequent calls to LCUControl at TelescopePrx proxy level 
    OUC::TelescopePrx _telescopePrx;
    OUC::TelescopePtr _telescope;
    _telescope = new TelescopeImpl;
    _telescopePrx = OUC::TelescopePrx::uncheckedCast(_adapter->addWithUUID(_telescope));
    return _telescopePrx;
}
