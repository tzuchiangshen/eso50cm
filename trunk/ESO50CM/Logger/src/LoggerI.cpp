#include "LoggerI.h"
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include "FakeConfigurationsStorage.h"
#include "FileStorageDAO.h"


using namespace Log;
LoggerI::LoggerI(CommunicatorPtr ic):communicator(ic)
{

    // Define the descriptions of the log levels
    levelDesc[FINEST]="FINEST";
    levelDesc[FINER]="FINER";
    levelDesc[FINE]="FINE";
    levelDesc[CONFIG]="CONFIG";
    levelDesc[INFO]="INFO";
    levelDesc[WARNING]="WARNING";
    levelDesc[SEVERE]="SEVERE";
    levelDesc[9]="Unknown log level";

    // Obtaining environment for publishing
    TopicManagerPrx topic_mgr = getTopicManager();
    if(!topic_mgr){
        cerr << " invalid proxy" << endl;
    }
    TopicPrx topic;
    try{
        topic = topic_mgr->retrieve("LogTopic");
    }
    catch (const NoSuchTopic& e){
        cerr << " no such topic found, created" << endl;
        topic = topic_mgr->create("LogTopic");
    }       
    assert(topic);
    // Get the topic's publisher object.
    ObjectPrx prxPublisher = topic->getPublisher();
    logPublisherPrx = LogPublisherPrx::uncheckedCast(prxPublisher);
    setupMessageStorageDAOs();
    // now that we read all the configurations using a local version of 'Configurations' class, we will get the reference to LoggingService.ini that will be used for the rest of the time
    configurations=NULL;
    try {
        configurations=new Configurations("LoggingService.ini");
    } catch (...) {
        // do nothing, in the worst key, we will continue with the default values
    }
    pthread_mutex_init(&configurations_access_mutex,NULL);
        
}
LoggerI::~LoggerI()
{
    for (int i=0;i<messageStorages.size();i++)
        delete messageStorages[i];
    if (configurations!=NULL)
	delete configurations;
}
void LoggerI::setupMessageStorageDAOs()
{
    

   FileStorage *fileStorage=new FileStorage;
   try {
       fileStorage->configureDAO();
       messageStorages.push_back(fileStorage);
   } catch (ConfigureDAOEx ex) {
       delete fileStorage;
       cerr << ex.details;
   } catch (...) {
       delete fileStorage;
       cerr << "unknown error when trying to configure fileStorage DAO";
   }
      //TODO: DBStorage dbStorage; add here the configuration for a DB DAO, using 
    
}


void LoggerI::logMessage(const LogMessageData &msg, const Ice::Current& c)
{
    // getting the time
    struct timeval tv;
    gettimeofday(&tv, NULL); 
    LogMessageData message=msg; 
    // timestamp created when the log is received by the logging system
    message.logtimestamp=(double)tv.tv_sec+(double)tv.tv_usec/1.0E6;  // unix time (sec) + microsec
    logPublisherPrx->logEvent(message);

    int daoIndex;
    if (message.level > getDiscardLevelLocal(message.source)) {
        logPublisherPrx->logEvent(message);  // publishing the event on IceStorm
        for (daoIndex=0; daoIndex < messageStorages.size() ; daoIndex++)
        {
            try {
                messageStorages[daoIndex]->saveMessage(message);
            } catch (...) {
                cerr << "There was an error trying to save message to an DAO";
            }
        }
    }    
}

/**
* Discard level setters
**/ 
void LoggerI::setDiscardLevel(const string& source, LogLevel level,const Ice::Current&)
{
    setDiscardLevelLocal(source,level);
}
void LoggerI::setDiscardLevelLocal(const string& source, LogLevel level)
{
    logLevelsTable[source]=level;
    if (configurations){
        cout << "Entering setDiscardLevelLocal for " << source << " level: " << level << endl;
	pthread_mutex_lock(&configurations_access_mutex);
        cout << "mutex acquired setDiscardLevelLocal for " << source << " level: " << level << endl;
	try {
		configurations->setIntValue("LogLevels",source,level);  
        } catch(KeyNotFoundEx ex) {
		cout << "There were a problem trying to save " << source  << "=" << level << " -> KeyNotFound" << endl;
	} catch(SectionNotFoundEx ex) {
                cout << "There were a problem trying to save " << source  << "=" << level << " -> SectionNotFound" << endl;
	} catch(InvalidValueEx ex) {
		cout << "There were a problem trying to save " << source  << "=" << level << " -> Invalid value" << endl;
	} catch (...) {
 		cout << "There were a problem trying to save " << source  << "=" << level << " -> unknown error" << endl;
	} 			
	cout << "Int value saved setDiscardLevelLocal for " << source << " level: " << level << endl;
	pthread_mutex_unlock(&configurations_access_mutex);
 	cout << "mutex released setDiscardLevelLocal for " << source << " level: " << level << endl;
     }
}
/**
* Discard level getters 
**/
int LoggerI::getDiscardLevel(const string& source,const Ice::Current&)
{
    return getDiscardLevelLocal(source);
}
int LoggerI::getDiscardLevelLocal(const string& source)
{
    int level=DEFAULT_DISCARD_LEVEL;
    if(logLevelsTable.find(source) == logLevelsTable.end())  // not found in the list! let's re-check file then
    {
        if (configurations){
        	try {
			cout << "getDiscardLevel for " << source;
			cout << "getDiscardLevel: about to acquire mutex" << endl;
			pthread_mutex_lock(&configurations_access_mutex);
			cout << "getDiscardLevel: mutex acquired" << endl;
           		level=configurations->getIntValue("LogLevels",source);
		} catch(KeyNotFoundEx ex) {
			cout << "Cannot found " << source << " in the log levels table, we will add it"  << endl; 
                       	logLevelsTable[source]=level;
			try {
                       		configurations->setIntValue("LogLevels",source,level);
			} catch (...) {
				cout << "There was an error inserting the new level for " << source;
			}
        	} catch(SectionNotFoundEx ex) {
                		cout << "There were a problem trying to get level of " << source   << " -> SectionNotFound" << endl;
        	} catch(InvalidValueEx ex) {
                		cout << "There were a problem trying to get level of " << source   << " -> Invalid value" <<  endl;
        	} catch (...) {
                		cout << "There were a problem trying to get level of " << source   << " -> unknown error" << endl;
        	} 
		pthread_mutex_unlock(&configurations_access_mutex);
		cout << "getDicardLevel mutex unlock" << endl;
           	logLevelsTable[source]=level;
        }
	return level;
    }	
    return logLevelsTable[source];
}


TopicManagerPrx LoggerI::getTopicManager() {
    PropertiesPtr properties = communicator->getProperties();       
    const string prop_key = "IceStormAdmin.TopicManager.Default";
    string strproxy = properties->getProperty(prop_key);       
    if (strproxy.empty()) {
        cerr << " property `" << prop_key << "' not set" << endl;
        return 0;
    }
    cout << "Using IceStorm in '" << strproxy << "' " << endl;       
    ObjectPrx base = communicator->stringToProxy(strproxy);
    return TopicManagerPrx::checkedCast(base);
}
