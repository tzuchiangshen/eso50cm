#ifndef LOGGER_I_H
#define LOGGER_I_H 

#include "Logger.h"
#include "dbstuff.h"
#define DEFAULT_GLOBAL_DISCARD_LEVEL 2
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
      // log level description
      int globalDiscardLevel;
      string levelDesc[10];  
      bool isDBConnected;
      MySQLCon db;
  };
};
#endif
