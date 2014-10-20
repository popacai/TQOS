#include "mailbox.h"
#include "copyright.h"
#include "system.h"


Mailbox::Mailbox(char* debugName, Condition* condition, Lock* lock, Lock* buf_lock)
{
    name = debugName;
    resource = 0;
    mailbox_cond = condition; 
    mailbox_lock = lock;
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
    if (resource == 1) {
        printf("Acquire Sender!\n");
        buffer_lock->Acquire();
    }

    if (resource > 0) {
        mailbox_cond->Wait(mailbox_lock);
        if (!buffer_lock->isHeldByCurrentThread()){
                mailbox_cond->Wait(mailbox_lock);
        }
        else {
                buffer_lock->Release();
        }
        printf("send is signaled!\n");
    }
    else{
        mailbox_cond->Signal(mailbox_lock);
        mailbox_lock->Release();
    }
    buffer = &message;
    printf("send buffer:%d\n", *buffer);
}

void Mailbox::Receive(int* message){
    mailbox_lock->Acquire();
    resource--;
    printf("resource number = %d\n",resource);
    if (resource < 0) {
        mailbox_cond->Wait(mailbox_lock);
    }else {
        printf("Signal!\n");
        mailbox_cond->Broadcast(mailbox_lock);
        mailbox_lock->Release();
        currentThread->Yield();
    }
    printf("Acquire Receiver\n");
    printf("%d\n",buffer_lock->isHeldByCurrentThread());
    buffer_lock->Acquire();
    ASSERT(buffer!=NULL);
    message = buffer;
    printf("receive buffer:%d\n", *message);
    buffer_lock->Release();
}
