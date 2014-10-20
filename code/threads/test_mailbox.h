#include "synch.h"
#include "mailbox.h"
#include "thread.h"
#include "system.h"

#ifndef TEST_MAILBOX_H
#define TEST_MAILBOX_H

int test_simple_sendreceive();
int test_simple_receivesend();
void test_mult_sender_one_receiver();

#endif
