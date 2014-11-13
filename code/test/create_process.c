#include "syscall.h"

void foo() {
};

int
main()
{
    Fork(foo);
    Halt();
    /* not reached */
}
