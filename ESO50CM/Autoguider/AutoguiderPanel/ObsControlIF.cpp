#include <stdio.h>
#include "ObsControlIF.h"


#define round(fp) (int)((fp) >= 0 ? (fp) + 0.5 : (fp) - 0.5)

using namespace std;
using namespace OUC;

ObsControlIF::ObsControlIF() :
   logger("ObsControlIF")
{
    //::Log::LogLevel level = ::Log::FINE;
    //logger.setDiscardLevel(level);
    lcu = NULL;
    communicator = NULL;
    data = new OUC::TelescopeData();
    enc = new OUC::RawEncoderData();
}

ObsControlIF::~ObsControlIF()
{
    disconnect();
    lcu = NULL;
    communicator = NULL;
}

//move this to OBSComponent 
//-------------

char * ObsControlIF::strfdegs( char * string, size_t max_len, const char * format, double degs )
{
    int gg, mm, ss, cc;
    double m_secs;
    double m_degs;
    double tmp;

    m_degs = fabs( degs );
    gg   = (int) floor( m_degs );
    tmp  = ((double) gg );
    mm   = (int) floor( ( m_degs - tmp ) * 60. );
    tmp += ((double) mm) / 60.;
    ss   = (int) floor( ( m_degs - tmp ) * 3600. );
    tmp += ((double) ss) / 3600.;
    cc   = (int) round( ( m_degs - tmp ) * 360000. );

    //logger.logINFO( "[myUtil] strfdegs: degs=%lf -> gg=%d mm=%d ss=%d cc=%d\n", degs, gg, mm, ss, cc );
    if( cc > 99 ) {
        cc = 0;
        ss ++;
    }
    if( ss > 59  ) {
        ss = 0;
        mm ++;
    }
    if( mm > 59 ) {
        mm = 0;
        gg ++;
    }
    if( degs  < 0 ){
        gg = -gg;
    }

    m_secs = (double) ss + ((double) cc)/100.;
    //snlogger.logINFO( string, max_len, format, gg, mm, m_secs );
	
	//snprintf( string, max_len, format, gg, mm, (double) ss );
    return string;
}

void ObsControlIF::getCurrentPositionRA(char *buffer, int maxlen)
{

    mutex.lock();
    double ra = data->currentPos.RA;
    mutex.unlock();
    //convert to sexagesimal
    strfdegs( buffer, maxlen, "%02d:%02d:%02.0lf\0", ra / 15.0 );
    //logger.logINFO("-----------------------%s\n", buffer);
    //return buffer;
}

void ObsControlIF::getCurrentPositionDec(char *buffer, int maxlen)
{

    mutex.lock();
    double dec = data->currentPos.Dec;
    mutex.unlock();
    //convert to sexagesimal
    strfdegs( buffer, maxlen, "%+03d:%02d:%02.0lf\0", dec);
    //logger.logINFO("-----------------------%s\n", buffer);
    //return buffer;
}

void ObsControlIF::formatRAPosition(double ra, char *buffer, int maxlen)
{
    //convert to sexagesimal
    strfdegs( buffer, maxlen, "%02d:%02d:%02.0lf\0", ra / 15.0 );
}

void ObsControlIF::formatDecPosition(double dec, char *buffer, int maxlen)
{
    strfdegs( buffer, maxlen, "%+03d:%02d:%02.0lf\0", dec);
}

//=============

