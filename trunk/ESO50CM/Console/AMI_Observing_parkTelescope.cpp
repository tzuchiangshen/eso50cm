#include "AMI_Observing_parkTelescopeImpl.h"

using namespace std;


AMI_Observing_parkTelescopeImpl::AMI_Observing_parkTelescopeImpl(QObject *_obj) {
    obj = _obj;
}

void AMI_Observing_parkTelescopeImpl::ice_response()
{
    ObsControlIF *obs = (ObsControlIF*)obj;
    obs->telescopeMovementFinished();
}

void AMI_Observing_parkTelescopeImpl::ice_exception(const Ice::Exception& ex)
{
    try{
        ex.ice_throw();
    } catch (const Ice::LocalException& e) {
        cerr << "error retriving data: " << e << endl;
    }
}




