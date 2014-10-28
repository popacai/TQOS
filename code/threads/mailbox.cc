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
    //printf("send is now!\n");
    mailbox_lock->Acquire();
    resource++;
    //printf("resource number = %d\n",resource);
    if (resource > 0) {
        mailbox_cond->Wait(mailbox_lock);
        //printf("send is signaled!\n");
    }
    else{
        
        mailbox_cond->Signal(mailbox_lock);
    }
    mailbox_lock->Release();

    empty->P();

    buffer_lock->Acquire();
    while (b_count == 1) {
        buffer_cond->Wait(buffer_lock);
        //buffer=&message;
        //b_count=1;
        //buffer_cond->Wait(mailbox_lock);
    }
    currentThread->Yield();
    buffer = &message;
    currentThread->Yield();
    b_count = 1;
    buffer_cond->Signal(buffer_lock);
    //buffer_cond->Signal(mailbox_lock);
    
    buffer_lock->Release();
    printf("send message is=%d\n",*buffer);
    full->V();

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
    buffer_lock->Acquire();
    
    while (b_count == 0) {
        buffer_cond->Wait(buffer_lock);
        //buffer_cond->Wait(mailbox_lock);
    }
    ASSERT(buffer!=NULL);
    currentThread->Yield();
    *message = *buffer;
    currentThread->Yield();
    b_count = 0;
    //buffer_cond->Signal(mailbox_lock);
    buffer_cond->Signal(buffer_lock);
    buffer_lock->Release();
    //mailbox_lock->Release();
    empty->V();
}
