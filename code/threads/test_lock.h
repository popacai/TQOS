#include "synch.h"
#include "system.h"

#ifndef TEST_LOCK_H
#define TEST_LOCK_H

int test_pass_arguments();
int test_single_lock();
int test_holder();
void test_release_not_held();
void release_not_held_lock();

#endif
