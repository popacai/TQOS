#ifndef KSYSCALL_H
#define KSYSCALL_H

#include "system.h"
#include "syscall.h"

// CAUTION: This is implementation of all system call functions
int kread(char *buffer, int size, OpenFileId id);
int kwrite(char *buffer, int size, OpenFileId id);

//thread fork
int kfork(int func_ptr);

//kexec, kexit, kfork, kjoin
void kexec();

void kjoin();

#endif
