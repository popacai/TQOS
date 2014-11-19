#include "syscall.h"

int main() {
    char a; 
    Read(&a, 1, ConsoleInput);
    Write(&a, 1, ConsoleOutput);
    Halt();
    return 0;
}
