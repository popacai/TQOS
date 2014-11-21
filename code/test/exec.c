#include "syscall.h"

int main() {
    int argc = 2;
    char** argv;
    argv[0] = "ab";
    argv[1] = "cd";
    unsigned long invalid_addr;
    char* nonull_addr;
    int a,b,c;
    int d = 1;
    int d_count = 0;
    int aa = 2147483647;
    int bb = 2147483647;
    long cc;
    char* bogus_addr;
    char res;
    void (*foo)(int);
    
    /* 
     case 0: test for excess address space, current space size is based on 
             how large this exec.c test file is
     case 1: test for name run off the end of addr space without a null
     case 2: test file exist or not
     case 3: test divide by zero exception
     case 4: test for arithmatic overflow and underflow
     case 5: test for AddressErrorException (initialize a pointer to a bogus value and try to dereference it)
    */
    int test_num = 11;
    
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
        case 3:
            a = 1;
            c = 0;
            b = a/c;
            break; 
        case 4:
            aa += bb;
            break;
            //Write((char)d_count, 1, ConsoleOutput);
        case 5:
            bogus_addr = 3000;
            res = *bogus_addr;
            break;
        case 6:
            foo = 3000;
            foo(2);
            break;
        case 10:
            invalid_addr = 2000;
            Write((char*)(invalid_addr),1000,1);
            break;
        case 11:
            invalid_addr = 2000;
            Read((char*)(invalid_addr),1000,0);
            break;
        default:
            Exec("/home/linux/ieng6/cs120f/jic023/last/TQOS/code/test/test_exec",argc,argv,0);
            break;
    }
    Halt();
}

