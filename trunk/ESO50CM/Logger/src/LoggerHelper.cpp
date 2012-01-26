#include "LoggerHelper.h"
#include <stdio.h>
#include <sys/time.h>
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
    string filename,swroot;
    ifstream configFile;
    size_t found;
    logginServiceProxyStr="";
    try {
        if (getenv("SWROOT")!=NULL) {
            swroot=getenv("SWROOT");
        } else {
            throw "SWROOT environmental variable not defined";
        }
        filename=swroot+string("/config/loggingService.config");
        configFile.open(filename.c_str());
        if (configFile.is_open()) {
            // we have patches! we look for the file in every directory
            size_t found;
            while( !configFile.eof() ) {
                string line;
                getline(configFile, line);
                // got a line, checking if the file exist
                if (!line.empty())
                    if (line[0]=='#')   // a comment  
                        continue;
                    found=line.find("LoggingService.Endpoints=");
                    if (found == string::npos)  // string not found
                        continue;
                    else
                        // string found!
                        logginServiceProxyStr=string("Logger -t:")+line.substr(line.find("=")+1);
            }
        } else {
            throw string(string("Couldn't open")+filename).c_str();
        }   
        if (logginServiceProxyStr.empty()) {
            // if at this point, loggin service is empty, the property is missing
            throw string(string("Couldn't find the property LoggingService.Endpoints in ")+filename).c_str();
        }
        
        cout << "Proxy for logging service: " << logginServiceProxyStr << endl;
#ifdef ARM
        int dummy=0;
        m_ic =  Ice::initialize(dummy,0);
#else
        Ice::InitializationData id;
        m_ic =  Ice::initialize(id);
#endif 
        m_base = m_ic->stringToProxy(logginServiceProxyStr.c_str());
        if (!m_base)
            printf("wrong m_base");
        try {
             oneway =  m_base->ice_oneway();
        } catch (const NoEndpointException) {
             cerr << "No endpoint for oneway invocations" << endl;
        }
        m_prx = LoggerPrx::uncheckedCast(oneway);
    //    if (!m_prx) throw "Invalid proxy";   
        if (!m_prx) printf("Invalid proxy");   
        m_source=src;
       // setDiscardLevel(CONFIG);
    } catch (const Ice::Exception& ex) {
#ifdef ARM
        cout << ex.toString() << endl;
#else
        cout << ex << endl;
#endif

    } catch (const char* msg) {
        cout << msg << endl;

}
    
};
LoggerHelper::~LoggerHelper()
{
/*    if (m_ic)
        m_ic->destroy();    */
};
void LoggerHelper::setDiscardLevel(LogLevel level)
{
    m_prx->setDiscardLevel(m_source,level);
};
LogLevel LoggerHelper::getDiscardLevel(string source)
{
    m_prx->getDiscardLevel(source);
};
void LoggerHelper::logMsg(LogLevel level,string log, string method, int lineNumber){
    // created an empty message
    LogMessageData message;
    // level
    message.level=level;
    // getting the time
    struct timeval tv;
    gettimeofday(&tv, NULL); 
    message.timestamp=convertDouble((double)tv.tv_sec+(double)tv.tv_usec/1.0E6);  // unix time (sec) + milisec
    message.source=m_source;
    message.data=log;
    message.method=method;
    message.lineNumber=lineNumber;
    m_prx->logMessage(message);
    
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

