// addrspace.h
//	Data structures to keep track of executing user programs
//	(address spaces).
//
//	For now, we don't keep any information about address spaces.
//	The user level CPU state is saved and restored in the thread
//	executing the user program (see thread.h).
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#ifndef ADDRSPACE_H
#define ADDRSPACE_H

#include "copyright.h"
#include "filesys.h"
#include "memorymanager.h"
#include "backstore.h"

#define UserStackSize		1024 	// increase this as necessary!
#define ArgvSize		128     // add space for argv

class BackStore;

class AddrSpace {
public:
    AddrSpace();	// Create an address space,
    // initializing it with the program
    // stored in the file "executable"
    ~AddrSpace();			// De-allocate an address space

    int Initialize(OpenFile *executable, int flag);
    int Initialize(OpenFile *executable);

    // This function will copy all currentspace pages into this space pages
    int CopyCurrentSpace();

    void InitRegisters();		// Initialize user-level CPU registers,
    // before jumping to user code

    void SaveState();			// Save/restore address space-specific
    void RestoreState();		// info on a context switch
    unsigned int getNumPages() {
        return numPages;
    }
    TranslationEntry* getPageTable() {
        return pageTable;
    }
    OpenFile* executable;

    // Get the entry from the virtual addr
    TranslationEntry* getTranslationEntry(int virtualAddr);

    // Load in to the entry file
    // Physical page has beed allocated
    int loadFromExecFile(TranslationEntry* entry);

    // set the physical page as zero
    int AllocStackPage(TranslationEntry* entry);

    // call the backingstore instead of call addrspace
    // return 0 as success, return -1 as fail
    // Call backstore directly
    // int SaveToSwap(int virtualPageIndex);

    // return 0 as success, return -1 as fail, return 1 as not found
    // Call backstore directly
    // int RestoreFromSwap(int virtualPageIndex);

    BackStore* backstore;

private:
    TranslationEntry *pageTable;	// Assume linear page table translation
    // for now!
    unsigned int numPages;		// Number of pages in the virtual
    // address space
};

int test_addrspace_getTranslationEntry();
#endif // ADDRSPACE_H
