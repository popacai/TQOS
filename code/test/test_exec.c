#include "syscall.h"

int main()
{
    int i;
    Exec("../test/thread_yield", 1, 0, 0);
        Yield();
    Exec("../test/thread_yield", 1, 0, 0);
        Yield();

    for (i = 0; i < 10000; i++){
        //Write("yield\n", 11, ConsoleOutput);
        Yield();
    }
    Write("main exit\n", 11, ConsoleOutput);

    return 0;
}
