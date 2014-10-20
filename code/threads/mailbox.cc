#include "mailbox.h"
#include "copyright.h"
#include "system.h"

Mailbox::Mailbox(char* debugName, Condition* condition, Lock* lock,Condition* buf_cond, Lock* buf_lock)
{
    name = debugName;
    resource = 0;
    mailbox_cond = condition; 
    mailbox_lock = lock;
    buffer_cond = buf_cond;
    buffer_lock = buf_lock;
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
    printf("resource number = %d\n",resource);
    if (resource > 0) {
        mailbox_cond->Wait(mailbox_lock);
        printf("send is signaled!\n");
    }
    else{
        mailbox_cond->Signal(mailbox_lock);
        mailbox_lock->Release();
    }
    buffer = &message;
    buffer_lock->Acquire();
    buffer_cond->Signal(buffer_lock);
    buffer_lock->Release();
    printf("buffer is=%d\n",*buffer);
}

void Mailbox::Receive(int* message){
    mailbox_lock->Acquire();
    resource--;
    printf("resource number = %d\n",resource);
    if (resource < 0) {
        mailbox_cond->Wait(mailbox_lock);
    }
    else{
        printf("Signal!\n");
        mailbox_cond->Signal(mailbox_lock);
        mailbox_lock->Release();
    }
    // add another condition let the receiver to wait for sender to actually write the message to buffer after it has signaled the sender to send.
    // otherwise if the Yield is plugged between the sender Wait() and write message to buffer, the Receiver still get nothing new in the buffer.
    buffer_lock->Acquire();
    buffer_cond->Wait(buffer_lock);
    ASSERT(buffer!=NULL);
    *message = *buffer;
    printf("message is %d\n",*message);
}
