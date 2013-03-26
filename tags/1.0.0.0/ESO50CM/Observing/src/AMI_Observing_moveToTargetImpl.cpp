#include "AMI_Observing_moveToTargetImpl.h"

using namespace std;


AMI_Observing_moveToTargetImpl::AMI_Observing_moveToTargetImpl(TelescopePrx _lcu) {
    lcu = _lcu;
}

void AMI_Observing_moveToTargetImpl::ice_response()
{
    //

}

void AMI_Observing_moveToTargetImpl::ice_exception(const Ice::Exception& ex)
{
    try{
        ex.ice_throw();
    } catch (const Ice::LocalException& e) {
        cerr << "error retriving data: " << e << endl;
    }
}