int ObsControlIF::getPosition()
{
    try {
        //char buffer[1000];
        mutex.lock();
        *data = lcu->getPosition();
        *enc = lcu->getRawEncodersPosition();
        logger.logFINE("ObsControlIF::getPosition: currentPos.RA =%.10lf", data->currentPos.RA);
        logger.logFINE("ObsControlIF::getPosition: crrentPos.Dec = %.10lf", data->currentPos.Dec);

        mutex.unlock();
        emit newData(1, data);
        emit newEncData(1, enc);
        ProcessStatus status = ProcessConnected;
        emit newObsControlStatusTriggered(status);
        emit newLCUControlStatusTriggered(status);
    } catch(Ice::ConnectionRefusedException& ex) {
        ProcessStatus status = ProcessDisconnected;
        emit newObsControlStatusTriggered(status);
        emit newLCUControlStatusTriggered(status);
        logger.logSEVERE("Connectivity error with ObsControl");
        logger.logSEVERE("Error in ObsControlIF:getPosition(). %s", ex.ice_name().c_str());
    } catch(Ice::ConnectionLostException& ex) {
        ProcessStatus status = ProcessDisconnected;
        emit newObsControlStatusTriggered(status);
        emit newLCUControlStatusTriggered(status);
        logger.logSEVERE("Lost connection to ObsControl");
        logger.logSEVERE("Error in ObsControlIF:getPosition(). %s", ex.ice_name().c_str());
    } catch(const Ice::Exception& ex) {
        logger.logSEVERE("Error in ObsControlIF:getPosition(). %s", ex.ice_name().c_str());
        cout << ex << endl;
        emit newData(2, data);
        ProcessStatus status = ProcessError;
        emit newObsControlStatusTriggered(status);
        emit newLCUControlStatusTriggered(status);
    }

    return EXIT_SUCCESS;
}

void ObsControlIF::stopTelescope() {
    try {
        OUC::TelescopeDirection dir = North;
        lcu->stopTelescope(dir);
        TelescopeStatus status = TelescopeStop;
        emit newTelescopeStatusTriggered(status);
        logger.logINFO("ObsControlIF::stopTelescope");
    } catch(const Ice::Exception& ex) {
        logger.logSEVERE("Error in ObsControlIF:stopTelescope(). %s", ex.ice_name().c_str());
        cout << ex << endl;
        //emit newData(2, data);
        //ProcessStatus status = Error;
        //emit newObsControlStatusTriggered(status);
    }
}

void ObsControlIF::startTracking() {
    try {
        logger.logINFO("ObsControlIF::startTracking");
        lcu->startTracking();
        TrackingStatus status = TrackingOn;
        emit newTrackingStatusTriggered(status);

    } catch(const Ice::Exception& ex) {
        logger.logSEVERE("Error in ObsControlIF:startTracking(). %s", ex.ice_name().c_str());
        TrackingStatus status = TrackingError;
        emit newTrackingStatusTriggered(status);
    }
}

void ObsControlIF::stopTracking() {
    try {
        logger.logINFO("ObsControlIF::stopTracking");
        lcu->stopTracking();
        TrackingStatus status = TrackingOff;
        emit newTrackingStatusTriggered(status);
    } catch(const Ice::Exception& ex) {
        logger.logSEVERE("Error in ObsControlIF:startTracking(). %s", ex.ice_name().c_str());
        TrackingStatus status = TrackingError;
        emit newTrackingStatusTriggered(status);
    }
}

void ObsControlIF::gotoTarget() {
    //try {
    //    logger.logINFO("goto_target invoked asynchronously");
    //    AMI_Observing_moveToTargetPtr cb = new AMI_Observing_moveToTargetImpl(this);
    //    obs->moveToTarget_async(cb);
    //    TelescopeStatus status = TelescopeMoving;
    //    emit newTelescopeStatusTriggered(status);
    //} catch(const Ice::Exception& ex) {
    //    logger.logSEVERE("Error in ObsControlIF:startTracking(). %s", ex.ice_name().c_str());
    //    cout << "ObsControlIF::gotoTarget(): Exception caught, " << ex << endl;
    //    TelescopeStatus status = TelescopeError;
    //    emit newTelescopeStatusTriggered(status);
    //}
}

void ObsControlIF::telescopeMovementFinished() {
    TelescopeStatus status = TelescopeStop;
    emit newTelescopeStatusTriggered(status);
}

void ObsControlIF::parkTelescope() {
    //try {
    //    logger.logINFO("park_telescope invoked asynchronously");
    //    AMI_Observing_parkTelescopePtr cb = new AMI_Observing_parkTelescopeImpl(this);
    //    obs->parkTelescope_async(cb);
    //    TelescopeStatus status = TelescopeMoving;
    //    emit newTelescopeStatusTriggered(status);
    //    TrackingStatus trackStatus = TrackingOff;
    //    emit newTrackingStatusTriggered(trackStatus);
    //} catch(const Ice::Exception& ex) {
    //    logger.logSEVERE("Error in ObsControlIF:parkTelescope(). %s", ex.ice_name().c_str());
    //    TelescopeStatus status = TelescopeError;
    //    cout << "ObsControlIF::parkTelescope(): Exception caught, " << ex << endl;
    //    emit newTelescopeStatusTriggered(status);
    //}
}

