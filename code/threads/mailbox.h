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

    Semaphore* p1_empty;
    Semaphore* p1_full;
    Semaphore* p2_empty;
    Semaphore* p2_full;

    int ack;


    int resource;
};    

#endif
