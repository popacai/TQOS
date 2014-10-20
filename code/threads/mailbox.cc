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
    }
    buffer = &message;
    mailbox_cond->Signal(mailbox_lock);
}

void Mailbox::Receive(int* message){
    mailbox_lock->Acquire();
    resource--;
    if (resource < 0) {
        mailbox_cond->Wait(mailbox_lock);
    }
    ASSERT(buffer!=NULL);
    message = buffer;
}
