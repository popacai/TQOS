// synch.cc
//	Routines for synchronizing threads.  Three kinds of
//	synchronization routines are defined here: semaphores, locks
//   	and condition variables (the implementation of the last two
//	are left to the reader).
//
// Any implementation of a synchronization routine needs some
// primitive atomic operation.  We assume Nachos is running on
// a uniprocessor, and thus atomicity can be provided by
// turning off interrupts.  While interrupts are disabled, no
// context switch can occur, and thus the current thread is guaranteed
// to hold the CPU throughout, until interrupts are reenabled.
//
// Because some of these routines might be called with interrupts
// already disabled (Semaphore::V for one), instead of turning
// on interrupts at the end of the atomic operation, we always simply
// re-set the interrupt state back to its original value (whether
// that be disabled or enabled).
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "synch.h"
#include "system.h"
#include <string.h>

//----------------------------------------------------------------------
// Semaphore::Semaphore
// 	Initialize a semaphore, so that it can be used for synchronization.
//
//	"debugName" is an arbitrary name, useful for debugging.
//	"initialValue" is the initial value of the semaphore.
//----------------------------------------------------------------------

Semaphore::Semaphore(char* debugName, int initialValue)
{
    name = debugName;
    value = initialValue;
    queue = new List;
}

//----------------------------------------------------------------------
// Semaphore::Semaphore
// 	De-allocate semaphore, when no longer needed.  Assume no one
//	is still waiting on the semaphore!
//----------------------------------------------------------------------

Semaphore::~Semaphore()
{
    delete queue;
}

//----------------------------------------------------------------------
// Semaphore::P
// 	Wait until semaphore value > 0, then decrement.  Checking the
//	value and decrementing must be done atomically, so we
//	need to disable interrupts before checking the value.
//
//	Note that Thread::Sleep assumes that interrupts are disabled
//	when it is called.
//----------------------------------------------------------------------

void
Semaphore::P()
{
    IntStatus oldLevel = interrupt->SetLevel(IntOff);	// disable interrupts

    while (value == 0) { 			// semaphore not available
        queue->Append((void *)currentThread);	// so go to sleep
        currentThread->Sleep();
    }
    value--; 					// semaphore available,
    // consume its value

    (void) interrupt->SetLevel(oldLevel);	// re-enable interrupts
}

//----------------------------------------------------------------------
// Semaphore::V
// 	Increment semaphore value, waking up a waiter if necessary.
//	As with P(), this operation must be atomic, so we need to disable
//	interrupts.  Scheduler::ReadyToRun() assumes that threads
//	are disabled when it is called.
//----------------------------------------------------------------------

void
Semaphore::V()
{
    Thread *thread;
    IntStatus oldLevel = interrupt->SetLevel(IntOff);

    thread = (Thread *)queue->Remove();
    if (thread != NULL)	   // make thread ready, consuming the V immediately
        scheduler->ReadyToRun(thread);
    value++;
    (void) interrupt->SetLevel(oldLevel);
}

// Dummy functions -- so we can compile our later assignments
// Note -- without a correct implementation of Condition::Wait(),
// the test case in the network assignment won't work!
Lock::Lock(char* debugName) {
	int stringLength = strlen(debugName);
	this->name = new char[stringLength + 1];
	strcpy(this->name, debugName);

	this->queue = new List();
	this->held = false;
	this->holder = NULL;
}

Lock::~Lock() {
    IntStatus oldLevel = interrupt->SetLevel(IntOff);
	ASSERT (this->held == false);

	delete this->name;
	delete this->queue;

    (void) interrupt->SetLevel(oldLevel);
}

void Lock::Acquire() {
    IntStatus oldLevel = interrupt->SetLevel(IntOff);	// disable interrupts
    if (this->held) {
    	ASSERT (this->holder != currentThread);  //Single thread can't acquire twice
    	this->queue->Append(currentThread);
    	currentThread->Sleep();
    }
    ASSERT(this->holder == NULL);
    this->held = true;
    this->holder = currentThread;
    (void) interrupt->SetLevel(oldLevel);
}

void Lock::Release() {
    Thread *thread;
    IntStatus oldLevel = interrupt->SetLevel(IntOff);	// disable interrupts

    ASSERT (this->held);  //This should be always true
    ASSERT (this->holder)
    ASSERT (this->holder == currentThread);  //Only the thread that acquire the lock may release it?

    if (!this->queue->IsEmpty()){
        thread = (Thread *)this->queue->Remove();
        scheduler->ReadyToRun(thread);
    }

    this->held = false;
    this->holder = NULL;
    (void) interrupt->SetLevel(oldLevel);
}

bool Lock::isHeldByCurrentThread() {
	bool flag;
    IntStatus oldLevel = interrupt->SetLevel(IntOff);	// disable interrupts
    flag = this->holder == currentThread && this->held == true;
    (void) interrupt->SetLevel(oldLevel);
	return flag;
}

Condition::Condition(char* debugName) {
	int stringLength = strlen(debugName);
	this->name = new char[stringLength + 1];
	strcpy(this->name, debugName);

	this->queue = new List;
}

Condition::~Condition() {
	delete this->name;

	//TODO: Do we need to empty the queue?
	ASSERT (!this->queue->IsEmpty());

	delete this->queue;
}

void Condition::Wait(Lock* conditionLock) {
	ASSERT (conditionLock->isHeldByCurrentThread());
    IntStatus oldLevel = interrupt->SetLevel(IntOff);	// disable interrupts
	this->queue->Append(currentThread);
	conditionLock->Release();
    currentThread->Sleep();
    conditionLock->Acquire();
    (void) interrupt->SetLevel(oldLevel);
}

void Condition::Signal(Lock* conditionLock) {
	ASSERT (conditionLock->isHeldByCurrentThread());
    IntStatus oldLevel = interrupt->SetLevel(IntOff);	// disable interrupts

	Thread* thread;
	if (!this->queue->IsEmpty()) {
        thread = (Thread*)this->queue->Remove();
        scheduler->ReadyToRun(thread);
	}
    (void) interrupt->SetLevel(oldLevel);
}

void Condition::Broadcast(Lock* conditionLock) {
	ASSERT (conditionLock->isHeldByCurrentThread());
    IntStatus oldLevel = interrupt->SetLevel(IntOff);	// disable interrupts
	Thread* thread;
	while (!this->queue->IsEmpty()) {
        thread = (Thread*)this->queue->Remove();
        scheduler->ReadyToRun(thread);
	}
    (void) interrupt->SetLevel(oldLevel);
}
