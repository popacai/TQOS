#include "syscall.h"

int has_q_inside(char* ch, int size) {
    int i;
    for (i = 0; i < size; i++) {
        if (ch[i++] == 'q')
            return 1;
    }
    return 0;
}

int main() {
    int i;
    char ch[30];
    int return_value;
    for (;;) {
        Read(ch, 3, ConsoleInput);
        return_value = Write(ch, 3, ConsoleOutput);
        if (return_value == 3)
            if (has_q_inside(ch, 3)) {
                return 0;
            };

        return_value = Read(ch, 5, ConsoleInput);
        if (return_value == 5)
            Write(ch, 5, ConsoleOutput);
        if (has_q_inside(ch, 5)) {
            return 0;
        };

        return_value = Read(ch, 2, ConsoleInput);
        if (return_value == 2)
            Write(ch, 2, ConsoleOutput);
        if (has_q_inside(ch, 2)) {
            return 0;
        };
    }

    return 0;
}
