#include "syscall.h"

int main()
{
    int j;
    for (j = 0; j < 10; j++) 
        Yield();
    Exit(3);
    return 3;
}
