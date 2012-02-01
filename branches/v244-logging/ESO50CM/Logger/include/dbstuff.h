#ifndef __DBSTUFF_H__
#define __DBSTUFF_H__ 
/* Standard C++ headers */
#include <iostream>
#include <sstream>
#include <memory>
#include <string>
#include <stdexcept>

/* MySQL Connector/C++ specific headers */

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>


#define DBHOST "tcp://127.0.0.1:3306"
#define USER "logmgr"
#define PASSWORD "logpass"
#define DATABASE "logs"
// Link to the Connector/C++ library
#pragma comment(lib, "mysqlcppconn.lib")
using namespace std;
using namespace sql;

class MySQLCon {
    Driver *driver;
    Connection *con;
    // Prepared statements for queries
    PreparedStatement *stmt_getSourceID;
    PreparedStatement *stmt_insertSource;
    PreparedStatement *stmt_getLogIdFromTimestamp;
    PreparedStatement *stmt_getLogsFromToLogId;
    PreparedStatement *stmt_setDiscardLevel;
    PreparedStatement *stmt_getDiscardLevel;
    PreparedStatement *stmt_getSourceDesc;
    PreparedStatement *stmt_insertLogMessage; 
  public:
    // Queries
    int getSourceID(string source);
    string getSourceDesc(int sourceId);
    int getDiscardLevel(string source);
    void setDiscardLevel(string source,int level);   
    ResultSet *queryDB(string query);
    ~MySQLCon();
    bool isConnected;
    MySQLCon() {isConnected=false;driver=NULL;con=NULL;
    };
    bool connect(string url,string user,string pass);
    void logMessage(int level, string source,double timestamp,string method,int linenumber,string data);
      
};
#endif