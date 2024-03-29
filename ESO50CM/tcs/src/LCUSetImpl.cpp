#include <LCUImpl.h>

using namespace std;
#define NUM_OF_TRY 1

void 
LCUImpl::setConfiguration(const string& fileName, const Ice::Current& c)
{
   setConfiguration(fileName);
}

void 
LCUImpl::setConfiguration(const string& fileName)
{
    logger.logINFO( "LCUImpl::setTelescopeConfig" );
    
    /** Is telescope configured **/
    if(m_configured)
        logger.logINFO("Telescope Already configured... re-loading configuration");
    
    
    /** Acquire Semaphore for SHM **/
    logger.logFINEST( "LCUImpl::setTelescopeConfig: wating for SHM semaphore" );
    m_lcu->waitSemaphore();
    logger.logFINEST( "LCUImpl::setTelescopeConfig: got SHM semaphore" );

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
                                            cout << variable << "   " << comment << "";
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
                                            cout << variable << "   " << comment << "";
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
                                            cout << variable << "   " << comment << "";
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
                                            cout <<  variable << "   " << comment << "";
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
                                            cout <<  variable << "   " << comment << "";
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
                                            cout << "[set_config] " << variable << "   " << comment << "";
                                    }
                                    break;
                            }
                            break;
                        default:
                            cout << "[set_config] " << line << "";
                    }
                } else {
                    cout << "[set_config] " << line << "";
                }
            }
        }
        m_lcu->telescope->setIsConfigured( true );
        m_lcu->telescope->setIsRunningMonitor( 0 );
        m_lcu->telescope->setIsRunningGoto( 0 );
        m_lcu->telescope->setIsTracking( 0 );
        config_file.close();
    } else {
      logger.logSEVERE( "LCUImpl::setTelescopeConfig: Unable to open Configuration File!" );
    }
    
    /** Release semaphore for SHM **/
    logger.logFINEST( "LCUImpl::setTelescopeConfig: preparing to release SHM semaphore" );
    m_lcu->postSemaphore(); 
    logger.logFINEST( "LCUImpl::setTelescopeConfig: released SHM semaphore" );

    /** Refresh Configuration and Tracking states **/
    getConfigState();
    getTrackingState();
     
    logger.logINFO( "LCUImpl::setTelescopeConfig: Telescope configured!!" );
}

void 
LCUImpl::setTarget(const OUC::TelescopePosition& targetPos, const Ice::Current& c)
{
    double RA, Dec =0.0;
    
    logger.logFINEST( "LCUImpl::setTarget" );

    /** Is telescope configured **/
    if(!m_configured)
    {
        OUC::TelescopeNotConfiguredEx ex;
        ex.reason = "Telecope Not Configured";
        logger.logSEVERE( "LCUImpl::setTarget: Telecope Not Configured");
        throw ex;
    }
    
    logger.logINFO( "LCUImpl::setTarget targetPos.RA=%lf", MiddleEndianToLittleEndian(targetPos.RA) );
    logger.logINFO( "LCUImpl::setTarget targetPos.Dec=%lf",MiddleEndianToLittleEndian(targetPos.Dec) );
    
    //convert to middle endian
    RA = MiddleEndianToLittleEndian(targetPos.RA);
    Dec = MiddleEndianToLittleEndian(targetPos.Dec);
    
    /** Acquire Semaphore for SHM **/
    m_lcu->waitSemaphore();
    
    /** Send new target **/
    if(m_lcu->telescope->setTarget(RA, Dec, (double*)&targetPos.Alt, (double*)&targetPos.Az) == 0) {
        char limits[255];
        OUC::TargetOutOfLimitsEx ex;
        sprintf(limits, "Target out of limits: Low Elevation: %lf, High Elevation: %lf", m_lcu->telescope->getLowElevation(), m_lcu->telescope->getHighElevation());
        logger.logINFO("LCUImpl::setTarget limits: %s ", limits);
        ex.reason = "Target out of limits. Try a new one";
        ex.reason.append(limits); 
        m_lcu->postSemaphore();
        throw ex;
    }
    
    /** Release semaphore for SHM **/
    m_lcu->postSemaphore();
    
    logger.logINFO( "LCUImpl::setTarget Target Accepted!!" );
}

