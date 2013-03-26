
#ifndef LCUImpl_I_H
#define LCUImpl_I_H

#include "LCU.h"
#include "myLCU.h"

class LCUImpl : public OUC::LCU
{
public:


    LCUImpl();
    virtual void sayHello(int delay, const Ice::Current&) const;
    virtual void shutdown(const Ice::Current&);
    virtual void getEncodersPosition(const Ice::Current&);

private:
    myLCU *m_lcu; 
    
};

#endif
