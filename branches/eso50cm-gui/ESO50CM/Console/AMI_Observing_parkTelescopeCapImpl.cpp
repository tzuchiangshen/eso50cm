#include "AMI_Observing_parkTelescopeCapImpl.h"

using namespace std;


AMI_Observing_parkTelescopeCapImpl::AMI_Observing_parkTelescopeCapImpl(QObject *_obj) {
    obj = _obj;
}

void AMI_Observing_parkTelescopeCapImpl::ice_response()
{
    ObsControlIF *obs = (ObsControlIF*)obj;
    obs->telescopeMovementFinished();
}

void AMI_Observing_parkTelescopeCapImpl::ice_exception(const Ice::Exception& ex)
{
    try{
        ex.ice_throw();
    } catch (const Ice::LocalException& e) {
        cerr << "error retriving data: " << e << endl;
    }
}




