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
    Condition* cond = new Condition("mailboxcond");
    Mailbox* mailbox = new Mailbox("simplemailbox", cond, lock);
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
    Condition* cond = new Condition("mailboxcond");
    Mailbox* mailbox = new Mailbox("simplemailbox", cond, lock);
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
    

