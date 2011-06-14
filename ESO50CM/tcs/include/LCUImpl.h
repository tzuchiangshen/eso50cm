#ifndef LCUImpl_I_H
#define LCUImpl_I_H

#include "LCU.h"
#include "myLCU.h"

#ifdef LCU
    #include <IceE/IceE.h>
#else
    #include <Ice/Ice.h>
#endif



class LCUImpl : public OUC::LCU
{
public:


    LCUImpl();
    virtual void sayHello(const int delay, const Ice::Current&);
    virtual void shutdown(const Ice::Current&);

    virtual bool isConfigured(const Ice::Current&);
    virtual bool isTracking(const Ice::Current&);
    virtual OUC::RawEncoderData getRawEncodersPosition(const Ice::Current&);
    virtual OUC::EncoderData getEncodersPosition(const Ice::Current&);
    virtual OUC::TelescopeData getPosition(const Ice::Current&);
    virtual OUC::TelescopeConfigData getConfiguration(const Ice::Current&);
    
    virtual void setConfiguration(const string& fileName, const Ice::Current&);
    virtual void setTarget(const ::OUC::TelescopePosition& targetPos, const Ice::Current&);
    virtual void setOffset(const ::OUC::TelescopePosition& offsetPos, const Ice::Current&);
    virtual void setTracking(const ::OUC::TrackingInfo& trkInfo, const Ice::Current&);
    virtual void parkTelescope(const Ice::Current&);

    virtual void getConfigState();
    virtual void getTrackingState();
protected:
    bool m_configured;
    bool m_tracking; 
private:
    void getCurrentPosition(OUC::TelescopeData *telescopeData_t);
    void getTargetPosition(OUC::TelescopeData *telescopeData_t);
    double MiddleEndianToLittleEndian(double MEData);
    OUC::RawEncoderData *rawEncoder_t;
    OUC::EncoderData *encoder_t;
    OUC::TelescopeConfigData *telescopeConfigData_t;
    OUC::TelescopeData *telescopeData_t;
    myLCU *m_lcu; 
    
};

#endif
