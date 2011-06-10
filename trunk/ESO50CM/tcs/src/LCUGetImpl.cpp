#include <LCUImpl.h>
#include <IceE/IceE.h>

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
  extern int verbose;
  
  if( verbose ) 
      printf( "LCUImpl::getConfigState" );

  /** Acquire Semaphore for SHM */
  m_lcu->waitSemaphore();

  m_configured = m_lcu->telescope->getIsConfigured();
  
  /** Release semaphore for SHM **/
  m_lcu->postSemaphore();
}

void LCUImpl::getTrackingState()
{
  extern int verbose;
  
  if( verbose ) 
      printf( "LCUImpl::getTrackingState" );

  /** Acquire Semaphore for SHM */
  m_lcu->waitSemaphore();

  m_tracking = m_lcu->telescope->getIsTracking();
  
  /** Release semaphore for SHM **/
  m_lcu->postSemaphore();
}

bool LCUImpl::isConfigured(const Ice::Current& c)
{

  extern int verbose;
  if( verbose ) 
    printf( "[LCUImpl::isConfigured" );

  return m_configured;
}

bool LCUImpl::isTracking(const Ice::Current& c)
{

  extern int verbose;
  if( verbose ) 
    printf( "[LCUImpl::isTracking" );

  return m_tracking;
}

	

