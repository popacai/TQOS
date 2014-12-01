#include "syscall.h"

int main()
{
    int i;
    int invalid_addr = 3000;
    char* nullst = "";
    char** ininvalid;
//    **ininvalid = 3000;
//    char outinvalid[1];
//    outinvalid[0] = &ininvalid;
//    char* outinvalid;
//    outinvalid = &ininvalid[0];
    *ininvalid = 2050;
/*    for (i=0; i<8; i++) {

    *(*ininvalid + i) = 'a';
    }*/

//    ininvalid = 1000;
    Write("exec1exec\n", 11, ConsoleOutput);
    Exec(nullst, 0, 0, 0);
        Yield();
    Write("exec2exec\n", 11, ConsoleOutput);
    Exec("../test/thread_yield", 0, 0, -1);
        Yield();
    Write("exec3exec\n", 11, ConsoleOutput);
    Exec("../test/thread_yield", -1, 0, 0);
        Yield();
    Write("exec4exec\n", 11, ConsoleOutput);
    Exec((char*)(invalid_addr), 0, 0, 0);
        Yield();
    Write("exec5exec\n", 11, ConsoleOutput);
    Exec("../test/thread_yield", 1, (char*)(invalid_addr), 0);
        Yield();
    Write("exec6exec\n", 11, ConsoleOutput);
    Exec("../test/threa", 0, 0, 0);    
        Yield();
    Write("exec7exec\n", 11, ConsoleOutput);
    Exec("../test/thread_yield", 1, ininvalid, 0);    
        Yield();

    for (i = 0; i < 100; i++){
        //Write("yield\n", 11, ConsoleOutput);
        Yield();
    }
    Write("main exit\n", 11, ConsoleOutput);

    return 0;
}
