// This is a mock implementation
//#define mock_backstore
//#ifdef mock_backstore
#include "backstore.h"
#include "system.h"
#include "addrspace.h"
#include "filesys.h"

BackStore::BackStore(AddrSpace * space, int spid) {
    bool success;
    int virtualPageCount;
    int fileSize;
    virtualPageCount = space->getNumPages();
    fileSize = virtualPageCount * PageSize;
    //fileName = itoa(spid);
    fileName = new char[20];
    sprintf(fileName, "%d", spid);
    //printf("fileName: %s\n", fileName);
    success = fileSystem->Create(fileName, fileSize); // make sure to delete the file later
    if(!success) {
        printf("Create file not successful!\n");
        ASSERT(0);
    }
    swapFile = fileSystem->Open(fileName);
    vpnIndices = new BitMap(virtualPageCount);
}

BackStore::~BackStore() {
    fileSystem->Remove(fileName);
    delete vpnIndices;
    delete fileName;
}

int BackStore::SavePage(TranslationEntry* entry) {
    //printf("in save page\n");
    int ppn;
    int vpn;
    int bytesWritten;
    ppn = entry->physicalPage;
    vpn = entry->virtualPage;
    // check dirty bit
    if(entry->dirty) {
        bytesWritten = swapFile->WriteAt(&(machine->mainMemory[ppn * PageSize]), PageSize, vpn * PageSize);
        if (bytesWritten == PageSize) {
            vpnIndices->Mark(vpn);
            //printf("write success\n");
            return 0; // success
        }
        else {
            printf("Write backing store error!\n");
            return -1;
        }
    }
    //printf("not dirty\n");
    // no need to update backing store
    return 0;

    /* mock
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
    */
}

int BackStore::RestorePage(TranslationEntry* entry) {
    //printf("in restore page\n");
    int ppn;
    int vpn;
    int bytesRead;
    // test if backing store exists
    vpn = entry->virtualPage;
    if(vpnIndices->Test(vpn)) {
        ppn = entry->physicalPage;
        bytesRead = swapFile->ReadAt(&(machine->mainMemory[ppn * PageSize]), PageSize, vpn * PageSize);
        if(bytesRead == PageSize) {
            //printf("read success\n");
            return 0;
        }
        else {
            printf("Read backing store error! Read bytes: %d of %d\n", bytesRead, PageSize);
            return -1;
        }
    }
    else {
        //printf("not in backing store\n");
        return 1; // not in backing store, should load from img
    }
    /* mock
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
    */
}

int test_backstore() {
    AddrSpace * space = new AddrSpace();
    OpenFile * executable = fileSystem->Open("../test/snake");
    space -> Initialize(executable, 1);
    BackStore * backstore = new BackStore(space, 100);
    TranslationEntry * testPage = new TranslationEntry();

    testPage->physicalPage = 3;
    testPage->virtualPage = 2;
    testPage->dirty = 1;


    memset(&(machine->mainMemory[3*PageSize]), 1, PageSize);
    printf("before save: %d\n", machine->mainMemory[40*PageSize]);
    backstore->SavePage(testPage);
    memset(&(machine->mainMemory[40*PageSize]), 0, PageSize);
    printf("after save: %d\n", machine->mainMemory[40*PageSize]);
    backstore->RestorePage(testPage);
    printf("after restore: %d\n", machine->mainMemory[40*PageSize]);

}


//#endif
