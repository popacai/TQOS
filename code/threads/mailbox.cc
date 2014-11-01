#include "mailbox.h"
#include "copyright.h"
#include "system.h"

Mailbox::Mailbox(char* debugName)
{
    name = debugName;
    buffer = new int;
    resource = 0;
    b_count = 0;
    
    empty= new Semaphore("empty",1);
    full =new Semaphore("full",0);

    mailbox_cond = new Condition("mailbox_cond"); 
    mailbox_lock = new Lock("mailbox_lock");
    buffer_cond = new Condition("send_cond");
    buffer_lock = new Lock("send_lock");
}

Mailbox::~Mailbox(){
    delete buffer;
    delete mailbox_cond;
    delete mailbox_lock;
    delete buffer_cond;
    delete buffer_lock;
}

void Mailbox::Send(int message){
    mailbox_lock->Acquire();
    resource++;
    if (resource > 0) {
        mailbox_cond->Wait(mailbox_lock);
    }
    else{
        
        mailbox_cond->Signal(mailbox_lock);
    }
    mailbox_lock->Release();
    currentThread->Yield();
    currentThread->Yield();

    empty->P();
    currentThread->Yield();
    currentThread->Yield();
    *buffer = message;
    currentThread->Yield();
    printf("send message is=%d\n",*buffer);
    currentThread->Yield();
    full->V();
    currentThread->Yield();
    currentThread->Yield();

}

void Mailbox::Receive(int* message){
    mailbox_lock->Acquire();
    resource--;
    if (resource < 0) {
        mailbox_cond->Wait(mailbox_lock);
    }
    else{
        mailbox_cond->Signal(mailbox_lock);
    }
    mailbox_lock->Release();
    // add another condition let the receiver to wait for sender to actually write the message to buffer after it has signaled the sender to send.
    // otherwise if the Yield is plugged between the sender Wait() and write message to buffer, the Receiver still get nothing new in the buffer.
    currentThread->Yield();

    full->P();
    currentThread->Yield();
    currentThread->Yield();

    ASSERT(buffer!=NULL);
    currentThread->Yield();
    *message = *buffer;
    currentThread->Yield();
    currentThread->Yield();
    currentThread->Yield();
    empty->V();
    currentThread->Yield();
    currentThread->Yield();
}

