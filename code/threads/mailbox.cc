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
    delete name;
    delete buffer;
    delete mailbox_cond;
    delete mailbox_lock;
    delete buffer_cond;
    delete buffer_lock;
}

void Mailbox::Send(int message){
    mailbox_lock->Acquire();
    currentThread->Yield();
    resource++;
    currentThread->Yield();
    if (resource > 0) {
    currentThread->Yield();
        mailbox_cond->Wait(mailbox_lock);
    currentThread->Yield();
    }
    else{
    currentThread->Yield();
        
        mailbox_cond->Signal(mailbox_lock);
    currentThread->Yield();
    }
    currentThread->Yield();
    mailbox_lock->Release();
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
    mailbox_lock->Acquire();
    currentThread->Yield();
    resource--;
    currentThread->Yield();
    if (resource < 0) {
    currentThread->Yield();
        mailbox_cond->Wait(mailbox_lock);
    currentThread->Yield();
    }
    else{
    currentThread->Yield();
        mailbox_cond->Signal(mailbox_lock);
    currentThread->Yield();
    }
    mailbox_lock->Release();
    currentThread->Yield();
    // add another condition let the receiver to wait for sender to actually write the message to buffer after it has signaled the sender to send.
    // otherwise if the Yield is plugged between the sender Wait() and write message to buffer, the Receiver still get nothing new in the buffer.
    currentThread->Yield();

    full->P();
    currentThread->Yield();
    *message = *buffer;
    currentThread->Yield();
    empty->V();
    currentThread->Yield();
}
