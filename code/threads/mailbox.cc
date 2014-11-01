#include "mailbox.h"
#include "copyright.h"
#include "system.h"

Mailbox::Mailbox(char* debugName)
{
    name = debugName;
    buffer = new int;
    
    empty= new Semaphore("empty",1);
    full =new Semaphore("full",0);
    sema_R = new Semaphore("receiver",0);
    sema_S = new Semaphore("sender",0);

}

Mailbox::~Mailbox(){
    delete name;
    delete buffer;
    delete empty;
    delete full;
    delete sema_R;
    delete sema_S;
}

void Mailbox::Send(int message){
    sema_R->V();
    currentThread->Yield();
    currentThread->Yield();
    sema_S->P();
    currentThread->Yield();
    currentThread->Yield();

    empty->P();
    currentThread->Yield();
    *buffer = message;
    currentThread->Yield();
    currentThread->Yield();
    printf("send message is=%d\n",*buffer);
    currentThread->Yield();
    currentThread->Yield();
    full->V();
    currentThread->Yield();

}

void Mailbox::Receive(int* message){
    sema_S->V();
    currentThread->Yield();
    currentThread->Yield();
    sema_R->P();
    currentThread->Yield();
    currentThread->Yield();

    full->P();
    currentThread->Yield();
    *message = *buffer;
    currentThread->Yield();
    empty->V();
    currentThread->Yield();
}
