#include "syscall.h"

int main() {
    char* a;
    a = "123123123\n";
    Write(a, 10, ConsoleOutput);
    return 0;
}
