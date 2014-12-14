#include "syscall.h"

int main()
{
    int i;
    int id;
    char *j[2];
    //j[0] = "arg 1";
    //j[1] = "arg 2";
    j[0] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    j[1] = "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
    id  = Exec("../test/thread_argv", 2, j, 1);
    Yield();
    for (i = 0; i < 1000; i++) {
        Yield();
    }
    Write("main exit\n", 11, ConsoleOutput);

    return 0;
}
