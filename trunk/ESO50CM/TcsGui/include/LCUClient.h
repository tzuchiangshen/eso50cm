#ifndef LCUClient_H
#define LCUClient_H


#include <IceE/IceE.h>
#include "LCU.h"
#include <QThread>

using namespace std;
using namespace OUC;

class LCUClient: public QThread
{
    Q_OBJECT
public:
    LCUClient();
    ~LCUClient();

    int connect();
    int disconnect();
    int getPosition();

public signals: 
    void newData(const QString &data);
private:
    Ice::CommunicatorPtr communicator;
    LCUPrx lcu;
};

#endif 




