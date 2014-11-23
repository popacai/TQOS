#include "system.h"
#include "synch.h"

#ifndef BUFFERPIPE_H
#define BUFFERPIPE_H

class BufferPipe {
public:
    BufferPipe();
    ~BufferPipe();

    char pop();
    int push(char);
private:
    List* queue;
    Semaphore* semaphore;
    Lock* lock;
};

int test_buffer_pipe();
#endif
