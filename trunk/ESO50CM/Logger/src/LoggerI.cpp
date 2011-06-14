#include "LoggerI.h"
#include <stdio.h>
#include <string.h>
#include "dbstuff.h"

using namespace Log;
LoggerI::LoggerI()
{
    globalDiscardLevel=DEFAULT_GLOBAL_DISCARD_LEVEL;
    // Define the descriptions of the log levels
    levelDesc[FINEST]="FINEST";
    levelDesc[FINER]="FINER";
    levelDesc[FINE]="FINE";
    levelDesc[CONFIG]="CONFIG";
    levelDesc[INFO]="INFO";
    levelDesc[WARNING]="WARNING";
    levelDesc[SEVERE]="SEVERE";
    levelDesc[9]="Unknown log level";
    if (!db.connect("tcp://127.0.0.1:3306","logmgr","logpass"))
    {
        cout << "Warning! I couldn't connect to the DB!" << endl;
        isDBConnected=false;
    }
    else
        isDBConnected=true;
}
void LoggerI::logMessage(const LogMessageData &message, const Ice::Current& c)
{
    if (!isDBConnected)
    {
        //  |level | timestamp | source | method | linenumber | message |
        printf("| %s | %lf | %s | %s | %i | %s |\n",levelDesc[message.level].c_str(), message.timestamp, message.source.c_str(),
                                                message.method.c_str(), message.lineNumber, message.message.c_str());
    } else {
        // we're accessing the DB!
        int discardLevel=db.getDiscardLevel(message.source);
        if ((message.level > discardLevel) && (message.level > globalDiscardLevel)) {
            db.logMessage(message.level, message.source,message.timestamp, message.method,message.lineNumber,message.message);
        }
    }
        
}

void LoggerI::setDiscardLevel(const string& source, LogLevel level,const Ice::Current&)
{
    if (!isDBConnected)
    {
        db.setDiscardLevel(source,level);
    }
}
void LoggerI::setGlobalDiscardLevel(LogLevel level,const Ice::Current& c)
{
    globalDiscardLevel=level;
}
/*
LogMessageDataSet LoggerI::getLogsFromIndex(int index,int maxlogs,const Ice::Current& c)
{   
    printf("getLogsFromIndex\n");
}
LogMessageDataSet LoggerI::getLogsFromDate(int timestamp,const Ice::Current& c)
{
    printf("getLogsFromDate\n");
}
*/
