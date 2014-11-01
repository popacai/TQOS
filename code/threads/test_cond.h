#include "synch.h"
#include "system.h"

#ifndef TEST_COND_H
#define TEST_COND_H 

int test_single_cond();
int test_nowaiter(int signal_or_broadcast);
int test_broadcast_cond();
int test_signal_cond();
#endif
