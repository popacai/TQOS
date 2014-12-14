#include "test_mailbox.h"

int sum = 0;
int turn = 0;

void test_send(int _args) {
    int* args = (int*) _args;
    int message = (int) args[1];
    Mailbox* mailbox = (Mailbox*) args[0];
    mailbox->Send(message);
}

void test_receive(int _args) {
    int* args = (int*) _args;
    int* message = new int;
    Mailbox* mailbox = (Mailbox*) args[0];
    mailbox->Receive(message);
    //ASSERT(message!=NULL);
}

int test_simple_sendreceive() {
    Mailbox* mailbox = new Mailbox("simplemailbox");//, bufflock);
    int message=1;
    int* args = new int[2];
    args[0] = (int)mailbox;
    args[1] = message;

    Thread* t;
    t = new Thread("send");
    t->Fork(test_send, (int)args);
    currentThread->Yield();

    printf("Send is wait.\n");

    t = new Thread("receive");
    t->Fork(test_receive, (int)args);
    currentThread->Yield();

    printf("Send is received.\n");

    return 1;
}

int test_simple_receivesend() {
    Mailbox* mailbox = new Mailbox("simplemailbox");//, bufflock);
    int message = 2;
    int* args = new int[2];
    args[0] = (int)mailbox;
    args[1] = message;

    Thread* t;
    t = new Thread("receive");
    t->Fork(test_receive, (int)args);
    currentThread->Yield();

    printf("Receive is wait.\n");

    t = new Thread("send");
    t->Fork(test_send, (int)args);
    currentThread->Yield();

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
    int* message=new int;
    mailbox->Receive(message);
    ASSERT(mailbox->getBuffer()!=NULL);
    ASSERT(message!=NULL);
    printf("Receive message is = %d\n",*message);
    sum += *message;
    turn ++;
    if (turn == 4) {
        ASSERT(sum == 10);
        printf("SUCCESS\n");
    }
}





void test_mult_sender_one_receiver() {
    /*
    Mailbox can be tested based on fuzzy.sh
    All the Yield instruction can be randomly ignored!
    */

    Mailbox* mailbox = new Mailbox("mailbox");

    int* args = new int[2];
    args[0] = (int)mailbox;

    Thread* t;

    args[1] = 3;
    t = new Thread("sender3");
    t->Fork(mult_sender_one_receiver_sender,(int)args);
    currentThread->Yield();
    currentThread->Yield();
    currentThread->Yield();

    t = new Thread("receiver2");
    t->Fork(mult_sender_one_receiver_receiver,(int)args);
    currentThread->Yield();
    currentThread->Yield();
    currentThread->Yield();

    t = new Thread("receiver1");
    t->Fork(mult_sender_one_receiver_receiver,(int)args);
    currentThread->Yield();
    currentThread->Yield();
    currentThread->Yield();

    Thread* t1;
    t1 = new Thread("receiver3");
    t1->Fork(mult_sender_one_receiver_receiver,(int)args);
    currentThread->Yield();
    currentThread->Yield();
    currentThread->Yield();

    args = new int[2];
    args[0] = (int)mailbox;
    args[1] = 2;
    t = new Thread("sender2");
    t->Fork(mult_sender_one_receiver_sender,(int)args);
    currentThread->Yield();
    currentThread->Yield();
    currentThread->Yield();

    t = new Thread("sender1");
    args = new int[2];
    args[0] = (int)mailbox;
    args[1] = 1;
    t->Fork(mult_sender_one_receiver_sender,(int)args);
    currentThread->Yield();
    currentThread->Yield();
    currentThread->Yield();

    args = new int[2];
    args[0] = (int)mailbox;
    args[1] = 4;
    t = new Thread("sender4");
    t->Fork(mult_sender_one_receiver_sender,(int)args);
    currentThread->Yield();

    t = new Thread("receiver4");
    t->Fork(mult_sender_one_receiver_receiver,(int)args);
    for (int i = 0; i < 1000; i++) {
        currentThread->Yield();
    }
    ASSERT(turn == 4);
}
