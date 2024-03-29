#include <LCUImpl.h>

using namespace std;

double LCUImpl::MiddleEndianToLittleEndian(double MEData) 
{
    double test;
    unsigned char *p;
    unsigned char *q;

    //RA
    test = 0;
    p = (unsigned char*)&MEData;
    q = (unsigned char*)&test;

    q += 4;
    memcpy(q, p, 4);
    q -= 4;
    p += 4;
    memcpy(q, p, 4);

    return test;
}

void LCUImpl::getConfigState()
{
  logger.logFINEST( "LCUImpl::getConfigState" );

  /** Acquire Semaphore for SHM */
  m_lcu->waitSemaphore();
  
  m_configured = m_lcu->telescope->getIsConfigured();
  
  /** Release semaphore for SHM **/
  m_lcu->postSemaphore();
}

void LCUImpl::getTrackingState()
{
  logger.logFINEST( "LCUImpl::getTrackingState" );

  /** Acquire Semaphore for SHM */
  m_lcu->waitSemaphore();

  m_tracking = m_lcu->telescope->getIsTracking();
  
  /** Release semaphore for SHM **/
  m_lcu->postSemaphore();
}

bool LCUImpl::isConfigured(const Ice::Current& c)
{ 
  logger.logFINEST( "LCUImpl::isConfigured" );
  return m_configured;
}

bool LCUImpl::isTracking(const Ice::Current& c)
{
  logger.logFINEST( "LCUImpl::isTracking" );
  return m_tracking;
}

	

