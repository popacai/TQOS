#include "synch.h"
#include "system.h"

#ifndef TEST_LOCK_H
#define TEST_LOCK_H

int test_pass_arguments();
int test_single_lock();
int test_holder();
int test_delete();
void test_release_not_held_by_itself();
void test_release_not_held_by_allthreads();
void test_require_lock_twice();

#endif
