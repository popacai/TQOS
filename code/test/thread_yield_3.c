#include "syscall.h"

int 
main()
{
    int i;
    for (i = 0; i < 10000; i++)
        Yield();
    Write("1",1,ConsoleOutput);
    return 1;
}
