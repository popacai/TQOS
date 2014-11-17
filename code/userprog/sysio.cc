#include "syscall.h"
#include "syscommon.h"
#include <stdio.h>

// This is just a test for the u2kmemcpy not the implementation of the read
int k_read(int buffer, int size, int a) {
    int len;
    //unsigned char temp [50] = {0};
    len = ustrlen(buffer);
    unsigned char* temp = new unsigned char[len];
    u2kmemcpy(temp, buffer, len);
    printf("str=%s\n",temp);
    printf("stringlen=%d\n", ustrlen(buffer));
    return 1;
}
