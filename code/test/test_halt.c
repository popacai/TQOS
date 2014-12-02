#include "syscall.h"

int main() {
    Exec("../test/test_read_write", 0, 0, 0);
    Exit(0);
}
