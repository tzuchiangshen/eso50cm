// **********************************************************************
//
// _LOGGER is the log publishing interface
// *********************************************************************


#ifndef _LOGGER_ICE
#define _LOGGER_ICE

#include <LogMessage.ice>

module Log
{
    /**
     *
     * This loggInterface represents a Logger class.
     *
     **/
    sequence<string> StringsVector;
    interface Logger
    {
            void logMessage(LogMessageData log);
            idempotent void setDiscardLevel(string source, LogLevel level);
            LogLevel getDiscardLevel(string source);
            StringsVector getSources(int fromSource);
    };
};
#endif

