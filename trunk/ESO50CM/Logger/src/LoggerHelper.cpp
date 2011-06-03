#include "LoggerHelper.h"
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

LoggerHelper::LoggerHelper(string src) 
{  
    try {
        printf("proxy: %s",getenv("LOG_SERVICE"));
#ifdef LCU
        int dummy=0;
        m_ic =  Ice::initialize(dummy,0);
#else
        Ice::InitializationData id;
        m_ic =  Ice::initialize(id);
#endif 
        m_base = m_ic->stringToProxy(getenv("LOG_SERVICE"));
        if (!m_base)
            printf("wrong mbase");
        m_prx = LoggerPrx::checkedCast(m_base);
    //    if (!m_prx) throw "Invalid proxy";   
        if (!m_prx) printf("Invalid proxy");   
        m_source=src;
        setDiscardLevel(CONFIG);
    } catch (const Ice::Exception& ex) {
#ifdef LCU
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
    m_discardLevel=level;
};
void LoggerHelper::logMsg(LogLevel level,string log, string method, int lineNumber){
    if (level<m_discardLevel)  // we discard the message if it's lower than the discard level
        return;  
    // created an empty message
    LogMessageData message;
    // level
    message.level=level;
    // getting the time
    struct timeval tv;
    gettimeofday(&tv, NULL); 
    message.timestamp=convertDouble((double)tv.tv_sec+(double)tv.tv_usec/1.0E6);  // unix time (sec) + milisec
    message.source=m_source;
    message.message=log;
    message.method=method;
    message.lineNumber=lineNumber;
    m_prx->logMessage(message);
    
}
void LoggerHelper::logFINEST(string log, string method, int lineNumber){    
    logMsg(FINEST,log, method, lineNumber);
};
void LoggerHelper::logFINER(string log, string method, int lineNumber){    
    logMsg(FINER,log, method, lineNumber);
};
void LoggerHelper::logFINE(string log, string method, int lineNumber){    
    logMsg(FINE,log, method, lineNumber);
};
void LoggerHelper::logCONFIG(string log, string method, int lineNumber){    
    logMsg(CONFIG,log, method, lineNumber);
};
void LoggerHelper::logINFO(string log, string method, int lineNumber){    
    logMsg(INFO,log, method, lineNumber);
};
void LoggerHelper::logWARNING(string log, string method, int lineNumber){    
    logMsg(WARNING,log, method, lineNumber);
};
void LoggerHelper::logSEVERE(string log, string method, int lineNumber){    
    logMsg(SEVERE,log, method, lineNumber);
};
#ifdef LCU
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