OUC::TelescopeConfigData LCUImpl::getConfiguration(const Ice::Current& c)
{
    logger.logFINEST( "LCUImpl::getTelescopeConfig" );
    
    /** Is telescope configured **/
    if(!m_configured) {
	    OUC::TelescopeNotConfiguredEx ex;
        logger.logWARNING( "LCUImpl::getTelescopeConfig: telescope is not configured." );
        ex.reason = "Telecope Not Configured";
        throw ex;
    }
	
    /** Acquire Semaphore for SHM **/
    m_lcu->waitSemaphore();

    /** Local Time **/
    telescopeConfigData_t->localTime =  MiddleEndianToLittleEndian(m_lcu->telescope->getMLocalTime());
 
    /** basic parameters **/
    telescopeConfigData_t->latitude = MiddleEndianToLittleEndian(m_lcu->telescope->getLatitude());
    telescopeConfigData_t->longitude = MiddleEndianToLittleEndian(m_lcu->telescope->getLongitude());
    telescopeConfigData_t->altitude = MiddleEndianToLittleEndian(m_lcu->telescope->getAltitude());
    
    /** Alpha parametres **/
    telescopeConfigData_t->AMT = MiddleEndianToLittleEndian(m_lcu->telescope->alpha->Motor->getTicsPerRev());
    telescopeConfigData_t->AMH = MiddleEndianToLittleEndian(m_lcu->telescope->alpha->Motor->getHomePosition());
    telescopeConfigData_t->AMR = MiddleEndianToLittleEndian(m_lcu->telescope->alpha->Motor->getEncoderToAxis_Reduction());
    telescopeConfigData_t->AWT = MiddleEndianToLittleEndian(m_lcu->telescope->alpha->WormE->getTicsPerRev());
    telescopeConfigData_t->AWH = MiddleEndianToLittleEndian(m_lcu->telescope->alpha->WormE->getHomePosition());
    telescopeConfigData_t->AWR = MiddleEndianToLittleEndian(m_lcu->telescope->alpha->WormE->getEncoderToAxis_Reduction());
    telescopeConfigData_t->AAT = MiddleEndianToLittleEndian(m_lcu->telescope->alpha->AxisE->getTicsPerRev());
    telescopeConfigData_t->AAH = MiddleEndianToLittleEndian(m_lcu->telescope->alpha->AxisE->getHomePosition());
    telescopeConfigData_t->AAR = MiddleEndianToLittleEndian(m_lcu->telescope->alpha->AxisE->getEncoderToAxis_Reduction());

    /** Delta parameters **/
    telescopeConfigData_t->DMT = MiddleEndianToLittleEndian(m_lcu->telescope->delta->Motor->getTicsPerRev());
    telescopeConfigData_t->DMH = MiddleEndianToLittleEndian(m_lcu->telescope->delta->Motor->getHomePosition());
    telescopeConfigData_t->DMR = MiddleEndianToLittleEndian(m_lcu->telescope->delta->Motor->getEncoderToAxis_Reduction());
    telescopeConfigData_t->DWT = MiddleEndianToLittleEndian(m_lcu->telescope->delta->WormE->getTicsPerRev());
    telescopeConfigData_t->DWH = MiddleEndianToLittleEndian(m_lcu->telescope->delta->WormE->getHomePosition());
    telescopeConfigData_t->DWR = MiddleEndianToLittleEndian(m_lcu->telescope->delta->WormE->getEncoderToAxis_Reduction());
    telescopeConfigData_t->DAT = MiddleEndianToLittleEndian(m_lcu->telescope->delta->AxisE->getTicsPerRev());
    telescopeConfigData_t->DAH = MiddleEndianToLittleEndian(m_lcu->telescope->delta->AxisE->getHomePosition());
    telescopeConfigData_t->DAR = MiddleEndianToLittleEndian(m_lcu->telescope->delta->AxisE->getEncoderToAxis_Reduction());
      
    /** LCU Local Time **/  
    telescopeConfigData_t->lcuTime = MiddleEndianToLittleEndian(m_lcu->getLCUMTime());
   
    /** Release semaphore for SHM **/
    m_lcu->postSemaphore();

    logger.logCONFIG("LCUImpl::getTelescopeConfig LT  = [%lf]", telescopeConfigData_t->localTime);	
    logger.logCONFIG("LCUImpl::getTelescopeConfig Latitude = %+11.4lf ", telescopeConfigData_t->latitude);
    logger.logCONFIG("LCUImpl::getTelescopeConfig Longitude = %+11.4lf ", telescopeConfigData_t->longitude);
    logger.logCONFIG("LCUImpl::getTelescopeConfig Altitude = %+11.4lf ", telescopeConfigData_t->altitude);
    logger.logCONFIG("LCUImpl::getTelescopeConfig AMT = %+11.4lf ", telescopeConfigData_t->AMT);
    logger.logCONFIG("LCUImpl::getTelescopeConfig AMH = %+11.4lf ", telescopeConfigData_t->AMH);
    logger.logCONFIG("LCUImpl::getTelescopeConfig AMR = %+11.4lf ", telescopeConfigData_t->AMR);
    logger.logCONFIG("LCUImpl::getTelescopeConfig AWT = %+11.4lf ", telescopeConfigData_t->AWT);	
    logger.logCONFIG("LCUImpl::getTelescopeConfig AWH = %+11.4lf ", telescopeConfigData_t->AWH);
    logger.logCONFIG("LCUImpl::getTelescopeConfig AWR = %+11.4lf ", telescopeConfigData_t->AWR);
    logger.logCONFIG("LCUImpl::getTelescopeConfig AAT = %+11.4lf ", telescopeConfigData_t->AAT);
    logger.logCONFIG("LCUImpl::getTelescopeConfig AAH = %+11.4lf ", telescopeConfigData_t->AAH);
    logger.logCONFIG("LCUImpl::getTelescopeConfig AAR = %+11.4lf ", telescopeConfigData_t->AAR);
    logger.logCONFIG("LCUImpl::getTelescopeConfig DMT = %+11.4lf ", telescopeConfigData_t->DMT);
    logger.logCONFIG("LCUImpl::getTelescopeConfig DMH = %+11.4lf ", telescopeConfigData_t->DMH);
    logger.logCONFIG("LCUImpl::getTelescopeConfig DMR = %+11.4lf ", telescopeConfigData_t->DMR);
    logger.logCONFIG("LCUImpl::getTelescopeConfig DWT = %+11.4lf ", telescopeConfigData_t->DWT);	
    logger.logCONFIG("LCUImpl::getTelescopeConfig DWH = %+11.4lf ", telescopeConfigData_t->DWH);
    logger.logCONFIG("LCUImpl::getTelescopeConfig DWR = %+11.4lf ", telescopeConfigData_t->DWR);
    logger.logCONFIG("LCUImpl::getTelescopeConfig DAT = %+11.4lf ", telescopeConfigData_t->DAT);
    logger.logCONFIG("LCUImpl::getTelescopeConfig DAH = %+11.4lf ", telescopeConfigData_t->DAH);
    logger.logCONFIG("LCUImpl::getTelescopeConfig DAR = %+11.4lf ", telescopeConfigData_t->DAR);
    logger.logCONFIG("LCUImpl::getTelescopeConfig Page generated at: [%lf]", telescopeConfigData_t->lcuTime);
    
    return *telescopeConfigData_t;
}


