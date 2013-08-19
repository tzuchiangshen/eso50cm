#ifndef __LOGGER_HELPER_H_
#define __LOGGER_HELPER_H_

#include <iostream>
#include <logMessage.h>
#include <QDebug>


using namespace std;
using namespace Log;

class LoggerHelper {
// Local methods
  public:
      // by default, oneWayCommunication is true
      LoggerHelper(string src, bool oneWayCom);
      LoggerHelper(string src);
      void initialize(string src, bool oneWayCom);
      ~LoggerHelper();
      void setDiscardLevel(LogLevel level,string source);
      void setDiscardLevel(LogLevel level);
      //StringsVector getSources(int fromSource);
      LogLevel getDiscardLevel(string source);
      void logFINEST(int linenumber, string method,const char* Format, ...);    
      void logFINEST(const char* Format, ... );    
      void logFINER(int lineNumber,string method, const char* Format, ...);    
      void logFINER(const char* Format, ... );    
      void logFINE(int lineNumber,string method,const char* Format, ...);    
      void logFINE(const char* Format, ... );    
      void logCONFIG(int lineNumber,string method,const char* Format, ...);    
      void logCONFIG(const char* Format, ... );    
      void logINFO(int lineNumber,string method,const char* Format, ...);    
      void logINFO(const char* Format, ... );    
      void logWARNING(int lineNumber,string method,const char* Format, ...);    
      void logWARNING(const char* Format, ... );    
      void logSEVERE(int lineNumber,string method,const char* Format, ...);    
      void logSEVERE(const char* Format, ... );    
      void  logFINEST(string log);
      void  logFINER(string log);
      void  logFINE(string log);
      void  logCONFIG(string log);
      void  logINFO(string log);
      void  logWARNING(string log);
      void  logSEVERE(string log);
      void logMsg(LogLevel, string,string,int);
      double convertDouble(double MEData); // helper for LCU middle endian problem
   private:
       //CommunicatorPtr m_ic;
       //ObjectPrx m_base;
       //ObjectPrx oneway;
       //LoggerPrx m_prx;
       string m_source;
       string logginServiceProxyStr;
       bool oneWayCommunication;
};
#endif
