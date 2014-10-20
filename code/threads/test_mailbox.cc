#include "test_mailbox.h"

void test_send(int _args) {
    int* args = (int*) _args;
    Lock* lock = (Lock*) args[0];
    Condition* cond = (Condition*) args[1];
    int message = (int) args[2];
    Mailbox* mailbox = new Mailbox("simplesend", cond, lock);
    mailbox->Send(message);
}
    
void test_receive(int* _args) {
    int* args = (int*) _args;
    Lock* lock = (Lock*) args[0];
    Condition* cond = (Condition*) args[1];
    int* message = (int) args[2];
    Mailbox* mailbox = new Mailbox("simplereceive", cond, lock);
    mailbox->Receive(message);
}

int test_simple_sendreceive() {
    Lock* lock = new Lock("mailboxlock");
    Condition* cond = new Condition("mailboxcond");
    int message;
    int* args = new int[3];
    args[0] = (int)lock;
    args[1] = (int)cond;
    args[3] = message;
    
    Thread* t;
    t = new Thread("send");
    message = 1;
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
    Condition* cond = new Condition("mailboxcond");
    int message;
    int* args = new int[3];
    args[0] = (int)lock;
    args[1] = (int)cond;
    args[3] = message;
    
    Thread* t;
    t = new Thread("receive");
    message = 1;
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
    

