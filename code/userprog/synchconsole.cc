#include "synchconsole.h"
#include "system.h"

static void
read_done (int arg) {
    SynchConsole* synchconsole_t = (SynchConsole*) arg;
    synchconsole_t->ReadDone();
}

static void
write_done (int arg) {
    SynchConsole* synchconsole_t = (SynchConsole*) arg;
    synchconsole_t->WriteDone();
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
    bool redirect_stdin;
    redirect_stdin = (currentThread->join & 0x4);
    if (redirect_stdin) {
        value = bufferpipe->pop();
        //fprintf(stderr, "%x, %c\n", value, value);
    } else {
        read_lock->Acquire();
        read_semaphore->P();
        value = console->GetChar();
        read_lock->Release();
    }
    return value;
}

void SynchConsole::PutChar(char input) {
    bool redirect_stdout;
    redirect_stdout = (currentThread->join & 0x2);
    if (redirect_stdout) {
        bufferpipe->push(input);
    } else {
        write_lock->Acquire();
        console->PutChar(input);
        write_semaphore->P();
        write_lock->Release();
    }
}

void SynchConsole::ReadDone() {
    read_semaphore->V();
}

void SynchConsole::WriteDone() {
    write_semaphore->V();
}

void test_synchconsole()
{
    //SynchConsole* synchconsole = new SynchConsole(0, 0);
    synchconsole = new SynchConsole(0, 0);
    char ch;
    ch = 'a';
    while (ch != 'q') {
        ch = synchconsole->GetChar();
        synchconsole->PutChar(ch);
    }
}

