#include "syscall.h"

int main() {
    char ch;
    for (;;) {
        Read(&ch, 1, ConsoleInput);
        Write(&ch, 1, ConsoleOutput);
        if (ch == 'q') {
            return 0;
        }
    }

    return 0;
}
