#ifndef LOGGER_I_H
#define LOGGER_I_H 

#include "Logger.h"
#include "LogPublisher.h"
#include <IceStorm/IceStorm.h>
#include <Ice/Application.h>

#define DEFAULT_GLOBAL_DISCARD_LEVEL 1
using namespace std;
using namespace Ice;
using namespace IceStorm;

namespace Log {
  class LoggerI: public Logger {
      LogPublisherPrx logPublisherPrx;
      TopicManagerPrx getTopicManager();
      CommunicatorPtr communicator;
    public:
      LoggerI(CommunicatorPtr ic);
      // ICE defined method
      //void logMessage(const LogMessageData log, const Ice::Current&);
      void logMessage(const LogMessageData&, const Ice::Current&);
      void setDiscardLevel(const string& source, LogLevel level,const Ice::Current&);
      void setGlobalDiscardLevel(LogLevel level,const Ice::Current&);
      int getDiscardLevel(const string& source,const Ice::Current&);
      int getGlobalDiscardLevel(const Ice::Current&);
      // log level description
      int globalDiscardLevel;
      string levelDesc[10];  
      bool isDBConnected;
  };
};
#endif
