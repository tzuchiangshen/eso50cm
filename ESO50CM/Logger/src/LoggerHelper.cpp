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
string findFilePath(string relFileName);

LoggerHelper::LoggerHelper(string src) 
{  
    string filename;
    ifstream configFile;
    size_t found;
    logginServiceProxyStr="";
    try {
        filename=findFilePath("config/loggingService.config");
        if (filename.empty()) 
            throw "config/loggingService.config not found";
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
        setDiscardLevel(CONFIG);
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
void LoggerHelper::logFINEST(int lineNumber,string method,const char* Format, ...){    
      char buffer[1000];
      va_list Arguments;
      va_start(Arguments, Format);      
      sprintf(buffer,Format,Arguments);
      string str(buffer);
      logMsg(FINEST,buffer, method,lineNumber);
};
void LoggerHelper::logFINEST(const char* Format, ... ){    
      char buffer[1000];
      va_list Arguments;
      va_start(Arguments, Format);      
      sprintf(buffer,Format,Arguments);
      string str(buffer);
      logMsg(FINEST,buffer, "", -1);
};
void LoggerHelper::logFINER(int lineNumber,string method,const char* Format, ...){    
      char buffer[1000];
      va_list Arguments;
      va_start(Arguments, Format);      
      sprintf(buffer,Format,Arguments);
      string str(buffer);
      logMsg(FINER,buffer, method,lineNumber);
};
void LoggerHelper::logFINER(const char* Format, ... ){    
      char buffer[1000];
      va_list Arguments;
      va_start(Arguments, Format);      
      sprintf(buffer,Format,Arguments);
      string str(buffer);
      logMsg(FINER,buffer, "", -1);
};
void LoggerHelper::logFINE(int lineNumber,string method,const char* Format, ...){    
      char buffer[1000];
      va_list Arguments;
      va_start(Arguments, Format);      
      sprintf(buffer,Format,Arguments);
      string str(buffer);
      logMsg(FINE,buffer, method,lineNumber);
};
void LoggerHelper::logFINE(const char* Format, ... ){    
      char buffer[1000];
      va_list Arguments;
      va_start(Arguments, Format);      
      sprintf(buffer,Format,Arguments);
      string str(buffer);
      logMsg(FINE,buffer, "", -1);
};
void LoggerHelper::logCONFIG(int lineNumber,string method,const char* Format, ...){    
      char buffer[1000];
      va_list Arguments;
      va_start(Arguments, Format);      
      sprintf(buffer,Format,Arguments);
      string str(buffer);
      logMsg(CONFIG,buffer, method,lineNumber);
};
void LoggerHelper::logCONFIG(const char* Format, ... ){    
      char buffer[1000];
      va_list Arguments;
      va_start(Arguments, Format);      
      sprintf(buffer,Format,Arguments);
      string str(buffer);
      logMsg(CONFIG,buffer, "", -1);
};
void LoggerHelper::logINFO(int lineNumber,string method,const char* Format, ...){    
      char buffer[1000];
      va_list Arguments;
      va_start(Arguments, Format);      
      sprintf(buffer,Format,Arguments);
      string str(buffer);
      logMsg(INFO,buffer, method,lineNumber);
};
void LoggerHelper::logINFO(const char* Format, ... ){    
      char buffer[1000];
      va_list Arguments;
      va_start(Arguments, Format);      
      sprintf(buffer,Format,Arguments);
      string str(buffer);
      logMsg(INFO,buffer, "", -1);
};
void LoggerHelper::logWARNING(int lineNumber,string method,const char* Format, ...){    
      char buffer[1000];
      va_list Arguments;
      va_start(Arguments, Format);      
      sprintf(buffer,Format,Arguments);
      string str(buffer);
      logMsg(WARNING,buffer, method,lineNumber);
};
void LoggerHelper::logWARNING(const char* Format, ... ){    
      char buffer[1000];
      va_list Arguments;
      va_start(Arguments, Format);      
      sprintf(buffer,Format,Arguments);
      string str(buffer);
      logMsg(WARNING,buffer, "", -1);
};

void LoggerHelper::logSEVERE(int lineNumber,string method,const char* Format, ...){    
      char buffer[1000];
      va_list Arguments;
      va_start(Arguments, Format);      
      sprintf(buffer,Format,Arguments);
      string str(buffer);
      logMsg(SEVERE,buffer, method,lineNumber);
};
void LoggerHelper::logSEVERE(const char* Format, ... ){    
      char buffer[1000];
      va_list Arguments;
      va_start(Arguments, Format);      
      sprintf(buffer,Format,Arguments);
      string str(buffer);
      logMsg(SEVERE,buffer, "", -1);
};
void  LoggerHelper::logFINEST(string log){
      logMsg(FINEST,log, "", -1);
}
void  LoggerHelper::logFINER(string log){
      logMsg(FINER,log, "", -1);
}
void  LoggerHelper::logFINE(string log){
      logMsg(FINE,log, "", -1);
}
void  LoggerHelper::logCONFIG(string log){
      logMsg(CONFIG,log, "", -1);
}
void  LoggerHelper::logINFO(string log){
      logMsg(INFO,log, "", -1);
}
void  LoggerHelper::logWARNING(string log){
      logMsg(WARNING,log, "", -1);
}
void  LoggerHelper::logSEVERE(string log){
      logMsg(SEVERE,log, "", -1);
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

// This method looks for the specified file in several dirs:
// TODO: move it to a common 'utils' library
string findFilePath(string relFileName)
{
    string fileName;
    string swroot,introot;
    if (getenv("SWROOT")!=NULL) {
        swroot=getenv("SWROOT");
    }
    else { 
        cout << "SWROOT not set, check you environmental variables";
        return string("");     
    }
    ifstream file;
    if (getenv("INTROOT")!=NULL) {
        // If we found and introot, we will check there first
        introot=getenv("INTROOT");
        fileName=introot+string("/")+relFileName;
        file.open(fileName.c_str());
        if (file.is_open())
            return fileName;
    }
    // Now we look in the patches files  
    ifstream patchesListFile((swroot+string("/patches/patches.list")).c_str());
    if (patchesListFile.is_open()) 
        // we have patches! we look for the file in every directory
    while( !patchesListFile.eof() ) {
        string line;
        getline(patchesListFile, line);
        // got a line, checking if the file exist
        if (!line.empty())
            fileName=swroot+string("/patches/")+line+string("/")+relFileName;
        file.open(fileName.c_str());
        if (file.is_open())
            return fileName;           
    }
    // at this point, we couldn't find the file in the patches or INTROOT
    // so we'll test our last chance: the SWROOT
    fileName=swroot+string("/")+relFileName;
    file.open(fileName.c_str());
    if (file.is_open())
        return fileName;
    // fuuuuuu. We cannot find the file, so we return an empy string
        return string("");
}