OUC::TelescopeConfigData LCUImpl::getConfiguration(const Ice::Current& c)
{
    extern int verbose;
    
    if( verbose )
      printf( "[LCUImpl::getTelescopeConfig" );
    
    /** Is telescope configured **/
    if(!m_configured)
      {
	OUC::TelescopeNotConfiguredEx ex;
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

    if(verbose)
      {
	printf("LCUImpl::getTelescopeConfig LT  = [%lf]\n", telescopeConfigData_t->localTime);	
	printf("LCUImpl::getTelescopeConfig Latitude = %+11.4lf \n", telescopeConfigData_t->latitude);
	printf("LCUImpl::getTelescopeConfig Longitude = %+11.4lf \n", telescopeConfigData_t->longitude);
	printf("LCUImpl::getTelescopeConfig Altitude = %+11.4lf \n", telescopeConfigData_t->altitude);
	printf("LCUImpl::getTelescopeConfig AMT = %+11.4lf \n", telescopeConfigData_t->AMT);
	printf("LCUImpl::getTelescopeConfig AMH = %+11.4lf \n", telescopeConfigData_t->AMH);
	printf("LCUImpl::getTelescopeConfig AMR = %+11.4lf \n", telescopeConfigData_t->AMR);
	printf("LCUImpl::getTelescopeConfig AWT = %+11.4lf \n", telescopeConfigData_t->AWT);	
	printf("LCUImpl::getTelescopeConfig AWH = %+11.4lf \n", telescopeConfigData_t->AWH);
	printf("LCUImpl::getTelescopeConfig AWR = %+11.4lf \n", telescopeConfigData_t->AWR);
	printf("LCUImpl::getTelescopeConfig AAT = %+11.4lf \n", telescopeConfigData_t->AAT);
	printf("LCUImpl::getTelescopeConfig AAH = %+11.4lf \n", telescopeConfigData_t->AAH);
	printf("LCUImpl::getTelescopeConfig AAR = %+11.4lf \n", telescopeConfigData_t->AAR);
	printf("LCUImpl::getTelescopeConfig DMT = %+11.4lf \n", telescopeConfigData_t->DMT);
	printf("LCUImpl::getTelescopeConfig DMH = %+11.4lf \n", telescopeConfigData_t->DMH);
	printf("LCUImpl::getTelescopeConfig DMR = %+11.4lf \n", telescopeConfigData_t->DMR);
	printf("LCUImpl::getTelescopeConfig DWT = %+11.4lf \n", telescopeConfigData_t->DWT);	
	printf("LCUImpl::getTelescopeConfig DWH = %+11.4lf \n", telescopeConfigData_t->DWH);
	printf("LCUImpl::getTelescopeConfig DWR = %+11.4lf \n", telescopeConfigData_t->DWR);
	printf("LCUImpl::getTelescopeConfig DAT = %+11.4lf \n", telescopeConfigData_t->DAT);
	printf("LCUImpl::getTelescopeConfig DAH = %+11.4lf \n", telescopeConfigData_t->DAH);
	printf("LCUImpl::getTelescopeConfig DAR = %+11.4lf \n", telescopeConfigData_t->DAR);
	printf("LCUImpl::getTelescopeConfig Page generated at: [%lf]\n", telescopeConfigData_t->lcuTime);
      }

    return *telescopeConfigData_t;
}


OUC::TelescopeData LCUImpl::getPosition(const Ice::Current& c)
{
    extern int verbose;

    if( verbose ) 
      printf( "[LCUImpl::getPosition" );

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

    if (verbose)
    {
		printf("LCUImpl::getPosition LT  = [%lf]\n", telescopeData_t->localTime);
		printf("LCUImpl::getPosition Time elapsed since last access: %lf\n",telescopeData_t->deltaT);
		printf("LCUImpl::getPosition JD  = %lf\n", telescopeData_t->julianDate);
		printf("LCUImpl::getTelescopeConfig Latitude = %+11.4lf \n", telescopeData_t->latitude);
		printf("LCUImpl::getTelescopeConfig Longitude = %+11.4lf \n", telescopeData_t->longitude);
		printf("LCUImpl::getTelescopeConfig Altitude = %+11.4lf \n", telescopeData_t->altitude);
		printf("LCUImpl::getTelescopeConfig High Elevation = %+11.4lf \n", telescopeData_t->highElevation);
		printf("LCUImpl::getTelescopeConfig Low Elevation = %+11.4lf \n", telescopeData_t->lowElevation);
		printf("LCUImpl::getPosition LST = %lf \n", telescopeData_t->currentPos.localSideralTime);
		printf("LCUImpl::getPosition Current RA = %lf \n", telescopeData_t->currentPos.RA);
		printf("LCUImpl::getPosition Current Dec = %lf \n", telescopeData_t->currentPos.Dec);
		printf("LCUImpl::getPosition Current HA = %lf \n", telescopeData_t->currentPos.HA);
		printf("LCUImpl::getPosition Current Alt = %lf \n", telescopeData_t->currentPos.Alt);
		printf("LCUImpl::getPosition Current Az = %lf \n", telescopeData_t->currentPos.Az);
		printf("LCUImpl::getPosition Target RA = %lf \n", telescopeData_t->targetPos.RA);
		printf("LCUImpl::getPosition Target Dec = %lf \n", telescopeData_t->targetPos.Dec);
		printf("LCUImpl::getPosition Target HA = %lf \n", telescopeData_t->targetPos.HA);
		printf("LCUImpl::getPosition Target Alt = %lf \n", telescopeData_t->targetPos.Alt);
		printf("LCUImpl::getPosition Target Az = %lf \n", telescopeData_t->targetPos.Az);
		printf("LCUImpl::getPosition Difference RA = %lf \n", telescopeData_t->differencePos.RA);
		printf("LCUImpl::getPosition Difference Dec = %lf \n", telescopeData_t->differencePos.Dec);	
		printf("LCUImpl::getPosition Page generated at: [%lf]\n", telescopeData_t->lcuTime);
    }


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


    return *telescopeData_t;
}
    

OUC::RawEncoderData LCUImpl::getRawEncodersPosition(const Ice::Current& c)
{
    extern int verbose;

    if( verbose )        
      printf( "LCUImpl::getRawEncoderPosition" );     
  
    /** Is telescope configured **/
    if(!m_configured)
      {
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

    /** LCU Local Time **/  
    rawEncoder_t->lcuTime = MiddleEndianToLittleEndian(m_lcu->getLCUMTime());
  
    /** Release semaphore for SHM **/
    m_lcu->postSemaphore(); 

    if (verbose)
      {
	printf("LCUImpl::getRawEncoderPosition Time elapsed since last access: %lf\n",rawEncoder_t->deltaT);
	printf("LCUImpl::getRawEncoderPosition Lecture Alpha AxisE: %6d\n", rawEncoder_t->lectAlphaAxisE);
	printf("LCUImpl::getRawEncoderPosition Position Alpha AxisE: %11d\n", rawEncoder_t->posAlphaAxisE);
	printf("LCUImpl::getRawEncoderPosition Lecture Alpha WormE: %6d\n", rawEncoder_t->lectAlphaWormE);
	printf("LCUImpl::getRawEncoderPosition Position Alpha WormE: %11d\n", rawEncoder_t->posAlphaWormE);
	printf("LCUImpl::getRawEncoderPosition Lecture Alpha Motor: %6d\n", rawEncoder_t->lectAlphaMotor);
	printf("LCUImpl::getRawEncoderPosition Position Alpha Motor: %11d\n", rawEncoder_t->posAlphaMotor);
	printf("LCUImpl::getRawEncoderPosition Lecture Delta AxisE: %6d\n", rawEncoder_t->lectDeltaAxisE);
	printf("LCUImpl::getRawEncoderPosition Position Delta AxisE: %11d\n", rawEncoder_t->posDeltaAxisE);
	printf("LCUImpl::getRawEncoderPosition Lecture Delta WormE: %6d\n", rawEncoder_t->lectDeltaWormE);
	printf("LCUImpl::getRawEncoderPosition Position Delta WormE: %11d\n", rawEncoder_t->posDeltaWormE);
	printf("LCUImpl::getRawEncoderPosition Lecture Delta Motor: %6d\n", rawEncoder_t->lectDeltaMotor);
	printf("LCUImpl::getRawEncoderPosition Position Delta Motor: %11d\n", rawEncoder_t->posDeltaMotor);
	printf("LCUImpl::getRawEncoderPosition Page generated at: [%lf]\n", rawEncoder_t->lcuTime);
      }

    return *rawEncoder_t;
}

    
OUC::EncoderData LCUImpl::getEncodersPosition(const Ice::Current& c)
{
    extern int verbose;

    if( verbose )        
      printf( "[LCUImpl::getEncoderPosition" );     
  
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

    if( verbose)
      {
	printf("LCUImpl::getEncoderPosition LT  = [%lf]\n", encoder_t->localTime);
        printf("LCUImpl::getEncoderPosition AW Position  = [%+10.0lf]\n", encoder_t->alphaWormE);    
	printf("LCUImpl::getEncoderPosition AA Position  = [%+10.0lf]\n", encoder_t->alphaAxisE);
	printf("LCUImpl::getEncoderPosition DW Position  = [%+10.0lf]\n", encoder_t->deltaWormE);
	printf("LCUImpl::getEncoderPosition DA Position  = [%+10.0lf]\n",  encoder_t->deltaAxisE);
	printf("LCUImpl::getEncoderPosition --------------------------------\n" );
	printf("LCUImpl::getEncoderPosition Page generated at: [%lf]\n", encoder_t->lcuTime);
	printf("LCUImpl::getEncoderPosition Good bye!\n" );
      }

    return *encoder_t;
}

void LCUImpl::getCurrentPosition(OUC::TelescopeData *telescopeData_t)
{
    extern int verbose;
  
    if( verbose ) 
      printf( "[LCUImpl::getCurrentPosition" );
    
    /** Is telescope configured **/
    if(!m_configured)
      {
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
    extern int verbose;
  
    if( verbose ) 
      printf( "[LCUImpl::getTargetPosition" );
    
    /** Is telescope configured **/
    if(!m_configured)
      {
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