OUC::TelescopeData LCUImpl::getPosition(const Ice::Current& c)
{
    logger.logFINEST( "LCUImpl::getPosition" );
    
      /** Is telescope configured **/
    if(!m_configured)
    {
	OUC::TelescopeNotConfiguredEx ex;
        ex.reason = "Telecope Not Configured";
        throw ex;
    }

    /** Get current and target telescope positions **/
    getCurrentPosition(telescopeData_t);
    getTargetPosition(telescopeData_t);

    /** Acquire Semaphore for SHM **/
    m_lcu->waitSemaphore();
    
    /** Acquire Times **/
    telescopeData_t->localTime = m_lcu->telescope->getMLocalTime();
    telescopeData_t->deltaT = m_lcu->telescope->getDeltaT();
    telescopeData_t->julianDate = m_lcu->telescope->getJulianDate();
    
    /** Telescope Data **/
    telescopeData_t->differencePos.RA = m_lcu->telescope->getDifferenceRA();
    telescopeData_t->differencePos.Dec = m_lcu->telescope->getDifferenceDec();
    telescopeData_t->latitude = m_lcu->telescope->getLatitude();
    telescopeData_t->longitude = m_lcu->telescope->getLongitude(); 
    telescopeData_t->altitude = m_lcu->telescope->getAltitude();
    telescopeData_t->lowElevation = m_lcu->telescope->getLowElevation(); 
    telescopeData_t->highElevation = m_lcu->telescope->getHighElevation();
        
    /** LCU Local Time **/  
    telescopeData_t->lcuTime = m_lcu->getLCUMTime();
    
    /** Release semaphore for SHM **/
    m_lcu->postSemaphore();  

    //convert to Little Endian    
    telescopeData_t->localTime = MiddleEndianToLittleEndian(m_lcu->telescope->getMLocalTime());
    telescopeData_t->deltaT = MiddleEndianToLittleEndian(m_lcu->telescope->getDeltaT());
    telescopeData_t->julianDate = MiddleEndianToLittleEndian(m_lcu->telescope->getJulianDate());
    
    /** Telescope Data **/
    telescopeData_t->differencePos.RA = MiddleEndianToLittleEndian(m_lcu->telescope->getDifferenceRA());
    telescopeData_t->differencePos.Dec = MiddleEndianToLittleEndian(m_lcu->telescope->getDifferenceDec());
    telescopeData_t->latitude = MiddleEndianToLittleEndian(m_lcu->telescope->getLatitude());
    telescopeData_t->longitude = MiddleEndianToLittleEndian(m_lcu->telescope->getLongitude()); 
    telescopeData_t->longitude = MiddleEndianToLittleEndian(m_lcu->telescope->getAltitude());
    telescopeData_t->lowElevation = MiddleEndianToLittleEndian(m_lcu->telescope->getLowElevation()); 
    telescopeData_t->highElevation = MiddleEndianToLittleEndian(m_lcu->telescope->getHighElevation());
        
    /** LCU Local Time **/  
    telescopeData_t->lcuTime = MiddleEndianToLittleEndian(m_lcu->getLCUMTime());

    /** Internal byte order conversion **/
    telescopeData_t->currentPos.localSideralTime =  MiddleEndianToLittleEndian(telescopeData_t->currentPos.localSideralTime);
    telescopeData_t->currentPos.RA = MiddleEndianToLittleEndian(telescopeData_t->currentPos.RA);
    telescopeData_t->currentPos.Dec = MiddleEndianToLittleEndian(telescopeData_t->currentPos.Dec);
    telescopeData_t->currentPos.Alt = MiddleEndianToLittleEndian(telescopeData_t->currentPos.Alt);
    telescopeData_t->currentPos.Az = MiddleEndianToLittleEndian(telescopeData_t->currentPos.Az);
    telescopeData_t->currentPos.HA = MiddleEndianToLittleEndian(telescopeData_t->currentPos.HA);

    /** Internal byte order conversion **/
    telescopeData_t->targetPos.localSideralTime =  MiddleEndianToLittleEndian(telescopeData_t->targetPos.localSideralTime);
    telescopeData_t->targetPos.RA = MiddleEndianToLittleEndian(telescopeData_t->targetPos.RA);
    telescopeData_t->targetPos.Dec = MiddleEndianToLittleEndian(telescopeData_t->targetPos.Dec);
    telescopeData_t->targetPos.Alt = MiddleEndianToLittleEndian(telescopeData_t->targetPos.Alt);
    telescopeData_t->targetPos.Az = MiddleEndianToLittleEndian(telescopeData_t->targetPos.Az);
    telescopeData_t->targetPos.HA = MiddleEndianToLittleEndian(telescopeData_t->targetPos.HA);

    logger.logFINER("LCUImpl::getPosition LT  = [%lf]", telescopeData_t->localTime);
    logger.logFINER("LCUImpl::getPosition Time elapsed since last access: %lf",telescopeData_t->deltaT);
    logger.logFINER("LCUImpl::getPosition JD  = %lf", telescopeData_t->julianDate);
    logger.logFINER("LCUImpl::getTelescopeConfig Latitude = %+11.4lf ", telescopeData_t->latitude);
    logger.logFINER("LCUImpl::getTelescopeConfig Longitude = %+11.4lf ", telescopeData_t->longitude);
    logger.logFINER("LCUImpl::getTelescopeConfig Altitude = %+11.4lf ", telescopeData_t->altitude);
    logger.logFINER("LCUImpl::getTelescopeConfig High Elevation = %+11.4lf ", telescopeData_t->highElevation);
    logger.logFINER("LCUImpl::getTelescopeConfig Low Elevation = %+11.4lf ", telescopeData_t->lowElevation);
    logger.logFINER("LCUImpl::getPosition LST = %lf ", telescopeData_t->currentPos.localSideralTime);
    logger.logFINER("LCUImpl::getPosition Current RA = %lf ", telescopeData_t->currentPos.RA);
    logger.logFINER("LCUImpl::getPosition Current Dec = %lf ", telescopeData_t->currentPos.Dec);
    logger.logFINER("LCUImpl::getPosition Current HA = %lf ", telescopeData_t->currentPos.HA);
    logger.logFINER("LCUImpl::getPosition Current Alt = %lf ", telescopeData_t->currentPos.Alt);
    logger.logFINER("LCUImpl::getPosition Current Az = %lf ", telescopeData_t->currentPos.Az);
    logger.logFINER("LCUImpl::getPosition Target RA = %lf ", telescopeData_t->targetPos.RA);
    logger.logFINER("LCUImpl::getPosition Target Dec = %lf ", telescopeData_t->targetPos.Dec);
    logger.logFINER("LCUImpl::getPosition Target HA = %lf ", telescopeData_t->targetPos.HA);
    logger.logFINER("LCUImpl::getPosition Target Alt = %lf ", telescopeData_t->targetPos.Alt);
    logger.logFINER("LCUImpl::getPosition Target Az = %lf ", telescopeData_t->targetPos.Az);
    logger.logFINER("LCUImpl::getPosition Difference RA = %lf ", telescopeData_t->differencePos.RA);
    logger.logFINER("LCUImpl::getPosition Difference Dec = %lf ", telescopeData_t->differencePos.Dec);	
    logger.logFINER("LCUImpl::getPosition Page generated at: [%lf]", telescopeData_t->lcuTime);

    return *telescopeData_t;
}
    

