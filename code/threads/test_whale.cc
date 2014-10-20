#include "test_whale.h"

int male_matched = 0;
int female_matched = 0;
int matcher_matched = 0;

void male(int arg) {
	Whale* whale = (Whale*)arg;
	currentThread->Yield();
	whale->Male();
	currentThread->Yield();
	male_matched++;
}

void female(int arg) {
	Whale* whale = (Whale*)arg;
	currentThread->Yield();
	whale->Female();
	currentThread->Yield();
	female_matched++;
}

void matcher(int arg) {
	Whale* whale = (Whale*)arg;
	currentThread->Yield();
	whale->Matchmaker();
	currentThread->Yield();
	matcher_matched++;
}

int test_simple_whale() {
	Whale* whale = new Whale("whale");
	Thread *t_male = new Thread("male");
	Thread *t_female = new Thread("female");
	Thread *t_matcher = new Thread("matcher");

	int args = (int)whale;

	t_male->Fork(male, args);
	currentThread->Yield();
	t_female->Fork(female, args);
	currentThread->Yield();
	t_matcher->Fork(matcher, args);

	currentThread->Yield();
	currentThread->Yield();
	currentThread->Yield();
	currentThread->Yield();
	currentThread->Yield();
	currentThread->Yield();
	currentThread->Yield();
	currentThread->Yield();
	currentThread->Yield();
	currentThread->Yield();
	currentThread->Yield();
	currentThread->Yield();
	currentThread->Yield();
	currentThread->Yield();
	currentThread->Yield();
	ASSERT(matcher_matched == 1);
	ASSERT(female_matched == 1);
	ASSERT(male_matched == 1);
	printf("SUCCESS\n");
	return 1;
}

int test_no_whale() {
    Whale* whale = new Whale("whale");
	Thread *t_male = new Thread("male");
	Thread *t_male2 = new Thread("male2");
	Thread *t_matcher = new Thread("matcher");

	int args = (int)whale;

	t_male->Fork(male, args);
	t_male2->Fork(male, args);
	t_matcher->Fork(matcher, args);

	currentThread->Yield();
	currentThread->Yield();
	currentThread->Yield();
	currentThread->Yield();
	currentThread->Yield();
	ASSERT(matcher_matched == 0);
	ASSERT(female_matched == 0);
	ASSERT(male_matched == 0);
	printf("SUCCESS\n");
    return 0;
}

int test_more_males() {
    Whale* whale = new Whale("whale");
	Thread *t_male;
	Thread *t_female = new Thread("female");
	Thread *t_matcher = new Thread("matcher");

	int args = (int)whale;

	t_female->Fork(female, args);
	currentThread->Yield();
	t_matcher->Fork(matcher, args);
	currentThread->Yield();
	for (int i = 0; i < 5; i++) {
        t_male = new Thread("male");
        t_male->Fork(male, args);
        currentThread->Yield();
	}

	currentThread->Yield();
	currentThread->Yield();
	currentThread->Yield();
	currentThread->Yield();
	currentThread->Yield();
	ASSERT(matcher_matched == 1);
	ASSERT(female_matched == 1);
	ASSERT(male_matched == 1);
	printf("SUCCESS\n");
    return 0;
}

int test_more_males_and_females() {
    Whale* whale = new Whale("whale");
	Thread *t_male;
	Thread *t_female;
	Thread *t_matcher = new Thread("matcher");
	int args = (int)whale;
	for (int i = 0; i < 5; i++) {
        t_male = new Thread("male");
        t_male->Fork(male, args);
	}

	for (int i = 0; i < 5; i++) {
        t_female = new Thread("female");
        t_female->Fork(female, args);
	}

    currentThread->Yield();
	t_matcher->Fork(matcher, args);
	currentThread->Yield();

	currentThread->Yield();
	currentThread->Yield();
	currentThread->Yield();
	currentThread->Yield();
	currentThread->Yield();
	ASSERT(matcher_matched == 1);
	ASSERT(female_matched == 1);
	ASSERT(male_matched == 1);
	printf("SUCCESS\n");
    return 0;
}

int test_males_females_matchers() {
    Whale* whale = new Whale("whale");
	Thread *t_male;
	Thread *t_female;
	Thread *t_matcher;
	int args = (int)whale;


	for (int i = 0; i < 5; i++) {
	    t_matcher = new Thread("matcher");
	    t_matcher->Fork(matcher, args);
        currentThread->Yield();
	}
	for (int i = 0; i < 5; i++) {
        t_male = new Thread("male");
        t_male->Fork(male, args);
        currentThread->Yield();
	}

	for (int i = 0; i < 5; i++) {
        t_female = new Thread("female");
        t_female->Fork(female, args);
        currentThread->Yield();
	}

    currentThread->Yield();
	currentThread->Yield();

	currentThread->Yield();
	currentThread->Yield();
	currentThread->Yield();
	currentThread->Yield();
	currentThread->Yield();
	ASSERT(matcher_matched == 5);
	ASSERT(female_matched == 5);
	ASSERT(male_matched == 5);
	printf("SUCCESS\n");
	return 1;
}

int test_random_whales() {
    Whale* whale = new Whale("whale");
	Thread *t_male;
	Thread *t_female;
	Thread *t_matcher;
	int args = (int)whale;

	for (int i = 0; i < 100; i++) {

	}
}
