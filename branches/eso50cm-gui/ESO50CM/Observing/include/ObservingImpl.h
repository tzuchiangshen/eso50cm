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
        virtual void moveToTarget(const Ice::Current&);
        virtual void startTracking(const Ice::Current&);
        virtual void stopTracking(const Ice::Current&);
        virtual void stopTelescope(const Ice::Current&);
        virtual void parkTelescope(const Ice::Current&);
        virtual void parkTelescopeCap(const Ice::Current&);

    private:
        const OUC::TelescopePtr _telescope;
        const OUC::TelescopePrx _telescopePrx;
        const Ice::ObjectAdapterPtr _adapter;
        LoggerHelper logger;
};
#endif
