#include "mailbox.h"
#include "copyright.h"
#include "system.h"

Mailbox::Mailbox(char* debugName, Condition* condition, Lock* lock)
{
    name = debugName;
    resource = 0;
    mailbox_cond = condition; 
    mailbox_lock = lock;
}

Mailbox::~Mailbox(){
    delete name;
    delete buffer;
    delete mailbox_cond;
    delete mailbox_lock;
}

void Mailbox::Send(int message){
    mailbox_lock->Acquire();
    resource++;
    if (resource > 0) {
        mailbox_cond->Wait(mailbox_lock);
    }else {
        mailbox_cond->Signal(mailbox_lock);
        mailbox_lock->Release();
    }
    buffer = &message;
    printf("send buffer:%d\n", *buffer);
}

void Mailbox::Receive(int* message){
    mailbox_lock->Acquire();
    resource--;
    if (resource < 0) {
        mailbox_cond->Wait(mailbox_lock);
    }else {
        mailbox_cond->Signal(mailbox_lock);
        mailbox_lock->Release();
        currentThread->Yield();
    }
    ASSERT(buffer!=NULL);
    message = buffer;
    printf("receive buffer:%d\n", *message);
}
