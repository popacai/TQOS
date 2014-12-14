#include "bufferpipe.h"

BufferPipe::BufferPipe() {
    queue = new List;
    semaphore = new Semaphore("buffer semaphore", 0);
    lock = new Lock("buffer lock");
}

BufferPipe::~BufferPipe() {
    delete queue;
    delete semaphore;
    delete lock;
}

char BufferPipe::pop() {
    char ch;
    semaphore->P();
    lock->Acquire();
    ch = (char)(int)queue->Remove();
    lock->Release();
    return ch;
}

int BufferPipe::push(char ch) {
    lock->Acquire();
    queue->Append((void*)(int)ch);
    lock->Release();
    semaphore->V();
    return 1;
}

int test_buffer_pipe() {
    BufferPipe* bufferpipe;
    char c[3] = {0};
    bufferpipe = new BufferPipe();

    bufferpipe->push('1');
    bufferpipe->push('2');
    c[0] = bufferpipe->pop();
    bufferpipe->push('3');
    c[1] = bufferpipe->pop();
    c[2] = bufferpipe->pop();

    ASSERT(c[0] = '1');
    ASSERT(c[1] = '2');
    ASSERT(c[2] = '3');
    printf("buffer_test\n");

    delete bufferpipe;
    return 1;
}
