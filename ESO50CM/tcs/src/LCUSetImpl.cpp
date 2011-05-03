#include <LCUImpl.h>
#include <IceE/IceE.h>

using namespace std;
#define NUM_OF_TRY 4

void 
LCUImpl::setConfiguration(const string& fileName, const Ice::Current& c)
{
    extern int verbose;
    
    if( verbose )
      printf( "LCUImpl::setTelescopeConfig" );
    
    /** Is telescope configured **/
    if(m_configured)
	printf("Telescope Already configured... re-loading configuration");
    
    
    /** Acquire Semaphore for SHM **/
    m_lcu->waitSemaphore();

    /*Reading configuration file and set parameters */
    string line;
    string keyword;
    string variable;
    string comment;
    int i, j;
    
    ifstream config_file(fileName.c_str());
    if( config_file.is_open() ) {
        while( ! config_file.eof() ) {
            getline( config_file, line );
            if( ! line.empty() ) {
                if( line[0] != '#' ) {
                    i = line.find_first_of( " ", 0 );
                    keyword = line.substr( 0, i );
                    j = line.find_first_not_of( " ", i );
                    i = line.find_first_of( " ", j );
                    variable = line.substr( j, i-j );
                    j = line.find_first_not_of( " ", i );
                    i = line.length();
                    comment = line.substr( j, j-i );
                    switch( keyword[0] ) {
                        int deg, min, sec;
                        double tmp;
                        case 'g':
                            sscanf( variable.c_str(), "%d*%d:%d", & deg, & min, & sec );
                            tmp = fabs( (double) deg) + ((double) min)/60.0 + ((double) sec)/3600.0;
                            if( deg < 0 ) {
                                tmp *= -1.0;
                            }
                            m_lcu->telescope->setLongitude( tmp );
                            m_lcu->telescope->setClockLongitude( tmp );
                            break;
                        case 't':
                            sscanf( variable.c_str(), "%d*%d:%d", & deg, & min, & sec );
                            tmp = fabs( (double) deg) + ((double) min)/60.0 + ((double) sec)/3600.0;
                            if( deg < 0 ) {
                                tmp *= -1.0;
                            }
                            m_lcu->telescope->setLatitude( tmp );
                            break;
                        case 'H':
                            tmp = atof( variable.c_str() );
                            m_lcu->telescope->setAltitude( tmp );
                            break;
                        case 'o':
                            tmp = atof( variable.c_str() );
                            m_lcu->telescope->setLowElevation( tmp );
                            break;
                        case 'h':
                            tmp = atof( variable.c_str() );
                            m_lcu->telescope->setHighElevation( tmp );
                            break;
                        case 'A':
                            switch( keyword[1] ) {
                                case 'M':
                                    switch( keyword[2] ) {
                                        case 'R':
                                            tmp = atof( variable.c_str() );
                                            m_lcu->telescope->alpha->Motor->setEncoderToAxis_Reduction( tmp );
                                            break;
                                        case 'T':
                                            tmp = atof( variable.c_str() );
                                            m_lcu->telescope->alpha->Motor->setTicsPerRev( tmp );
                                            break;
                                        case 'H':
                                            tmp = atof( variable.c_str() );
                                            m_lcu->telescope->alpha->Motor->setHomePosition( tmp );
                                            break;
                                        default:
                                            cout << variable << "   " << comment << "\n";
                                    }
                                    break;
                                case 'W':
                                    switch( keyword[2] ) {
                                        case 'R':
                                            tmp = atof( variable.c_str() );
                                            m_lcu->telescope->alpha->WormE->setEncoderToAxis_Reduction( tmp );
                                            break;
                                        case 'T':
                                            tmp = atof( variable.c_str() );
                                            m_lcu->telescope->alpha->WormE->setTicsPerRev( tmp );
                                            break;
                                        case 'H':
                                            tmp = atof( variable.c_str() );
                                            m_lcu->telescope->alpha->WormE->setHomePosition( tmp );
                                            break;
                                        default:
                                            cout << variable << "   " << comment << "\n";
                                    }
                                    break;
                                case 'A':
                                    switch( keyword[2] ) {
                                        case 'R':
                                            tmp = atof( variable.c_str() );
                                            m_lcu->telescope->alpha->AxisE->setEncoderToAxis_Reduction( tmp );
                                            break;
                                        case 'T':
                                            tmp = atof( variable.c_str() );
                                            m_lcu->telescope->alpha->AxisE->setTicsPerRev( tmp );
                                            break;
                                        case 'H':
                                            tmp = atof( variable.c_str() );
                                            m_lcu->telescope->alpha->AxisE->setHomePosition( tmp );
                                            break;
                                        default:
                                            cout << variable << "   " << comment << "\n";
                                    }
                                    break;
                            }
                            break;
                        case 'D':
                            switch( keyword[1] ) {
                                case 'M':
                                    switch( keyword[2] ) {
                                        case 'R':
                                            tmp = atof( variable.c_str() );
                                            m_lcu->telescope->delta->Motor->setEncoderToAxis_Reduction( tmp );
                                            break;
                                        case 'T':
                                            tmp = atof( variable.c_str() );
                                            m_lcu->telescope->delta->Motor->setTicsPerRev( tmp );
                                            break;
                                        case 'H':
                                            tmp = atof( variable.c_str() );
                                            m_lcu->telescope->delta->Motor->setHomePosition( tmp );
                                            break;
                                        default:
                                            cout <<  variable << "   " << comment << "\n";
                                    }
                                    break;
                                case 'W':
                                    switch( keyword[2] ) {
                                        case 'R':
                                            tmp = atof( variable.c_str() );
                                            m_lcu->telescope->delta->WormE->setEncoderToAxis_Reduction( tmp );
                                            break;
                                        case 'T':
                                            tmp = atof( variable.c_str() );
                                            m_lcu->telescope->delta->WormE->setTicsPerRev( tmp );
                                            break;
                                        case 'H':
                                            tmp = atof( variable.c_str() );
                                            m_lcu->telescope->delta->WormE->setHomePosition( tmp );
                                            break;
                                        default:
                                            cout <<  variable << "   " << comment << "\n";
                                    }
                                    break;
                                case 'A':
                                    switch( keyword[2] ) {
                                        case 'R':
                                            tmp = atof( variable.c_str() );
                                            m_lcu->telescope->delta->AxisE->setEncoderToAxis_Reduction( tmp );
                                            break;
                                        case 'T':
                                            tmp = atof( variable.c_str() );
                                            m_lcu->telescope->delta->AxisE->setTicsPerRev( tmp );
                                            break;
                                        case 'H':
                                            tmp = atof( variable.c_str() );
                                            m_lcu->telescope->delta->AxisE->setHomePosition( tmp );
                                            break;
                                        default:
                                            cout << "[set_config] " << variable << "   " << comment << "\n";
                                    }
                                    break;
                            }
                            break;
                        default:
                            cout << "[set_config] " << line << "\n";
                    }
                } else {
                    cout << "[set_config] " << line << "\n";
                }
            }
        }
        m_lcu->telescope->setIsConfigured( true );
        m_lcu->telescope->setIsRunningMonitor( 0 );
        m_lcu->telescope->setIsRunningGoto( 0 );
        m_lcu->telescope->setIsTracking( 0 );
        config_file.close();
    } else {
      if( verbose ) printf( "LCUImpl::setTelescopeConfig: Unable to open Configuration File!\n" );
    }
    
    /** Release semaphore for SHM **/
    m_lcu->postSemaphore(); 

    /** Telescope configured **/
    m_configured = true;

    if( verbose ) 
      printf( "LCUImpl::setTelescopeConfig: Telescope configured!!\n" );
}

