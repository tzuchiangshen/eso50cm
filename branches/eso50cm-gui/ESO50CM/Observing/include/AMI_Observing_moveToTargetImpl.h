#ifndef AMI_OBSERVING_MOVETOTARGETIMPL_H
#define AMI_OBSERVING_MOVETOTARGETIMPL_H

#include "Observing.h"
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>

using namespace std;
using namespace OUC;

class AMI_Observing_moveToTargetImpl : public ::OUC::AMI_Observing_moveToTarget {

public:
    TelescopePrx lcu;
    AMI_Observing_moveToTargetImpl(TelescopePrx lcu);
    virtual void ice_response();
    virtual void ice_exception(const Ice::Exception& ex);
};


#endif // AMI_OBSERVING_MOVETOTARGETIMPL_H
