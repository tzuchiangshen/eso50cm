#ifndef SIMPLECLIENT_I_H
#define SIMPLECLIENT_I_H 

#include <Ice/Application.h>
#include <IceStorm/IceStorm.h>
#include <IceUtil/UUID.h>
#include "LogPublisher.h"
#include <time.h>
#include <iostream>
#include <iomanip>
#include <sstream>



using namespace std;
using namespace Ice;
using namespace IceStorm;
using namespace Log;


class SimpleClientI : public LogPublisher {
    
    
    string timestampToString(double timestamp)
    {
    
    time_t rawtime=(time_t)timestamp;
    struct tm * timeinfo;
    char buffer [80];
    timeinfo = localtime ( &rawtime );
    strftime (buffer,80,"%Y-%m-%dT%H:%M:%S",timeinfo); 
    stringstream ss (stringstream::in | stringstream::out);
    ss << buffer << "." << setfill('0') << setw(3) <<  (int)((timestamp-rawtime)*1000);
    return ss.str();
    
    }    
    void logEvent(const LogMessageData &message, const Ice::Current& c) {
        string levelDesc[10]; 
 /*       levelDesc[FINEST]="FINEST";
        levelDesc[FINER]="FINER";
        levelDesc[FINE]="FINE";
        levelDesc[CONFIG]="CONFIG";
        levelDesc[INFO]="INFO";
        levelDesc[WARNING]="WARNING";
        levelDesc[SEVERE]="SEVERE";
 */
        levelDesc[0]="FINEST";
        levelDesc[1]="FINER";
        levelDesc[2]="FINE";
        levelDesc[3]="CONFIG";
        levelDesc[4]="INFO";
        levelDesc[5]="WARNING";
        levelDesc[6]="SEVERE";
        levelDesc[7]="Unknown log level";
        cout << "| " <<  timestampToString(message.logtimestamp) << " | "  <<  timestampToString(message.timestamp) << " | " <<  levelDesc[message.level] << " | " << 
         message.source << " | " << message.method  << " | " << message.lineNumber << " |  " << message.data << " |" << endl;
	}
};

class Subscriber : public Application {

	TopicManagerPrx get_topic_manager() {
		PropertiesPtr properties = communicator()->getProperties();

		const string prop_key = "IceStormAdmin.TopicManager.Default";
		string strproxy = properties->getProperty(prop_key);

		if (strproxy.empty()) {
			cerr << appName() << ": property `" << prop_key << "' not set" << endl;
			return 0;
		}

		cout << "Using IceStorm in '" << strproxy << "' " << endl;

		ObjectPrx base = communicator()->stringToProxy(strproxy);
		return TopicManagerPrx::checkedCast(base);
	}

public:
	virtual int run(int argc, char* argv[]) {

		TopicManagerPrx topic_mgr = get_topic_manager();
		if (!topic_mgr) {
			cerr << appName() << ": invalid proxy" << endl;
			return EXIT_FAILURE;
		}

		// Create the servant to receive the events.
		ObjectAdapterPtr adapter = communicator()->createObjectAdapter("LogPublisher.Subscriber");
		ObjectPtr servant = new SimpleClientI;

		// Add a Servant for the Ice Object.
		ObjectPrx base = adapter->addWithUUID(servant);
		TopicPrx topic;

		try {
			topic = topic_mgr->retrieve("LogTopic");
			QoS qos;
			topic->subscribeAndGetPublisher(qos, base);
		}
		catch(const NoSuchTopic& e){
			cerr << appName() << ": " << e << " name: " << e.name << endl;
			return EXIT_FAILURE;
		}

		cout << "Waiting events... " << base << endl;

		adapter->activate();
		shutdownOnInterrupt();
		communicator()->waitForShutdown();

		topic->unsubscribe(base);

		return EXIT_SUCCESS;
	}
};

int main(int argc, char* argv[]) {
	Subscriber app;
	return app.main(argc, argv);
}
#endif
