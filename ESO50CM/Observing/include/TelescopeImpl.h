#ifndef TelescopeImpl_I_H
#define TelescopeImpl_I_H

#include <string.h>
#include <Observing.h>
#include <IceUtil/Thread.h>
#include <IceUtil/ScopedArray.h>
#include "LoggerHelper.h"

using namespace std;

class TelescopeImpl : public OUC::Telescope
{
    public:
        TelescopeImpl();

        virtual void sayHelloTelescope(const int delay, const Ice::Current&); 

        virtual bool isConfigured(const Ice::Current&);
        virtual bool isTracking(const Ice::Current&);
        virtual OUC::RawEncoderData getRawEncodersPosition(const Ice::Current&);
        virtual OUC::EncoderData getEncodersPosition(const Ice::Current&);
        virtual OUC::TelescopeData getPosition(const Ice::Current&);
        virtual OUC::TelescopeConfigData getConfiguration(const Ice::Current&);

        virtual void setConfiguration(const string& fileName, const Ice::Current&);
        virtual void setTarget(const ::OUC::TelescopePosition& targetPos, const Ice::Current&);
        virtual void handsetSlew(const ::OUC::SlewInfo& slewInfo, const Ice::Current&);
        virtual void setOffset(const ::OUC::TelescopePosition& offsetPos, const Ice::Current&);
        virtual void setTracking(const OUC::TrackingInfo& trkInfo, const Ice::Current&);
        virtual void startTracking(const Ice::Current&);
        virtual void stopTracking(const Ice::Current&);
        virtual void parkTelescope(const Ice::Current&);
        virtual void parkTelescopeCap(const Ice::Current&);
        virtual void stopTelescope(const Ice::Current&);
        virtual void stopTelescope(const ::OUC::TelescopeDirection dir, const Ice::Current&);
        virtual void moveToTarget(const Ice::Current&);
    private:
        OUC::LCUPrx lcuPrx;
        OUC::RawEncoderData *rawEncoder_t;
        OUC::EncoderData *encoder_t;
        OUC::TelescopeConfigData *telescopeConfigData_t;
        OUC::TelescopeData *telescopeData_t;
        LoggerHelper logger;
};

#endif

