#include "syscall.h"


int main() {
    char string[30] = "reader___:____________________";
    int i;
    for (i = 0; i < 3; i++) {
        Read(string + 10, 20, ConsoleInput);
        Write(string, 30, ConsoleOutput);
    }
    return 0;
}
