#ifndef LCU_ICE
#define LCU_ICE

module OUC 
{

struct RawEncoderData
{
	int lectAlphaWormE;
	int lectAlphaAxisE;
	int lectAlphaMotor;
	int posAlphaWormE;
	int posAlphaAxisE;
	int posAlphaMotor;
	int lectDeltaWormE;
	int lectDeltaAxisE;
	int lectDeltaMotor;
	int posDeltaWormE;
	int posDeltaAxisE;
	int posDeltaMotor;
	double deltaT;
	double lcuTime;
};

struct EncoderData 
{
	double alphaWormE;
 	double alphaAxisE;
	double deltaWormE;
	double deltaAxisE;
	double localTime;
	double lcuTime;
};

struct TrackingInfo
{
	bool trackState;
	double degVel;
	int ticVel;
};

struct TelescopeConfigData
{
	double localTime;
	double lcuTime;
	double latitude;
 	double longitude;
	double altitude;
	double AMT;
	double AMH;
	double AMR;
	double AWT;
	double AWH;
	double AWR;
	double AAT;
	double AAH;
	double AAR;
	double DMT;
	double DMH;
	double DMR;
	double DWT;
	double DWH;
	double DWR;
	double DAT;
	double DAH;
	double DAR;
};


struct TelescopePosition
{
	double localSideralTime;	
	double RA;	
	double HA;
    	double Dec;
    	double Alt;
    	double Az;
};

struct TelescopeData
{
	double localTime;
	double lcuTime;
	double deltaT;
	double localSideralTime;
	double julianDate;
	double latitude;
	double longitude;
	double altitude;
	double lowElevation;
	double highElevation;
	string targetName;	
	TelescopePosition currentPos;
	TelescopePosition targetPos;
	TelescopePosition differencePos;
};

exception ErrorEx {
	string reason;
};

exception TelescopeNotConfiguredEx extends ErrorEx{};
exception TargetOutOfLimitsEx extends ErrorEx{};	   

interface LCU
{
    idempotent void sayHello(int delay);
    idempotent void shutdown();
  
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
};
	

#endif
