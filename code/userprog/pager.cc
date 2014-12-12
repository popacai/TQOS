#include "pager.h" 
#include "system.h"
#include "memorymanager.h"

Pager::Pager() {
    pagerLock = new Lock("pagerLock");

    Tlist = new List;
}

Pager::~Pager() {
    delete pagerLock;
    delete Tlist;
}

int Pager::handlePageFault(int virtualAddr) {
    pagerLock->Acquire();
    TranslationEntry* pageToBePagedOut;
    TranslationEntry* faultPage;
    if (memoryManager->GetFreePageCount() < 1) {
        //No free page any more
        pageToBePagedOut = this->findLRUEntry();
        this->pageOut(pageToBePagedOut);
    }

    faultPage = currentThread->space->getTranslationEntry(virtualAddr);
    this->pageIn(faultPage);

    pagerLock->Release();
    return 0;
}

 /*
   get a page from memory manager
   if  seach for backstore
   elif Load in data if in execute space
   elif in stack space, allocate free
*/
int Pager::pageIn(TranslationEntry* entry) {
    int freePhysicalPage;
    int section;
    int totalPages, stackPages, sharedPages;
    //section = 1 as stack
    //section = 0 as data+code

    freePhysicalPage = memoryManager->AllocPage();

    entry->physicalPage = freePhysicalPage;
    //TODO: search from backstore

    //shared page is the code+data. 
    totalPages = currentThread->space->getNumPages();
    stackPages = UserStackSize / PageSize; //div Round Down. The rest of the stack pages will be shared 
    sharedPages = totalPages - stackPages;

    //TODO:
    if (entry->virtualPage <= (sharedPages)) {
        stats->numPageIns++;
        entry->addrspace->loadFromExecFile(entry);
    }

    if (entry->virtualPage > (sharedPages)) {
        entry->addrspace->AllocStackPage(entry);
    }

    return 0;
}

int Pager::pageOut(TranslationEntry* entry) {
    return 0;
}
/* if dirty?(write back)
      write to backstore
    free this page(memory manager)
*/

int Pager::removeEntry(TranslationEntry* entry) {
    return 0;
}

int Pager::addEntry(TranslationEntry* entry) {
    return 0;
}

TranslationEntry* Pager::findLRUEntry() {
    return 0;
}
