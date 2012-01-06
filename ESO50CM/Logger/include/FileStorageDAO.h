#ifndef __FILE_STORAGE_H_
#define __FILE_STORAGE_H_


#include "MessageStorageDAO.h"
#include <sys/time.h>
#include <string>
#include <iostream>
#include <fstream>
/**
* FileStorage implements the MessageStorageDAO interface. It uses files as storage of messages
**/
class FileStorage:public MessageStorageDAO {
        time_t currentTime;
        string getCurrentTime();
        void renewLogFile();

        int MAX_HOURS;  // if currentime-now > MAX_HOURS, we create a new file
        int MAX_SIZE;   // if size of current file > MAX_SIZE (in MB), we create a new file
        string logsDirectory;
        ofstream file;
        string levelDesc[10]; 
    public:        
        // used by the called to initialize the DAO object, return true if the DAO was correctly configured
        void configureDAO();
        // save a message in the store
        void saveMessage(LogMessageData &message);
        // get a vector of messages from the time specified by fromTimestamp to toTimestamp (timestamps of SAVING not from the message)  and store it in the the vector provided. The max number of entries can be specified
        // by default is unlimited (ok ,actually is just a big value). Return the timestamp of the last message (useful for continue reading from the last message) 
        double getMessage(vector<LogMessageData> &messagesList,double fromTimestamp,double toTimeStamp,int maxNumOfEntries=UNLIMITED_ENTRIES);
        FileStorage() {};
};
#endif
