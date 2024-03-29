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
    void handsetSlew(SlewInfo slew)
    	 throws TelescopeNotConfiguredEx, TargetOutOfLimitsEx,
                NotLCUReferenceAvailableEx;
    void setOffset(TelescopePosition offsetPos)
    	 throws TelescopeNotConfiguredEx, TargetOutOfLimitsEx,
                NotLCUReferenceAvailableEx;
    void startTracking()
         throws TelescopeNotConfiguredEx, 
                NotLCUReferenceAvailableEx;
    void stopTracking()
         throws TelescopeNotConfiguredEx, 
                NotLCUReferenceAvailableEx;
    void setTracking(TrackingInfo trkInfo)
         throws TelescopeNotConfiguredEx, 
                NotLCUReferenceAvailableEx;
    void parkTelescope()
         throws TelescopeNotConfiguredEx,
                NotLCUReferenceAvailableEx; 
    void parkTelescopeCap()
         throws TelescopeNotConfiguredEx,
                NotLCUReferenceAvailableEx; 
    void stopTelescope(TelescopeDirection dir)
         throws TelescopeNotConfiguredEx,
                NotLCUReferenceAvailableEx; 
    void moveToTarget()
         throws TelescopeNotConfiguredEx,
                NotLCUReferenceAvailableEx; 
};

interface Observing
{
    idempotent void sayHello(int delay);
    idempotent void shutdown();
    Telescope* getTelescope();
    ["ami"] void moveToTarget() throws TelescopeNotConfiguredEx, NotLCUReferenceAvailableEx; 
    ["ami"] void startTracking() throws TelescopeNotConfiguredEx, NotLCUReferenceAvailableEx;
    ["ami"] void stopTracking() throws TelescopeNotConfiguredEx, NotLCUReferenceAvailableEx;
    ["ami"] void stopTelescope() throws TelescopeNotConfiguredEx, NotLCUReferenceAvailableEx;
    ["ami"] void parkTelescope() throws TelescopeNotConfiguredEx, NotLCUReferenceAvailableEx;
    ["ami"] void parkTelescopeCap() throws TelescopeNotConfiguredEx, NotLCUReferenceAvailableEx;

};
};
#endif
