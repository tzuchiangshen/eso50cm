#ifndef LOGGER_I_H
#define LOGGER_I_H 

#include "Logger.h"
using namespace std;
using namespace Ice;
namespace Log {
  class LoggerI: public Logger {
    public:
      LoggerI();
      // ICE defined method
      //void logMessage(const LogMessageData log, const Ice::Current&);
      void logMessage(const LogMessageData&, const Ice::Current&);
      void setDiscardLevel(const string& source, LogLevel level,const Ice::Current&);
      void setGlobalDiscardLevel(LogLevel level,const Ice::Current&);
      LogMessageDataSet getLogsFromIndex(int index,int maxlogs,const Ice::Current&);
      LogMessageDataSet getLogsFromDate(int timestamp,const Ice::Current&);   
  };
};
#endif
