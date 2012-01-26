#ifndef CS_CONTAINER_SERVICES_H
#define CS_CONTAINER_SERVICES_H


#include <IceE/IceE.h>
#include "LCU.h"
#include <QThread>
#include <QMutex>
#include <QDebug>
#include <math.h>
#include <string.h>


using namespace std;
using namespace OUC;

class cppContainerServices : public QThread
{
	Q_OBJECT
public:
    cppContainerServices();
    ~cppContainerServices();

	void run();

    int connect();
    int disconnect();
	LCUPrx getLCUReference();
	void test();
	OUC::TelescopeData getTelescopeData();
	void getCurrentPositionRA(char *buffer, int maxlen);
	void getCurrentPositionDec(char *buffer, int maxlen);


signals: 
	void newData( const QString &data);
private:
    int getPosition();
	char * strfdegs( char * string, size_t max_len, const char * format, double degs );
    Ice::CommunicatorPtr communicator;
    LCUPrx lcu;
	OUC::TelescopeData *data;
	QMutex mutex;
};
#endif 




