#include "syscall.h"

int main() {
    char cmd[50];
    char ch = 1;
    int i = 0;
    Write("$ ", 3, ConsoleOutput);
    for (;;) {
        Read(&ch, 1, ConsoleInput);
        if (ch == '\n') {
            ch = '\0';
            cmd[i++] = ch;
            Write(cmd, i, ConsoleOutput);
            Exec(cmd, 1, 0, 0);
            i = 0;
            Write("$ ", 3, ConsoleOutput);
        } else {
            cmd[i++] = ch;
        }
    }
    Halt();
    return 0;
}
