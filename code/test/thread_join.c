#include "syscall.h"

int
main()
{
    int i[5] = {1};
    Join(2);
    Write("joiner exit\n", 13, ConsoleOutput);
    return 0;
    /* not reached */
}
