// addrspace.cc
//	Routines to manage address spaces (executing user programs).
//
//	In order to run a user program, you must:
//
//	1. link with the -N -T 0 option
//	2. run coff2noff to convert the object file to Nachos format
//		(Nachos object code format is essentially just a simpler
//		version of the UNIX executable object code format)
//	3. load the NOFF file into the Nachos file system
//		(if you haven't implemented the file system yet, you
//		don't need to do this last step)
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "addrspace.h"
#include "noff.h"
#include "memorymanager.h"
#ifdef HOST_SPARC
#include <strings.h>
#endif

//#define PDEBUG
//----------------------------------------------------------------------
// SwapHeader
// 	Do little endian to big endian conversion on the bytes in the
//	object file header, in case the file was generated on a little
//	endian machine, and we're now running on a big endian machine.
//----------------------------------------------------------------------

static void
SwapHeader (NoffHeader *noffH)
{
    noffH->noffMagic = WordToHost(noffH->noffMagic);
    noffH->code.size = WordToHost(noffH->code.size);
    noffH->code.virtualAddr = WordToHost(noffH->code.virtualAddr);
    noffH->code.inFileAddr = WordToHost(noffH->code.inFileAddr);
    noffH->initData.size = WordToHost(noffH->initData.size);
    noffH->initData.virtualAddr = WordToHost(noffH->initData.virtualAddr);
    noffH->initData.inFileAddr = WordToHost(noffH->initData.inFileAddr);
    noffH->uninitData.size = WordToHost(noffH->uninitData.size);
    noffH->uninitData.virtualAddr = WordToHost(noffH->uninitData.virtualAddr);
    noffH->uninitData.inFileAddr = WordToHost(noffH->uninitData.inFileAddr);
}

//----------------------------------------------------------------------
// AddrSpace::AddrSpace
// 	Create an address space to run a user program.
//	Load the program from a file "executable", and set everything
//	up so that we can start executing user instructions.
//
//	Assumes that the object code file is in NOFF format.
//
//	First, set up the translation from program memory to physical
//	memory.  For now, this is really simple (1:1), since we are
//	only uniprogramming, and we have a single unsegmented page table
//
//	"executable" is the file containing the object code to load into memory
//----------------------------------------------------------------------

AddrSpace::AddrSpace() {
    pageTable = NULL;
}

int
AddrSpace::Initialize(OpenFile *executable)
{
    return Initialize(executable, 1); // default has lock
}

