#include "pager.h"
#include "system.h"
#include "memorymanager.h"

#define LRU
Pager::Pager() {
    int i;
    pagerLock = new Lock("pagerLock");

    Tlist = new List;
    addrspaceList = new AddrSpace*[NumPhysPages];
    for(i = 0; i < NumPhysPages; i++) {
        addrspaceList[i] = NULL;
    }

    age = new int[NumPhysPages];
    for (i = 0; i < NumPhysPages; i++) {
        age[i] = -1;
    }

    inMemoryPage = new TranslationEntry*[NumPhysPages];
    for(i = 0; i < NumPhysPages; i++) {
        inMemoryPage[i] = 0;
    }
}

Pager::~Pager() {
    delete pagerLock;
    delete Tlist;
    delete addrspaceList;
}

int Pager::handlePageFault(int virtualAddr) {
    pagerLock->Acquire();
    stats->numPageFaults++;
    //fprintf(stderr, "=====start=====\n");
    TranslationEntry* pageToBePagedOut;
    TranslationEntry* faultPage;

    faultPage = currentThread->space->getTranslationEntry(virtualAddr);
    //fprintf(stderr, "handle pagefault for spid=%d,v=%d\n", currentThread->spid, faultPage->virtualPage);

    if (memoryManager->GetFreePageCount() < 1) {
        //No free page any more
        pageToBePagedOut = this->findLRUEntry();
        //fprintf(stderr, "is dirty?                          %d\n", pageToBePagedOut->dirty);
        stats->numPageOuts++;
        this->pageOut(pageToBePagedOut);
        pageToBePagedOut->valid = FALSE;
        //fprintf(stderr, "page out for v=%d, p=%d\n", pageToBePagedOut->virtualPage, pageToBePagedOut->physicalPage);
    }

    faultPage->physicalPage = memoryManager->AllocPage();
    this->pageIn(faultPage);
    faultPage->valid = TRUE;

    this->addEntry(faultPage);

    //fprintf(stderr, "page in for v=%d, p=%d\n", faultPage->virtualPage, faultPage->physicalPage);

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

    int ppn;
    AddrSpace * space;

    ppn = entry->physicalPage;
    this->addrspaceList[ppn] = currentThread->space;
    space = this->addrspaceList[ppn];
    ASSERT(space != NULL);
    foundInSwap = space->backstore->RestorePage(entry);
    if (foundInSwap == 0) {
        //printf("found in swap\n");
        stats->numPageIns++;
        entry->dirty = FALSE;
        return 0;
    }

    ASSERT(foundInSwap == 1);

    /*
    if (foundInSwap == 1) {
        //printf("not found in swap\n");
    }
    */


    //shared page is the code+data.
    totalPages = currentThread->space->getNumPages();
    stackPages = UserStackSize / PageSize; //div Round Down. The rest of the stack pages will be shared
    sharedPages = totalPages - stackPages;

    //TODO:
    if (entry->virtualPage < (sharedPages)) {
        stats->numPageIns++;
        //entry->addrspace->loadFromExecFile(entry);
        entry->dirty = FALSE;
        space->loadFromExecFile(entry);
    }

    if (entry->virtualPage >= (sharedPages)) {
        // entry->addrspace->AllocStackPage(entry);
        entry->dirty = FALSE;
        space->AllocStackPage(entry);
        //printf("alloc stack space\n");
    }

    return 0;
}

int Pager::pageOut(TranslationEntry* entry) {
    int ppn;
    AddrSpace * space;

    ppn = entry->physicalPage;
    space = addrspaceList[ppn];
    space->backstore->SavePage(entry);
    addrspaceList[ppn] = NULL;
    memoryManager->FreePage(entry->physicalPage);
}
/* if dirty?(write back)
      write to backstore
    free this page(memory manager)
*/

int Pager::addEntry(TranslationEntry* entry) {
    //printf("%d\n", entry->physicalPage);
    ASSERT(age[entry->physicalPage] == -1);
    ASSERT(inMemoryPage[entry->physicalPage] == 0);
    age[entry->physicalPage] = stats->totalTicks;
    inMemoryPage[entry->physicalPage] = entry;
    //Tlist->Append((void*)entry);

#ifdef LRU
    int i;
    for (i = 0; i < NumPhysPages; i++) {
        if (inMemoryPage[i] != 0) {
            if ((inMemoryPage[i])->use == 1) {
                age[i] = stats->totalTicks - 1;
            }
            (inMemoryPage[i])->use = 0;       //set all as not used
        }
    }
#endif

    return 0;
}

TranslationEntry* Pager::findLRUEntry() {

#ifdef LRU
    TranslationEntry* entry;
    int i;
    int oldest_age, oldest_page;
    //check use bit. If not used then, pick the oldest one
    oldest_age = stats->totalTicks;
    oldest_page = -1;
    for (i = 0; i < NumPhysPages; i++) {
        if (inMemoryPage[i] == 0)
            continue;
        if (inMemoryPage[i]->use == 0) {
            if (oldest_age > age[i]) {
                oldest_age = age[i];
                oldest_page = i;
            }
        }
    }

    if (oldest_page == -1) {
        //All pages are used
        //research all pages and pick up the oldest page
        for (i = 0; i < NumPhysPages; i++) {
            if (inMemoryPage[i] == 0)
                continue;
            ASSERT(inMemoryPage[i]->use == 1);
            if (oldest_age > age[i]) {
                oldest_age = age[i];
                oldest_page = i;
            }
        }
    }

    ASSERT(oldest_page != -1);
    entry = inMemoryPage[oldest_page];
    inMemoryPage[oldest_page] = 0;
    age[oldest_page] = -1;
    return entry;
#else
    int i;
    TranslationEntry* entry;
    int oldest_age, oldest_page;
    oldest_age = stats->totalTicks - 1;
    oldest_page = -1;
    for (i = 0; i < NumPhysPages; i++) {
        if (oldest_age > age[i]) {
            oldest_age = age[i];
            oldest_page = i;
        }
    }
    ASSERT(oldest_page != -1);
    entry = inMemoryPage[oldest_page];
    inMemoryPage[oldest_page] = 0;
    age[oldest_page] = -1;
    return entry;



#endif 
    /*
    do {
        entry = (TranslationEntry*)Tlist->Remove();
    } while (entry->valid == FALSE);
    */
    return entry;
}

int Pager::removePhysicalPage(int physicalPage) {

    ASSERT(inMemoryPage[physicalPage] != 0);
    inMemoryPage[physicalPage] = 0;
    age[physicalPage] = -1;
    return 0;
}

int Pager::stop() {
    pagerLock->Acquire();
}

int Pager::start() {
    pagerLock->Release();
}
