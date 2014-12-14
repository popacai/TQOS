#include "processmanager.h"
#include "system.h"

ProcessManager::ProcessManager(int size)
{
    int i;
    indexArray = (void**) new int[size];
    this->arraySize = size;
    for(i = 0; i < size; i++) {
        indexArray[i] = NULL;
    }
    this->lock = new Lock("process manager lock");
}

ProcessManager::~ProcessManager()
{
    int i;
    for(i = 0; i < arraySize; i++) {
        delete (int*)indexArray[i];
    }
    delete indexArray;
    delete lock;
}

/*
 * return index + 1 as spid
 * return 0 if no space
 */
int
ProcessManager::Alloc(void *object)
{
    lock->Acquire();
    int i;
    int spid = 0;
    for(i = 0; i < arraySize; i++) {
        if(indexArray[i] == NULL) {
            indexArray[i] = object;
            spid = i + 1;
            break;
        }
    }
    lock->Release();
    return spid;
}

void *
ProcessManager::Get(int index)
{
    lock->Acquire();
    void * process;
    if(index >= 1 && index <= arraySize) {
        process = indexArray[index-1];
    }
    else {
        process = NULL;
    }
    lock->Release();
    return process;
}

void
ProcessManager::Release(int index)
{
    lock->Acquire();
    if(index >= 0 && index < arraySize) {
        indexArray[index-1] = NULL;
    }
    lock->Release();
}

// return 1 if we are the last to exit
int
ProcessManager::TestForExit()
{
    lock->Acquire();
    int result = 0;
    int i;
    for(i = 0; i < arraySize; i++) {
        if(indexArray[i] != NULL) result++;
    }
    if(result == 0) ASSERT(false);
    lock->Release();
    return result;
}
