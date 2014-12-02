#include "syscall.h"

int main() {
    for (;;) {
        Yield();
    }
    return 0;
}
