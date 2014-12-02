#include "syscall.h"

int share_value;

void foo() {
    int i;
    share_value++;
    for (i = 0; i < 10000; i++) {
        Yield();
    }
    return;
}

int main() {
    char output_char;
    int i;
    int return_value;
    share_value = 0;
    Exec("../test/dead_loop", 0, 0, 0);
    Fork(foo);
    Fork(foo);
    Fork(foo);
    Fork(foo);
    /*
    Fork(foo);
    Fork(foo);
    Fork(foo);
    Fork(foo);
    Fork(foo);
    Fork(foo);
    Fork(foo);
    Fork(foo);
    Fork(foo);
    Fork(foo);
    Fork(foo);
    Fork(foo);
    Fork(foo);
    Fork(foo);
    Fork(foo);
    return_value = Fork(foo);
    output_char = return_value + '1';
    Write(&output_char, 1, ConsoleOutput);
    output_char = '\n';
    Write(&output_char, 1, ConsoleOutput);

    for (i = 0; i < 1000; i++) {
        Yield();
    }
    */
    for (i = 0; i < 1000; i++) {
        Yield();
    }


    output_char = share_value + '0';
    Write(&output_char, 1, ConsoleOutput);
    output_char = '\n';
    Write(&output_char, 1, ConsoleOutput);
    for (i = 0; i < 10000; i++) {
        Yield();
    }
    return 0;
}
