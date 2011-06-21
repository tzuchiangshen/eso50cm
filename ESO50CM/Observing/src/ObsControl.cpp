#include <ObservingImpl.h>
#include <Ice/Ice.h>

using namespace std;

//global variable: to be fix
int verbose = 1;

int
run(int argc, char* argv[], const Ice::CommunicatorPtr& communicator)
{
    Ice::ObjectAdapterPtr adapter = communicator->createObjectAdapter("ObsAdapter");
    Ice::ObjectPtr object = new ObservingImpl(adapter);
    adapter->add(object, communicator->stringToIdentity("Observing"));
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
        configPath = configPath + "/config/Obs-config";
        Ice::InitializationData initData;
	initData.properties = Ice::createProperties();
        initData.properties->load(configPath);
	initData.properties->setProperty("Ice.Override.Timeout", "100");
	communicator = Ice::initialize(argc, argv, initData);
	status = run(argc, argv, communicator);
    }
    catch(const Ice::Exception& ex)
    {
	cerr << ex << endl;
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
	    cerr << ex << endl;;
	    status = EXIT_FAILURE;
	}
    }

    return status;
}
