#include "syscall.h"

int main()
{
    int i;
    Exec("../test/thread_yield", 0, 0, 0);
        Yield();
    Exec("../test/thread_yield", 0, 0, 0);
        Yield();
    Exec("../test/thread_yield", 0, 0, 0);
        Yield();
    Exec("../test/thread_yield", 0, 0, 0);
        Yield();
    Exec("../test/thread_yield", 0, 0, 0);
        Yield();
    Exec("../test/thread_yield", 0, 0, 0);

    for (i = 0; i < 100; i++){
        //Write("yield\n", 11, ConsoleOutput);
        Yield();
    }
    Write("main exit\n", 11, ConsoleOutput);

    return 0;
}
