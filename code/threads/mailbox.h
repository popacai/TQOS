
#ifndef MAILBOX_H
#define MAILBOX_H

#include "synch.h"

class Mailbox {
public:
    Mailbox(char* debugName);
    ~Mailbox();
    void Send(int message); // increase the resource
    void Receive(int* message);          // decrease the resource
    //Lock* getBufferLock() {return buffer_lock;}
    int* getBuffer() {return buffer;}
private:
    int* buffer;
    char* name;
    Condition* mailbox_cond;
    Lock* mailbox_lock;
    Condition* buffer_cond;
    Lock* buffer_lock;
    int b_count;
    int resource;
};    

#endif

