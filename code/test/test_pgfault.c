#include "syscall.h"

int main() {
    int i, j;
    for (i = 0; i < 20; i++) {
        if (i % 4 == 0) {  // yield 5 time
            Exec("../test/thread_yield",0,0,0);
            Yield();
        } else if (i % 4 == 1){  // yield 100 time
            Exec("../test/thread_yield_1",0,0,0);
            Yield();
        }
        else if (i % 4 == 2){  // stack busy
            Exec("../test/recursive_call",0,0,0);
            Yield();
        }
        else {   // large data
            Exec("../test/thread_yield_2",0,0,0);
            Yield();
        }    
    }
    Exec("../test/thread_yield_3",0,0,0);
    Write("1",1,ConsoleOutput);   
 //   Halt();
}
