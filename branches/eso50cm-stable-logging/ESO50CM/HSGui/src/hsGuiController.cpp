
#include "hsGuiController.h"

using namespace std;
using namespace OUC;



void hsGuiController::handset_slew(string rate, string direction)
{
    OUC::SlewInfo *slewInfo; 
	try 
	{
        slewInfo = new OUC::SlewInfo();
        slewInfo->rateName = rate;
        slewInfo->direction =  direction;
        
		lcu->handsetSlew(*slewInfo);
		printf(">>>>>>>>>>>>> slew->rate = %s \n", slewInfo->rateName.c_str());
		printf(">>>>>>>>>>>>> slew->direction = %s\n", slewInfo->direction.c_str());
	}
	catch(const Ice::Exception& ex)
	{
	    cout << ex << endl;
	}

}


hsGuiController::hsGuiController()
{
	lcu = NULL;
	communicator = NULL;
}

hsGuiController::~hsGuiController()
{
	disconnect();
	lcu = NULL;
	communicator = NULL;

}

TelescopePrx hsGuiController::getLCUReference() {
	if (lcu) 
		return lcu;
	else {
		connect();
		return lcu;
	}
}


int hsGuiController::connect() 
{
    int status;

    int argc = 0;
    char *argv[] = {};

    //Initialize the communicator
    try {
		string configPath = getenv("SWROOT");
		configPath = configPath + "/config/Obs-config";
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
        if(proxy.empty()) {
        	fprintf(stderr, "%s: property `%s' not set\n", argv[0], proxyProperty);
        	return EXIT_FAILURE;
        }

        Ice::ObjectPrx base = communicator->stringToProxy(proxy);
        obs = ObservingPrx::checkedCast(base->ice_twoway()->ice_timeout(-1));
        if(!obs) {
        	fprintf(stderr, "%s: invalid proxy\n", argv[0]);
        	return EXIT_FAILURE;
        }

        lcu = obs->getTelescope();
        if(!lcu) {
        	fprintf(stderr, "%s: invalid proxy\n", argv[0]);
        	return EXIT_FAILURE;
        }
        
        return status;

    } catch(const Ice::ConnectionRefusedException& ex) {
        cout << "ERROR!!! Unable to connect to server: " << ex << endl;
		status = EXIT_FAILURE;
    } catch(const Ice::Exception& ex){
		cout << ex << endl;
		status = EXIT_FAILURE;
    }

}

int hsGuiController::disconnect() 
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

void hsGuiController::run() 
{
	while(1) 
	{
		sleep(1.0);
	}
}

