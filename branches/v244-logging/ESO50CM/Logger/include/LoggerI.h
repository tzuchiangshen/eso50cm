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
      // ICE defined method
      //void logMessage(const LogMessageData log, const Ice::Current&);
      void logMessage(const LogMessageData&, const Ice::Current&);
      void setDiscardLevel(const string& source, LogLevel level,const Ice::Current&);
      void setDiscardLevelLocal(const string& source, LogLevel level);  // To be called from inside the class (used by the ICE version)
      int getDiscardLevel(const string& source,const Ice::Current&);
      int getDiscardLevelLocal(const string& source);  // To be called from inside the class (used by the ICE version)
      // log level description
      string levelDesc[10];  
  };
};
#endif
