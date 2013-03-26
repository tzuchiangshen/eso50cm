#ifndef AMI_OBSERVING_PARKTELESCOPECAPIMPL_H
#define AMI_OBSERVING_PARKTELESCOPECAPIMPL_H

#include "Observing.h"
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <QObject>
#include <ObsControlIF.h>

using namespace std;
using namespace OUC;

class AMI_Observing_parkTelescopeCapImpl : public ::OUC::AMI_Observing_parkTelescopeCap {

public:
    QObject *obj;
    AMI_Observing_parkTelescopeCapImpl(QObject *obj);
    virtual void ice_response();
    virtual void ice_exception(const Ice::Exception& ex);
};


#endif // AMI_OBSERVING_PARKTELESCOPECAPIMPL_H
