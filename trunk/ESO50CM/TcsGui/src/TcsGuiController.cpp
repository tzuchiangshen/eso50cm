
#include "TcsGuiController.h"

using namespace std;
using namespace OUC;

TcsGuiController::TcsGuiController() :
   logger("TcsGUI")
{
    ::Log::LogLevel level = ::Log::FINE;
    logger.setDiscardLevel(level);
	lcu = NULL;
	communicator = NULL;
	data = new OUC::TelescopeData();
}

TcsGuiController::~TcsGuiController()
{
	disconnect();
	lcu = NULL;
	communicator = NULL;

}

//move this to OBSComponent 
//-------------

char * TcsGuiController::strfdegs( char * string, size_t max_len, const char * format, double degs )
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

    //printf( "[myUtil] strfdegs: degs=%lf -> gg=%d mm=%d ss=%d cc=%d\n", degs, gg, mm, ss, cc );
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
    //snprintf( string, max_len, format, gg, mm, m_secs );
    snprintf( string, max_len, format, gg, mm, (double) ss );
    return string;
}

void TcsGuiController::getCurrentPositionRA(char *buffer, int maxlen) 
{

	mutex.lock();
	double ra = data->currentPos.RA;
	mutex.unlock();
	//convert to sexagesimal 
	strfdegs( buffer, maxlen, "%02d:%02d:%02.0lf\0", ra / 15.0 );
	//printf("-----------------------%s\n", buffer);
	//return buffer;
}

void TcsGuiController::getCurrentPositionDec(char *buffer, int maxlen) 
{

	mutex.lock();
	double dec = data->currentPos.Dec;
	mutex.unlock();
	//convert to sexagesimal 
	strfdegs( buffer, maxlen, "%+03d:%02d:%02.0lf\0", dec);
	//printf("-----------------------%s\n", buffer);
	//return buffer;
}

void TcsGuiController::formatRAPosition(double ra, char *buffer, int maxlen) 
{
	//convert to sexagesimal 
	strfdegs( buffer, maxlen, "%02d:%02d:%02.0lf\0", ra / 15.0 );
}

void TcsGuiController::formatDecPosition(double dec, char *buffer, int maxlen) 
{
	strfdegs( buffer, maxlen, "%+03d:%02d:%02.0lf\0", dec);
}

//=============

int TcsGuiController::getPosition()
{
	try 
	{
        char buffer[1000];
		mutex.lock();
		*data = lcu->getPosition();
        sprintf(buffer, "currentPos.RA =%.10lf\n", data->currentPos.RA);
		logger.logFINE(buffer);
		sprintf(buffer, "crrentPos.Dec = %.10lf \n", data->currentPos.Dec);
		logger.logINFO(buffer);
		mutex.unlock();
		emit newData(1, data);
	}
	catch(const Ice::Exception& ex)
	{
	    cout << ex << endl;
	}

    return EXIT_SUCCESS;
}

