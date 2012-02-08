#include <LCUImpl.h>

using namespace std;
#define NUM_OF_TRY 4

void 
LCUImpl::setConfiguration(const string& fileName, const Ice::Current& c)
{
   setConfiguration(fileName);
}

void 
LCUImpl::setConfiguration(const string& fileName)
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
      printf( "LCUImpl::setTarget\n" );

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
      printf( "LCUImpl::setTarget Target Accepted!!\n" );
}

void 
LCUImpl::setOffset(const ::OUC::TelescopePosition& offset, const Ice::Current& c)
{
    OUC::TelescopePosition tmp;
	tmp.RA = MiddleEndianToLittleEndian(offset.RA);
	tmp.Dec = MiddleEndianToLittleEndian(offset.Dec);
	tmp = (const ::OUC::TelescopePosition&)tmp;
    setOffset(tmp); 
    
}

void LCUImpl::setOffset(const ::OUC::TelescopePosition& offset) {
	/** 1. Stop tracking 
	    2. Calculate the offset 
		2.1. Set & retrieve the current position  RA, Dec in degree
		2.2. Set & retrieve tht target position   RA, Dec in degree (may be done in advance)
		2.3. Calculate the difference (offset) RA, Dec in degree
		2.4. Convert offset in term of count of encoders in the motors (incremental encoders)
		3. Move the telescope 
		3.1. Set the amount of counts of encoders to be move in the motors, and start the movement
		3.2. Monitor until the abs(counts) < 50
		4. Start tracking 
	**/
	extern int verbose;
	int  alpha_mtr_counts;
	int  delta_mtr_counts;
	int  alpha_axisE_counts;
	int  delta_axisE_counts;
	int  alpha_wormE_counts;
	int  delta_wormE_counts;
	char mem_address;
    OUC::TelescopePosition offsetPos;
	
	int no_quit;
	int goto_alpha_flag = false;
	int goto_delta_flag = false;

	if( verbose )
		printf( "LCUImpl::setOffset\n" );
	
	/** Is telescope configured **/
	if(!m_configured)
	{
		OUC::TelescopeNotConfiguredEx ex;
		ex.reason = "Telecope Not Configured";
		throw ex;
	}
	/** 2.3. Calculate the difference (offset) RA, Dec in degree **/
	printf("[LCUImpl::setOffset] ra=%.lf, dec=%.lf\n", offset.RA, offset.Dec);
	offsetPos.RA = offset.RA;
	offsetPos.Dec = offset.Dec;
	printf("[LCUImpl::setOffset] ra=%.lf, dec=%.lf\n", offsetPos.RA, offsetPos.Dec);
    
	/** 2.4. Convert offset in term of count of encoders in the motors (incremental encoders) **/
	m_lcu->waitSemaphore();
	m_lcu->telescope->setIsRunningGoto(true);
    //TSH: why we need to change the sign??
	//offsetPos.RA *= -1.;
    alpha_mtr_counts = m_lcu->telescope->alpha->degToCountMotorEnc(offsetPos.RA);
    offsetPos.Dec *= -1.;
    delta_mtr_counts = m_lcu->telescope->delta->degToCountMotorEnc(offsetPos.Dec);
    printf("LCUImpl::setOffset: Will add an offset in the telescope position in RA: %lf, Dec: %lf\n", offsetPos.RA, offsetPos.Dec);
    printf("LCUImpl::setOffset: Will move the motors to alpha_mtr_counts: %d, delta_mtr_counts: %d\n", alpha_mtr_counts, delta_mtr_counts);
    alpha_axisE_counts = m_lcu->telescope->alpha->degToCountAxisEnc(offsetPos.RA);
    delta_axisE_counts = m_lcu->telescope->delta->degToCountAxisEnc(offsetPos.Dec);
    printf("LCUImpl::setOffset: Will move the axisE to alpha_axisE_counts: %d, delta_axisE_counts: %d\n", alpha_axisE_counts, delta_axisE_counts);
    alpha_wormE_counts = m_lcu->telescope->alpha->degToCountWormEnc(offsetPos.RA);
    delta_wormE_counts = m_lcu->telescope->delta->degToCountWormEnc(offsetPos.Dec);
    printf("LCUImpl::setOffset: Will move the WormE to alpha_wormE_counts: %d, delta_wormE_counts: %d\n", alpha_wormE_counts, delta_wormE_counts);
     
	/** 3. Move the telescope  **/
	/**	3.1. Set the amount of counts of encoders to be move in the motors, and start the movement **/

	if( alpha_mtr_counts < -50 || 50 < alpha_mtr_counts ) {
        m_lcu->telescope->alpha->Motor->runEncSteps(alpha_mtr_counts);
		goto_alpha_flag = true;
	}
	if( delta_mtr_counts < -50 || 50 < delta_mtr_counts ) 
	{
        m_lcu->telescope->delta->Motor->runEncSteps(delta_mtr_counts);
		goto_delta_flag = true;
	}
	m_lcu->postSemaphore();

	/** 3.2. Monitor until the abs(counts) < 50 **/
	/** Goto target loop */
	no_quit = 180;
	do 
	{
		/** Reading current position */
		m_lcu->waitSemaphore();
		if( goto_alpha_flag ) 
		{
			m_lcu->telescope->alpha->Motor->readDeviceMemory( 7, & alpha_mtr_counts, 0  );
			printf("LCUImpl::setOffset: MotorE returns alpha_counts: %d\n", alpha_mtr_counts);
			if( -50 < alpha_mtr_counts && alpha_mtr_counts < 50 ) {
				printf("LCUImpl::setOffset: reached to the target, stopping the motor in RA\n");
				goto_alpha_flag = false;
			}
		}
		if( goto_delta_flag ) 
		{
		    m_lcu->telescope->delta->Motor->readDeviceMemory( 7, & delta_mtr_counts, 0  );
			printf("LCUImpl::setOffset: MotorE returns delta_counts: %d\n", delta_mtr_counts);
		    if( -50 < delta_mtr_counts && delta_mtr_counts < 50 ) {
			   printf("LCUImpl::setOffset: reached to the target, stopping the motor in Dec\n");
			   goto_delta_flag = false;
			}
		}
		m_lcu->postSemaphore();
  	    
		if( ! goto_delta_flag && ! goto_alpha_flag ) 
			no_quit = false;
		else 
			no_quit --;

	    sleep(1);
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
    printf( "[LCUImpl::setTracking]\n" );
  
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
    printf( "[LCUImpl::setTracking] Tracking State: %d, ticks velocity: %d\n",trkInfo.trackState,trkInfo.ticVel);
}
  
void 
LCUImpl::parkTelescope(const Ice::Current& c)
{
	/** 1. Stop tracking 
	    2. Calculate the offset 
		2.1. Set & retrieve the current position  RA, Dec in degree
		2.2. Set & retrieve tht target position   RA, Dec in degree (may be done in advance)
		2.3. Calculate the difference (offset) RA, Dec in degree
		2.4. Convert offset in term of count of encoders in the motors (incremental encoders)
		3. Move the telescope 
		3.1. Set the amount of counts of encoders to be move in the motors, and start the movement
		3.2. Monitor until the abs(counts) < 50
		4. Start tracking 
	**/

    extern int verbose;
    int  alpha_mtr_counts;
    int  delta_mtr_counts;
    int  alpha_axisE_counts;
    int  delta_axisE_counts;
    int  alpha_wormE_counts;
    int  delta_wormE_counts;
    int goto_alpha_flag = true;
    int goto_delta_flag = true;
    OUC::TelescopePosition targetPos;
    int setup_ready;
    int no_quit;
  
    printf( "LCUImpl::parkTelescope" );
    
    /** Is telescope configured **/
    if(!m_configured) {
        OUC::TelescopeNotConfiguredEx ex;
        ex.reason = "Telecope Not Configured";
        throw ex;
    }  
    
    /** 1. Stop Tracking **/
    if(m_lcu->telescope->getIsTracking()) {
        int ticVel = 0;
        m_lcu->telescope->alpha->Motor->setDeviceMemory(3, &ticVel);
        m_lcu->telescope->setIsTracking(false);
        /* Refresh tracking state */
        m_tracking = false;
  	    printf("LCUImpl::parkTelescope: Tracking OFF!!");
    }

	/**    2. Calculate the offset  **/

	/**	2.1. Retrieve the current position  RA, Dec in degree **/
    m_lcu->waitSemaphore();
    m_lcu->telescope->currentPosition(&targetPos.localSideralTime, &targetPos.RA, &targetPos.Dec, &targetPos.Alt, &targetPos.Az, &targetPos.HA);

	/**2.2. Set & retrieve tht target position   RA, Dec in degree (may be done in advance) **/
    /** Set target = zenith */
	printf("LCUImpl::parkTelescope: LST for setTarget: %lf, Latitude for setTarget: %lf \n", targetPos.localSideralTime, m_lcu->telescope->getLatitude());
    targetPos.RA =  targetPos.localSideralTime;
    targetPos.Dec = m_lcu->telescope->getLatitude();
    printf("LCUImpl::parkTelescope: RA for setTarget: %lf, Dec for setTarget: %lf \n", targetPos.RA, targetPos.Dec);
    m_lcu->telescope->setTarget(targetPos.RA, targetPos.Dec, &targetPos.Alt, &targetPos.Az);

	/** 2.3. Calculate the difference (offset) RA, Dec in degree **/
    targetPos.RA = m_lcu->telescope->getDifferenceRA();
    targetPos.Dec = m_lcu->telescope->getDifferenceDec();
    printf("required offset in RA---------------> %lf\n", targetPos.RA);
    printf("required offset in Dec--------------> %lf\n", targetPos.Dec);

	/** 2.4. Convert offset in term of count of encoders in the motors (incremental encoders) **/
	//calculalate the amount of encoder counts (motor) to move in RA
    targetPos.RA *= -1.;
    alpha_mtr_counts = m_lcu->telescope->alpha->degToCountMotorEnc(targetPos.RA);
    targetPos.Dec *= -1.;
    delta_mtr_counts = m_lcu->telescope->delta->degToCountMotorEnc(targetPos.Dec);
    printf("LCUImpl::parkTelescope: Will move the telescope to RA: %lf, Dec: %lf\n", targetPos.RA, targetPos.Dec);
    printf("LCUImpl::parkTelescope: Will move the motors to alpha_mtr_counts: %d, delta_mtr_counts: %d\n", alpha_mtr_counts, delta_mtr_counts);
    alpha_axisE_counts = m_lcu->telescope->alpha->degToCountAxisEnc(targetPos.RA);
    delta_axisE_counts = m_lcu->telescope->delta->degToCountAxisEnc(targetPos.Dec);
    printf("LCUImpl::parkTelescope: Will move the axisE to alpha_axisE_counts: %d, delta_axisE_counts: %d\n", alpha_axisE_counts, delta_axisE_counts);
    alpha_wormE_counts = m_lcu->telescope->alpha->degToCountWormEnc(targetPos.RA);
    delta_wormE_counts = m_lcu->telescope->delta->degToCountWormEnc(targetPos.Dec);
    printf("LCUImpl::parkTelescope: Will move the WormE to alpha_wormE_counts: %d, delta_wormE_counts: %d\n", alpha_wormE_counts, delta_wormE_counts);
     
	/** 3. Move the telescope  **/
	/**	3.1. Set the amount of counts of encoders to be move in the motors, and start the movement **/
    // Move telescope to zenith 
    if( alpha_mtr_counts < -50 || 50 < alpha_mtr_counts ) {
        m_lcu->telescope->alpha->Motor->runEncSteps(alpha_mtr_counts);
        goto_alpha_flag = true;
    }

    if( delta_mtr_counts < -50 || 50 < delta_mtr_counts ) {
        m_lcu->telescope->delta->Motor->runEncSteps(delta_mtr_counts);
        goto_delta_flag = true;
    }

    m_lcu->postSemaphore();
  
	/** 3.2. Monitor until the abs(counts) < 50 **/
    //setup_ready = NUM_OF_TRY;
	// TSH: if the HW works properly, there is no reason to send the same command 4 times
    setup_ready = 0;
    do {
        no_quit = 180;
        do {
            /** Reading current position */
    	    m_lcu->waitSemaphore();
    	    {
    	        if( goto_alpha_flag ) {
    	            m_lcu->telescope->alpha->Motor->readDeviceMemory( 7, & alpha_mtr_counts, 0  );
    	            printf("LCUImpl::parkTelescope: HW returns alpha_counts: %d\n", alpha_mtr_counts);
    	            if( -50 < alpha_mtr_counts && alpha_mtr_counts < 50 ) {
						printf("LCUImpl::parkTelescope: reached to the target, stopping the motor in RA\n");
    	                //m_lcu->telescope->alpha->Motor->readDeviceMemory( 7, & alpha_mtr_counts, 0  );
    	                goto_alpha_flag = false;
					}

    	        }
    	        if( goto_delta_flag ) {
    	            m_lcu->telescope->delta->Motor->readDeviceMemory( 7, & delta_mtr_counts, 0  );
    	            printf("LCUImpl::parkTelescope: HW returns delta_mtr_counts: %d\n",delta_mtr_counts);
    	            if( -50 < delta_mtr_counts && delta_mtr_counts < 50 ) {
						printf("LCUImpl::parkTelescope: reached to the target, stopping the motor in Dec\n");
    	                //m_lcu->telescope->delta->Motor->readDeviceMemory( 7, & delta_mtr_counts, 0  );
    	                goto_delta_flag = false;
					}
    	        }
    	    }
    	    m_lcu->postSemaphore();
    	    
    	    if( ! goto_delta_flag && ! goto_alpha_flag )
    	        no_quit = false;
    	    else 
    	        no_quit --;

    	    sleep( 1 );
        } while(no_quit);


//            setup_ready --;    
    } while(setup_ready);


	/** 4. Start tracking  **/ 
	//no necessary in parkTelescope
	printf("LCUImpl::parkTelescope: telescope parked\n");
  
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
        printf( "[LCUImpl::stopTelescope]\n" );
    
    /** Acquire Semaphore for SHM **/
    m_lcu->waitSemaphore();
    
    ticsPerSeconds = 0;
    m_lcu->telescope->delta->Motor->setDeviceMemory(6, & ticsPerSeconds, 0);
    m_lcu->telescope->alpha->Motor->setDeviceMemory(6, &ticsPerSeconds, 0);
    
    /** Release semaphore for SHM **/
    m_lcu->postSemaphore(); 
    if( verbose )
        printf( "[LCUImpl::stopTelescope]: stop command sent to alpha and delta motors\n" );
  
}

void 
LCUImpl::moveToTarget(const Ice::Current& c)
{
 	/** 1. Stop tracking 
	    2. Calculate the offset 
		2.1. Set & retrieve the current position  RA, Dec in degree
		2.2. Set & retrieve tht target position   RA, Dec in degree (may be done in advance)
		2.3. Calculate the difference (offset) RA, Dec in degree
		2.4. Convert offset in term of count of encoders in the motors (incremental encoders)
		3. Move the telescope 
		3.1. Set the amount of counts of encoders to be move in the motors, and start the movement
		3.2. Monitor until the abs(counts) < 50
		4. Start tracking 
	**/
    extern int verbose;
    double offset_ra, offset_dec;
    int  alpha_mtr_counts;
    int  delta_mtr_counts;
    int  alpha_axisE_counts;
    int  delta_axisE_counts;
    int  alpha_wormE_counts;
    int  delta_wormE_counts;
    int no_quit;
    int setup_ready;
    int goto_alpha_flag = false;
    int goto_delta_flag = false;
    int value;
    int ticVel;
  
    if( verbose )
        printf( "LCUImpl::moveToTarget\n" );
    
    /** Is telescope configured **/
    if(!m_configured) {
        OUC::TelescopeNotConfiguredEx ex;
        ex.reason = "Telecope Not Configured";
        throw ex;
    }
    
    /** Acquire Semaphore for SHM **/
    m_lcu->waitSemaphore();
    
    // 1. Stop tracking  
    if(m_lcu->telescope->getIsTracking()){
        ticVel = 0;
        m_lcu->telescope->alpha->Motor->setDeviceMemory(3, &ticVel, 0);
        m_lcu->telescope->setIsTracking(false);
        /* Refresh tracking state */
        m_tracking = false;
        if(verbose)
	        printf("[LCUImpl::moveToTarget]: Tracking OFF!!");
    }

	// 2. Calculate the offset 
    // 2.3. Calculate the difference (offset) RA, Dec in degree
    offset_ra = m_lcu->telescope->getDifferenceRA();
    offset_dec = m_lcu->telescope->getDifferenceDec();
		if(verbose) { 
		    printf("LCUImpl::moveToTarget: Moving Telescope to RA: %lf, Dec: %lf\n", m_lcu->telescope->getTargetRA(), m_lcu->telescope->getTargetDec());
    }

	/** 2.4. Convert offset in term of count of encoders in the motors (incremental encoders) **/
    offset_ra *= -1.;
    alpha_mtr_counts = m_lcu->telescope->alpha->degToCountMotorEnc(offset_ra);
    offset_dec *= -1.;
    delta_mtr_counts = m_lcu->telescope->delta->degToCountMotorEnc(offset_dec);
    printf("[LCUImpl::moveToTarget]: Will add an offset in the telescope position in RA: %lf, Dec: %lf\n", offset_ra, offset_dec);
    printf("[LCUImpl::moveToTarget]: Will move the motors to alpha_mtr_counts: %d, delta_mtr_counts: %d\n", alpha_mtr_counts, delta_mtr_counts);
    alpha_axisE_counts = m_lcu->telescope->alpha->degToCountAxisEnc(offset_ra);
    delta_axisE_counts = m_lcu->telescope->delta->degToCountAxisEnc(offset_dec);
    printf("[LCUImpl::moveToTarget]: Will move the axisE to alpha_axisE_counts: %d, delta_axisE_counts: %d\n", alpha_axisE_counts, delta_axisE_counts);
    alpha_wormE_counts = m_lcu->telescope->alpha->degToCountWormEnc(offset_ra);
    delta_wormE_counts = m_lcu->telescope->delta->degToCountWormEnc(offset_dec);
    printf("[LCUImpl::moveToTarget]: Will move the WormE to alpha_wormE_counts: %d, delta_wormE_counts: %d\n", alpha_wormE_counts, delta_wormE_counts);


	/** 3. Move the telescope  **/
	/**	3.1. Set the amount of counts of encoders to be move in the motors, and start the movement **/
 	if( alpha_mtr_counts < -50 || 50 < alpha_mtr_counts ) {
        m_lcu->telescope->alpha->Motor->runEncSteps(alpha_mtr_counts);
		goto_alpha_flag = true;
	}
	if( delta_mtr_counts < -50 || 50 < delta_mtr_counts ) 
	{
        m_lcu->telescope->delta->Motor->runEncSteps(delta_mtr_counts);
		goto_delta_flag = true;
	}
    m_lcu->postSemaphore(); 

	/** 3.2. Monitor until the abs(counts) < 50 **/
	/** Goto target loop */
    setup_ready = 0;
    do {
        no_quit = 180;
        do {
            //sleep( 1 );
            m_lcu->waitSemaphore();
           	if( goto_alpha_flag ) {
		    	m_lcu->telescope->alpha->Motor->readDeviceMemory( 7, & alpha_mtr_counts, 0  );
		    	printf("[LCUImpl::moveToTarget]: MotorE returns alpha_counts: %d\n", alpha_mtr_counts);
		    	if( -50 < alpha_mtr_counts && alpha_mtr_counts < 50 ) {
		    		printf("[LCUImpl::moveToTarget]: reached to the target, stopping the motor in RA\n");
		    		goto_alpha_flag = false;
		    	}
		    }
		    if( goto_delta_flag ) {
		        m_lcu->telescope->delta->Motor->readDeviceMemory( 7, & delta_mtr_counts, 0  );
		    	printf("[LCUImpl::moveToTarget]: MotorE returns delta_counts: %d\n", delta_mtr_counts);
		        if( -50 < delta_mtr_counts && delta_mtr_counts < 50 ) {
		    	   printf("LCUImpl::moveToTarget]: reached to the target, stopping the motor in Dec\n");
		    	   goto_delta_flag = false;
		    	}
		    }
            m_lcu->postSemaphore();
            if( ! goto_delta_flag && ! goto_alpha_flag ) 
                no_quit = false;
            else 
                no_quit --; 

			if(no_quit == 0) {
				printf("[LCUImpl::moveToTarget]: Time out to reach the target. 180 seconds.\n");
			}
			sleep(1);
        } while( no_quit );
	} while(setup_ready);

    //4. Start tracking 
    m_lcu->waitSemaphore();
    ticVel = 602;
    m_lcu->telescope->alpha->Motor->setDeviceMemory(3, &ticVel, 0);
    m_lcu->telescope->setIsTracking(true);
    m_tracking = true;
    m_lcu->telescope->setIsRunningGoto(false);
    if (verbose)
        printf("[LCUImpl::moveToTarget]: Tracking ON!\n");
    m_lcu->postSemaphore();
}


void 
LCUImpl::handsetSlew(const OUC::SlewInfo& slewInfo, const Ice::Current& c)
{

    char slew_rate = 255;
    char slew_dir = 255;
	char special_case = 255;
    bool is_telescope_configured = false;
    double degs_per_sec;
    int tics_per_sec;

    slew_rate = slewInfo.rateName[0];
	special_case = slewInfo.rateName[1];
    slew_dir = slewInfo.direction[0];

    if( slew_rate == 'S' ) {
        degs_per_sec = 8./15.;         //128x  32['/s]
    } else if( slew_rate == 's' ) {
		if(special_case == 't') {
		    //rateName = 'stop'	
		    degs_per_sec = 0.0;
		} else {
			//rateName = 'set'
            degs_per_sec = 1./15.;         //128x  32['/s]
		}
    } else if( slew_rate == 'G' ) {
        degs_per_sec = 1./120.;         //128x  32['/s]
    } else if( slew_rate == 'O' ) {
        degs_per_sec = 1./240.;         //128x  32['/s]
    }
	//printf( "[handset_command] degs_per_sec = %lf\n", degs_per_sec );

    m_lcu->waitSemaphore();
    is_telescope_configured = m_lcu->telescope->getIsConfigured();
    m_lcu->postSemaphore();

    if( is_telescope_configured == 0 ) {
        printf( "Information: Please, config telescope first!\n" );
        return;
    }  

	if ( degs_per_sec > 0.0) {
        printf("[handset_command]: Slewing to the direction =%c at rate=%.2lf\n", slew_dir, degs_per_sec);
	} else {
        printf("[handset_command]: Stop slewing to dir=%c \n", slew_dir );
	} 

    m_lcu->waitSemaphore();

    double tmp;
    if( slew_dir == 'N' ) {
        tmp = -(m_lcu->telescope->delta->Motor->getTicsPerRev() *
                m_lcu->telescope->delta->Motor->getEncoderToAxis_Reduction() *
                degs_per_sec / 360.);
        tics_per_sec = (int) tmp;
        printf("[handset_command]: N... tics_per_sec = %d\n", tics_per_sec );
        m_lcu->telescope->delta->Motor->setDeviceMemory( 6, & tics_per_sec, 0  );
    } else if( slew_dir == 'S' ) {
        tmp =  (m_lcu->telescope->delta->Motor->getTicsPerRev() *
            m_lcu->telescope->delta->Motor->getEncoderToAxis_Reduction() *
            degs_per_sec / 360.);
        tics_per_sec = (int) tmp;
        printf("[handset_command]: S... tics_per_sec = %d\n", tics_per_sec );
        m_lcu->telescope->delta->Motor->setDeviceMemory( 6, & tics_per_sec, 0  );
    } else if( slew_dir == 'E' ) {
        tmp = -(m_lcu->telescope->alpha->Motor->getTicsPerRev() *
                m_lcu->telescope->alpha->Motor->getEncoderToAxis_Reduction() *
                degs_per_sec / 360.);
        tics_per_sec = (int) tmp;
        printf("[handset_command]: E... tics_per_sec = %d\n", tics_per_sec );
        m_lcu->telescope->alpha->Motor->setDeviceMemory( 6, & tics_per_sec, 0  );
    } else if( slew_dir == 'W' ) {
        tmp =  (m_lcu->telescope->alpha->Motor->getTicsPerRev() *
                m_lcu->telescope->alpha->Motor->getEncoderToAxis_Reduction() *
                degs_per_sec / 360.);
        tics_per_sec = (int) tmp;
        printf("[handset_command]: N... tics_per_sec = %d\n", tics_per_sec );
        m_lcu->telescope->alpha->Motor->setDeviceMemory( 6, & tics_per_sec, 0  );
    }

    m_lcu->postSemaphore();
}

