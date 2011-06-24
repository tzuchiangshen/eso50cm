#include <LCUImpl.h>

using namespace std;

//global variable: to be fix
int verbose = 1;

int
run(int argc, char* argv[], const Ice::CommunicatorPtr& communicator)
{
    Ice::ObjectAdapterPtr adapter = communicator->createObjectAdapter("LCUAdapter");
    Ice::ObjectPtr object = new LCUImpl;
    adapter->add(object, communicator->stringToIdentity("LCU"));
    adapter->activate();
    communicator->waitForShutdown();
    return EXIT_SUCCESS;
}

int
main(int argc, char* argv[])
{
    int status;
    Ice::CommunicatorPtr communicator;

    try
    {
        string configPath =  getenv("SWROOT");
        configPath = configPath + "/config/LCU-config";
        Ice::InitializationData initData;
	initData.properties = Ice::createProperties();
        initData.properties->load(configPath);
	initData.properties->setProperty("Ice.Override.Timeout", "100");
	communicator = Ice::initialize(argc, argv, initData);
	status = run(argc, argv, communicator);
    }
    catch(const Ice::Exception& ex)
    {
#ifdef ARM
	fprintf(stderr, "%s\n", ex.toString().c_str());
#else
        cerr << ex << endl;
#endif
	status = EXIT_FAILURE;
    }

    if(communicator)
    {
	try
	{
	    communicator->destroy();
	}
	catch(const Ice::Exception& ex)
	{
#ifdef ARM
	    fprintf(stderr, "%s\n", ex.toString().c_str());
#else   
        cerr << ex << endl;
#endif
	    status = EXIT_FAILURE;
	}
    }

    return status;
}
