#include "syscall.h"

int main() {
    int i;
    char ch[10];
    for (;;) {
        Read(ch, 10, ConsoleInput);
        Yield();
        Write(ch, 10, ConsoleOutput);
        for (i = 0; i < 10; i++){
            if (ch[i] == 'q')
                return 0;
        }
    }
}
