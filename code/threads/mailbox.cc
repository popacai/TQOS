#include "mailbox.h"
#include "copyright.h"
#include "system.h"

Mailbox::Mailbox(char* debugName)
{
    name = debugName;
    buffer = new int;
    
    p1_empty= new Semaphore("empty",1);
    p1_full =new Semaphore("full",0);
    p2_empty= new Semaphore("empty",1);
    p2_full =new Semaphore("full",0);
    ack = 0;

}

Mailbox::~Mailbox(){
    delete buffer;
    delete p1_empty;
    delete p1_full;
    delete p2_empty;
    delete p2_full;
}

void Mailbox::Send(int message){
    printf("%d\n", message);
    p1_empty->P();
    currentThread->Yield();
    currentThread->Yield();
    *buffer = message;
    currentThread->Yield();
    currentThread->Yield();
    p1_full->V();
    currentThread->Yield();
    currentThread->Yield();

    p2_full->P();
    currentThread->Yield();
    currentThread->Yield();
    ack = 0;
    currentThread->Yield();
    currentThread->Yield();
    //p2_empty->V();
    currentThread->Yield();
    currentThread->Yield();
}

void Mailbox::Receive(int* message){
    p1_full->P();
    currentThread->Yield();
    currentThread->Yield();
    *message = *buffer;
    currentThread->Yield();
    currentThread->Yield();
    p1_empty->V();
    currentThread->Yield();
    currentThread->Yield();

    //p2_empty->P();
    currentThread->Yield();
    currentThread->Yield();
    ack = 1;
    currentThread->Yield();
    currentThread->Yield();
    p2_full->V();
    currentThread->Yield();
    currentThread->Yield();
}
