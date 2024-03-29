#ifndef CS_CONTAINER_SERVICES_H
#define CS_CONTAINER_SERVICES_H


#include <Ice/Ice.h>
#include "Observing.h"
#include <QThread>
#include <QMutex>
#include <QDebug>
#include <math.h>
#include <string.h>
#include "LoggerHelper.h"


using namespace std;
using namespace OUC;

class TcsGuiController : public QThread
{
	Q_OBJECT
public:
    TcsGuiController();
    ~TcsGuiController();

	void run();

    int connect();
    int disconnect();
	OUC::TelescopePrx getLCUReference();
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
    ObservingPrx obs;
	TelescopePrx lcu;
	OUC::TelescopeData *data;
	QMutex mutex;
    LoggerHelper logger;
};
#endif 




