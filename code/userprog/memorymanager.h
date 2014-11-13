#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include "bitmap.h"

class Thread;
class Lock;
#include "synch.h"


class MemoryManager{
public:
    MemoryManager(int numPages);	
    ~MemoryManager();	
    int AllocPage();
    void FreePage(int physPageNum);
    bool PageIsAllocated(int physPageNum);
private:
    int pageCount;
    BitMap * bitmap;
    Lock *memoryLock;
};

#endif
