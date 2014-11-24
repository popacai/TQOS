#include "syscall.h"

int main() {
    int i;
    Exec("../test/test_pipe_read", 0, 0, 0x2);
    Exec("../test/test_pipe_write", 0, 0, 0x4);
    for (i = 0; i < 10000; i++) {
        Yield();
    }
    return 0;
}
