#ifndef __LOGGER_HELPER_DUMMY_H_
#define __LOGGER_HELPER_DUMMY_H_
#include<string>
typedef enum { FINEST,FINER, FINE,CONFIG, INFO,WARNING,SEVERE} LogLevel;

using namespace std;
class LoggerHelper {
// Local methods
  public:
      LoggerHelper(string src);
      ~LoggerHelper();
      void setDiscardLevel(LogLevel level);
      void logFINEST(string log, string method="", int lineNumber=-1);
      void logFINER(string log, string method="", int lineNumber=-1);
      void logFINE(string log, string method="", int lineNumber=-1);    
      void logCONFIG(string log, string method="", int lineNumber=-1);
      void logINFO(string log, string method="", int lineNumber=-1);
      void logWARNING(string log, string method="", int lineNumber=-1);
      void logSEVERE(string log, string method="", int lineNumber=-1);
      void logMsg(LogLevel, string,string,int);
      double convertDouble(double MEData); // helper for LCU middle endian problem
   private:
//       CommunicatorPtr m_ic;
//       ObjectPrx m_base;
//       ObjectPrx oneway;
//       LoggerPrx m_prx;
       string m_source;
       LogLevel m_discardLevel;
       string logginServiceProxyStr;
};
#endif
