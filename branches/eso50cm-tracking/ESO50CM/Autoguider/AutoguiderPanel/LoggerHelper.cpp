#include "LoggerHelper.h"
#include <stdio.h>
//#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>
using namespace std;
LoggerHelper::LoggerHelper(string src)
{
    initialize(src,true);
}
LoggerHelper::LoggerHelper(string src,bool oneWayCom)
{
    initialize(src,oneWayCom);
}

void LoggerHelper::initialize(string src,bool oneWayCom)
{  
    
};
LoggerHelper::~LoggerHelper()
{
/*    if (m_ic)
        m_ic->destroy();    */
};
void LoggerHelper::setDiscardLevel(LogLevel level, string source)
{
  
};
/*StringsVector LoggerHelper::getSources(int fromSource)
{
    if (oneWayCommunication)
    {
        cout  << "getSources: This method cannot be used when using One Way communication" << endl;
        StringsVector empty;
        return empty;
    } else
    	return m_prx->getSources(fromSource);
};*/

void LoggerHelper::setDiscardLevel(LogLevel level)
{
    
};

Log::LogLevel LoggerHelper::getDiscardLevel(string source)
{
	return Log::INFO;
};
void LoggerHelper::logMsg(LogLevel level,string log, string method, int lineNumber){
   
    qDebug() << method.c_str()<< ":" << lineNumber << " " << log.c_str() << endl;
}
void LoggerHelper::logFINEST(int lineNumber,string method,const char* Format, ...){    
      char buffer[1000];
      va_list Arguments;
      va_start(Arguments, Format);      
      vsprintf(buffer,Format,Arguments);
      va_end(Arguments);
      string str(buffer);
      logMsg(FINEST,buffer, method,lineNumber);
};
void LoggerHelper::logFINEST(const char* Format, ... ){    
      char buffer[1000];
      va_list Arguments;
      va_start(Arguments, Format);      
      vsprintf(buffer,Format,Arguments);
      va_end(Arguments);
      string str(buffer);
      logMsg(FINEST,buffer, "not specified", 0);
};
void LoggerHelper::logFINER(int lineNumber,string method,const char* Format, ...){    
      char buffer[1000];
      va_list Arguments;
      va_start(Arguments, Format);      
      vsprintf(buffer,Format,Arguments);
      va_end(Arguments);
      string str(buffer);
      logMsg(FINER,buffer, method,lineNumber);
};
void LoggerHelper::logFINER(const char* Format, ... ){    
      char buffer[1000];
      va_list Arguments;
      va_start(Arguments, Format);      
      vsprintf(buffer,Format,Arguments);
      va_end(Arguments);
      string str(buffer);
      logMsg(FINER,buffer,  "not specified", 0);
};
void LoggerHelper::logFINE(int lineNumber,string method,const char* Format, ...){    
      char buffer[1000];
      va_list Arguments;
      va_start(Arguments, Format);      
      vsprintf(buffer,Format,Arguments);
      va_end(Arguments);
      string str(buffer);
      logMsg(FINE,buffer, method,lineNumber);
};
void LoggerHelper::logFINE(const char* Format, ... ){    
      char buffer[1000];
      va_list Arguments;
      va_start(Arguments, Format);      
      vsprintf(buffer,Format,Arguments);
      va_end(Arguments);
      string str(buffer);
      logMsg(FINE,buffer,  "not specified", 0);
};
void LoggerHelper::logCONFIG(int lineNumber,string method,const char* Format, ...){    
      char buffer[1000];
      va_list Arguments;
      va_start(Arguments, Format);      
      vsprintf(buffer,Format,Arguments);
      va_end(Arguments);
      string str(buffer);
      logMsg(CONFIG,buffer, method,lineNumber);
};
void LoggerHelper::logCONFIG(const char* Format, ... ){    
      char buffer[1000];
      va_list Arguments;
      va_start(Arguments, Format);      
      vsprintf(buffer,Format,Arguments);
      va_end(Arguments);
      string str(buffer);
      logMsg(CONFIG,buffer,  "not specified", 0);
};
void LoggerHelper::logINFO(int lineNumber,string method,const char* Format, ...){    
      char buffer[1000];
      va_list Arguments;
      va_start(Arguments, Format);      
      vsprintf(buffer,Format,Arguments);
      va_end(Arguments);
      string str(buffer);
      logMsg(INFO,buffer, method,lineNumber);
};
void LoggerHelper::logINFO(const char* Format, ... ){    
      char buffer[1000];
      va_list Arguments;
      va_start(Arguments, Format);      
      vsprintf(buffer,Format,Arguments);
      va_end(Arguments);
      string str(buffer);
      logMsg(INFO,buffer, "not specified", 0);
};
void LoggerHelper::logWARNING(int lineNumber,string method,const char* Format, ...){    
      char buffer[1000];
      va_list Arguments;
      va_start(Arguments, Format);      
      vsprintf(buffer,Format,Arguments);
      va_end(Arguments);
      string str(buffer);
      logMsg(WARNING,buffer, method,lineNumber);
};
void LoggerHelper::logWARNING(const char* Format, ... ){    
      char buffer[1000];
      va_list Arguments;
      va_start(Arguments, Format);      
      vsprintf(buffer,Format,Arguments);
      va_end(Arguments);
      string str(buffer);
      logMsg(WARNING,buffer,  "not specified", 0);
};

void LoggerHelper::logSEVERE(int lineNumber,string method,const char* Format, ...){    
      char buffer[1000];
      va_list Arguments;
      va_start(Arguments, Format);      
      vsprintf(buffer,Format,Arguments);
      va_end(Arguments);
      string str(buffer);
      logMsg(SEVERE,buffer, method,lineNumber);
};
void LoggerHelper::logSEVERE(const char* Format, ... ){    
      char buffer[1000];
      va_list Arguments;
      va_start(Arguments, Format);      
      vsprintf(buffer,Format,Arguments);
      va_end(Arguments);
      string str(buffer);
      logMsg(SEVERE,buffer,  "not specified", 0);
};
void  LoggerHelper::logFINEST(string log){
      logMsg(FINEST,log, "not specified", 0);
}
void  LoggerHelper::logFINER(string log){
      logMsg(FINER,log,  "not specified", 0);
}
void  LoggerHelper::logFINE(string log){
      logMsg(FINE,log,  "not specified", 0);
}
void  LoggerHelper::logCONFIG(string log){
      logMsg(CONFIG,log,  "not specified", 0);
}
void  LoggerHelper::logINFO(string log){
      logMsg(INFO,log, "not specified", 0);
}
void  LoggerHelper::logWARNING(string log){
      logMsg(WARNING,log,  "not specified", 0);
}
void  LoggerHelper::logSEVERE(string log){
      logMsg(SEVERE,log,  "not specified", 0);
}

#ifdef ARM
double LoggerHelper::convertDouble(double MEData)
{
    double test;
    unsigned char *p;
    unsigned char *q;
    
    //RA
    test = 0;
    p = (unsigned char*)&MEData;
    q = (unsigned char*)&test;
    
    q += 4;
    memcpy(q, p, 4);
    q -= 4;
    p += 4;
    memcpy(q, p, 4);
    
    return test;
}
#else
double LoggerHelper::convertDouble(double MEData)
{
    return MEData;
}
#endif

