// threadtest.cc
//      Simple test case for the threads assignment.
//
//      Create two threads, and have them context switch
//      back and forth between themselves by calling Thread::Yield,
//      to illustratethe inner workings of the thread system.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "test_lock.h"
#include "test_cond.h"
#include "test_semaphore.h"
#include "test_join.h"
#include "test_priority.h"
#include "test_whale.h"
#include "test_mailbox.h"
#include "test_schedule.h"

// testnum is set in main.cc
int testnum = 1;

//----------------------------------------------------------------------
// SimpleThread
//      Loop 5 times, yielding the CPU to another ready thread
//      each iteration.
//
//      "which" is simply a number identifying the thread, for debugging
//      purposes.
//----------------------------------------------------------------------

void
SimpleThread(int which)
{
    int num;

    for (num = 0; num < 5; num++) {
        printf("*** thread %d looped %d times\n", which, num);
        currentThread->Yield();
    }
}

//----------------------------------------------------------------------
// ThreadTest1
//      Set up a ping-pong between two threads, by forking a thread
//      to call SimpleThread, and then calling SimpleThread ourselves.
//----------------------------------------------------------------------

void
ThreadTest1()
{
    DEBUG('t', "Entering ThreadTest1");

    Thread *t = new Thread("forked thread");

    t->Fork(SimpleThread, 1);
    SimpleThread(0);
}

//----------------------------------------------------------------------
// ThreadTest
//      Invoke a test routine.
//----------------------------------------------------------------------

void
ThreadTest()
{
    switch (testnum) {
    case 1:
        ThreadTest1();
        break;
    case 2:
        test_semaphore();
        break;
    case 10:
        test_pass_arguments();
        break;
    case 11:
        test_single_lock();
        break;
    case 12:
        test_holder();
        break;
    case 21:
        test_delete();
        break;
    case 22:
        test_release_not_held_by_itself();
        break;
    case 23:
        test_broadcast_cond();
        break;
    case 26:
        test_release_not_held_by_allthreads();
        break;
    case 27:
        test_require_lock_twice();
        break;
    case 24:
        test_nowaiter(0); //signal with no waiter
        break;
    case 25:
        test_nowaiter(1); //broadcast with no waiter
        break;
    case 31:
        test_single_cond();
        break;
    case 32:
        test_signal_cond();
        break;
    case 100:
        test_simple_whale();
        break;
    case 101:
        test_no_whale();
        break;
    case 102:
        test_more_males();
        break;
    case 103:
        test_more_males_and_females();
        break;
    case 104:
        test_males_females_matchers();
        break;
    case 105:
        test_random_whales();
        break;
    case 201:
        test_simple_sendreceive();
        break;
    case 202:
        test_simple_receivesend();
        break;
    case 203:
        test_mult_sender_one_receiver();
        break;
    case 200:
        test_join();
        break;
    case 301:
        test_destroy_after_join();
        break;
    case 302:
        test_child_finish_before_join();
        break;
    case 303:
        test_cannot_join_itself();
        break;
    case 304:
        test_join_notjoinable_thread();
        break;
    case 305:
        test_join_must_fork();
        break;
    case 306:
        test_must_not_join_twice();
        break;
    case 400:
        test_priority();
        break;
    case 1000:
        test_priority_lock();
        break;
    case 1001:
        test_priority_cond();
        break;
    case 1002:
        test_priority_broadcast();
        break;
    case 1003:
        test_priority_cv();
        break;
    case 1004:
        test_inherient_lock();
        break;
    case 1005:
        test_complex_inherient_lock();
        break;
    default:
        printf("No test specified.\n");
        break;
    }
}

