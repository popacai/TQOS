#include "synch.h"
#include "system.h"

#ifndef TEST_JOIN_H
#define TEST_JOIN_H

int test_join();
int test_destroy_after_join();
int test_child_finish_before_join();
int test_cannot_join_itself();
int test_join_notjoinable_thread();
int test_join_must_fork();
int test_must_not_join_twice();

#endif
