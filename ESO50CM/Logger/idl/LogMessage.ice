// **********************************************************************
//
// _LOGGER_MESSAGE define the log mesage and some other
// associated definitions
// **********************************************************************

#ifndef _LOGGER_MESSAGE_ICE
#define _LOGGER_MESSAGE_ICE

module Log
{

    /**
     *
     * This exception is raised in the case of a  failure.
     *
     **/
    exception LoggerException
    {
        string message;
    };
    

    /**
     *
     * Log Levels (corresponding to the standart java log levels
     *
     **/
    enum LogLevel { FINEST, FINER, FINE, CONFIG, INFO, WARNING, SEVERE };
   
    /**
     *
     * Log message
     *
     **/

    struct LogMessageData
    {
        LogLevel  level;
        double    timestamp;
        string    source;
        string    data;
        string    message;
        string    method;
        int       lineNumber;
    };

    sequence<LogMessageData> LogsVector;
    
    struct LogMessageDataSet 
    {
        LogsVector logs;
        int firstIndex;
        int lastIndex;
        double startTime;
        double endTime;
    };
    
};

#endif
