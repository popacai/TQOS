#include "syscall.h"

int 
main()
{
    int i;
    for (i = 0; i < 100000; i++)
        Yield();
    Write("\n",1,ConsoleOutput);
    return 1;
}