void 
LCUImpl::setTarget(OUC::TelescopePosition& targetPos, const Ice::Current& c)
{
    extern int verbose;

    if( verbose )
      printf( "LCUImpl::setTarget" );

    /** Is telescope configured **/
    if(!m_configured)
      {
	OUC::TelescopeNotConfiguredEx ex;
        ex.reason = "Telecope Not Configured";
        throw ex;
      }

    /** Acquire Semaphore for SHM **/
    m_lcu->waitSemaphore();

    /** Send new target **/
    if(m_lcu->telescope->setTarget(targetPos.RA, targetPos.Dec, &targetPos.Alt, &targetPos.Az) == 0) 
      {
	char *limits;
	OUC::TargetOutOfLimitsEx ex;
	sprintf(limits, "Low Elevation: %lf, High Elevation: %lf", m_lcu->telescope->getHighElevation(), m_lcu->telescope->getHighElevation());
	ex.reason = "Target Out of Limits. Try a new one\n";
	ex.reason.append(limits); 
	throw ex;
      }
    
    /** Release semaphore for SHM **/
    m_lcu->postSemaphore();
    
    if( verbose )
      printf( "LCUImpl::setTarget Target Accepted!!" );
}

void 
LCUImpl::setOffset(OUC::TelescopePosition& offsetPos, const Ice::Current& c)
{
    extern int verbose;
    int  alpha_mtr_counts;
    int  delta_mtr_counts;
    char mem_address;
    
    int no_quit;
    int goto_alpha_flag = false;
    int goto_delta_flag = false;

    if( verbose )
      printf( "LCUImpl::setTarget" );

    /** Is telescope configured **/
    if(!m_configured)
      {
	OUC::TelescopeNotConfiguredEx ex;
        ex.reason = "Telecope Not Configured";
        throw ex;
      }
    
    /** Set telescope in running state and define offset **/
    m_lcu->waitSemaphore();
    m_lcu->telescope->setIsRunningGoto(true);
    alpha_mtr_counts = m_lcu->telescope->alpha->offsetAxisInDeg(offsetPos.RA);
    offsetPos.Dec *= -1.;
    delta_mtr_counts = m_lcu->telescope->delta->offsetAxisInDeg(offsetPos.Dec);
    if( alpha_mtr_counts < -50 || 50 < alpha_mtr_counts ) {
      m_lcu->telescope->alpha->Motor->setDeviceMemory( 7, & alpha_mtr_counts, 0  );
      goto_alpha_flag = true;
    }
    if( delta_mtr_counts < -50 || 50 < delta_mtr_counts ) {
      m_lcu->telescope->delta->Motor->setDeviceMemory( 7, & delta_mtr_counts, 0  );
      goto_delta_flag = true;
    }
    m_lcu->postSemaphore();
    
    /** Goto target loop */
    no_quit = 180;
    do {
      sleep( 1 );
      mem_address = 7;
      m_lcu->waitSemaphore();
      if( goto_alpha_flag ) {
	m_lcu->telescope->alpha->Motor->readDeviceMemory( 7, & alpha_mtr_counts, 0  );
	if( -50 < alpha_mtr_counts && alpha_mtr_counts < 50 )
	  goto_alpha_flag = false;
      }
      if( goto_delta_flag ) {
	m_lcu->telescope->delta->Motor->readDeviceMemory( 7, & delta_mtr_counts, 0  );
	if( -50 < delta_mtr_counts && delta_mtr_counts < 50 )
	  goto_delta_flag = false;
      }
      m_lcu->postSemaphore();
      
      if( ! goto_delta_flag && ! goto_alpha_flag ) {
	no_quit = false;
      } else {
	no_quit --;
      }
    } while( no_quit );

    /** Stop telescope status */
    m_lcu->waitSemaphore();
    m_lcu->telescope->setIsRunningGoto(false);
    m_lcu->postSemaphore();
}

