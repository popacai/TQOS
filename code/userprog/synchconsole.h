#ifndef SYNCHCONSOLE_H
#define SYNCHCONSOLE_H

#include "console.h"
#include "synch.h"

class SynchConsole {
public:
    SynchConsole(char* input, char* output);
    ~SynchConsole();

    char GetChar();
    void PutChar(char);
    void ReadDone();
    void WriteDone();

private:
    Console* console;
    Semaphore *read_semaphore;
    Lock *read_lock;
    Semaphore *write_semaphore;
    Lock *write_lock;
};

void test_synchconsole();

#endif