int 
AddrSpace::Initialize(OpenFile *executable, int flag)
{
    NoffHeader noffH;
    unsigned int i, size;

    executable->ReadAt((char *)&noffH, sizeof(noffH), 0);
    if ((noffH.noffMagic != NOFFMAGIC) &&
            (WordToHost(noffH.noffMagic) == NOFFMAGIC))
        SwapHeader(&noffH);
    ASSERT(noffH.noffMagic == NOFFMAGIC);

// how big is address space?
    size = noffH.code.size + noffH.initData.size + noffH.uninitData.size
           + UserStackSize + ArgvSize;	// we need to increase the size
    // to leave room for the stack
    numPages = divRoundUp(size, PageSize);
    size = numPages * PageSize;

    if(numPages > (unsigned int)memoryManager->GetFreePageCount() || numPages > NumPhysPages) {
        return -1; // TODO: handle fail
    }
    pageTable = new TranslationEntry[numPages];
#ifdef PDEBUG
    printf("********************** NUM PHYS PAGES: %d, PAGE SIZE: 0x%x\n", NumPhysPages, PageSize);
#endif
    // to run anything too big --
    // at least until we have
    // virtual memory

    DEBUG('a', "Initializing address space, num pages %d, size %d\n",
          numPages, size);
// first, set up the translation
#ifdef PDEBUG
    printf("num pages = %d\n",numPages);
#endif
    for (i = 0; i < numPages; i++) {
        pageTable[i].virtualPage = i;	// for now, virtual page # = phys page #
        // For Project 2, virtual page number should be i 
        // There is no real virtual memory, just assign a physical memory page
        pageTable[i].physicalPage = memoryManager->AllocPage(flag);
        ASSERT(pageTable[i].physicalPage >= 0);
        //bzero(&machine->mainMemory[pageTable[i].physicalPage * PageSize], PageSize);
        memset(&machine->mainMemory[pageTable[i].physicalPage * PageSize], 'A', PageSize);
        pageTable[i].valid = TRUE;
        pageTable[i].use = FALSE;
        pageTable[i].dirty = FALSE;
        pageTable[i].readOnly = FALSE;  // if the code segment was entirely on
        pageTable[i].reference = 0;
        // a separate page, we could set its
        // pages to be read-only
    }

// zero out the entire address space, to zero the unitialized data segment
// and the stack segment
    // For Project 2, we can no longer do this...
    // bzero(machine->mainMemory, size);

// then, copy in the code and data segments into memory
    unsigned int copySize= noffH.code.size;
    unsigned int copyStart = noffH.code.virtualAddr; 
    unsigned int copyInFileStart = noffH.code.inFileAddr;
    unsigned int copyOffset;
    unsigned int ppn;
#ifdef PDEBUG
printf("code: \n");
printf("virtual addr: 0x%x\n", noffH.code.virtualAddr);
printf("in file addr: 0x%x\n", noffH.code.inFileAddr);
printf("code size: 0x%x\n", noffH.code.size);
printf("begin copying....\n");
printf("page size: 0x%x\n", PageSize);
#endif
    copyOffset = copyStart & (PageSize - 1);
    while(copySize >= PageSize - copyOffset) {
        // start at non-boundry and is long enough to fill up the page
        // calculate physical page number using page table
        ppn = pageTable[copyStart / PageSize].physicalPage;
#ifdef PDEBUG
printf("writing phys page %d\n", ppn);
#endif
        executable->ReadAt(&(machine->mainMemory[ppn*PageSize + copyOffset]),
                           PageSize - copyOffset, copyInFileStart);
        copySize -= (PageSize - copyOffset);
        copyStart += (PageSize - copyOffset);
        copyInFileStart += (PageSize - copyOffset);
        copyOffset = copyStart & (PageSize - 1);
        ASSERT(copyOffset == 0);
    }
    if(copySize > 0) {
        ppn = pageTable[copyStart / PageSize].physicalPage;
#ifdef PEDBUG
printf("writing phys page %d\n", ppn);
#endif
        executable->ReadAt(&(machine->mainMemory[ppn*PageSize + copyOffset]),
                             copySize, copyInFileStart);
    }

    copySize = noffH.initData.size;
    copyStart = noffH.initData.virtualAddr;
    copyInFileStart= noffH.initData.inFileAddr;
    copyOffset = copyStart & (PageSize - 1);
    while(copySize >= PageSize - copyOffset) {
        // start at non-boundry and is long enough to fill up the page
        // calculate physical page number using page table
        ppn = pageTable[copyStart / PageSize].physicalPage;
        executable->ReadAt(&(machine->mainMemory[ppn*PageSize + copyOffset]),
                           PageSize - copyOffset, copyInFileStart);
        copySize -= (PageSize - copyOffset);
        copyStart += (PageSize - copyOffset);
        copyInFileStart += (PageSize - copyOffset);
        copyOffset = copyStart & (PageSize - 1);
    }
    if(copySize > 0) {
        ppn = pageTable[copyStart / PageSize].physicalPage;
        executable->ReadAt(&(machine->mainMemory[ppn*PageSize + copyOffset]),
                             copySize, copyInFileStart);
    }
    return 1; // success
}

int SeekForPhysicalPage(TranslationEntry* vm, int vmSize,int virtualPage) {
    int i;
    for (i = 0; i < vmSize; i++) {
        if (vm[i].virtualPage == virtualPage) {
            return i;
        }
    }
    return -1;
}