void ObsControlIF::parkTelescopeCap() {
    /*try {
        logger.logINFO("park_telescope_cap invoked asynchronously");
        AMI_Observing_parkTelescopeCapPtr cb = new AMI_Observing_parkTelescopeCapImpl(this);
        obs->parkTelescopeCap_async(cb);
        TelescopeStatus status = TelescopeMoving;
        emit newTelescopeStatusTriggered(status);
        TrackingStatus trackStatus = TrackingOff;
        emit newTrackingStatusTriggered(trackStatus);
    } catch(const Ice::Exception& ex) {
        logger.logSEVERE("Error in ObsControlIF:parkTelescopeCap(). %s", ex.ice_name().c_str());
        cout << "ObsControlIF::parkTelescopeCap(): Exception caught, " << ex << endl;
        TelescopeStatus status = TelescopeError;
        emit newTelescopeStatusTriggered(status);
    }*/
}

int ObsControlIF::setTargetPositionRA(const char *ra)
{
    logger.logFINEST("ObsControlIF::setTargetPositionRA");
    int trg_ra_hrs;
    int trg_ra_min;
    int trg_ra_sec;
    char * ptr = NULL;
    double trg_ra = 0;
    OUC::TelescopePosition *new_pos;


    try {
        if( (ptr = strstr( (char *)ra, "ra=" )) != NULL ) {

            if( sscanf(ra, "ra=%d:%d:%d", & trg_ra_hrs, & trg_ra_min, & trg_ra_sec ) == 3 ) {
                logger.logFINE("ObsControlIF::setTargetPositionRA %s", ra);
                logger.logFINE("ObsControlIF::setTargetPositionRA: target ra =%+03d:%02d:%02d\n", trg_ra_hrs, trg_ra_min, trg_ra_sec );
                trg_ra  = fabs( (double) trg_ra_hrs ) + ((double) trg_ra_min / 60.) + ((double) trg_ra_sec / 3600.);
                trg_ra *= 15.0;
                if( trg_ra_hrs < 0 ) {
                    trg_ra *= -1.;
                } else if( trg_ra_hrs == 0 && ( strstr( ra, "ra=-" ) != NULL) ) {
                    trg_ra *= -1.;
                }
                logger.logFINE( "ObsControlIF::setTargetPositionRA: Target RA =%lf %s", trg_ra,ra );
            } else {
                logger.logWARNING( "ObsControlIF::setTargetPositionRA: No Target RA data\n" );
            }
        }

        new_pos = new OUC::TelescopePosition();

        mutex.lock();
        //copy current target position
        new_pos->RA = trg_ra;
        data->targetPos.RA = trg_ra;
        new_pos->Dec = data->targetPos.Dec;
        //new_pos->Alt = 60.0;
        //new_pos->Az = 150.0;
        //new_pos->HA = 100.0;
        logger.logFINE("ObsControlIF::setTargetPositionRA: Target.RA = %.10lf  trg_ra=%.10lf\n", new_pos->RA, trg_ra);
        try {
            lcu->setTarget(*new_pos);
        } catch (OUC::TelescopeNotConfiguredEx& ex) {
            logger.logSEVERE("ObsControlIF::setTargetPositionRA: LCU returned: %s. Execute setConfiguration method first", ex.reason.c_str());
        } catch (OUC::TargetOutOfLimitsEx& ex) {
            logger.logSEVERE("ObsControlIF::setTargetPositionRA: LCU returned: %s. Try another target!!", ex.reason.c_str());
        } catch (Ice::Exception& ex) {
            logger.logSEVERE("ObsControlIF::setTargetPositionRA: Unexpected run-time error");
            throw ex;
        }   
        mutex.unlock();
    } catch(const Ice::Exception& ex) {
        cout << ex << endl;
    }

    return EXIT_SUCCESS;
}

