#include "test_lock.h"

void runThread(int args) {
	Lock* lock = (Lock*) args;
	char* name = lock->getName();
    // DEBUG('t', "ASSERT NAME");
	printf("ASSERT NAME %s\n", name);
	ASSERT(strcmp(name, "simplelock") == 0);
}

int test_mock_lock() {
	Lock *lock1 = new Lock("simplelock");
	Thread *t = new Thread("t1");
	t->Fork(runThread, (int)lock1);
	runThread((int)lock1);
	return 1;
}

