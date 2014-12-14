#include "memorymanager.h"
#include "system.h"
#include "bitmap.h"

MemoryManager::MemoryManager(int numPages)
{
    this->freePageCount = numPages;
    this->bitmap = new BitMap(numPages);
    this->memoryLock = new Lock("memory lock");
}

MemoryManager::~MemoryManager()
{
    delete bitmap;
    delete memoryLock;
}

int
MemoryManager::AllocPage(int flag)
{
    int pagenum;
    if(flag) {
        memoryLock->Acquire();
        pagenum = this->bitmap->Find();
        memoryLock->Release();
    }
    else {
        pagenum = this->bitmap->Find();
    }
    if(pagenum != -1) {
        freePageCount--;
    }
    return pagenum;
}


void
MemoryManager::FreePage(int physPageNum)
{
    memoryLock->Acquire();
    if(this->bitmap->Test(physPageNum)) {
        freePageCount++;
    }
    this->bitmap->Clear(physPageNum);
    memoryLock->Release();
}

int
MemoryManager::GetFreePageCount()
{
    return this->freePageCount;
}

bool
MemoryManager::PageIsAllocated(int physPageNum)
{
    memoryLock->Acquire();
    bool used = this->bitmap->Test(physPageNum);
    memoryLock->Release();
    return used;
}

void
MemoryManager::RandomInitializationTest()
{
    // randomly disable 30 pages
    int i;
    int j;
    for(i=0; i<30; i++) {
        j = ((i+2) * 2) % 128;
        // Test and Mark
        if(!this->bitmap->Test(j)) {
            this->bitmap->Mark(j);
            this->freePageCount--;
            printf("DEBUG RAMDOM INIT TEST: mark page %d\n", j);
        }
    }
}
