#include "syscall.h"

int
main()
{
    int i[5] = {1};
    int j;
    for(j = 0; j < 100; j++)
        Yield();
    //Write("joinee exit\n", 13, ConsoleOutput);
    Write("1",1,ConsoleOutput);
    Exit(5);
    return 5;
    /* not reached */
}
