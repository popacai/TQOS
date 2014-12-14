#include "syscall.h"

int
main()
{
    int i[5] = {1};
    int k = Join(2);
    if(k == 5) {
        Write("join return 5\n", 15, ConsoleOutput);
    }
    Write("joiner exit\n", 13, ConsoleOutput);
    Exit(3);
    return 3;
    /* not reached */
}
