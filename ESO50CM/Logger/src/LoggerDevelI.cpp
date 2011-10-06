#include "LoggerDevelI.h"
#include <stdio.h>
#include <string.h>


using namespace Log;
LoggerI::LoggerI(CommunicatorPtr ic):communicator(ic)
{
    globalDiscardLevel=DEFAULT_GLOBAL_DISCARD_LEVEL;
    // Define the descriptions of the log levels
    levelDesc[FINEST]="FINEST";
    levelDesc[FINER]="FINER";
    levelDesc[FINE]="FINE";
    levelDesc[CONFIG]="CONFIG";
    levelDesc[INFO]="INFO";
    levelDesc[WARNING]="WARNING";
    levelDesc[SEVERE]="SEVERE";
    levelDesc[9]="Unknown log level";

    isDBConnected=false;
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
        
        
}
void LoggerI::logMessage(const LogMessageData &message, const Ice::Current& c)
{
        logPublisherPrx->logEvent(message);
        //  |level | timestamp | source | method | linenumber | message |
        printf("| %s | %lf | %s | %s | %i | %s |\n",levelDesc[message.level].c_str(), message.timestamp, message.source.c_str(),message.method.c_str(), message.lineNumber, message.message.c_str());
        
}

void LoggerI::setDiscardLevel(const string& source, LogLevel level,const Ice::Current&)
{
     // we're not managin discard levels at server side without accesing the DB
}
void LoggerI::setGlobalDiscardLevel(LogLevel level,const Ice::Current& c)
{
    globalDiscardLevel=level;
}
int LoggerI::getDiscardLevel(const string& source,const Ice::Current&)
{
   // if (isDBConnected)
  //      return db.getDiscardLevel(source);
    return 1;  /// we will asume it's always 1
}
int LoggerI::getGlobalDiscardLevel(const Ice::Current& c)
{
    return globalDiscardLevel;
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
