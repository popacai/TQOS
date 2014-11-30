#include "syscall.h"

int main() {
    char* bogus_addr;
    void (*foo)(int);
    char res;
    

    int num = 2;
    switch(num) {
        case 1:
            bogus_addr = 3000;
            res = *bogus_addr;
            break;
        case 2:
            foo = 800;
            foo(2);
            break;
        default:
            Write("no test\n",9,ConsoleOutput);
            break;
    }

    Halt();
}        
    
