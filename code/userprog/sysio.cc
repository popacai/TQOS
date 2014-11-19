#include "ksyscall.h"
#include "syscall.h"
#include "syscommon.h"
#include "system.h"
#include <stdio.h>
#include "synchconsole.h"

// This is just a test for the u2kmemcpy not the implementation of the read

int kread(char *buffer, int size, OpenFileId id) {
    int i;
    unsigned char kernel_char;
    SynchConsole* synchconsole = new SynchConsole(0, 0);
    if (id == ConsoleInput) {
        //reading from the buffer char by char
        for (i = 0; i < size; i++) {
            kernel_char = synchconsole->GetChar();
            k2umemcpy((int)(buffer + i), &kernel_char, 1);
        }
    } else {
        ASSERT(false);
        //TODO: call OS filesys/openfile.h
    }
    delete synchconsole;
    return size;
}

// Buffer is the pointer in User-level memory
void kwrite(char *buffer, int size, OpenFileId id) {
    int i;
    int copied_size;
    unsigned char kernel_char;
    SynchConsole* synchconsole = new SynchConsole(0, 0);
    if (id == ConsoleOutput) {
        //reading from the buffer char by char
        for (i = 0; i < size; i++) {
            copied_size = u2kmemcpy(&kernel_char, (int)(buffer + i), 1);
            if (copied_size != 1) {
                ASSERT(false);
            }
            synchconsole->PutChar(kernel_char);
        }
    } else {
        ASSERT(false);
        //TODO: call OS filesys/openfile.h
    }
    delete synchconsole;
}

