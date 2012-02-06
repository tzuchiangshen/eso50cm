#ifndef LOGGER_I_H
#define LOGGER_I_H 
#include "Logger.h"
#include "LogPublisher.h"
#include <IceStorm/IceStorm.h>
#include <Ice/Application.h>
#include <pthread.h>
#include "FakeConfigurationsStorage.h"
#include "MessageStorageDAO.h"

#define DEFAULT_DISCARD_LEVEL 1
using namespace std;
using namespace Ice;
using namespace IceStorm;

namespace Log {
  class LoggerI: public Logger {
      LogPublisherPrx logPublisherPrx;
      TopicManagerPrx getTopicManager();
      CommunicatorPtr communicator;
      pthread_mutex_t configurations_access_mutex;
      Configurations *configurations;
    public:
      LoggerI(CommunicatorPtr ic);
      ~LoggerI();
      void setupMessageStorageDAOs();
      vector<MessageStorageDAO *> messageStorages;
      map<string, int> logLevelsTable;
      vector<string> sourcesList;  // since the maps are sorted by value, when returning the source list to 
                                  // the getSources method using a 'fromSource' we will not be sure if the last
                                  // source are the newest, we had to keep track of them in a differente place, like this 
                                  // vector. Since no sources are deleted from the table, we only have to add the new
                                  // source to the vector when it arrives. No other sync. task is needed.
      // ICE defined method
      void logMessage(const LogMessageData&, const Ice::Current&);
      void setDiscardLevel(const string& source, LogLevel level,const Ice::Current&);
      void setDiscardLevelLocal(const string& source, LogLevel level);  // To be called from inside the class (used by the ICE version)
      LogLevel getDiscardLevel(const string& source,const Ice::Current&);
      StringsVector getSources(Ice::Int fromSource, const Ice::Current&);
      LogLevel getDiscardLevelLocal(const string& source);  // To be called from inside the class (used by the ICE version)
      // log level description
      string levelDesc[10];  
  };
};
#endif
