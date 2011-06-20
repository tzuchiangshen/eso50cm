#ifndef OBSERVING_ICE
#define OBSERVING_ICE

#include <LCU.ice>

module OUC 
{

exception NotLCUReferenceAvailableEx extends ErrorEx{};

class Telescope 
{
    void sayHelloTelescope(int delay);
	 
    EncoderData getEncodersPosition()
         throws TelescopeNotConfiguredEx,
	        NotLCUReferenceAvailableEx; 
    RawEncoderData getRawEncodersPosition() 
         throws TelescopeNotConfiguredEx,
	        NotLCUReferenceAvailableEx;		    
    TelescopeConfigData getConfiguration()
    	 throws TelescopeNotConfiguredEx,
                NotLCUReferenceAvailableEx;	
    TelescopeData getPosition()
    	 throws TelescopeNotConfiguredEx,
                NotLCUReferenceAvailableEx;	
    bool isConfigured()
         throws NotLCUReferenceAvailableEx;
    bool isTracking()
         throws NotLCUReferenceAvailableEx;

    void setConfiguration(string fileName)
         throws NotLCUReferenceAvailableEx;
    void setTarget(TelescopePosition targetPos)
    	 throws TelescopeNotConfiguredEx, TargetOutOfLimitsEx,
                NotLCUReferenceAvailableEx;
    void setOffset(TelescopePosition offsetPos)
    	 throws TelescopeNotConfiguredEx, TargetOutOfLimitsEx,
                NotLCUReferenceAvailableEx;
    void setTracking(TrackingInfo trkInfo)
         throws TelescopeNotConfiguredEx, 
                NotLCUReferenceAvailableEx;
    void parkTelescope()
         throws TelescopeNotConfiguredEx,
                NotLCUReferenceAvailableEx; 
};

interface Observing
{
    idempotent void sayHello(int delay);
    idempotent void shutdown();
    Telescope* getTelescope();
};
};
#endif
