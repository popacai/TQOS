#ifndef PAGER_H
#define PAGER_H

#include "synch.h"
#include "backstore.h"

class Pager {
public:
    Pager();
    ~Pager();

    int handlePageFault(int virtualAddr);
    /* Ask mem manager, freepgecount
       if not free page:
            //Find LRU Page from Tlist();
            pageout one page(pageNum)
       //Insert the page into the Tlist
       page into this
    */

    // In killing a process, we need to remove the entries in the Memory
    int removePhysicalPage(int physicalPage);

    //Stop paging if we are trying to kill a process
    int stop();
    int start();

private:

    int pageIn(TranslationEntry* entry);
    /*
      get a page from memory manager
      if  seach for backstore
      elif Load in data if in execute space
      elif in stack space, allocate free
    */

    int pageOut(TranslationEntry* entry);
    /*  write to backstore, backstore will check whether it is a dirty page and decide whether write back
        free this page(memory manager)
    */

    BackStore *backstore;

    Lock* pagerLock;

    // LRU related functions & data structure
    int addEntry(TranslationEntry* entry);
    TranslationEntry* findLRUEntry();
    TranslationEntry** inMemoryPage;
    int* age; //this should be renamed as ticks. But for the history reason, we keep it as "age"
              //we will page out the lowest-age page

    // We need addrspace to load in pages
    AddrSpace ** addrspaceList; // usd ppn as index, addrspace as element
};

#endif


