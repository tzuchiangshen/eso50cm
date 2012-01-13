#include <LCUImpl.h>

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

    /** Refresh Configuration and Tracking states **/
    getConfigState();
    getTrackingState();
    
    if( verbose ) 
      printf( "LCUImpl::setTelescopeConfig: Telescope configured!!\n" );
}

void 
LCUImpl::setTarget(const OUC::TelescopePosition& targetPos, const Ice::Current& c)
{
    extern int verbose;
    double RA, Dec =0.0;
    
    if( verbose )
      printf( "LCUImpl::setTarget" );

    /** Is telescope configured **/
    if(!m_configured)
    {
		OUC::TelescopeNotConfiguredEx ex;
        ex.reason = "Telecope Not Configured";
        throw ex;
    }
    
    if(verbose) 
      {
	printf( "LCUImpl::setTarget targetPos.RA=%lf\n", MiddleEndianToLittleEndian(targetPos.RA) );
	printf( "LCUImpl::setTarget targetPos.Dec=%lf\n",MiddleEndianToLittleEndian(targetPos.Dec) );
      }

    //convert to middle endian
    RA = MiddleEndianToLittleEndian(targetPos.RA);
    Dec = MiddleEndianToLittleEndian(targetPos.Dec);
    
    /** Acquire Semaphore for SHM **/
    m_lcu->waitSemaphore();
    
    /** Send new target **/
    if(m_lcu->telescope->setTarget(RA, Dec, (double*)&targetPos.Alt, (double*)&targetPos.Az) == 0) 
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
LCUImpl::setOffset(const OUC::TelescopePosition& offsetPos, const Ice::Current& c)
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
	//offsetPos.Dec *= -1.;
	double offsetDec = offsetPos.Dec * -1.0;
	delta_mtr_counts = m_lcu->telescope->delta->offsetAxisInDeg(offsetDec);
	if( alpha_mtr_counts < -50 || 50 < alpha_mtr_counts ) {
		m_lcu->telescope->alpha->Motor->setDeviceMemory( 7, & alpha_mtr_counts, 0  );
		goto_alpha_flag = true;
	}
	if( delta_mtr_counts < -50 || 50 < delta_mtr_counts ) 
	{
		m_lcu->telescope->delta->Motor->setDeviceMemory( 7, & delta_mtr_counts, 0  );
		goto_delta_flag = true;
	}
	m_lcu->postSemaphore();

	/** Goto target loop */
	no_quit = 180;
	do 
	{
		sleep( 1 );
		mem_address = 7;
		m_lcu->waitSemaphore();
		if( goto_alpha_flag ) 
		{
			m_lcu->telescope->alpha->Motor->readDeviceMemory( 7, & alpha_mtr_counts, 0  );
			if( -50 < alpha_mtr_counts && alpha_mtr_counts < 50 )
				goto_alpha_flag = false;
		}
		if( goto_delta_flag ) 
		{
		m_lcu->telescope->delta->Motor->readDeviceMemory( 7, & delta_mtr_counts, 0  );
		if( -50 < delta_mtr_counts && delta_mtr_counts < 50 )
			goto_delta_flag = false;
		}
		m_lcu->postSemaphore();
  	    
		if( ! goto_delta_flag && ! goto_alpha_flag ) 
		{
			no_quit = false;
		} else 
		{
			no_quit --;
		}
	} while( no_quit );

	/** Stop telescope status */
	m_lcu->waitSemaphore();
	m_lcu->telescope->setIsRunningGoto(false);
	m_lcu->postSemaphore();
}

