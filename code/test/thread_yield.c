#include "syscall.h"

int
main()
{
    int i[5] = {1};
    Yield();
    return 0;
    /* not reached */
}
