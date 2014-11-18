#include "synchconsole.h"

static void 
read_done (int arg){
    SynchConsole* synchconsole = (SynchConsole*) arg;
    synchconsole->ReadDone();
}

static void 
write_done (int arg){
    SynchConsole* synchconsole = (SynchConsole*) arg;
    synchconsole->WriteDone();
}

SynchConsole::SynchConsole(char* input, char* output) {
    console = new Console(input, output, (VoidFunctionPtr)read_done, (VoidFunctionPtr)write_done, (int)this);
    read_lock = new Lock("synch console lock");
    read_semaphore = new Semaphore("synch console semaphore", 0);
    write_lock = new Lock("synch console lock");
    write_semaphore = new Semaphore("synch console semaphore", 0);
}

SynchConsole::~SynchConsole() {
    delete read_lock;
    delete read_semaphore;
    delete write_lock;
    delete write_semaphore;
    delete console;
}

char SynchConsole::GetChar() {
    char value;
    read_lock->Acquire();
    read_semaphore->P();
    value = console->GetChar();
    read_lock->Release();
    return value;
}

void SynchConsole::PutChar(char input) {
    write_lock->Acquire();
    console->PutChar(input);
    write_semaphore->P();
    write_lock->Release();
}

void SynchConsole::ReadDone() {
    read_semaphore->V();
}

void SynchConsole::WriteDone() {
    write_semaphore->V();
}

void test_synchconsole()
{
    SynchConsole* synchconsole = new SynchConsole(0, 0);
    char ch;
    ch = 'a';
    while (ch != 'q') {
        ch = synchconsole->GetChar(); 
        synchconsole->PutChar(ch);
    }
}

