#ifndef SIMPLECLIENT_I_H
#define SIMPLECLIENT_I_H 

#include <Ice/Application.h>
#include <IceStorm/IceStorm.h>
#include <IceUtil/UUID.h>
#include "LogPublisher.h"


using namespace std;
using namespace Ice;
using namespace IceStorm;
using namespace Log;


class SimpleClientI : public LogPublisher {
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
        printf("| %s | %lf | %s | %s | %i | %s |\n",levelDesc[message.level].c_str(), message.timestamp, message.source.c_str(),message.method.c_str(), message.lineNumber, message.data.c_str());
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
