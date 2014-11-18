#include "syscall.h"

int main() {
    int argc = 2;
    char** argv;
    argv[0] = "ab";
    argv[1] = "cd";
    unsigned long invalid_addr;
    char* nonull_addr;
    
    /* 
     case 0: test for excess address space, current space size is based on 
             how large this exec.c test file is
     case 1: test for name run off the end of addr space without a null
     case 2: test file exist or not
    */
    int test_num = 2;
    
    switch(test_num) {
        case 0:
            invalid_addr = 2000;
            Exec((char*)(invalid_addr),argc,argv,0);
            break;
        case 1:
            nonull_addr = 1662; // current addr space size = nonull_addr + 2
            *nonull_addr = 'a';
            *(nonull_addr + 1) = 'b';
            Exec((char*)(nonull_addr),argc,argv,0);
            break;
        case 2:
            Exec("/home/linux/ieng6/cs120f/jic023/last/TQOS/code/test/no",argc,argv,0);
            break;
        default:
            Exec("/home/linux/ieng6/cs120f/jic023/last/TQOS/code/test/test_exec",argc,argv,0);
            break;
    }
    Halt();
}

