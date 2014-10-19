#include "test_whale.h"

void male(int arg) {
	Whale* whale = (Whale*)arg;
	currentThread->Yield();
	whale->Male();
	currentThread->Yield();
	printf("male matched\n");
}

void female(int arg) {
	Whale* whale = (Whale*)arg;
	currentThread->Yield();
	whale->Female();
	currentThread->Yield();
	printf("female matched\n");
}

void matcher(int arg) {
	Whale* whale = (Whale*)arg;
	currentThread->Yield();
	whale->Matchmaker();
	currentThread->Yield();
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
	t_female->Fork(female, args);
	t_matcher->Fork(matcher, args);

	currentThread->Yield();
	currentThread->Yield();
	currentThread->Yield();
	currentThread->Yield();
	currentThread->Yield();
	return 0;
}
