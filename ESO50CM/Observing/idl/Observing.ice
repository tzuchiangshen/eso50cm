#ifndef OBSERVING_ICE
#define OBSERVING_ICE

#include <LCU.ice>

module OUC 
{

class Telescope 
{
    string message;
    void sayHelloTelescope(int delay);
	 
    EncoderData getEncodersPosition()
         throws TelescopeNotConfiguredEx;
    RawEncoderData getRawEncodersPosition() 
         throws TelescopeNotConfiguredEx;	    
    TelescopeConfigData getConfiguration()
    	 throws TelescopeNotConfiguredEx;	
    TelescopeData getPosition()
    	 throws TelescopeNotConfiguredEx;	
    bool isConfigured();
    bool isTracking();

    void setConfiguration(string fileName);
    void setTarget(TelescopePosition targetPos)
    	 throws TelescopeNotConfiguredEx, TargetOutOfLimitsEx;
    void setOffset(TelescopePosition offsetPos)
    	 throws TelescopeNotConfiguredEx, TargetOutOfLimitsEx;
    void setTracking(TrackingInfo trkInfo)
         throws TelescopeNotConfiguredEx;
    void parkTelescope()
         throws TelescopeNotConfiguredEx; 
};

interface Observing
{
    idempotent void sayHello(int delay);
    idempotent void shutdown();
    Telescope* getTelescope();
};
};
#endif
