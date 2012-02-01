
#include <IceE/IceE.h>
#include "LCU.h"

using namespace std;
using namespace OUC;

void
menu()
{
    printf("usage:\n");
    printf("t: send greeting as twoway\n");
    printf("o: send greeting as oneway\n");
#ifdef ICEE_HAS_BATCH
    printf("O: send greeting as batch oneway\n");
    printf("f: flush all batch requests\n");
#endif
    printf("P: get encoders positions\n");
    printf("s: shutdown server\n");
    printf("x: exit\n");
    printf("?: help\n");
}

int
run(int argc, char* argv[], const Ice::CommunicatorPtr& communicator)
{
    Ice::PropertiesPtr properties = communicator->getProperties();
    const char* proxyProperty = "LCUAdapter.Proxy";
    string proxy = properties->getProperty(proxyProperty);
    if(proxy.empty())
    {
	fprintf(stderr, "%s: property `%s' not set\n", argv[0], proxyProperty);
	return EXIT_FAILURE;
    }

    Ice::ObjectPrx base = communicator->stringToProxy(proxy);
    LCUPrx twoway = LCUPrx::checkedCast(base->ice_twoway()->ice_timeout(-1));
    if(!twoway)
    {
	fprintf(stderr, "%s: invalid proxy\n", argv[0]);
	return EXIT_FAILURE;
    }
    LCUPrx oneway = LCUPrx::uncheckedCast(twoway->ice_oneway());
#ifdef ICEE_HAS_BATCH
    LCUPrx batchOneway = LCUPrx::uncheckedCast(twoway->ice_batchOneway());
#endif

    int timeout = -1;
    int delay = 0;

    menu();

    char c = EOF;
    do
    {
	try
	{
	    printf("==> ");
	    do
	    {
	        c = getchar();
	    }
	    while(c != EOF && c == '\n');
	    if(c == 't')
	    {
		twoway->sayHello(delay);
	    }
	    else if(c == 'o')
	    {
		oneway->sayHello(delay);
	    }
#ifdef ICEE_HAS_BATCH
	    else if(c == 'O')
	    {
		batchOneway->sayHello(delay);
	    }
	    else if(c == 'f')
	    {
		communicator->flushBatchRequests();
	    }
#endif
	    else if(c == 'T')
	    {
		if(timeout == -1)
		{
		    timeout = 2000;
		}
		else
		{
		    timeout = -1;
		}
		
		twoway = LCUPrx::uncheckedCast(twoway->ice_timeout(timeout));
		oneway = LCUPrx::uncheckedCast(oneway->ice_timeout(timeout));
#ifdef ICEE_HAS_BATCH
		batchOneway = LCUPrx::uncheckedCast(batchOneway->ice_timeout(timeout));
#endif		
		if(timeout == -1)
		{
		    printf("timeout is now switched off\n");
		}
		else
		{
		    printf("timeout is now set to 2000ms\n");
		}
	    }
	    else if(c == 'P')
	    {
		oneway->getEncodersPosition();
	    }
	    else if(c == 's')
	    {
		twoway->shutdown();
	    }
	    else if(c == 'x')
	    {
		// Nothing to do
	    }
	    else if(c == '?')
	    {
		menu();
	    }
	    else
	    {
		printf("unknown command `%c'\n", c);
		menu();
	    }
	}
	catch(const Ice::Exception& ex)
	{
	    fprintf(stderr, "%s\n", ex.toString().c_str());
	}
    }
    while(c != EOF && c != 'x');

    return EXIT_SUCCESS;
}

int
main(int argc, char* argv[])
{
    int status;
    Ice::CommunicatorPtr communicator;

    try
    {
    	Ice::InitializationData initData;
	initData.properties = Ice::createProperties();
        initData.properties->load("config");
	communicator = Ice::initialize(argc, argv, initData);
	status = run(argc, argv, communicator);
    }
    catch(const Ice::Exception& ex)
    {
	fprintf(stderr, "%s\n", ex.toString().c_str());
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
	    fprintf(stderr, "%s\n", ex.toString().c_str());
	    status = EXIT_FAILURE;
	}
    }

    return status;
}
