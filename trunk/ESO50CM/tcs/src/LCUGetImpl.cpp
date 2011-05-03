#include <LCUImpl.h>
#include <IceE/IceE.h>

using namespace std;

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

bool LCUImpl::isConfigured(const Ice::Current& c)
{

  extern int verbose;
  if( verbose ) 
    printf( "[LCUImpl::isConfigured" );

  return m_configured;
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
    telescopeConfigData_t->localTime = mktime(m_lcu->telescope->getLocalTime());
 
    /** basic parameters **/
    telescopeConfigData_t->latitude = m_lcu->telescope->getLatitude();
    telescopeConfigData_t->longitude = m_lcu->telescope->getLongitude();
    telescopeConfigData_t->altitude = m_lcu->telescope->getAltitude();
    
    /** Alpha parametres **/
    telescopeConfigData_t->AMT = m_lcu->telescope->alpha->Motor->getTicsPerRev();
    telescopeConfigData_t->AMH = m_lcu->telescope->alpha->Motor->getHomePosition();
    telescopeConfigData_t->AMR = m_lcu->telescope->alpha->Motor->getEncoderToAxis_Reduction();
    telescopeConfigData_t->AWT = m_lcu->telescope->alpha->WormE->getTicsPerRev();
    telescopeConfigData_t->AWH = m_lcu->telescope->alpha->WormE->getHomePosition();
    telescopeConfigData_t->AWR = m_lcu->telescope->alpha->WormE->getEncoderToAxis_Reduction();
    telescopeConfigData_t->AAT = m_lcu->telescope->alpha->AxisE->getTicsPerRev();
    telescopeConfigData_t->AAH = m_lcu->telescope->alpha->AxisE->getHomePosition();
    telescopeConfigData_t->AAR = m_lcu->telescope->alpha->AxisE->getEncoderToAxis_Reduction();

    /** Delta parameters **/
    telescopeConfigData_t->DMT = m_lcu->telescope->delta->Motor->getTicsPerRev();
    telescopeConfigData_t->DMH = m_lcu->telescope->delta->Motor->getHomePosition();
    telescopeConfigData_t->DMR = m_lcu->telescope->delta->Motor->getEncoderToAxis_Reduction();
    telescopeConfigData_t->DWT = m_lcu->telescope->delta->WormE->getTicsPerRev();
    telescopeConfigData_t->DWH = m_lcu->telescope->delta->WormE->getHomePosition();
    telescopeConfigData_t->DWR = m_lcu->telescope->delta->WormE->getEncoderToAxis_Reduction();
    telescopeConfigData_t->DAT = m_lcu->telescope->delta->AxisE->getTicsPerRev();
    telescopeConfigData_t->DAH = m_lcu->telescope->delta->AxisE->getHomePosition();
    telescopeConfigData_t->DAR = m_lcu->telescope->delta->AxisE->getEncoderToAxis_Reduction();
      
    /** LCU Local Time **/  
    telescopeConfigData_t->lcuTime = mktime(m_lcu->getLCU_Time());
   
    /** Release semaphore for SHM **/
    m_lcu->postSemaphore();

    if(verbose)
      {
	printf("LCUImpl::getTelescopeConfig LT  = [%ld]\n", (long int)telescopeConfigData_t->localTime);	
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
	printf("LCUImpl::getTelescopeConfig Page generated at: [%ld]\n", (long int)telescopeConfigData_t->lcuTime);
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

    /** Acquire Semaphore for SHM **/
    m_lcu->waitSemaphore();
    
    /** Acquire Times **/
    telescopeData_t->localTime = mktime(m_lcu->telescope->getLocalTime());
    telescopeData_t->deltaT = m_lcu->telescope->getDeltaT();
    telescopeData_t->julianDate = m_lcu->telescope->getJulianDate();
    
    /** Telescope Position **/
    m_lcu->telescope->currentPosition(&telescopeData_t->currentPos.localSideralTime, &telescopeData_t->currentPos.RA, &telescopeData_t->currentPos.Dec, &telescopeData_t->currentPos.Alt, &telescopeData_t->currentPos.Az, &telescopeData_t->currentPos.HA);
    m_lcu->telescope->targetPosition(&telescopeData_t->targetPos.localSideralTime, &telescopeData_t->targetPos.RA, &telescopeData_t->targetPos.Dec, &telescopeData_t->targetPos.Alt, &telescopeData_t->targetPos.Az, &telescopeData_t->targetPos.HA);
    telescopeData_t->latitude = m_lcu->telescope->getLatitude();
    telescopeData_t->longitude = m_lcu->telescope->getLongitude(); 
    telescopeData_t->longitude = m_lcu->telescope->getAltitude();
    telescopeData_t->lowElevation = m_lcu->telescope->getLowElevation(); 
    telescopeData_t->highElevation = m_lcu->telescope->getHighElevation();
        
    /** LCU Local Time **/  
    telescopeData_t->lcuTime = mktime(m_lcu->getLCU_Time());
    
    /** Release semaphore for SHM **/
    m_lcu->postSemaphore();  

    if (verbose)
      {
	printf("LCUImpl::getPosition LT  = [%ld]\n", (long int)telescopeData_t->localTime);
	printf("LCUImpl::getPosition Time elapsed since last access: %lf\n",telescopeData_t->deltaT);
	printf("LCUImpl::getPosition JD  = %lf\n", telescopeData_t->julianDate);
	printf("LCUImpl::getTelescopeConfig Latitude = %+11.4lf \n", telescopeData_t->latitude);
	printf("LCUImpl::getTelescopeConfig Longitude = %+11.4lf \n", telescopeData_t->longitude);
	printf("LCUImpl::getTelescopeConfig Altitude = %+11.4lf \n", telescopeData_t->altitude);
	printf("LCUImpl::getTelescopeConfig High Elevation = %+11.4lf \n", telescopeData_t->highElevation);
	printf("LCUImpl::getTelescopeConfig Low Elevation = %+11.4lf \n", telescopeData_t->lowElevation);
	printf("LCUImpl::getPosition LST = %lf \n", telescopeData_t->currentPos.localSideralTime);
	printf("LCUImpl::getPosition RA = %lf \n", telescopeData_t->currentPos.RA);
	printf("LCUImpl::getPosition Dec = %lf \n", telescopeData_t->currentPos.Dec);
	printf("LCUImpl::getPosition HA = %lf \n", telescopeData_t->currentPos.HA);
	printf("LCUImpl::getPosition Alt = %lf \n", telescopeData_t->currentPos.Alt);
	printf("LCUImpl::getPosition Az = %lf \n", telescopeData_t->currentPos.Az);
	printf("LCUImpl::getPosition RA = %lf \n", telescopeData_t->targetPos.RA);
	printf("LCUImpl::getPosition Dec = %lf \n", telescopeData_t->targetPos.Dec);
	printf("LCUImpl::getPosition HA = %lf \n", telescopeData_t->targetPos.HA);
	printf("LCUImpl::getPosition Alt = %lf \n", telescopeData_t->targetPos.Alt);
	printf("LCUImpl::getPosition Az = %lf \n", telescopeData_t->targetPos.Az);
	printf("LCUImpl::getPosition Page generated at: [%ld]\n", (long int)telescopeData_t->lcuTime);
      }

    return *telescopeData_t;
}
    



OUC::EncoderData LCUImpl::getEncodersPosition(const Ice::Current& c)
{
    extern int verbose;
    char string_data[32];

    if( verbose ) {
        printf( "[LCUImpl::getEncoderPosition" );
        //if( * arguments != 0 ) {
        //    printf( "[LCUImpl::get_encoder_position] %s\n", arguments );
        //}
    }
    //myLCU * lcu;
    //lcu = new myLCU();
    //m_lcu->createTelescope();
    //double lst, ra, dec, alt, az;

    /** Acquire Semaphore for SHM */
    m_lcu->waitSemaphore();

    /** We have gor access to the User Shared Memory segment
        then we can translate low level telescope data
        to user's format.
        The corresponding telescope routine waits until it
        have access to Instrument Shared memory
        segment, and after read it will free the acces. */
    //m_lcu->telescope->currentPosition( & lst, & ra, & dec, &alt, &az );
    //printf( "[LCUImpl::get_encoder_position] No HTML output!\n" );
   

    /** Local Time */
    encoder_t->localTime = mktime(m_lcu->telescope->getLocalTime());
 
    /** Encoder Position **/
    //Alpha
    encoder_t->alphaWormE =  m_lcu->telescope->alpha->WormE->getPosition();
    encoder_t->alphaAxisE =  m_lcu->telescope->alpha->AxisE->getPosition();
    //Delta
    encoder_t->deltaWormE = m_lcu->telescope->delta->WormE->getPosition();
    encoder_t->deltaAxisE = m_lcu->telescope->delta->AxisE->getPosition();
  

    /** LCU Local Time **/  
    encoder_t->lcuTime = mktime(m_lcu->getLCU_Time());
   
    /** Release semaphore for SHM **/
    m_lcu->postSemaphore();

    if( verbose)
      {
	printf("LCUImpl::getEncoderPosition LT  = [%ld]\n", (long int)encoder_t->localTime);
        printf("LCUImpl::getEncoderPosition AW Position  = [%+10.0lf]\n", encoder_t->alphaWormE);    
	printf("LCUImpl::getEncoderPosition AA Position  = [%+10.0lf]\n", encoder_t->alphaAxisE);
	printf("LCUImpl::getEncoderPosition DW Position  = [%+10.0lf]\n", encoder_t->deltaWormE);
	printf("LCUImpl::getEncoderPosition DA Position  = [%+10.0lf]\n",  encoder_t->deltaAxisE);
	printf("LCUImpl::getEncoderPosition --------------------------------\n" );
	printf("LCUImpl::getEncoderPosition Page generated at: [%ld]\n", (long int)encoder_t->lcuTime);
	printf("LCUImpl::getEncoderPosition Good bye!\n" );
      }

    return *encoder_t;
}
