
#include <ObservingImpl.h>
#include <TelescopeImpl.h>
#include <IceE/IceE.h>

using namespace std;

ObservingImpl::ObservingImpl(const Ice::ObjectAdapterPtr& adapter) :
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
    printf("Hello World!\n");
}

void
ObservingImpl::shutdown(const Ice::Current& c)
{
    printf("Shutting down...\n");
    c.adapter->getCommunicator()->shutdown();
}


OUC::TelescopePrx
ObservingImpl::getTelescope(const Ice::Current& c)
{
    printf("Getting Telescope Reference!!");
    return _telescopePrx;
}
