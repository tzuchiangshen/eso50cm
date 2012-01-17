#ifndef __MESSAGE_STORAGE_H_
#define __MESSAGE_STORAGE_H_


/**
* DAO Interface for reading/writing messages to any store type *
**/
#include <string>
#include <vector>
#include "LogMessage.h"
#define UNLIMITED_ENTRIES 1000000

using namespace std;
using namespace Log;
// Exception definition
class ConfigureDAOEx { public: string details; };
class SavingMessageDAOEx { public: string details; };
class RetrievingMessageDAOEx { public: string details; };

/**
* class MessageStorageDAO is an interface that defines a DAO to access message storages (like files, DB, etc)
**/

class MessageStorageDAO {
    public:
        // used by the called to initialize the DAO object, return true if the DAO was correctly
        virtual void configureDAO() = 0;    //throws ConfigureDAOEx
        // save a message in the store  
        virtual void saveMessage(LogMessageData &message)=0;   // throws SavingMessageDAOEx
        // get a vector of messages from the time specified by fromTimestamp to toTimestamp (timestamps of SAVING not from the message)  and store it in the the vector provided. The max number of entries can be specified
        // by default is unlimited (ok ,actually is just a big value). Return the timestamp of the last message (useful for continue reading from the last message) 
        virtual double getMessages(vector<LogMessageData> &messagesList,double fromTimestamp,double toTimeStamp,int maxNumOfEntries=UNLIMITED_ENTRIES) = 0;   // throws RetrievingMessageDAOEx
};
#endif
