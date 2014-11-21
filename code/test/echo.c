#include "syscall.h"

int main() {
    char a[10]; 
    while (1)
    {
        Read(a, 10, ConsoleInput);
        Write(a, 10, ConsoleOutput);
        if (a[0] == 'q')
            break;
    }
    Halt();
    return 0;
}
