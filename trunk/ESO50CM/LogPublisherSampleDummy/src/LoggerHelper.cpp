#include "LoggerHelperDummy.h"
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
using namespace std;

LoggerHelper::LoggerHelper(string src) 
{  
    
};
LoggerHelper::~LoggerHelper()
{
};
void LoggerHelper::setDiscardLevel(LogLevel level)
{
    printf("Log level changed to %i\n", level);
}
void LoggerHelper::logMsg(LogLevel level,string log, string method, int lineNumber){
    struct timeval tv;
    gettimeofday(&tv, NULL); 
    printf("%f: %s | %s | %i |\n",(double)tv.tv_sec+(double)tv.tv_usec/1.0E6,log.c_str(),method.c_str(),lineNumber); 
    
}
void LoggerHelper::logFINEST(string log, string method, int lineNumber){    
    logMsg(FINEST,log, method, lineNumber);
}
void LoggerHelper::logFINER(string log, string method, int lineNumber){    
    logMsg(FINER,log, method, lineNumber);
}
void LoggerHelper::logFINE(string log, string method, int lineNumber){    
    logMsg(FINE,log, method, lineNumber);
}
void LoggerHelper::logCONFIG(string log, string method, int lineNumber){    
    logMsg(CONFIG,log, method, lineNumber);
}
void LoggerHelper::logINFO(string log, string method, int lineNumber){    
    logMsg(INFO,log, method, lineNumber);
}
void LoggerHelper::logWARNING(string log, string method, int lineNumber){    
    logMsg(WARNING,log, method, lineNumber);
}
void LoggerHelper::logSEVERE(string log, string method, int lineNumber){    
    logMsg(SEVERE,log, method, lineNumber);
}
double LoggerHelper::convertDouble(double MEData)
{
    return MEData;
}

