#include "syscall.h"
int a[100000];

int main() {
    int i;
    for (i = 0; i < 1000; i++) {
        a [i] = i;
        Yield();
    }

    Write("1",1,ConsoleOutput);
    return 1;
}
