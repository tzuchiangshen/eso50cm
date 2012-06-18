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

class ObsControlIF : public QThread
{
	Q_OBJECT
public:
    ObsControlIF();
    ~ObsControlIF();

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
    void handset_slew(string rate, string direction);


signals: 
    void newData(const int type,  OUC::TelescopeData *data);
    void newEncData(const int type, OUC::RawEncoderData *enc);
private:
    int getPosition();
    char * strfdegs( char * string, size_t max_len, const char * format, double degs );
    double degs2double(char *str);
    Ice::CommunicatorPtr communicator;
    ObservingPrx obs;
    TelescopePrx lcu;
    OUC::TelescopeData *data;
    OUC::RawEncoderData *enc;
    QMutex mutex;
    LoggerHelper logger;
};
#endif 




