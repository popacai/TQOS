#ifndef PAGER_H 
#define PAGER_H

#include "synch.h"

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

private:
 
    int pageIn(TranslationEntry* entry);
     /*
       get a page from memory manager
       if  seach for backstore
       elif Load in data if in execute space
       elif in stack space, allocate free
    */

    int pageOut(TranslationEntry* entry);
    /* if dirty?(write back)
          write to backstore
        free this page(memory manager)
    */
    

    Lock* pagerLock;
    
    //TODO: I need a counter here

    List* Tlist; //TranslationEntry list, All TranslationEntry in Memory
    //Use a vector instead
    int removeEntry(TranslationEntry* entry);
    int addEntry(TranslationEntry* entry);
    TranslationEntry* findLRUEntry();
};

#endif 

