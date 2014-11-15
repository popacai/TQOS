#include "syscall.h"
#include "syscommon.h"
#include <stdio.h>

// This is just a test for the u2kmemcpy not the implementation of the read
int k_read(int buffer, int size, int a) {
    printf("%d\n", buffer);
    unsigned char temp [50] = {0};
    u2kmemcpy(temp, buffer, 4);
    printf("string=%s\n", temp);
    printf("stringlen=%d\n", ustrlen(buffer));
    return 1;
}
