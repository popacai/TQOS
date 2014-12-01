#include "syscall.h"

void foo() {
    //pass
}

int share_value;

int main() {
    char output_char;
    int i;
    share_value = 0;
    Fork(foo);

    share_value++;
    share_value++;



    output_char = share_value + '0';
    Write(&output_char, 1, ConsoleOutput);
    output_char = '\n';
    Write(&output_char, 1, ConsoleOutput);
    for (i = 0; i < 10000; i++) {
        Yield();
    }
    return 0;
}
