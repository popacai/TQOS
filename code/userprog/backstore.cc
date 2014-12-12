// This is a mock implementation
#ifdef mock_backstore

#include "backstore.h"
#include "system.h"

//qi

BackStore::BackStore() {
}

BackStore::~BackStore() {
}

int BackStore::SavePage(TranslationEntry* entry) {
    int ppn;
    unsigned char* pageData;

    ppn = entry->physicalPage;
    size_t i;
    for (i = 0; i < pageIndexs.size(); i++) {
        if (pageIndexs[i] == entry) {
            memcpy(pages[i], &(machine->mainMemory[ppn * PageSize]), PageSize); 
            return 0;
        }
    }

    pageData = new unsigned char[PageSize];

    pageIndexs.push_back(entry);
    memcpy(pageData, &(machine->mainMemory[ppn * PageSize]), PageSize);
    pages.push_back(pageData);

    return 0;
}

int BackStore::RestorePage(TranslationEntry* entry) {
    int ppn;
    //unsigned char* pageData;
    size_t i;
    ppn = entry->physicalPage;

    for (i = 0; i < pageIndexs.size(); i++) {
        if (pageIndexs[i] == entry) {
            memcpy(&(machine->mainMemory[ppn * PageSize]), pages[i], PageSize); 
            return 0;
        }
    }
    return 1;
}

#endif