void 
LCUImpl::setOffset(const OUC::TelescopePosition& offset, const Ice::Current& c)
{
    OUC::TelescopePosition tmp;
    tmp.RA = MiddleEndianToLittleEndian(offset.RA);
    tmp.Dec = MiddleEndianToLittleEndian(offset.Dec);
    tmp = (const ::OUC::TelescopePosition&)tmp;
    setOffset(tmp);
}

void 
LCUImpl::setOffset(const OUC::TelescopePosition& offsetPos)
{
    logger.logFINER( "--> LCUImpl::setOffset()" );

    int  alpha_mtr_counts;
    int  delta_mtr_counts;
    char mem_address;
    
    int no_quit;
    int goto_alpha_flag = false;
    int goto_delta_flag = false;

    double offsetRA = 0.0; 
    double offsetDec = 0.0;

    
    /** Is telescope configured **/
    if(!m_configured)
    {
        OUC::TelescopeNotConfiguredEx ex;
        ex.reason = "Telecope Not Configured";
        throw ex;
    }


    logger.logINFO( "LCUImpl::setOffset ra=%.lf dec=%.lf", offsetPos.RA, offsetPos.Dec );
    
    /** Set telescope in running state and define offset **/
    m_lcu->waitSemaphore();
    m_lcu->telescope->setIsRunningGoto(true);
    alpha_mtr_counts = m_lcu->telescope->alpha->offsetAxisInDeg(offsetPos.RA);
    //offsetPos.Dec *= -1.;
    offsetDec = offsetPos.Dec * -1.0;
    delta_mtr_counts = m_lcu->telescope->delta->offsetAxisInDeg(offsetDec);
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

    /** Goto target loop */
    no_quit = 180;
    do 
    {
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
    logger.logFINER( "<-- LCUImpl::setOffset()" );
}

void 
LCUImpl::setTracking(const OUC::TrackingInfo& trkInfo, const Ice::Current& c)
{
    int velocity = trkInfo.ticVel;
    logger.logFINER( "--> LCUImpl::setTracking()" );
  
    /** Is telescope configured **/
    if(!m_configured)
    {
        OUC::TelescopeNotConfiguredEx ex;
        ex.reason = "Telecope Not Configured";
        throw ex;
    }
  
    /** Read Tracking Flag **/
    if(trkInfo.trackState & !trkInfo.ticVel)
        velocity = 1200;
    
    /** Set Velocity **/
    m_lcu->waitSemaphore();
    m_lcu->telescope->alpha->Motor->setDeviceMemory(3, &velocity, 0);
    if(velocity > 0) {
        m_lcu->telescope->setIsTracking(true);
        logger.logINFO( "Start tracking, ticks velocity: %d", velocity);
    } else {
        logger.logINFO( "Stop tracking, ticks velocity: %d", velocity);
        m_lcu->telescope->setIsTracking(false);
    }
    m_lcu->postSemaphore();

    /** Refresh tracking state **/
    getTrackingState();
    logger.logFINE( "LCUImpl::setTracking Tracking State: %d, ticks velocity: %d",trkInfo.trackState,trkInfo.ticVel);
    logger.logFINER( "<-- LCUImpl::setTracking()" );
}

void 
LCUImpl::parkTelescopeCap(const Ice::Current& c)
{  
    parkTelescopeAdvance(true, c);
}

void 
LCUImpl::parkTelescope(const Ice::Current& c)
{  
    parkTelescopeAdvance(false, c);
}

void 
LCUImpl::parkTelescopeAdvance(bool cap, const Ice::Current& c)
{
    int  alpha_mtr_counts;
    int  delta_mtr_counts;
    int goto_alpha_flag = false;
    int goto_delta_flag = false;
    OUC::TelescopePosition targetPos;
    int setup_ready;
    int no_quit;

    logger.logFINEST( "--> LCUImpl::parkTelescope()" );
    m_stop_telescope = 0;
  
    /** Is telescope configured **/
    if(!m_configured) {
        OUC::TelescopeNotConfiguredEx ex;
        ex.reason = "Telecope Not Configured";
        logger.logSEVERE( "LCUImpl::parkTelescope: Telecope Not Configured" );
        throw ex;
    }  
  
    /**Calculate Parking Position **/
    m_lcu->waitSemaphore();
    /** Current Position */
    m_lcu->telescope->currentPosition(&targetPos.localSideralTime, &targetPos.RA, &targetPos.Dec, &targetPos.Alt, &targetPos.Az, &targetPos.HA);
    /** Set target = zenith */
    if(cap) {
        logger.logINFO("LCUImpl::parkTelescope: Moving telescope to park_cap position. (RA=LST+80, Dec=0.0)");
        logger.logFINE("LCUImpl::parkTelescope: LST for setTarget: %lf, Latitude for setTarget: %lf ", targetPos.localSideralTime + 80.0, m_lcu->telescope->getLatitude());
        targetPos.RA =  targetPos.localSideralTime + 80.0;
        targetPos.Dec = 0.0;
    } else {
        logger.logINFO("LCUImpl::parkTelescope: Moving telescope to park position.");
        logger.logFINE("LCUImpl::parkTelescope: LST for setTarget: %lf, Latitude for setTarget: %lf ", targetPos.localSideralTime, m_lcu->telescope->getLatitude());
        targetPos.RA =  targetPos.localSideralTime;
        targetPos.Dec = m_lcu->telescope->getLatitude();
    }

    logger.logFINE("LCUImpl::parkTelescope: targetRA: %lf, targetDec: %lf ", targetPos.RA, targetPos.Dec);
    m_lcu->telescope->setTarget(targetPos.RA, targetPos.Dec, &targetPos.Alt, &targetPos.Az);
    /** Set Offsets **/
    targetPos.RA = m_lcu->telescope->getDifferenceRA();
    targetPos.Dec = m_lcu->telescope->getDifferenceDec();
    targetPos.RA *= -1.;
    alpha_mtr_counts = m_lcu->telescope->alpha->offsetAxisInDeg(targetPos.RA);
    targetPos.Dec *= -1.;
    delta_mtr_counts = m_lcu->telescope->delta->offsetAxisInDeg(targetPos.Dec);
    logger.logFINE("LCUImpl::parkTelescope: Moving Telescope to RA: %lf, Dec: %lf", targetPos.RA, targetPos.Dec);
    logger.logFINE("LCUImpl::parkTelescope: Moving Telescope in alpha_counts: %d, delta_mtr_counts: %d", alpha_mtr_counts, delta_mtr_counts);

    /** Stop Tracking **/
    if(m_lcu->telescope->getIsTracking()) {
        int ticVel = 0;
        m_lcu->telescope->alpha->Motor->setDeviceMemory(3, &ticVel);
        m_lcu->telescope->setIsTracking(false);
        /* Refresh tracking state */
        m_tracking = false;
        logger.logINFO("LCUImpl::parkTelescope: Tracking OFF!!");
    }

    /** Move telescope to zenith **/
    if(m_stop_telescope == 1) {
        logger.logSEVERE("LCUImpl::parkTelescope: Someone stopped the telescope, quit moving to the target!!!!");
        return;
    }

    if( alpha_mtr_counts < -50 || 50 < alpha_mtr_counts ) {
        m_lcu->telescope->alpha->Motor->setDeviceMemory( 7, & alpha_mtr_counts, 0  );
        goto_alpha_flag = true;
    } else 
        logger.logINFO("LCUImpl::parkTelescope: ALPHA axis reached to the target (remaining enc count=%d)", alpha_mtr_counts);

    if( delta_mtr_counts < -50 || 50 < delta_mtr_counts ) {
        m_lcu->telescope->delta->Motor->setDeviceMemory( 7, & delta_mtr_counts, 0  );
        goto_delta_flag = true;
    } else 
        logger.logINFO("LCUImpl::moveToTarget: DELTA axis reached to the target (remaining enc count=%d)", delta_mtr_counts);

    m_lcu->postSemaphore();
    
    /** Goto target loop*/
    setup_ready = NUM_OF_TRY;
    do 
    {
        no_quit = 180;
        do {
            /** Reading current position */
            sleep( 1 );
            if(m_stop_telescope == 1) {
               logger.logSEVERE("LCUImpl::parkTelescope: Someone stopped the telescope, quit moving to the target!!!!");
               return;
            }

            m_lcu->waitSemaphore();
            {
                if( goto_alpha_flag ) {
                    m_lcu->telescope->alpha->Motor->readDeviceMemory( 7, & alpha_mtr_counts, 0  );
                    logger.logFINE("LCUImpl::parkTelescope: HW returns alpha_counts: %d", alpha_mtr_counts);
                    if( -50 < alpha_mtr_counts && alpha_mtr_counts < 50 ) {
                        logger.logINFO("LCUImpl::parkTelescope: ALPHA axis reached to the target (remaining enc count=%d)", alpha_mtr_counts);
                        goto_alpha_flag = false;
                    }
                }
            
                if( goto_delta_flag ) {
                    m_lcu->telescope->delta->Motor->readDeviceMemory( 7, & delta_mtr_counts, 0  );
                    logger.logFINE("LCUImpl::parkTelescope: HW returns delta_mtr_counts: %d",delta_mtr_counts);
                    if( -50 < delta_mtr_counts && delta_mtr_counts < 50 ) {
                        logger.logINFO("LCUImpl::moveToTarget: DELTA axis reached to the target (remaining enc count=%d)", delta_mtr_counts);
                        goto_delta_flag = false;
                    }
                }
            }
            m_lcu->postSemaphore();
        
            if( ! goto_delta_flag && ! goto_alpha_flag )
                no_quit = false;
            else 
                no_quit --;
        } while(no_quit);
   
        goto_alpha_flag = false;
        goto_delta_flag = false;
        if(m_stop_telescope == 1) {
           logger.logSEVERE("LCUImpl::parkTelescope: Someone stopped the telescope, quit moving to the target!!!!");
           return;
        }
        m_lcu->waitSemaphore();
        { 
            /** Current Position */
            m_lcu->telescope->currentPosition(&targetPos.localSideralTime, &targetPos.RA, &targetPos.Dec, &targetPos.Alt, &targetPos.Az, &targetPos.HA);
            if(cap) {
                targetPos.RA =  targetPos.localSideralTime + 80.0;
                targetPos.Dec = 0.0;
            } else {
                targetPos.RA =  targetPos.localSideralTime;
                targetPos.Dec = m_lcu->telescope->getLatitude();
            }
            //logger.logINFO("LCUImpl::parkTelescope: RA for setTarget: %lf, Dec for setTarget: %lf ", targetPos.RA, targetPos.Dec);
            /** Set Position **/
            m_lcu->telescope->setTarget(targetPos.RA, targetPos.Dec, &targetPos.Alt, &targetPos.Az);
            /** Set Offsets **/
            targetPos.RA = m_lcu->telescope->getDifferenceRA();
            targetPos.Dec = m_lcu->telescope->getDifferenceDec();
            targetPos.RA *= -1.;
            alpha_mtr_counts = m_lcu->telescope->alpha->offsetAxisInDeg(targetPos.RA);
            targetPos.Dec *= -1.;
            delta_mtr_counts = m_lcu->telescope->delta->offsetAxisInDeg(targetPos.Dec);
            //logger.logINFO("LCUImpl::parkTelescope: Moving Telescope to RA: %lf, Dec: %lf", targetPos.RA, targetPos.Dec);
            //logger.logINFO("LCUImpl::parkTelescope: Moving Telescope to alpha_counts: %d, delta_mtr_counts: %d", alpha_mtr_counts, delta_mtr_counts);
        }
        if( alpha_mtr_counts < -50 || 50 < alpha_mtr_counts ) {
            m_lcu->telescope->alpha->Motor->setDeviceMemory( 7, & alpha_mtr_counts, 0  );
            goto_alpha_flag = true;
        }
        if( delta_mtr_counts < -50 || 50 < delta_mtr_counts ) {
            m_lcu->telescope->delta->Motor->setDeviceMemory( 7, & delta_mtr_counts, 0  );
            goto_delta_flag = true;
        }
        m_lcu->postSemaphore();
        if( ! goto_alpha_flag && ! goto_delta_flag )
            setup_ready = false;
        else 
            setup_ready --;    
    } while(setup_ready);
    
    /** Stop telescope status */
    m_lcu->waitSemaphore();
    m_lcu->telescope->setIsRunningGoto(false);
    m_lcu->postSemaphore();
    if(cap) 
       logger.logINFO("LCUImpl::parkTelescope: reached to the park_cap position");
    else
       logger.logINFO("LCUImpl::parkTelescope: reached to the park position");

    logger.logFINEST( "<-- LCUImpl::parkTelescope()" );
}

void 
LCUImpl::stopTelescope(OUC::TelescopeDirection dir, const Ice::Current& c)
{
    logger.logFINER("--> LCUImpl::stopTelescope()");

    int alphaTicsPerSeconds= 0;
    int alphaTics=0;
    int deltaTicsPerSeconds= 0;
    int deltaTics=0;

    //should protect this variable with mutes
    m_stop_telescope = 1;
  
    /** Acquire Semaphore for SHM **/
    m_lcu->waitSemaphore();
  
    //m_lcu->telescope->alpha->Motor->setDeviceMemory(7, &ticsPerSeconds, 0);
    //m_lcu->telescope->alpha->Motor->setDeviceMemory(6, &ticsPerSeconds, 0);
    //m_lcu->telescope->delta->Motor->setDeviceMemory(7, &ticsPerSeconds, 0);
    //m_lcu->telescope->delta->Motor->setDeviceMemory(6, &ticsPerSeconds, 0);
    //setDeviceMemory(7, (int*)&alphaTics, 0, c);
    //setDeviceMemory(6, (int*)&alphaTicsPerSeconds, 0, c);
    //setDeviceMemory(7, (int*)&deltaTics, 0, c);
    //setDeviceMemory(6, (int*)&deltaTicsPerSeconds, 0, c);
    setDeviceMemory(0xA2, 7, alphaTics, c);
    setDeviceMemory(0xA2, 6, alphaTicsPerSeconds, c);
    setDeviceMemory(0xA4, 7, deltaTics, c);
    setDeviceMemory(0xA4, 6, deltaTicsPerSeconds, c);
 
    /** Release semaphore for SHM **/
    m_lcu->postSemaphore();    
    logger.logFINE("LCUImpl::stopTelescope: Stopping Telescope in both axes!!... Tics per seconds: %d", alphaTicsPerSeconds);
    logger.logINFO("Stopping Telescope in all directions!!");
    logger.logFINER("<-- LCUImpl::stopTelescope()" );
}

void 
LCUImpl::moveToTarget(const Ice::Current& c)
{
    double offset_ra, offset_dec;
    int alpha_mtr_counts;
    int delta_mtr_counts;
    int no_quit;
    int setup_ready;
    int goto_alpha_flag = false;
    int goto_delta_flag = false;
    int value;
    int ticVel;

    logger.logFINEST( "LCUImpl::setTarget" );
  
    /** Is telescope configured **/
    if(!m_configured) {
        OUC::TelescopeNotConfiguredEx ex;
        ex.reason = "Telecope Not Configured";
        logger.logSEVERE( "LCUImpl::setTarget: Telecope Not Configured" );
        throw ex;
    }

    m_stop_telescope = 0;
  
    /** Acquire Semaphore for SHM **/
    m_lcu->waitSemaphore();
  
    /** Differesnce RA Dec  */
    offset_ra = m_lcu->telescope->getDifferenceRA();
    offset_dec = m_lcu->telescope->getDifferenceDec();
    offset_ra *= -1.;
    alpha_mtr_counts = m_lcu->telescope->alpha->offsetAxisInDeg( offset_ra );
    offset_dec *= -1.;
    delta_mtr_counts = m_lcu->telescope->delta->offsetAxisInDeg( offset_dec );
    logger.logINFO("LCUImpl::moveToTarget: Offset Telescope in RA: %lf [deg], Dec: %lf [deg]", offset_ra, offset_dec);
    logger.logINFO("LCUImpl::moveToTarget: Offset Telescope in alpha_counts: %d [enc], delta_mtr_counts: %d [enc]", alpha_mtr_counts, delta_mtr_counts);

    /** Stop Tracking **/
//    if(m_lcu->telescope->getIsTracking()) 
//    {
//        ticVel = 0;
//        m_lcu->telescope->alpha->Motor->setDeviceMemory(3, &ticVel, 0);
//        m_lcu->telescope->setIsTracking(false);
//        /* Refresh tracking state */
//        m_tracking = false;
//        logger.logINFO("LCUImpl::moveToTarget: Tracking OFF!!");
//    }

    /** Move Telescope to the requested position **/
    if(m_stop_telescope == 1) {
        logger.logSEVERE("LCUImpl::moveToTarget: Someone stopped the telescope, quit moving to the target!!!!");
        return;
    }

    if( alpha_mtr_counts < -50 || 50 < alpha_mtr_counts ) {
        m_lcu->telescope->alpha->Motor->setDeviceMemory( 7, & alpha_mtr_counts, 0  );
        goto_alpha_flag = true;
    } else 
        logger.logINFO("LCUImpl::moveToTarget: ALPHA axis reached to the target (remaining enc count=%d)", alpha_mtr_counts);

    if( delta_mtr_counts < -50 || 50 < delta_mtr_counts ) {
        m_lcu->telescope->delta->Motor->setDeviceMemory( 7, & delta_mtr_counts, 0  );
        goto_delta_flag = true;
    } else 
        logger.logINFO("LCUImpl::moveToTarget: DELTA axis reached to the target (remaining enc count=%d)", delta_mtr_counts);
  
    /** Release semaphore for SHM **/
    m_lcu->postSemaphore(); 

    /** Goto target loop*/
    setup_ready = NUM_OF_TRY;
    do 
    {
       no_quit = 180;
       do {
           sleep( 1 );
           if(m_stop_telescope == 1) {
               logger.logSEVERE("LCUImpl::moveToTarget: Someone stopped the telescope, quit moving to the target!!!!");
               return;
           }

           m_lcu->waitSemaphore();
           {
               if( goto_alpha_flag ) {
                   m_lcu->telescope->alpha->Motor->readDeviceMemory( 7, & alpha_mtr_counts, 0  );
                   logger.logFINE("LCUImpl::moveToTarget: HW returns alpha_counts: %d", alpha_mtr_counts);
                   if( -50 < alpha_mtr_counts && alpha_mtr_counts < 50 ) {
                       logger.logINFO("LCUImpl::moveToTarget: ALPHA axis reached to the target (remaining enc count=%d)", alpha_mtr_counts);
                       goto_alpha_flag = false;
                   }
               }
               if( goto_delta_flag ) {
                   m_lcu->telescope->delta->Motor->readDeviceMemory( 7, & delta_mtr_counts, 0  );
                   logger.logFINE("LCUImpl::moveToTarget: HW returns delta_mtr_counts: %d",delta_mtr_counts);
                   if( -50 < delta_mtr_counts && delta_mtr_counts < 50 ) {
                       logger.logINFO("LCUImpl::moveToTarget: DELTA axis reached to the target (remaining enc count=%d)", delta_mtr_counts);
                       goto_delta_flag = false;
                   }
               }
           }
           m_lcu->postSemaphore();
         
           if( ! goto_delta_flag && ! goto_alpha_flag ) 
               no_quit = false;
           else 
               no_quit --;  
           logger.logFINE( "LCUImpl::moveToTarget: To quit (if something was wrong)... %d sec.", no_quit );
       } while( no_quit );

       goto_alpha_flag = false;
       goto_delta_flag = false;

       if(m_stop_telescope == 1) {
           logger.logSEVERE("LCUImpl::moveToTarget: Someone stopped the telescope, quit moving to the target!!!!");
           return;
       }
 
       m_lcu->waitSemaphore();
       {
           /** Differesnce RA Dec  */
           offset_ra  = m_lcu->telescope->getDifferenceRA();
           offset_dec = m_lcu->telescope->getDifferenceDec(); 
           offset_ra *= -1.;
           alpha_mtr_counts = m_lcu->telescope->alpha->offsetAxisInDeg( offset_ra );
           offset_dec *= -1.;
           delta_mtr_counts = m_lcu->telescope->delta->offsetAxisInDeg( offset_dec );
           //logger.logINFO("LCUImpl::moveToTarget: Offset Telescope in RA: %lf [deg], Dec: %lf [deg]", offset_ra, offset_dec);
           //logger.logINFO("LCUImpl::moveToTarget: Offset Telescope in alpha_counts: %d [enc], delta_mtr_counts: %d [enc]", alpha_mtr_counts, delta_mtr_counts);
         
           /** Move Telescope to the requested position **/
           if( alpha_mtr_counts < -50 || 50 < alpha_mtr_counts ) {
               m_lcu->telescope->alpha->Motor->setDeviceMemory( 7, & alpha_mtr_counts, 0  );
               goto_alpha_flag = true;
           }
         
           if( delta_mtr_counts < -50 || 50 < delta_mtr_counts ) {
               m_lcu->telescope->delta->Motor->setDeviceMemory( 7, & delta_mtr_counts, 0  );
               goto_delta_flag = true;
           }
       }
       m_lcu->postSemaphore();
       
       if( ! goto_alpha_flag && ! goto_delta_flag ) 
           setup_ready = false;
       else 
           setup_ready --;
   
           logger.logFINE( "LCUImpl::moveToTarget: Attempt No %d finished.", NUM_OF_TRY - setup_ready );
    } while( setup_ready );
  
    if(m_stop_telescope == 1) {
        logger.logSEVERE("LCUImpl::moveToTarget: Someone stopped the telescope, quit moving to the target!!!!");
        return;
    }

    logger.logINFO("LCUImpl::moveToTarget: reached to the target");


     /* Start Tracking */
//    m_lcu->waitSemaphore();
//    {
//        ticVel = 602;
//        m_lcu->telescope->alpha->Motor->setDeviceMemory(3, &ticVel, 0);
//        m_lcu->telescope->setIsTracking(true);
//        m_tracking = true;
//        m_lcu->telescope->setIsRunningGoto(false);
//        logger.logINFO("LCUImpl::moveToTarget: Tracking ON!");
//    }
//    m_lcu->postSemaphore();
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
             degs_per_sec = 0.008333; //i 1./120.;         //128x  32['/s]
        }
    } else if( slew_rate == 'G' ) {
        degs_per_sec = 0.002083; // 1./480.;         //128x  32['/s]
    } else if( slew_rate == 'O' ) {
        degs_per_sec = 0.001389;         //128x  32['/s]
    }
    logger.logINFO( "LCUImpl::handsetSlew: degs_per_sec = %lf", degs_per_sec );
    
    m_lcu->waitSemaphore();
    is_telescope_configured = m_lcu->telescope->getIsConfigured();
    m_lcu->postSemaphore();
    
    if( is_telescope_configured == 0 ) 
    {
        logger.logINFO( "LCUImpl::handsetSlew: Information: Please, config telescope first!" );
        return;
    }  

    if ( degs_per_sec > 0.0) 
    {
        logger.logINFO("LCUImpl::handsetSlew: Slewing to the direction =%c at rate=%.2lf", slew_dir, degs_per_sec);
    } else {
        logger.logINFO("LCUImpl::handsetSlew: Stop slewing to dir=%c ", slew_dir );
    } 

    m_lcu->waitSemaphore();

    double tmp;
    if( slew_dir == 'N' ) 
    {
        tmp = -(m_lcu->telescope->delta->Motor->getTicsPerRev() *
                m_lcu->telescope->delta->Motor->getEncoderToAxis_Reduction() *
                degs_per_sec / 360.);
        tics_per_sec = (int) tmp;
        logger.logINFO("LCUImpl::handsetSlew: N... tics_per_sec = %d", tics_per_sec );
        m_lcu->telescope->delta->Motor->setDeviceMemory( 6, & tics_per_sec, 0  );
    } else if( slew_dir == 'S' ) {
        tmp =  (m_lcu->telescope->delta->Motor->getTicsPerRev() *
        m_lcu->telescope->delta->Motor->getEncoderToAxis_Reduction() *
        degs_per_sec / 360.);
        tics_per_sec = (int) tmp;
        logger.logINFO("LCUImpl::handsetSlew: S... tics_per_sec = %d", tics_per_sec );
        m_lcu->telescope->delta->Motor->setDeviceMemory( 6, & tics_per_sec, 0  );
    } else if( slew_dir == 'E' ) {
        tmp = -(m_lcu->telescope->alpha->Motor->getTicsPerRev() *
                m_lcu->telescope->alpha->Motor->getEncoderToAxis_Reduction() *
                degs_per_sec / 360.);
        tics_per_sec = (int) tmp;
        logger.logINFO("LCUImpl::handsetSlew: E... tics_per_sec = %d", tics_per_sec );
        m_lcu->telescope->alpha->Motor->setDeviceMemory( 6, & tics_per_sec, 0  );
    } else if( slew_dir == 'W' ) {
        tmp =  (m_lcu->telescope->alpha->Motor->getTicsPerRev() *
                m_lcu->telescope->alpha->Motor->getEncoderToAxis_Reduction() *
                degs_per_sec / 360.);
        tics_per_sec = (int) tmp;
        logger.logINFO("LCUImpl::handsetSlew: N... tics_per_sec = %d", tics_per_sec );
        m_lcu->telescope->alpha->Motor->setDeviceMemory( 6, & tics_per_sec, 0  );
    }

    m_lcu->postSemaphore();
}

