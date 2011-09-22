#ifndef ObservingImpl_I_H
#define ObservingImpl_I_H

#include "Observing.h"
#include <IceUtil/Thread.h>
#include <IceUtil/ScopedArray.h>
#include "LoggerHelper.h"

class ObservingImpl : public OUC::Observing
{
public:

    ObservingImpl(const Ice::ObjectAdapterPtr&);

    virtual void sayHello(const int delay, const Ice::Current&);
    virtual void shutdown(const Ice::Current&);
    
    virtual OUC::TelescopePrx getTelescope(const Ice::Current&);

private:
    const OUC::TelescopePtr _telescope;
    const OUC::TelescopePrx _telescopePrx;
    LoggerHelper logger;
};
#endif
