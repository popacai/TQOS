#include "syscall.h"

int a(int level);

int a(int level) {
    if (level < 20) {
        a(level + 1);
    }
}
int main() {
    a(0);
    //Halt();
}