void 
LCUImpl::setTracking(const OUC::TrackingInfo& trkInfo, const Ice::Current& c)
{
  extern int verbose;
  int velocity = trkInfo.ticVel;

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
      velocity = 600;

  /** Set Velocity **/
  m_lcu->waitSemaphore();
  m_lcu->telescope->alpha->Motor->setDeviceMemory(3, &velocity, 0);
  if(velocity > 0)
    m_lcu->telescope->setIsTracking(true);
  else
    m_lcu->telescope->setIsTracking(false);
  m_lcu->postSemaphore();

  /** Refresh tracking state **/
  getTrackingState();

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
  if(verbose)
    printf("LCUImpl::parkTelescope: LST for setTarget: %lf, Latitude for setTarget: %lf \n", targetPos.localSideralTime, m_lcu->telescope->getLatitude());
  targetPos.RA =  targetPos.localSideralTime;
  targetPos.Dec = m_lcu->telescope->getLatitude();
  if(verbose)
    printf("LCUImpl::parkTelescope: RA for setTarget: %lf, Dec for setTarget: %lf \n", targetPos.RA, targetPos.Dec);
  m_lcu->telescope->setTarget(targetPos.RA, targetPos.Dec, &targetPos.Alt, &targetPos.Az);
  /** Set Offsets **/
  targetPos.RA = m_lcu->telescope->getDifferenceRA();
  targetPos.Dec = m_lcu->telescope->getDifferenceDec();
  targetPos.RA *= -1.;
  alpha_mtr_counts = m_lcu->telescope->alpha->offsetAxisInDeg(targetPos.RA);
  targetPos.Dec *= -1.;
  delta_mtr_counts = m_lcu->telescope->delta->offsetAxisInDeg(targetPos.Dec);
  if(verbose)
    {
      printf("LCUImpl::parkTelescope: Moving Telescope to RA: %lf, Dec: %lf\n", targetPos.RA, targetPos.Dec);
      printf("LCUImpl::parkTelescope: Moving Telescope to alpha_counts: %d, delta_mtr_counts: %d\n", alpha_mtr_counts, delta_mtr_counts);
    }

  /** Stop Tracking **/
  if(m_lcu->telescope->getIsTracking()) 
    {
      int ticVel = 0;
      m_lcu->telescope->alpha->Motor->setDeviceMemory(3, &ticVel);
      m_lcu->telescope->setIsTracking(false);
      /* Refresh tracking state */
      m_tracking = false;
      if(verbose)
	printf("LCUImpl::parkTelescope: Tracking OFF!!");

    }

  /** Move telescope to zenith **/
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
        /** Reading current position */
	sleep( 1 );
	m_lcu->waitSemaphore();
	{
	  if( goto_alpha_flag ) {
	    m_lcu->telescope->alpha->Motor->readDeviceMemory( 7, & alpha_mtr_counts, 0  );
	    if(verbose)
	      printf("LCUImpl::parkTelescope: HW returns alpha_counts: %d\n", alpha_mtr_counts);
	    if( -50 < alpha_mtr_counts && alpha_mtr_counts < 50 )
	      goto_alpha_flag = false;
	  }
	  if( goto_delta_flag ) {
	    m_lcu->telescope->delta->Motor->readDeviceMemory( 7, & delta_mtr_counts, 0  );
	    if(verbose)
	      printf("LCUImpl::parkTelescope: HW returns delta_mtr_counts: %d\n",delta_mtr_counts);
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
      targetPos.RA =  targetPos.localSideralTime;
      targetPos.Dec = m_lcu->telescope->getLatitude();
      if(verbose)
	printf("LCUImpl::parkTelescope: RA for setTarget: %lf, Dec for setTarget: %lf \n", targetPos.RA, targetPos.Dec);
      /** Set Position **/
      m_lcu->telescope->setTarget(targetPos.RA, targetPos.Dec, &targetPos.Alt, &targetPos.Az);
      /** Set Offsets **/
      targetPos.RA = m_lcu->telescope->getDifferenceRA();
      targetPos.Dec = m_lcu->telescope->getDifferenceDec();
      targetPos.RA *= -1.;
      alpha_mtr_counts = m_lcu->telescope->alpha->offsetAxisInDeg(targetPos.RA);
      targetPos.Dec *= -1.;
      delta_mtr_counts = m_lcu->telescope->delta->offsetAxisInDeg(targetPos.Dec);
      if(verbose)
	{
	  printf("LCUImpl::parkTelescope: Moving Telescope to RA: %lf, Dec: %lf\n", targetPos.RA, targetPos.Dec);
	  printf("LCUImpl::parkTelescope: Moving Telescope to alpha_counts: %d, delta_mtr_counts: %d\n", alpha_mtr_counts, delta_mtr_counts);
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

void 
LCUImpl::stopTelescope(OUC::TelescopeDirection dir, const Ice::Current& c)
{
  extern int verbose;
  int     ticsPerSeconds= 0;

  if( verbose )
    printf( "LCUImpl::stopTelescope" );
  
  /** Is telescope configured **/
  if(!m_configured)
    {
      OUC::TelescopeNotConfiguredEx ex;
      ex.reason = "Telecope Not Configured";
      throw ex;
    }

  /** Acquire Semaphore for SHM **/
  m_lcu->waitSemaphore();
  
  ticsPerSeconds = 0;
  if(dir == OUC::North) {
    if (verbose) printf("Stopping North... Tics per second: %d\n", ticsPerSeconds);
    m_lcu->telescope->delta->Motor->setDeviceMemory(6, & ticsPerSeconds, 0);
  } else if(dir == OUC::South) {
    if (verbose) printf("Stopping South... Tics per seconds: %d\n", ticsPerSeconds);
    m_lcu->telescope->delta->Motor->setDeviceMemory(6, & ticsPerSeconds, 0  );
  } else if(dir == OUC::East) {
    if (verbose) printf("Stopping East... Tics per seconds: %d\n", ticsPerSeconds);
    m_lcu->telescope->alpha->Motor->setDeviceMemory(6, &ticsPerSeconds, 0);
  } else if(dir == OUC::West) {
    if (verbose) printf("Stopping West... Tics per seconds: %d\n", ticsPerSeconds);
    m_lcu->telescope->alpha->Motor->setDeviceMemory(6, &ticsPerSeconds, 0);
  }
  
  /** Release semaphore for SHM **/
  m_lcu->postSemaphore(); 
  
}

void 
LCUImpl::moveToTarget(const Ice::Current& c)
{
  extern int verbose;
  double offset_ra, offset_dec;
  int  alpha_mtr_counts;
  int  delta_mtr_counts;
  int no_quit;
  int setup_ready;
  int goto_alpha_flag = false;
  int goto_delta_flag = false;
  int value;
  int ticVel;

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
  
  /** Differesnce RA Dec  */
  offset_ra = m_lcu->telescope->getDifferenceRA();
  offset_dec = m_lcu->telescope->getDifferenceDec();
  offset_ra *= -1.;
  alpha_mtr_counts = m_lcu->telescope->alpha->offsetAxisInDeg( offset_ra );
  offset_dec *= -1.;
  delta_mtr_counts = m_lcu->telescope->delta->offsetAxisInDeg( offset_dec );
  if(verbose)
    {
      printf("LCUImpl::moveToTarget: Moving Telescope to RA: %lf, Dec: %lf\n", offset_ra, offset_dec);
      printf("LCUImpl::moveToTarget: Moving Telescope to alpha_counts: %d, delta_mtr_counts: %d\n", alpha_mtr_counts, delta_mtr_counts);
    }

  /** Stop Tracking **/
  if(m_lcu->telescope->getIsTracking()) 
    {
      ticVel = 0;
      m_lcu->telescope->alpha->Motor->setDeviceMemory(3, &ticVel, 0);
      m_lcu->telescope->setIsTracking(false);
      /* Refresh tracking state */
      m_tracking = false;
      if(verbose)
	printf("LCUImpl::moveToTarget: Tracking OFF!!");
    }

  /** Move Telescope to the requested position **/
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
  
  /** Release semaphore for SHM **/
  m_lcu->postSemaphore(); 

  /** Goto target loop*/
  setup_ready = NUM_OF_TRY;
  do {
    no_quit = 180;
    do {
      sleep( 1 );
      m_lcu->waitSemaphore();
      {
	if( goto_alpha_flag ) 
	  {
	    m_lcu->telescope->alpha->Motor->readDeviceMemory( 7, & alpha_mtr_counts, 0  );
	    if(verbose)
	      printf("LCUImpl::moveToTarget: HW returns alpha_counts: %d\n", alpha_mtr_counts);
	    if( -50 < alpha_mtr_counts && alpha_mtr_counts < 50 )
	      goto_alpha_flag = false;
	  }
	if( goto_delta_flag ) 
	  {
	    m_lcu->telescope->delta->Motor->readDeviceMemory( 7, & delta_mtr_counts, 0  );
	    if(verbose)
	      printf("LCUImpl::moveToTarget: HW returns delta_mtr_counts: %d\n",delta_mtr_counts);
	    if( -50 < delta_mtr_counts && delta_mtr_counts < 50 )
	      goto_delta_flag = false;
	  }
      }
      m_lcu->postSemaphore();
      
      if( ! goto_delta_flag && ! goto_alpha_flag ) 
	no_quit = false;
      else 
	no_quit --; 
      if( verbose) 
	printf( "LCUImpl::moveToTarget: To quit (if something was wrong)... %d sec.\n", no_quit );
    } while( no_quit );

    goto_alpha_flag = false;
    goto_delta_flag = false;
    
    m_lcu->waitSemaphore();
    {
      /** Differesnce RA Dec  */
      offset_ra  = m_lcu->telescope->getDifferenceRA();
      offset_dec = m_lcu->telescope->getDifferenceDec(); 
      offset_ra *= -1.;
      alpha_mtr_counts = m_lcu->telescope->alpha->offsetAxisInDeg( offset_ra );
      offset_dec *= -1.;
      delta_mtr_counts = m_lcu->telescope->delta->offsetAxisInDeg( offset_dec );
      if(verbose)
	{
	  printf("LCUImpl::moveToTarget: Moving Telescope to RA: %lf, Dec: %lf\n", offset_ra, offset_dec);
	  printf("LCUImpl::moveToTarget: Moving Telescope to alpha_counts: %d, delta_mtr_counts: %d\n", alpha_mtr_counts, delta_mtr_counts);
	}
      
      /** Move Telescope to the requested position **/
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
    
    if(verbose)
      printf( "LCUImpl::moveToTarget: Try No %d\n", NUM_OF_TRY - setup_ready );
  } while( setup_ready );
  
  /* Start Tracking */
  m_lcu->waitSemaphore();
  {
    ticVel = 602;
    m_lcu->telescope->alpha->Motor->setDeviceMemory(3, &ticVel, 0);
    m_lcu->telescope->setIsTracking(true);
    m_tracking = true;
    m_lcu->telescope->setIsRunningGoto(false);
    if (verbose)
      printf("LCUImpl::moveToTarget: Tracking ON!\n");
  }
  m_lcu->postSemaphore();
}
