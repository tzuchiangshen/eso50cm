
#ifndef LCU_ICE
#define LCU_ICE

module OUC 
{

interface LCU
{
    ["cpp:const"] idempotent void sayHello(int delay);
    idempotent void shutdown();
    idempotent void getEncodersPosition();
};

};

#endif