OUC::RawEncoderData LCUImpl::getRawEncodersPosition(const Ice::Current& c)
{
         
    logger.logFINEST( "LCUImpl::getRawEncoderPosition" );     
  
    /** Is telescope configured **/
    if(!m_configured) {
	    OUC::TelescopeNotConfiguredEx ex;
        ex.reason = "Telecope Not Configured";
        throw ex;
    }
    
    /** Acquire Semaphore for SHM */
    m_lcu->waitSemaphore();

    /** Time elapsed since last access **/
    rawEncoder_t->deltaT = MiddleEndianToLittleEndian(m_lcu->telescope->getDeltaT());
    
    /** Alpha Raw Encoder **/
    //Axis
    m_lcu->telescope->alpha->AxisE->readDeviceMemory( 2, &rawEncoder_t->lectAlphaAxisE, 0 );
    m_lcu->telescope->alpha->AxisE->readDeviceMemory( 4, &rawEncoder_t->posAlphaAxisE, 0 );
    //Worm
    m_lcu->telescope->alpha->WormE->readDeviceMemory( 2, &rawEncoder_t->lectAlphaWormE, 0 );
    m_lcu->telescope->alpha->WormE->readDeviceMemory( 4, &rawEncoder_t->posAlphaWormE, 0 );
    //Motor
    m_lcu->telescope->alpha->Motor->readDeviceMemory( 2, &rawEncoder_t->lectAlphaMotor, 0 );
    m_lcu->telescope->alpha->Motor->readDeviceMemory( 4, &rawEncoder_t->posAlphaMotor, 0 );
    m_lcu->telescope->alpha->Motor->readDeviceMemory( 7, &rawEncoder_t->remAlphaMotor, 0 );

    /** Delta Raw Encoder **/
    //Axis
    m_lcu->telescope->delta->AxisE->readDeviceMemory( 2, &rawEncoder_t->lectDeltaAxisE, 0 );
    m_lcu->telescope->delta->AxisE->readDeviceMemory( 4, &rawEncoder_t->posDeltaAxisE, 0 );
    //Worm
    m_lcu->telescope->delta->WormE->readDeviceMemory( 2, &rawEncoder_t->lectDeltaWormE, 0 );
    m_lcu->telescope->delta->WormE->readDeviceMemory( 4, &rawEncoder_t->posDeltaWormE, 0 );
    //Motor
    m_lcu->telescope->delta->Motor->readDeviceMemory( 2, &rawEncoder_t->lectDeltaMotor, 0 );
    m_lcu->telescope->delta->Motor->readDeviceMemory( 4, &rawEncoder_t->posDeltaMotor, 0 );
    m_lcu->telescope->delta->Motor->readDeviceMemory( 7, &rawEncoder_t->remDeltaMotor, 0 );

    /** LCU Local Time **/  
    rawEncoder_t->lcuTime = MiddleEndianToLittleEndian(m_lcu->getLCUMTime());
  
    /** Release semaphore for SHM **/
    m_lcu->postSemaphore(); 

    logger.logFINER("LCUImpl::getRawEncoderPosition Time elapsed since last access: %lf",rawEncoder_t->deltaT);
    logger.logFINER("LCUImpl::getRawEncoderPosition Lecture Alpha AxisE: %6d", rawEncoder_t->lectAlphaAxisE);
    logger.logFINER("LCUImpl::getRawEncoderPosition Position Alpha AxisE: %11d", rawEncoder_t->posAlphaAxisE);
    logger.logFINER("LCUImpl::getRawEncoderPosition Lecture Alpha WormE: %6d", rawEncoder_t->lectAlphaWormE);
    logger.logFINER("LCUImpl::getRawEncoderPosition Position Alpha WormE: %11d", rawEncoder_t->posAlphaWormE);
    logger.logFINER("LCUImpl::getRawEncoderPosition Lecture Alpha Motor: %6d", rawEncoder_t->lectAlphaMotor);
    logger.logFINER("LCUImpl::getRawEncoderPosition Position Alpha Motor: %11d", rawEncoder_t->posAlphaMotor);
    logger.logFINER("LCUImpl::getRawEncoderPosition Lecture Delta AxisE: %6d", rawEncoder_t->lectDeltaAxisE);
    logger.logFINER("LCUImpl::getRawEncoderPosition Position Delta AxisE: %11d", rawEncoder_t->posDeltaAxisE);
    logger.logFINER("LCUImpl::getRawEncoderPosition Lecture Delta WormE: %6d", rawEncoder_t->lectDeltaWormE);
    logger.logFINER("LCUImpl::getRawEncoderPosition Position Delta WormE: %11d", rawEncoder_t->posDeltaWormE);
    logger.logFINER("LCUImpl::getRawEncoderPosition Lecture Delta Motor: %6d", rawEncoder_t->lectDeltaMotor);
    logger.logFINER("LCUImpl::getRawEncoderPosition Position Delta Motor: %11d", rawEncoder_t->posDeltaMotor);
    logger.logFINER("LCUImpl::getRawEncoderPosition Page generated at: [%lf]", rawEncoder_t->lcuTime);
    
    return *rawEncoder_t;
}

    
OUC::EncoderData LCUImpl::getEncodersPosition(const Ice::Current& c)
{
    logger.logFINEST( "LCUImpl::getEncoderPosition" );     
    
    /** Is telescope configured **/
    if(!m_configured)
    {
        OUC::TelescopeNotConfiguredEx ex;
        ex.reason = "Telecope Not Configured";
        throw ex;
    }
    
    /** Acquire Semaphore for SHM */
    m_lcu->waitSemaphore();

    /** Local Time */
    encoder_t->localTime = MiddleEndianToLittleEndian(m_lcu->telescope->getMLocalTime());
 
    /** Encoder Position **/
    //Alpha
    encoder_t->alphaWormE =  MiddleEndianToLittleEndian(m_lcu->telescope->alpha->WormE->getPosition());
    encoder_t->alphaAxisE =  MiddleEndianToLittleEndian(m_lcu->telescope->alpha->AxisE->getPosition());
    //Delta
    encoder_t->deltaWormE = MiddleEndianToLittleEndian(m_lcu->telescope->delta->WormE->getPosition());
    encoder_t->deltaAxisE = MiddleEndianToLittleEndian(m_lcu->telescope->delta->AxisE->getPosition());
  

    /** LCU Local Time **/  
    encoder_t->lcuTime = MiddleEndianToLittleEndian(m_lcu->getLCUMTime());
   
    /** Release semaphore for SHM **/
    m_lcu->postSemaphore();

    logger.logFINER("LCUImpl::getEncoderPosition LT  = [%lf]", encoder_t->localTime);
    logger.logFINER("LCUImpl::getEncoderPosition AW Position  = [%+10.0lf]", encoder_t->alphaWormE);    
    logger.logFINER("LCUImpl::getEncoderPosition AA Position  = [%+10.0lf]", encoder_t->alphaAxisE);
    logger.logFINER("LCUImpl::getEncoderPosition DW Position  = [%+10.0lf]", encoder_t->deltaWormE);
    logger.logFINER("LCUImpl::getEncoderPosition DA Position  = [%+10.0lf]",  encoder_t->deltaAxisE);
    logger.logFINER("LCUImpl::getEncoderPosition --------------------------------" );
    logger.logFINER("LCUImpl::getEncoderPosition Page generated at: [%lf]", encoder_t->lcuTime);
    logger.logFINER("LCUImpl::getEncoderPosition Good bye!" );
    
    return *encoder_t;
}

