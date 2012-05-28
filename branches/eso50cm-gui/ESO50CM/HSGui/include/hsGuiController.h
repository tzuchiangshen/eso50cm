#ifndef HS_GUI_CONTROLLER_H
#define HS_GUI_CONTROLLER_H


#include <Ice/Ice.h>
#include "Observing.h"
#include <QThread>
#include <QMutex>
#include <QDebug>
#include <math.h>
#include <string.h>


using namespace std;
using namespace OUC;

class hsGuiController : public QThread
{
	Q_OBJECT
public:
    hsGuiController();
    ~hsGuiController();

	void run();
    int connect();
    int disconnect();
	OUC::TelescopePrx getLCUReference();
    void handset_slew(string rate, string direction);
signals: 
private:
    Ice::CommunicatorPtr communicator;
    ObservingPrx obs;
	TelescopePrx lcu;
};
#endif 




