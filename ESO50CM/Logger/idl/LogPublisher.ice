// **********************************************************************
//
// _LOGGER is the log publishing interface
// *********************************************************************


#ifndef _LOG_CONSUMER_ICE
#define _LOG_CONSUMER_ICE

#include <LogMessage.ice>

module Log
{
    /**
     *
     * This class interface should be used by the clients to retrieve the log on real time
     *
     **/
    interface LogPublisher
    {
            void logEvent(LogMessageData log);     
    };
};
#endif

