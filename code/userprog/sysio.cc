#include "syscall.h"
#include "syscommon.h"
#include "system.h"
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

int Read(char *buffer, int size, OpenFileId id) {
    int i;
    int copied_size;
    unsigned char kernel_char;
    if (id == ConsoleOutput) {
        //reading from the buffer char by char
        for (i = 0; i < size; i++) {
            copied_size = u2kmemcpy(&kernel_char, (int)(buffer + i), 1);
            if (copied_size != 0) {
                //TODO: throw exception for system call error
            }
            putchar(kernel_char);
        }
    } else {
        ASSERT(false);
        //TODO: call OS filesys/openfile.h
    }
}

// Buffer is the pointer in User-level memory
void Write(char *buffer, int size, OpenFileId id) {
    int i;
    int copied_size;
    unsigned char kernel_char;
    if (id == ConsoleOutput) {
        //reading from the buffer char by char
        for (i = 0; i < size; i++) {
            copied_size = u2kmemcpy(&kernel_char, (int)(buffer + i), 1);
            if (copied_size != 0) {
                //TODO: throw exception for system call error
            }
            putchar(kernel_char);
        }
    } else {
        ASSERT(false);
        //TODO: call OS filesys/openfile.h
    }
}