int ObsControlIF::setTargetPositionDec(const char *arguments)
{
    logger.logFINEST("ObsControlIF::setTargetPositionDec");
    int trg_dec_deg;
    int trg_dec_min;
    int trg_dec_sec;
    char * ptr = NULL;
    double trg_dec = 0;
    OUC::TelescopePosition *new_pos;

    logger.logFINE("ObsControlIF::setTargetPositionDec arguments=%s", arguments);

    try {
        if( (ptr = strstr( (char *)arguments, "dec=" )) != NULL ) {
            if( sscanf( ptr, "dec=%d:%d:%d", & trg_dec_deg, & trg_dec_min, & trg_dec_sec ) == 3 ) {
                logger.logFINE( "ObsControlIF::setTargetPositionDec: trg dec=%+03d:%02d:%02d\n", trg_dec_deg, trg_dec_min, trg_dec_sec );
                trg_dec  = fabs( (double) trg_dec_deg ) + ((double) trg_dec_min / 60.) + ((double) trg_dec_sec / 3600.);
                if( trg_dec_deg < 0 ) {
                    trg_dec *= -1.;
                } else if( trg_dec_deg == 0 && ( strstr( arguments, "dec=-" ) != NULL) ) {
                    trg_dec *= -1.;
                }
                logger.logFINE( "ObsControlIF::setTargetPositionDec trg dec=%lf", trg_dec );
            } else if( sscanf( ptr, "dec=%d*%d:%d", & trg_dec_deg, & trg_dec_min, & trg_dec_sec ) == 3 ) {
                logger.logFINE( "ObsControlIF::setTargetPositionDec trg dec=%+03d*%02d:%02d\n", trg_dec_deg, trg_dec_min, trg_dec_sec );
                trg_dec  = fabs( (double) trg_dec_deg ) + ((double) trg_dec_min / 60.) + ((double) trg_dec_sec / 3600.);
                if( trg_dec_deg < 0 ) {
                    trg_dec *= -1.;
                } else if( trg_dec_deg == 0 && ( strstr( arguments, "dec=-" ) != NULL) ) {
                    trg_dec *= -1.;
                }
                logger.logFINE( "ObsControlIF::setTargetPositionDec: trg dec=%lf", trg_dec );
            } else {
                logger.logWARNING( "ObsControlIF::setTargetPositionDec: No target dec data" );
            }
        }

        new_pos = new OUC::TelescopePosition();
        mutex.lock();
        //copy current target position
        new_pos->RA = data->targetPos.RA;
        new_pos->Dec = trg_dec;
        data->targetPos.Dec = trg_dec;
        //new_pos->Alt = 60.0;
        //new_pos->Az = 150.0;
        //new_pos->HA = 100.0;
        logger.logINFO("ObsControlIF::setTargetPositionDec: Target.Dec = %.10lf \n", new_pos->Dec);
        try {
            lcu->setTarget(*new_pos);
        } catch (OUC::TelescopeNotConfiguredEx& ex) {
            logger.logSEVERE("ObsControlIF::setTargetPositionDec: LCU returned: %s. Execute setConfiguration method first", ex.reason.c_str());
        } catch (OUC::TargetOutOfLimitsEx& ex) {
            logger.logSEVERE("ObsControlIF::setTargetPositionDec: LCU returned: %s. Try another target!!", ex.reason.c_str());
        } catch (Ice::Exception& ex) {
            logger.logSEVERE("ObsControlIF::setTargetPositionDec: Unexpected run-time error");
            throw ex;
        }	
        mutex.unlock();
    } catch(const Ice::Exception& ex) {
        cout << ex << endl;
    }

    return EXIT_SUCCESS;
}

OUC::TelescopeData ObsControlIF::getTelescopeData()
{
    OUC::TelescopeData newData;
    mutex.lock();
    //make a copy of telescope data
    newData.currentPos.RA = data->currentPos.RA;
    newData.currentPos.Dec = data->currentPos.Dec;
    mutex.unlock();
    return newData;
}



TelescopePrx ObsControlIF::getLCUReference() {
    if (lcu)
        return lcu;
    else {
        connect();
        return lcu;
    }
}


