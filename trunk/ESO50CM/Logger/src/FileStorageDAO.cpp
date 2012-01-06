#include "FileStorageDAO.h"
#include "FakeConfigurationsStorage.h"
#include <time.h>
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;
/**
* FileStorage implements the MessageStorageDAO interface. It uses files as storage of messages
**/


string FileStorage::getCurrentTime()
{
  time_t rawtime;
  struct tm * timeinfo;
  char buffer [80];
  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  strftime (buffer,80,"%Y-%m-%d_%H.%M.%S",timeinfo);
  return string(buffer);
}
string timestampToString(double timestamp)
{
  
  time_t rawtime=(time_t)timestamp;
  struct tm * timeinfo;
  char buffer [80];
  timeinfo = localtime ( &rawtime );
  strftime (buffer,80,"%Y-%m-%dT%H:%M:%S",timeinfo); 
  stringstream ss (stringstream::in | stringstream::out);
  ss << buffer << "." << setfill('0') << setw(3) <<  (timestamp-rawtime)*1000;
  return ss.str();
  
}

void FileStorage::renewLogFile()
{
   if(file.is_open())
      file.close();
    string currTimeStr=getCurrentTime();
    try {
        file.open((logsDirectory+"/"+currTimeStr+".log").c_str());
    } catch (...) {
        ConfigureDAOEx ex;
        ex.details="";
        ex.details=ex.details+"There was an error opening "+logsDirectory+"/"+currTimeStr+".log";
        throw ex;
    }
    if (!file.is_open())
    {
        ConfigureDAOEx ex;
        ex.details="";
        ex.details=ex.details+"There was an error opening "+logsDirectory+"/"+currTimeStr+".log";
        throw ex;
    }

}
           
void FileStorage::configureDAO()
{
    levelDesc[Log::FINEST]="FINEST";
    levelDesc[Log::FINER]="FINER";
    levelDesc[Log::FINE]="FINE";
    levelDesc[Log::CONFIG]="CONFIG";
    levelDesc[Log::INFO]="INFO";
    levelDesc[Log::WARNING]="WARNING";
    levelDesc[Log::SEVERE]="SEVERE";
    levelDesc[9]="Unknown log level";
    try {
        Configurations confs("LoggingService.ini");
        MAX_HOURS=confs.getIntValue("FileStorage","MAX_HOURS");
        MAX_SIZE=confs.getIntValue("FileStorage","MAX_SIZE");
        logsDirectory=confs.getStringValue("FileStorage","LOGS_DIRECTORY");
    } catch (...) {
        ConfigureDAOEx ex;
        ex.details="Error reading FileStorage configuration";
        throw ex;
    }
    time(&currentTime);
    renewLogFile(); 
}

double FileStorage::getMessage(vector<LogMessageData> &messagesList,double fromTimestamp,double toTimeStamp,int maxNumOfEntries) {return 0;};
// TODO: implement this method

void FileStorage::saveMessage(LogMessageData &message)
{
    time_t new_time;
    time(&new_time);
    if (difftime(new_time,currentTime)/3600 >  MAX_HOURS)
    {
	currentTime=new_time;
        renewLogFile();
    }
    // | logtimestamp | timestamp | log level | source | method | lineNumber |  data |
    file << "| " <<  timestampToString(message.logtimestamp) << " | "  <<  timestampToString(message.timestamp) << " | " <<  levelDesc[message.level] << " | " << 
         message.source << " | " << message.method  << " | " << message.lineNumber << " |  " << message.data << " |" << endl;
    
}