int TcsGuiController::setTargetPositionRA(const char *ra)
{
	printf("<<<<<<<<<<<<<<<<<<<<<<<<< setTargetPosition ra=%s\n", ra);
    int trg_ra_hrs;
    int trg_ra_min;
    int trg_ra_sec;
	char * ptr = NULL;
	double trg_ra = 0;
	OUC::TelescopePosition *new_pos;


	try 
	{
		if( (ptr = strstr( (char *)ra, "ra=" )) != NULL ) {
			cout << "llegue : " << ra << endl;

			if( sscanf(ra, "ra=%d:%d:%d", & trg_ra_hrs, & trg_ra_min, & trg_ra_sec ) == 3 ) {
			    printf( "[set_target] trg ra =%+03d:%02d:%02d\n", trg_ra_hrs, trg_ra_min, trg_ra_sec );
			    trg_ra  = fabs( (double) trg_ra_hrs ) + ((double) trg_ra_min / 60.) + ((double) trg_ra_sec / 3600.);
			    trg_ra *= 15.0;
			    if( trg_ra_hrs < 0 ) {
			        trg_ra *= -1.;
			    } else if( trg_ra_hrs == 0 && ( strstr( ra, "ra=-" ) != NULL) ) {
			        trg_ra *= -1.;
			    }
			    printf( "[set_target] trg ra =%lf\n", trg_ra );
			} else {
			    printf( "[set_target] No target ra data\n" );
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
		printf(">>>>>>>>>>>>> Target.RA = %.10lf  trg_ra=%.10lf\n", new_pos->RA, trg_ra);
		printf(">>>>>>>>>>>>> Target.Dec = %.10lf \n", new_pos->Dec);
		lcu->setTarget(*new_pos);
		mutex.unlock();
	}
	catch(const Ice::Exception& ex)
	{
	    cout << ex << endl;
	}

    return EXIT_SUCCESS;
}

int TcsGuiController::setTargetPositionDec(const char *arguments)
{
	printf("<<<<<<<<<<<<<<<<<<<<<<<<< setTargetPosition dec=%s\n", arguments);
    int trg_dec_deg;
    int trg_dec_min;
    int trg_dec_sec;
	char * ptr = NULL;
	double trg_dec = 0;
	OUC::TelescopePosition *new_pos;


	try 
	{

	   if( (ptr = strstr( (char *)arguments, "dec=" )) != NULL ) {
	        if( sscanf( ptr, "dec=%d:%d:%d", & trg_dec_deg, & trg_dec_min, & trg_dec_sec ) == 3 ) {
	            printf( "[set_target] trg dec=%+03d:%02d:%02d\n", trg_dec_deg, trg_dec_min, trg_dec_sec );
	            trg_dec  = fabs( (double) trg_dec_deg ) + ((double) trg_dec_min / 60.) + ((double) trg_dec_sec / 3600.);
	            if( trg_dec_deg < 0 ) {
	                trg_dec *= -1.;
	            } else if( trg_dec_deg == 0 && ( strstr( arguments, "dec=-" ) != NULL) ) {
	                trg_dec *= -1.;
	            }
	            printf( "[set_target] trg dec=%lf\n", trg_dec );
	        } else if( sscanf( ptr, "dec=%d*%d:%d", & trg_dec_deg, & trg_dec_min, & trg_dec_sec ) == 3 ) {
	            printf( "[set_target] trg dec=%+03d*%02d:%02d\n", trg_dec_deg, trg_dec_min, trg_dec_sec );
	            trg_dec  = fabs( (double) trg_dec_deg ) + ((double) trg_dec_min / 60.) + ((double) trg_dec_sec / 3600.);
	            if( trg_dec_deg < 0 ) {
	                trg_dec *= -1.;
	            } else if( trg_dec_deg == 0 && ( strstr( arguments, "dec=-" ) != NULL) ) {
	                trg_dec *= -1.;
	            }
	            printf( "[set_target] trg dec=%lf\n", trg_dec );
	        } else {
	            printf( "[set_target] No target dec data\n" );
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
		printf(">>>>>>>>>>>>> Target.RA = %.10lf %.10lf \n", new_pos->RA, trg_dec);
		printf(">>>>>>>>>>>>> Target.Dec = %.10lf \n", new_pos->Dec);
		lcu->setTarget(*new_pos);
		mutex.unlock();
	}
	catch(const Ice::Exception& ex)
	{
	    cout << ex << endl;
	}

    return EXIT_SUCCESS;
}

OUC::TelescopeData TcsGuiController::getTelescopeData() 
{
	OUC::TelescopeData newData;
	mutex.lock();
	//make a copy of telescope data 
	newData.currentPos.RA = data->currentPos.RA;
	newData.currentPos.Dec = data->currentPos.Dec;
	mutex.unlock();
	return newData;
}



TelescopePrx TcsGuiController::getLCUReference() {
	if (lcu) 
		return lcu;
	else {
		connect();
		return lcu;
	}
}


int TcsGuiController::connect() 
{
    int status;

    int argc = 0;
    char *argv[] = {};

    //Initialize the communicator
    try
    {
		string configPath = getenv("SWROOT");
		configPath = configPath + "/config/Obs-config";
    	Ice::InitializationData initData;
		initData.properties = Ice::createProperties();
        initData.properties->load(configPath);
		communicator = Ice::initialize(argc, argv, initData);
		//communicator = Ice::initialize(initData, 0);
		//status = getPosition(argc, argv, communicator);
    }
    catch(const Ice::Exception& ex)
    {
		cout << ex << endl;
		status = EXIT_FAILURE;
    }

    // get reference to LCUImpl

    Ice::PropertiesPtr properties = communicator->getProperties();
    const char* proxyProperty = "ObsAdapter.Proxy";
    string proxy = properties->getProperty(proxyProperty);
    if(proxy.empty())
    {
		fprintf(stderr, "%s: property `%s' not set\n", argv[0], proxyProperty);
		return EXIT_FAILURE;
    }

    Ice::ObjectPrx base = communicator->stringToProxy(proxy);
    obs = ObservingPrx::checkedCast(base->ice_twoway()->ice_timeout(-1));
    if(!obs)
    {
		fprintf(stderr, "%s: invalid proxy\n", argv[0]);
		return EXIT_FAILURE;
    }

	lcu = obs->getTelescope();
    if(!lcu)
    {
		fprintf(stderr, "%s: invalid proxy\n", argv[0]);
		return EXIT_FAILURE;
    }

    return status;
}

int TcsGuiController::disconnect() 
{
    int status;
    if(communicator)
    {
	try
	{
	    communicator->destroy();
	}
	catch(const Ice::Exception& ex)
	{
	    cout << ex << endl;
	    status = EXIT_FAILURE;
	}
    }

    return status;
}

void TcsGuiController::run() 
{
	while(1) 
	{
		getPosition();
		sleep(1.0);
	}
}

void TcsGuiController::test() 
{
	OUC::TelescopeData data;
	data = lcu->getPosition();
	printf(">>>>>>>>>>>>> RA = %.10lf \n", data.currentPos.RA);
	printf(">>>>>>>>>>>>> Dec = %.10lf \n", data.currentPos.Dec);
}

