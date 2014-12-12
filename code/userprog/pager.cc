#include "pager.h" 
#include "system.h"
#include "memorymanager.h"

Pager::Pager() {
    pagerLock = new Lock("pagerLock");

    Tlist = new List;
    backstore = new BackStore();
}

Pager::~Pager() {
    delete pagerLock;
    delete Tlist;
    delete backstore;
}

int Pager::handlePageFault(int virtualAddr) {
    //printf("=====start=====\n");
    pagerLock->Acquire();
    TranslationEntry* pageToBePagedOut;
    TranslationEntry* faultPage;
    
    faultPage = currentThread->space->getTranslationEntry(virtualAddr);
    //printf("handle pagefault for v=%d\n", faultPage->virtualPage);

    if (memoryManager->GetFreePageCount() < 1) {
        //No free page any more
        pageToBePagedOut = this->findLRUEntry();
        this->pageOut(pageToBePagedOut);
        pageToBePagedOut->valid = FALSE;
        //printf("page out for v=%d, p=%d\n", pageToBePagedOut->virtualPage, pageToBePagedOut->physicalPage);
    }

    faultPage->physicalPage = memoryManager->AllocPage();

    this->pageIn(faultPage);
    this->addEntry(faultPage);
    faultPage->valid = TRUE;

    //printf("page in for v=%d, p=%d\n", faultPage->virtualPage, faultPage->physicalPage);

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

    int foundInSwap;
    //section = 1 as stack
    //section = 0 as data+code
    //printf("in  page in  1 : machine->pageTable=%d\n", machine->pageTable);

    //freePhysicalPage = memoryManager->AllocPage();

    //entry->physicalPage = freePhysicalPage;
    //printf("in  page in  2 : machine->pageTable=%d\n", machine->pageTable);
    //TODO: search from backstore

    foundInSwap = backstore->RestorePage(entry);
    if (foundInSwap == 0) {
        //printf("found in swap\n");
        return 0;
    }

    if (foundInSwap == 1) {
        //printf("not found in swap\n");
    }
    

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
        //printf("alloc stack space\n");
    }

    return 0;
}

int Pager::pageOut(TranslationEntry* entry) {
    backstore->SavePage(entry);
    memoryManager->FreePage(entry->physicalPage);
}
/* if dirty?(write back)
      write to backstore
    free this page(memory manager)
*/

int Pager::addEntry(TranslationEntry* entry) {
    Tlist->Append((void*)entry);
    return 0;
}

TranslationEntry* Pager::findLRUEntry() {
    TranslationEntry* entry;
    entry = (TranslationEntry*)Tlist->Remove();
    return entry;
}
