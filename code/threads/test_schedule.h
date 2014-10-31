#include "synch.h"
#include "system.h"

#ifndef TEST_SCHEDULE_H
#define TEST_SCHEDULE_H

int test_priority_lock();
int test_priority_cond();
int test_priority_cv();
int test_inherient_lock();
int test_complex_inherient_lock();
#endif