void LCUImpl::getCurrentPosition(OUC::TelescopeData *telescopeData_t)
{
    logger.logFINEST( "LCUImpl::getCurrentPosition" );
  
    /** Is telescope configured **/
    if(!m_configured) {
	    OUC::TelescopeNotConfiguredEx ex;
        ex.reason = "Telecope Not Configured";
        throw ex;
    }
    
    /** Acquire Semaphore for SHM **/
    m_lcu->waitSemaphore();
    
    /** Get current position from HW **/
    m_lcu->telescope->currentPosition(&telescopeData_t->currentPos.localSideralTime, &telescopeData_t->currentPos.RA, &telescopeData_t->currentPos.Dec, &telescopeData_t->currentPos.Alt, &telescopeData_t->currentPos.Az, &telescopeData_t->currentPos.HA);
    
    /** Release semaphore for SHM **/
    m_lcu->postSemaphore();
}

void LCUImpl::getTargetPosition(OUC::TelescopeData *telescopeData_t)
{
    logger.logFINEST( "LCUImpl::getTargetPosition" );
    
    /** Is telescope configured **/
    if(!m_configured) {
	    OUC::TelescopeNotConfiguredEx ex;
        ex.reason = "Telecope Not Configured";
        throw ex;
    }
    
    /** Acquire Semaphore for SHM **/
    m_lcu->waitSemaphore();
    
    /** Get current position from HW **/
    m_lcu->telescope->targetPosition(&telescopeData_t->targetPos.localSideralTime, &telescopeData_t->targetPos.RA, &telescopeData_t->targetPos.Dec, &telescopeData_t->targetPos.Alt, &telescopeData_t->targetPos.Az, &telescopeData_t->targetPos.HA);
    
    /** Release semaphore for SHM **/
    m_lcu->postSemaphore();
}