int ObsControlIF::connect()
{
    int status;

    int argc = 0;
    //char *argv[] = {};
	char **argv;

    //Initialize the communicator
    try {
        //string configPath = getenv("SWROOT");
        string configPath = "Obs-config";
		qDebug() << "configPath = " << configPath.c_str(); 
    	Ice::InitializationData initData;
        initData.properties = Ice::createProperties();
        initData.properties->load(configPath);
        communicator = Ice::initialize(argc, argv, initData);
        //communicator = Ice::initialize(initData, 0);
        //status = getPosition(argc, argv, communicator);
        // get reference to LCUImpl
   
        Ice::PropertiesPtr properties = communicator->getProperties();
        const char* proxyProperty = "ObsAdapter.Proxy";
        string proxy = properties->getProperty(proxyProperty);
		qDebug() << "proxy=" << proxy.c_str();
        if(proxy.empty()) {
            logger.logSEVERE("ObsControlIF::connect: %s: property `%s' not set", argv[0], proxyProperty);
            return EXIT_FAILURE;
        } 
   
        Ice::ObjectPrx base = communicator->stringToProxy(proxy);
        obs = ObservingPrx::checkedCast(base->ice_twoway()->ice_timeout(-1));
        if(!obs) {
			qDebug() << "ObsControl::connect: invalid proxy" << argv[0];
            logger.logSEVERE("ObsControlIF::connect: %s: invalid proxy", argv[0]);
            ProcessStatus status = ProcessDisconnected;
            emit newObsControlStatusTriggered(status);
            return EXIT_FAILURE;
        } else {
            qDebug() << "emitiendo newObsControlStatus(1)";
            ProcessStatus status = ProcessConnected;
            emit newObsControlStatusTriggered(status);
            qDebug() << "listo";
        }
    
		qDebug() << "Getting reference to the Telescope!!!!!!!!!!!";
        lcu = obs->getTelescope();
        if(!lcu) {
            logger.logSEVERE("ObsControlIF::connect: %s: invalid proxy", argv[0]);
            return EXIT_FAILURE;
        }
    } catch(const Ice::ConnectionRefusedException& ex) {
        logger.logSEVERE("ERROR!!! Unable to connect to server: %s", ex.ice_name().c_str());
        status = EXIT_FAILURE;
        ProcessStatus status = ProcessDisconnected;
        emit newObsControlStatusTriggered(status);
    } catch(const Ice::Exception& ex) {
        logger.logSEVERE("ERROR!!! Unable to connect to server: %s", ex.ice_name().c_str());
        status = EXIT_FAILURE;
        //emit newData(2, data);
        ProcessStatus status = ProcessError;
        emit newObsControlStatusTriggered(status);
    }

    return status;
}

int ObsControlIF::disconnect() {
    int status;
    if(communicator) {
        try {
	    communicator->destroy();
        } catch(const Ice::Exception& ex) {
	    cout << ex << endl;
	    status = EXIT_FAILURE;
	}
    }
    return status;
}

void ObsControlIF::run()
{
    while(1) {
        try {
            getPosition();
            sleep(1.0);
        } catch (...) {
            logger.logSEVERE("ObsControlIF::run: Unexpected exception, continue ... ");
        }
    }
}

void ObsControlIF::test() {
    OUC::TelescopeData data;
    data = lcu->getPosition();
    logger.logINFO(">>>>>>>>>>>>> RA = %.10lf \n", data.currentPos.RA);
    logger.logINFO(">>>>>>>>>>>>> Dec = %.10lf \n", data.currentPos.Dec);
}

void ObsControlIF::handset_slew(string rate, string direction)
{
    OUC::SlewInfo *slewInfo;
    try {
        slewInfo = new OUC::SlewInfo();
        slewInfo->rateName = rate;
        slewInfo->direction =  direction;

        lcu->handsetSlew(*slewInfo);
        printf(">>>>>>>>>>>>> slew->rate = %s \n", slewInfo->rateName.c_str());
        printf(">>>>>>>>>>>>> slew->direction = %s\n", slewInfo->direction.c_str());
    } catch(const Ice::Exception& ex) {
        cout << ex << endl;
    }
}

//
// WorkerThread implementation
//
void workThread::run() {
    qDebug() << "Thread running: "  << thread()->currentThreadId();
    //logger.logINFO("Thread running");
    try {
        lcu->moveToTarget();
        //logger.logINFO("ObsControlIF::goto target");
    } catch(const Ice::Exception& ex) {
        //logger.logSEVERE("Error in ObsControlIF:gotoTarget(). %s", ex.ice_name().c_str());
        cout << ex << endl;
        //emit newData(2, data);
    }
}
