#include "syscall.h"

int main()
{
    int j;
    for (j = 0; j < 2; j++) 
        Yield();
    Write("1",1,ConsoleOutput);
    Exit(3);
    return 3;
}