void 
LCUImpl::setTracking(OUC::TrackingInfo& trkInfo, const Ice::Current& c)
{
  extern int verbose;
  if( verbose )
    printf( "LCUImpl::setTracking" );
  
  /** Is telescope configured **/
  if(!m_configured)
    {
      OUC::TelescopeNotConfiguredEx ex;
      ex.reason = "Telecope Not Configured";
      throw ex;
    }
  
  /** Read Tracking Flag **/
  if(trkInfo.trackState & !trkInfo.ticVel)
    trkInfo.ticVel = 600;

  /** Set Velocity **/
  m_lcu->waitSemaphore();
  m_lcu->telescope->alpha->Motor->setDeviceMemory(3, &trkInfo.ticVel, 0);
  if(trkInfo.ticVel > 0)
    m_lcu->telescope->setIsTracking(true);
  else
    m_lcu->telescope->setIsTracking(false);
  m_lcu->postSemaphore();

  if( verbose )
    printf( "LCUImpl::setTracking Tracking State: %d, ticks velocity: %d",trkInfo.trackState,trkInfo.ticVel);
}
  
void 
LCUImpl::parkTelescope(const Ice::Current& c)
{
  extern int verbose;
  int  alpha_mtr_counts;
  int  delta_mtr_counts;
  int goto_alpha_flag = false;
  int goto_delta_flag = false;
  OUC::TelescopePosition targetPos;
  int setup_ready;
  int no_quit;

  if( verbose )
    printf( "LCUImpl::parkTelescope" );
  
  /** Is telescope configured **/
  if(!m_configured)
    {
      OUC::TelescopeNotConfiguredEx ex;
      ex.reason = "Telecope Not Configured";
      throw ex;
    }  
  
  /**Calculate Parking Position **/
  m_lcu->waitSemaphore();
   /** Current Position */
  m_lcu->telescope->currentPosition(&targetPos.localSideralTime, &targetPos.RA, &targetPos.Dec, &targetPos.Alt, &targetPos.Az, &targetPos.HA);
  /** Set target = zenith */
  targetPos.RA = targetPos.localSideralTime;
  targetPos.Dec = m_lcu->telescope->getLatitude();
  m_lcu->telescope->setTarget(targetPos.RA, targetPos.Dec, &targetPos.Alt, &targetPos.Az);
  /** Set Offsets **/
  targetPos.RA = m_lcu->telescope->getDifferenceRA();
  targetPos.Dec = m_lcu->telescope->getDifferenceDec();
  targetPos.RA *= -1.;
  alpha_mtr_counts = m_lcu->telescope->alpha->offsetAxisInDeg(targetPos.RA);
  targetPos.Dec *= -1.;
  delta_mtr_counts = m_lcu->telescope->delta->offsetAxisInDeg(targetPos.Dec);
  if(verbose)
    printf("LCUImpl::parkTelescope: Moving Telescope to RA: %lf, Dec: %lf", targetPos.RA, targetPos.Dec);
  
  /** Stop Tracking **/
  if(m_lcu->telescope->getIsTracking()) 
    {
      int ticVel = 0;
      m_lcu->telescope->alpha->Motor->setDeviceMemory(3, &ticVel);
      m_lcu->telescope->setIsTracking(false);
      if(verbose)
	printf("LCUImpl::parkTelescope: Tracking OFF!!");

    }
  if( alpha_mtr_counts < -50 || 50 < alpha_mtr_counts ) 
    {
      m_lcu->telescope->alpha->Motor->setDeviceMemory( 7, & alpha_mtr_counts, 0  );
      goto_alpha_flag = true;
    }
  if( delta_mtr_counts < -50 || 50 < delta_mtr_counts ) 
    {
      m_lcu->telescope->delta->Motor->setDeviceMemory( 7, & delta_mtr_counts, 0  );
      goto_delta_flag = true;
    }
  m_lcu->postSemaphore();

  /** Goto target loop*/
  setup_ready = NUM_OF_TRY;
  do {
    no_quit = 180;
    do 
      {
	sleep( 1 );
	m_lcu->waitSemaphore();
	{
	  if( goto_alpha_flag ) {
	    m_lcu->telescope->alpha->Motor->readDeviceMemory( 7, & alpha_mtr_counts, 0  );
	    if( -50 < alpha_mtr_counts && alpha_mtr_counts < 50 )
	      goto_alpha_flag = false;
	  }
	  if( goto_delta_flag ) {
	    m_lcu->telescope->delta->Motor->readDeviceMemory( 7, & delta_mtr_counts, 0  );
	    if( -50 < delta_mtr_counts && delta_mtr_counts < 50 )
	      goto_delta_flag = false;
	  }
	}
	m_lcu->postSemaphore();
	
	if( ! goto_delta_flag && ! goto_alpha_flag )
	  no_quit = false;
	else 
	  no_quit --;
      }while(no_quit);
   
    goto_alpha_flag = false;
    goto_delta_flag = false;
    m_lcu->waitSemaphore();
    { 
      /** Current Position */
      m_lcu->telescope->currentPosition(&targetPos.localSideralTime, &targetPos.RA, &targetPos.Dec, &targetPos.Alt, &targetPos.Az, &targetPos.HA);
      targetPos.RA = targetPos.localSideralTime;
      targetPos.Dec = m_lcu->telescope->getLatitude();    
      /** Set Position **/
      m_lcu->telescope->setTarget(targetPos.RA, targetPos.Dec, &targetPos.Alt, &targetPos.Az);
      /** Set Offsets **/
      targetPos.RA = m_lcu->telescope->getDifferenceRA();
      targetPos.Dec = m_lcu->telescope->getDifferenceDec();
      targetPos.RA *= -1.;
      alpha_mtr_counts = m_lcu->telescope->alpha->offsetAxisInDeg(targetPos.RA);
      targetPos.Dec *= -1.;
      delta_mtr_counts = m_lcu->telescope->delta->offsetAxisInDeg(targetPos.Dec);
      if( alpha_mtr_counts < -50 || 50 < alpha_mtr_counts ) 
	{
	  m_lcu->telescope->alpha->Motor->setDeviceMemory( 7, & alpha_mtr_counts, 0  );
	  goto_alpha_flag = true;
	}
      if( delta_mtr_counts < -50 || 50 < delta_mtr_counts ) 
	{
	  m_lcu->telescope->delta->Motor->setDeviceMemory( 7, & delta_mtr_counts, 0  );
	  goto_delta_flag = true;
	}
    }
    m_lcu->postSemaphore();
    if( ! goto_alpha_flag && ! goto_delta_flag )
      setup_ready = false;
    else 
      setup_ready --;    
  }while(setup_ready);

  /** Stop telescope status */
  m_lcu->waitSemaphore();
  m_lcu->telescope->setIsRunningGoto(false);
  m_lcu->postSemaphore();
}
