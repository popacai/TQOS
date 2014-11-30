#ifndef KSYSCALL_H
#define KSYSCALL_H

#include "system.h"
#include "syscall.h"

// CAUTION: This is implementation of all system call functions
int kread(char *buffer, int size, OpenFileId id);
void kwrite(char *buffer, int size, OpenFileId id);

//kexec, kexit, kfork, kjoin
void kexec();
void kfork(int arg1);

#endif
