#include "memorymanager.h"
#include "bitmap.h"

MemoryManager::MemoryManager(int numPages) 
{
    this->pageCount = numPages;
    this->bitmap = new BitMap(numPages);
    this->memoryLock = new Lock("memory lock");
}

MemoryManager::~MemoryManager()
{
    delete bitmap;
    delete memoryLock;
}

int 
MemoryManager::AllocPage()
{
    memoryLock->Acquire();
    int pagenum = this->bitmap->Find(); 
    memoryLock->Release();
    return pagenum;
}


void
MemoryManager::FreePage(int physPageNum)
{
    memoryLock->Acquire();
    this->bitmap->Clear(physPageNum);
    memoryLock->Release();
}

bool 
MemoryManager::PageIsAllocated(int physPageNum)
{
    memoryLock->Acquire();
    bool used = this->bitmap->Test(physPageNum);
    memoryLock->Release();
    return used;
}
