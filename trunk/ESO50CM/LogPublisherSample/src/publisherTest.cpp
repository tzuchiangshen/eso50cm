
#ifndef ____client_cpp__
#define ____client_cpp__

#include "LoggerHelper.h"
#include <stdlib.h>
#include <cstdlib>
#include <string.h>

//using namespace Log;
using namespace std;

class myLoggerTest{
private:
    LoggerHelper logger; 
    int counter;
public:
      myLoggerTest():logger("SOURCE")  {  counter=0; };
      void someMethod(int numlogs) { 
              char message[200];
              for (int i=0;i<numlogs;i++)
              {
                  sprintf(message,"This is a test %i",counter++);
                  logger.logWARNING(message);
              }
    }
};


int main(int argc, char* argv[]) {
    myLoggerTest logx=myLoggerTest();
    
    if (argc>=2)
        logx.someMethod(atoi(argv[1])); //
	return 0;
}
#endif
