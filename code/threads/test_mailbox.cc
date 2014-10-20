#include "test_mailbox.h"

void test_send(int _args) {
    int* args = (int*) _args;
    int message = (int) args[1];
    Mailbox* mailbox = (Mailbox*) args[0];
    mailbox->Send(message);
}
    
void test_receive(int _args) {
    int* args = (int*) _args;
    int* message = NULL;
    Mailbox* mailbox = (Mailbox*) args[0];
    mailbox->Receive(message);
}

int test_simple_sendreceive() {
    Lock* lock = new Lock("mailboxlock");
    Lock* bufflock = new Lock("bufflock");
    Condition* cond = new Condition("mailboxcond");
    Mailbox* mailbox = new Mailbox("simplemailbox", cond, lock, bufflock);
    int message=1;
    int* args = new int[2];
    args[0] = (int)mailbox;
    args[1] = message;
    
    Thread* t;
    t = new Thread("send");
    t->Fork(test_send, (int)args);
    currentThread->Yield();
    
    ASSERT(!cond->waitqueue_isempty());
    printf("Send is wait.\n");

    t = new Thread("receive");
    t->Fork(test_receive, (int)args);
    currentThread->Yield();

    ASSERT(cond->waitqueue_isempty());
    printf("Send is received.\n");

    return 1;
}

int test_simple_receivesend() {
    Lock* lock = new Lock("mailboxlock");
    Lock* bufflock = new Lock("bufflock");
    Condition* cond = new Condition("mailboxcond");
    Mailbox* mailbox = new Mailbox("simplemailbox", cond, lock, bufflock);
    int message = 2;
    int* args = new int[2];
    args[0] = (int)mailbox;
    args[1] = message;
    
    Thread* t;
    t = new Thread("receive");
    t->Fork(test_receive, (int)args);
    currentThread->Yield();
    
    ASSERT(!cond->waitqueue_isempty());
    printf("Receive is wait.\n");

    t = new Thread("send");
    t->Fork(test_send, (int)args);
    currentThread->Yield();

    ASSERT(cond->waitqueue_isempty());
    printf("Send is received.\n");

    return 1;
}
    
void mult_sender_one_receiver_sender(int _args) {
    int* args = (int*)_args;
    Mailbox* mailbox = (Mailbox*)args[0];
    int message = (int) args[1];
    mailbox->Send(message);
}

void mult_sender_one_receiver_receiver(int _args) {
    int* args = (int*)_args;
    Mailbox* mailbox = (Mailbox*)args[0];
    int* message = NULL;
    printf("Receive Begin!\n");
    mailbox->Receive(message);
    ASSERT(message!=NULL);
    printf("Receive message is = %d\n",*message);
}   
    
void test_mult_sender_one_receiver() {
    Condition* mailbox_cond = new Condition("mailbox_cond");
    Lock* mailbox_lock = new Lock("mailbox_lock");
    Lock* buf_lock = new Lock("buffer_lock");
    Mailbox* mailbox = new Mailbox("mailbox",mailbox_cond,mailbox_lock,buf_lock);

    int* args = new int[2];
    args[0] = (int)mailbox;
    
    Thread* t = new Thread("mailbox_thread");
    args[1] = 1;
    t->Fork(mult_sender_one_receiver_sender,(int)args);   
    currentThread->Yield();
    
    args[1] = 2;
    t->Fork(mult_sender_one_receiver_sender,(int)args);
    currentThread->Yield();
    
    args[1] = 3;
    t->Fork(mult_sender_one_receiver_sender,(int)args);
    currentThread->Yield();
    
    t->Fork(mult_sender_one_receiver_receiver,(int)args);
    
    currentThread->Yield();   
    
}
