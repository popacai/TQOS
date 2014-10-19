#include "test_whale.h"

void male(int arg) {
	Whale* whale = (Whale*)arg;
	whale->Male();
	currentThread->Yield();
	printf("male matched\n");
}

void female(int arg) {
	Whale* whale = (Whale*)arg;
	whale->Female();
	currentThread->Yield();
	printf("female matched\n");
}

void matcher(int arg) {
	Whale* whale = (Whale*)arg;
	whale->Matchmaker();
	currentThread->Yield();
	printf("matcher matched\n");
}

int test_simple_whale() {
	printf("test_whale\n");
	Whale* whale = new Whale("whale");
	Thread *t_male = new Thread("male");
	Thread *t_female = new Thread("female");
	Thread *t_matcher = new Thread("matcher");

	int args = (int)whale;

	t_male->Fork(male, args);
	t_female->Fork(male, args);
	t_matcher->Fork(male, args);

	currentThread->Yield();
	return 0;
}