int AddrSpace::CopyCurrentSpace() {
    int i, physicalPageIndex;
    int vmIndex;
    int temp;
    int newPhysicalPage, oldPhysicalPage;
    int sharedPages;
    int stackPages;
    TranslationEntry* currentThreadVM;
    IntStatus oldLevel = interrupt->SetLevel(IntOff);	// disable interrupts

    numPages = currentThread->space->getNumPages();
    stackPages = UserStackSize / PageSize; //div Round Down. The rest of the stack pages will be shared 
    sharedPages = numPages - stackPages;

    if(stackPages > memoryManager->GetFreePageCount() || numPages > NumPhysPages) {
        return 1; // TODO: handle fail
    }

    //printf("numPages=%d\n", numPages);
    //printf("sharedPages=%d\n", sharedPages);
    //printf("stackPages=%d\n", stackPages); 

    //init new Entries
    pageTable = new TranslationEntry[numPages];
    currentThreadVM = currentThread->space->getPageTable();

    // redirect the shared memory
    for (i = 0; i < sharedPages; i++) {
        pageTable[i].virtualPage = i;
        vmIndex = SeekForPhysicalPage(currentThreadVM, numPages, i);
        pageTable[i].physicalPage = currentThreadVM[vmIndex].physicalPage;

        pageTable[i].valid = TRUE;
        pageTable[i].use = FALSE;
        pageTable[i].dirty = FALSE;
        pageTable[i].readOnly = FALSE;  // if the code segment was entirely on
        if (!(currentThreadVM[vmIndex].reference)) {
            //This is the first thread
            currentThreadVM[vmIndex].reference = new int;
            *(currentThreadVM[vmIndex].reference) = 1;
        }

        pageTable[i].reference = currentThreadVM[vmIndex].reference;
        *(pageTable[i].reference)++;

        ASSERT(vmIndex >= 0);
    }

    // start a new memory space
    for (i = sharedPages; i < numPages; i++) {
        pageTable[i].virtualPage = i;
        oldPhysicalPage = SeekForPhysicalPage(currentThreadVM, numPages, i);
        //oldPhysicalPage = i;
        newPhysicalPage = memoryManager->AllocPage(1); //has the lock?
        pageTable[i].physicalPage = newPhysicalPage;

        pageTable[i].valid = TRUE;
        pageTable[i].use = FALSE;
        pageTable[i].dirty = FALSE;
        pageTable[i].readOnly = FALSE;  // if the code segment was entirely on

        memset(&machine->mainMemory[pageTable[i].physicalPage * PageSize], 'A', PageSize);
        /*
        memcpy((void*)(machine->mainMemory + newPhysicalPage * PageSize), 
                      (void*)(machine->mainMemory + oldPhysicalPage * PageSize), 
                      PageSize);
        */

    }
    (void) interrupt->SetLevel(oldLevel);	// re-enable interrupts


    return 0;
}

//----------------------------------------------------------------------
// AddrSpace::~AddrSpace
// 	Dealloate an address space.  Nothing for now!
//----------------------------------------------------------------------
AddrSpace::~AddrSpace()
{
    //TODO: To be fixed in future
    unsigned int i;
    for(i = 0; i < numPages; i++) {
        if ((pageTable[i].reference)) {
            (*(pageTable[i].reference)) --;
            if (*(pageTable[i].reference) == 0) {
                memoryManager->FreePage(pageTable[i].physicalPage);
            }
        }
        else
        {
            memoryManager->FreePage(pageTable[i].physicalPage);
        }
    }
    if(pageTable != NULL) {
        delete [] pageTable;
    }
}

//----------------------------------------------------------------------
// AddrSpace::InitRegisters
// 	Set the initial values for the user-level register set.
//
// 	We write these directly into the "machine" registers, so
//	that we can immediately jump to user code.  Note that these
//	will be saved/restored into the currentThread->userRegisters
//	when this thread is context switched out.
//----------------------------------------------------------------------

void
AddrSpace::InitRegisters()
{
    int i;

    for (i = 0; i < NumTotalRegs; i++)
        machine->WriteRegister(i, 0);

    // Initial program counter -- must be location of "Start"
    machine->WriteRegister(PCReg, 0);

    // Need to also tell MIPS where next instruction is, because
    // of branch delay possibility
    machine->WriteRegister(NextPCReg, 4);

    // Set the stack register to the end of the address space, where we
    // allocated the stack; but subtract off a bit, to make sure we don't
    // accidentally reference off the end!
    machine->WriteRegister(StackReg, numPages * PageSize - 16);
    DEBUG('a', "Initializing stack register to %d\n", numPages * PageSize - 16);
}

//----------------------------------------------------------------------
// AddrSpace::SaveState
// 	On a context switch, save any machine state, specific
//	to this address space, that needs saving.
//
//	For now, nothing!
//----------------------------------------------------------------------

void AddrSpace::SaveState()
{
    //why???????
    //pageTable = machine->pageTable;
    //numPages = machine->pageTableSize;
}

//----------------------------------------------------------------------
// AddrSpace::RestoreState
// 	On a context switch, restore the machine state so that
//	this address space can run.
//
//      For now, tell the machine where to find the page table.
//----------------------------------------------------------------------

void AddrSpace::RestoreState()
{
    machine->pageTable = pageTable;
    machine->pageTableSize = numPages;
}
