
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
      myLoggerTest(string source):logger(source)  {  counter=0; };
      void someMethod(int numlogs) { 
              char message[1000];
              for (int i=0;i<numlogs;i++)
              {
                  sprintf(message,"This is a test %i . Adding extra data to make it more interesting.  Asdafasdaf 10004032040320320 llllll 111111111111222ssss    ccccddd e",counter++);
                  logger.logWARNING(message);

                  logger.logFINEST("This is a very good %i %s %itest",10,"hola",10);
                  logger.logINFO(message);
		  string teststr="this is a tesssst";
		  
                  logger.logSEVERE(teststr);
		  logger.logSEVERE( __LINE__,__PRETTY_FUNCTION__,"test %i",10);
                  logger.logFINEST("THis is the finest message EVER");
                  logger.logFINE("fine!");
              }
    }
};


int main(int argc, char* argv[]) {
    myLoggerTest log1=myLoggerTest("device1");
    myLoggerTest log2=myLoggerTest("device2");
    myLoggerTest log3=myLoggerTest("device3");

    printf("\nargc=%i\n",argc);
    if (argc>=2)
    {
        log1.someMethod(atoi(argv[1])); //
        log2.someMethod(atoi(argv[1])); //
        log3.someMethod(atoi(argv[1])); //
    }
	return 0;
}
#endif
