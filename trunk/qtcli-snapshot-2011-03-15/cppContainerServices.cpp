
#include "cppContainerServices.h"

using namespace std;
using namespace OUC;


//move this to OBSComponent 
//-------------

char * cppContainerServices::strfdegs( char * string, size_t max_len, const char * format, double degs )
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

void cppContainerServices::getCurrentPositionRA(char *buffer, int maxlen) 
{

	mutex.lock();
	double ra = data->currentPos.RA;
	mutex.unlock();
	//convert to sexagesimal 
	strfdegs( buffer, maxlen, "%02d:%02d:%02.0lf\0", ra / 15.0 );
	//printf("-----------------------%s\n", buffer);
	//return buffer;
}

void cppContainerServices::getCurrentPositionDec(char *buffer, int maxlen) 
{

	mutex.lock();
	double dec = data->currentPos.Dec;
	mutex.unlock();
	//convert to sexagesimal 
	strfdegs( buffer, maxlen, "%+03d:%02d:%02.0lf\0", dec);
	//printf("-----------------------%s\n", buffer);
	//return buffer;
}
//=============

int cppContainerServices::getPosition()
{
	try 
	{
		mutex.lock();
		*data = lcu->getPosition();
		printf(">>>>>>>>>>>>> RA = %.10lf \n", data->currentPos.RA);
		printf(">>>>>>>>>>>>> Target.RA = %.10lf \n", data->targetPos.RA);
		mutex.unlock();
	}
	catch(const Ice::Exception& ex)
	{
	    fprintf(stderr, "%s\n", ex.toString().c_str());
	}

    return EXIT_SUCCESS;
}

OUC::TelescopeData cppContainerServices::getTelescopeData() 
{
	OUC::TelescopeData newData;
	mutex.lock();
	//make a copy of telescope data 
	newData.currentPos.RA = data->currentPos.RA;
	newData.currentPos.Dec = data->currentPos.Dec;
	mutex.unlock();
	return newData;
}

cppContainerServices::cppContainerServices()
{
	lcu = NULL;
	communicator = NULL;
	data = new OUC::TelescopeData();
}

cppContainerServices::~cppContainerServices()
{
	disconnect();
	lcu = NULL;
	communicator = NULL;

}

LCUPrx cppContainerServices::getLCUReference() {
	if (lcu) 
		return lcu;
	else {
		connect();
		return lcu;
	}
}


int cppContainerServices::connect() 
{
    int status;

    int argc = 0;
    char *argv[] = {};

    //Initialize the communicator
    try
    {
    	Ice::InitializationData initData;
		initData.properties = Ice::createProperties();
        initData.properties->load("config");
		communicator = Ice::initialize(argc, argv, initData);
		//communicator = Ice::initialize(initData, 0);
		//status = getPosition(argc, argv, communicator);
    }
    catch(const Ice::Exception& ex)
    {
		fprintf(stderr, "%s\n", ex.toString().c_str());
		status = EXIT_FAILURE;
    }

    // get reference to LCUImpl

    Ice::PropertiesPtr properties = communicator->getProperties();
    const char* proxyProperty = "LCUAdapter.Proxy";
    string proxy = properties->getProperty(proxyProperty);
    if(proxy.empty())
    {
	fprintf(stderr, "%s: property `%s' not set\n", argv[0], proxyProperty);
	return EXIT_FAILURE;
    }

    Ice::ObjectPrx base = communicator->stringToProxy(proxy);
    lcu = LCUPrx::checkedCast(base->ice_twoway()->ice_timeout(-1));
    if(!lcu)
    {
	fprintf(stderr, "%s: invalid proxy\n", argv[0]);
	return EXIT_FAILURE;
    }

    return status;

}

int cppContainerServices::disconnect() 
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
	    fprintf(stderr, "%s\n", ex.toString().c_str());
	    status = EXIT_FAILURE;
	}
    }

    return status;
}

void cppContainerServices::run() 
{
	while(1) 
	{
		sleep(3);
		getPosition();
	}
}

void cppContainerServices::test() 
{
	OUC::TelescopeData data;
	data = lcu->getPosition();
	printf(">>>>>>>>>>>>> RA = %.10lf \n", data.currentPos.RA);
	printf(">>>>>>>>>>>>> Dec = %.10lf \n", data.currentPos.Dec);
}

