#include "test_mailbox.h"

void mult_sender_one_receiver_sender(int _args) {
    int* args = (int*)_args;
    Mailbox* mailbox = (Mailbox*)args[0];
    int message = (int) args[1];
    mailbox->Send(message);
}

void mult_sender_one_receiver_receiver(int _args) {
    int* args = (int*)_args;
    Mailbox* mailbox = (Mailbox*)args[0];
    int* message=new int;
    printf("Receive Begin!\n");
    //printf("buffer lock is held? %d\n",mailbox->getBufferLock()->isHeldByCurrentThread());
    mailbox->Receive(message);
    ASSERT(mailbox->getBuffer()!=NULL);
    ASSERT(message!=NULL);
    printf("Receive message is = %d\n",*message);
}   
    

void test_mult_sender_one_receiver() {
    Condition* mailbox_cond = new Condition("mailbox_cond");
    Lock* mailbox_lock = new Lock("mailbox_lock");
    Mailbox* mailbox = new Mailbox("mailbox",mailbox_cond,mailbox_lock);
    

    int* args = new int[2];
    args[0] = (int)mailbox;
    
    Thread* t = new Thread("mailbox_thread");
    args[1] = 1;
    t->Fork(mult_sender_one_receiver_sender,(int)args);   
    currentThread->Yield();
    
    args[1] = 2;
    t = new Thread("sender1");
    t->Fork(mult_sender_one_receiver_sender,(int)args);
    currentThread->Yield();
    
    args[1] = 3;
    t = new Thread("sender2");
    t->Fork(mult_sender_one_receiver_sender,(int)args);
    currentThread->Yield();
    
    t = new Thread("receiver");
    t->Fork(mult_sender_one_receiver_receiver,(int)args);
    
    //printf("buffer lock is held? %d\n",buf_lock->isHeldByCurrentThread());
    
    //currentThread->Yield();   
    
}
