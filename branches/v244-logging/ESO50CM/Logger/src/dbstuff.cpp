#include "dbstuff.h"

bool  MySQLCon::connect(string url,string user,string pass)
{
    try {
        driver = get_driver_instance();        
        /* create a database connection using the Driver */
        con = driver -> connect(url, user, pass);
        
        /* alternate syntax using auto_ptr to create the db connection */
        //auto_ptr  con (driver -> connect(url, user, password));
        /* turn off the autocommit */
        //con -> setAutoCommit(0);
        //cout << "\nDatabase connection\'s autocommit mode = " << con -> getAutoCommit() << endl;
        
        /* select appropriate database schema */
        con -> setSchema("logs");
        isConnected=true;
        stmt_getSourceID=con -> prepareStatement ("select SOURCE_ID  from LOG_SOURCES where SOURCE_DESCRIPTION = lower(?)");
        stmt_insertSource=con -> prepareStatement ("INSERT INTO LOG_SOURCES (SOURCE_DESCRIPTION) VALUES (lower(?))");
        stmt_getSourceDesc=con -> prepareStatement ("select SOURCE_DESCRIPTION from LOG_SOURCES where SOURCE_ID = ?");
        //stmt_getLogIdFromTimestamp;
        //stmt_getLogsFromToLogId;
        stmt_setDiscardLevel=con -> prepareStatement("update LOG_SOURCES  SET DISCARD_LEVEL = ? where SOURCE_ID = ? ");
        stmt_getDiscardLevel=con -> prepareStatement("select DISCARD_LEVEL from LOG_SOURCES where SOURCE_ID = ?");
        stmt_insertLogMessage=con -> prepareStatement ("insert into LOGS(LEVEL,SOURCE_ID,TIMESTAMP,METHOD,LINE_NUMBER,DATA) values (?,?,?,?,?,?)");
        return true;
        
    } catch (SQLException &e) {
        cout << "ERROR: SQLException in " << __FILE__;
        cout << " (" << __func__<< ") on line " << __LINE__ << endl;
        cout << "ERROR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << ")" << endl;
        
        if (e.getErrorCode() == 1047) {
            /*
             *            Error: 1047 SQLSTATE: 08S01 (ER_UNKNOWN_COM_ERROR)
             *            Message: Unknown command
             */
            cout << "\nYour server does not seem to support Prepared Statements at all. ";
            cout << "Perhaps MYSQL < 4.1?" << endl;
        }     
        return false;
    } catch (std::runtime_error &e) {
        
        cout << "ERROR: runtime_error in " << __FILE__;
        cout << " (" << __func__ << ") on line " << __LINE__ << endl;
        cout << "ERROR: " << e.what() << endl;
        return false;
    }
    
}

MySQLCon::~MySQLCon()
{
    if (con!=NULL)
    {
        con -> close();
        delete con;
    }
}
    
    
    
ResultSet *MySQLCon::queryDB(string query)
{
    Statement *stmt;
    ResultSet *rs=NULL;
    try {
        /* create a statement object */
        stmt = con -> createStatement();
        cout << "Executing the Query: \""+query+"\" .." << endl;
        /* run a query which returns exactly one result set */
        rs = stmt -> executeQuery (query);
        return rs;
    } catch (SQLException &e) {
        cout << "ERROR: SQLException in " << __FILE__;
        cout << " (" << __func__<< ") on line " << __LINE__ << endl;
        cout << "ERROR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << ")" << endl;
        
        if (e.getErrorCode() == 1047) {
            /*
             *            Error: 1047 SQLSTATE: 08S01 (ER_UNKNOWN_COM_ERROR)
             *            Message: Unknown command
             */
            cout << "\nYour server does not seem to support Prepared Statements at all. ";
            cout << "Perhaps MYSQL < 4.1?" << endl;
        }
        return NULL;
    } catch (std::runtime_error &e) {
        cout << "ERROR: runtime_error in " << __FILE__;
        cout << " (" << __func__ << ") on line " << __LINE__ << endl;
        cout << "ERROR: " << e.what() << endl;
        return NULL;
    }  
}

int MySQLCon::getSourceID(string source)
{
    ResultSet *rs=NULL;
    stmt_getSourceID->setString(1, source);
    rs=stmt_getSourceID->executeQuery();
    if (rs -> rowsCount() == 0) {
        // if the source is new, we just add it
        stmt_insertSource->setString(1,source);
        stmt_insertSource->executeQuery();
        rs=stmt_getSourceID->executeQuery();
    }
    rs->next();
    int sourceId=rs->getInt(1);
//    cout << "the source id is" << sourceId << endl;
    return sourceId;
}
string MySQLCon::getSourceDesc(int sourceId)
{
    string sourceDesc="";
    ResultSet *rs=NULL;
    stmt_getSourceDesc->setInt(1, sourceId);
    rs=stmt_getSourceID->executeQuery();
    if (rs -> rowsCount() != 0) {
        rs->next();
        sourceDesc=rs->getString(1);
    }
    cout << "the source desc is" << sourceDesc << endl;
    return sourceDesc;    
}

void MySQLCon::logMessage(int level, string source,double timestamp,string method,int linenumber,string data)
{
    int source_id;
    source_id=getSourceID(source);
    stmt_insertLogMessage->setInt(1,level);
    stmt_insertLogMessage->setInt(2,source_id);
    stmt_insertLogMessage->setDouble(3,timestamp);
    stmt_insertLogMessage->setString(4,method);
    stmt_insertLogMessage->setInt(5,linenumber);
    stmt_insertLogMessage->setString(6,data);
    stmt_insertLogMessage->executeQuery();
    return;
}

void MySQLCon::setDiscardLevel(string source,int level)
{
    int sourceId=getSourceID(source);
    stmt_setDiscardLevel->setInt(1,level);
    stmt_setDiscardLevel->setInt(2,sourceId);
    stmt_setDiscardLevel->executeQuery();
    return;
}

int MySQLCon::getDiscardLevel(string source)
{
    int sourceId=getSourceID(source);
    int discardLevel=-1;
    ResultSet *rs=NULL;
    stmt_getDiscardLevel->setInt(1, sourceId);
    rs=stmt_getDiscardLevel->executeQuery();
    if (rs -> rowsCount() != 0) {
        rs->next();
        discardLevel=rs->getInt(1);
    }
    return discardLevel;    
}