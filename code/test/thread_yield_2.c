#include "syscall.h"
int a[100000];

int main() {
    int i;
    for (i = 0; i < 100000; i++) {
        a [i] = i;
    }

    Write("1",1,ConsoleOutput);
    return 1;
}
