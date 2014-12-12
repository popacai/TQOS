// This is mock implementation.
// undefine the mock_backstore will work
#ifdef mock_backstore
#ifdef mock_backstore

#define mock_backstore 1
#ifndef BACKSTORE_H 
#define BACKSTORE_H

#include "machine.h"


#include <vector>

using namespace std;

class BackStore {
public:
    BackStore();
    ~BackStore();
    //TranslationEntry has all the information you need, I guess.
    //return 0 as succeess. return -1 if fail(if there is a failure situation)
    //check the dirty bit in this function. 
    int SavePage(TranslationEntry* entry);

    //get the physical page from the entry. Directly change the machine->mainMemory.
    //return 0 as success, return 1 if cannot found in swapfile(You don't need to set the mainmemory in this case), return -1 if fail(if there is a failure situation)
    int RestorePage(TranslationEntry* entry);


private:
    vector<TranslationEntry*> pageIndexs;
    vector<unsigned char*> pages;
};

//Write you test case here
//Call test_backstore in test_console
//Call nachos -rs 1 -c
int test_backstore();
#endif 

#endif
