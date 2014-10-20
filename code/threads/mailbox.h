
#ifndef MAILBOX_H
#define MAILBOX_H

#include "synch.h"

class Mailbox {
public:
    Mailbox(char* debugName, Condition* cond, Lock* lock, Lock* buf_lock);
    ~Mailbox();
    void Send(int message); // increase the resource
    void Receive(int* message);          // decrease the resource
private:
    int* buffer;
    char* name;
    Condition* mailbox_cond;
    Lock* mailbox_lock;
    Lock* buffer_lock;
    int resource;
};    

#endif

