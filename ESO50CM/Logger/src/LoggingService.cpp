#include <Ice/Ice.h>
#include <Ice/Application.h>
#include "LoggerI.h"

using namespace std;
using namespace Ice;
using namespace Log;


class LoggingService : public Application {
    int run(int argc, char* args[]) {
        shutdownOnInterrupt();
        
        CommunicatorPtr ic = communicator();
        ObjectAdapterPtr adapter = ic->createObjectAdapter("LoggingService");
        
        ObjectPtr servant = new LoggerI;
        ObjectPrx base = adapter->add(servant, ic->stringToIdentity("Logger"));
        adapter->activate();
        
        cout << ic->proxyToString(base) << endl;
        
        ic->waitForShutdown();
        return 0;
    }
};

int main(int argc, char* argv[]) {
    LoggingService* app = new LoggingService();
    return app->main(argc, argv);   
}


