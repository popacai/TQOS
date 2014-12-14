#include "syscall.h"

int main()
{
    int i;
    Write("exec1exec\n", 11, ConsoleOutput);
    Exec("../test/thread_yield", 0, 0, 0);
    Yield();
    Write("exec2exec\n", 11, ConsoleOutput);
    Exec("../test/thread_yield", 0, 0, 0);
    Yield();
    Write("exec3exec\n", 11, ConsoleOutput);
    Exec("../test/thread_yield", 0, 0, 0);
    Yield();
    Write("exec4exec\n", 11, ConsoleOutput);
    Exec("../test/thread_yield", 0, 0, 0);
    Yield();
    Write("exec5exec\n", 11, ConsoleOutput);
    Exec("../test/thread_yield", 0, 0, 0);
    Yield();
    Write("exec6exec\n", 11, ConsoleOutput);
    Exec("../test/thread_yield", 0, 0, 0);
    Yield();
    for (i = 0; i < 100; i++) {
        //Write("yield\n", 11, ConsoleOutput);
        Yield();
    }
    Write("main exit\n", 11, ConsoleOutput);

    return 0;
}
