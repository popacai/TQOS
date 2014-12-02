#include "syscall.h"

int
main(char argc, char** argv)
{
    int i[5] = {1};
    int j;
    char *arg1 = argv[0];
    char *arg2 = argv[1];
    Write(arg1, 5, ConsoleOutput);
    Write("\n", 2, ConsoleOutput);
    Write(arg2, 5, ConsoleOutput);
    Write("\n", 2, ConsoleOutput);
   
    for(j = 0; j < 500; j++)
        Yield();
    Exit(1);
    /* not reached */
}
