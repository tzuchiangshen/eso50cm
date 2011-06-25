#ifndef CS_CONTAINER_SERVICES_H
#define CS_CONTAINER_SERVICES_H


#include <Ice/Ice.h>
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
	void formatRAPosition(double, char *buffer, int maxlen);
	void formatDecPosition(double, char *buffer, int maxlen);
	int setTargetPositionRA(const char* ra);
	int setTargetPositionDec(const char* arguments);


signals: 
	void newData(const int type,  OUC::TelescopeData *data);
private:
    int getPosition();
	char * strfdegs( char * string, size_t max_len, const char * format, double degs );
	double degs2double(char *str);
    Ice::CommunicatorPtr communicator;
    LCUPrx lcu;
	OUC::TelescopeData *data;
	QMutex mutex;
};
#endif 




