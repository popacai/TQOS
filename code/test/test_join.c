#include "syscall.h"

int main()
{
    int i;
    int id;
    id  = Exec("../test/thread_yield", 0, 0, 1);
        Yield();
    Exec("../test/thread_join", 0, 0, 0);
        Yield();
    for (i = 0; i < 1000; i++){
        Yield();
    }
    Write("main exit\n", 11, ConsoleOutput);

    return 0;
}
