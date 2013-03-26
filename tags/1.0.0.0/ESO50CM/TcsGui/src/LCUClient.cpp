
#include "LCUClient.h"

using namespace std;
using namespace OUC;

int
LCUClient::getPosition()
{

	try 
	{
		OUC::TelescopeData *data = new OUC::TelescopeData();
		*data = lcu->getPosition();
		printf(">>>>>>>>>>>>> RA = %.10lf \n", data->currentPos.RA);
		printf(">>>>>>>>>>>>> Target.RA = %.10lf \n", data->targetPos.RA);
	}
	catch(const Ice::Exception& ex)
	{
	    fprintf(stderr, "%s\n", ex.toString().c_str());
	}

    return EXIT_SUCCESS;
}

LCUClient::LCUClient()
{

}

int LCUClient::connect() 
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

int LCUClient::disconnect() 
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
