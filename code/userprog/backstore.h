#ifndef BACKSTORE_H 
#define BACKSTORE_H

#include "machine.h"

class BackStore {
public:
    //TranslationEntry has all the information you need, I guess.
    //return 0 as succeess. return -1 if fail(if there is a failure situation)
    //check the dirty bit in this function. 
    int SavePage(TranslationEntry* entry);

    //get the physical page from the entry. Directly change the machine->mainMemory.
    //return 0 as success, return 1 if cannot found in swapfile(You don't need to set the mainmemory in this case), return -1 if fail(if there is a failure situation)
    int RestorePage(TranslationEntry* entry);
};
#endif 
