#include "LoggerI.h"
#include <stdio.h>
#include <string.h>

using namespace Log;
LoggerI::LoggerI()
{
    // do nothing
}
void LoggerI::logMessage(const LogMessageData &message, const Ice::Current& c)
{
    char levelStr[10];
    switch(message.level) {
        case FINEST:
            strcpy(levelStr,"FINEST");
            break;
        case FINER:
            strcpy(levelStr,"FINER");
            break;
        case FINE:
            strcpy(levelStr,"FINE");
            break;
        case CONFIG:
            strcpy(levelStr,"CONFIG");
            break;
        case INFO:
            strcpy(levelStr,"INFO");
            break;
        case WARNING:
            strcpy(levelStr,"WARNING");
            break;
        case SEVERE:
            strcpy(levelStr,"SEVERE");
            break;
        default:
            strcpy(levelStr,"Unknown level");
            break;
    }
    //  |level | timestamp | source | method | linenumber | message |
    printf("| %s | %lf | %s | %s | %i | %s |\n",levelStr, message.timestamp, message.source.c_str(),
                                                message.method.c_str(), message.lineNumber, message.message.c_str());
}

void LoggerI::setDiscardLevel(const string& source, LogLevel level,const Ice::Current&)
{
    printf("discard level set to %i for %s\n",level,source.c_str());
}
void LoggerI::setGlobalDiscardLevel(LogLevel level,const Ice::Current& c)
{
    printf("setGlobalDiscardLevel\n");
}
LogMessageDataSet LoggerI::getLogsFromIndex(int index,int maxlogs,const Ice::Current& c)
{   
    printf("getLogsFromIndex\n");
}
LogMessageDataSet LoggerI::getLogsFromDate(int timestamp,const Ice::Current& c)
{
    printf("getLogsFromDate\n");
}

