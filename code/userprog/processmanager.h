#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H

class Thread;
class Lock;
#include "synch.h"


class ProcessManager{
public:
    ProcessManager(int size); // Table in description	
    ~ProcessManager();	
    int Alloc(void *object);
    void *Get(int index);
    void Release(int index);
private:
    void **indexArray;
    Lock * lock;
    int arraySize;
};

#endif
