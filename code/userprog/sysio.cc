#include "syscall.h"
#include "syscommon.h"

// This is just a test for the u2kmemcpy not the implementation of the read
int k_read(char* buffer, int size, OpenFileId id) {
    char temp [50];
    u2kmemcpy(temp, buffer, 4);
    printf("%s\n", temp);
}
