#include "syscall.h"

/*int a_20(int level);
int a_50(int level);

int a_20(int level) {
    if (level < 20) {
        Exec("../test/thread_yield",0,0,0);
        Yield();
        a_20(level + 1);
    }
}

int a_50(int level) {
    if (level < 50) {
        Exec("../test/thread_complete",0,0,0);
        a_50(level + 1);
    }
}*/

int main() {
    int i;
    //a_20(0);
    //a_50(0);
    for (i = 0; i < 100; i++) {
        if (i % 3 == 0) {
            // run long time
            Exec("../test/thread_yield",0,0,0);
            Yield();
        } else if(i % 3 == 1){
            // run short time 
            Exec("../test/thread_yield_1",0,0,0);
            Yield();
        }
        else {
            Exec("../test/sort",0,0,0);
            Yield();
        }    
    }
    
    Halt();
}    
