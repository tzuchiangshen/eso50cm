#ifndef CS_CONTAINER_SERVICES_H
#define CS_CONTAINER_SERVICES_H


#include <Ice/Ice.h>
#include "Observing.h"
//#include "AMI_Observing_moveToTargetImpl.h"
//#include "AMI_Observing_parkTelescopeImpl.h"
//#include "AMI_Observing_parkTelescopeCapImpl.h"
#include <QThread>
#include <QMutex>
#include <QDebug>
#include <math.h>
#include <string.h>
#include "LoggerHelper.h"
#include "Console.h"


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
    void stopTelescope();
    void startTracking();
    void stopTracking();
    void parkTelescope();
    void parkTelescopeCap();
    void gotoTarget();
    void telescopeMovementFinished();

signals: 
    void newData(const int type,  OUC::TelescopeData *data);
    void newEncData(const int type, OUC::RawEncoderData *enc);
    void newTelescopeStatusTriggered(TelescopeStatus status);
    void newObsControlStatusTriggered(ProcessStatus status);
    void newLCUControlStatusTriggered(ProcessStatus status);
    void newTrackingStatusTriggered(TrackingStatus status);

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

class workThread: public QThread
{
    Q_OBJECT
public:

    void setLCU(TelescopePrx lcu) {
        this->lcu = lcu;
    }

private:
    void run();
    TelescopePrx lcu;
    LoggerHelper logger;
};

#endif 




