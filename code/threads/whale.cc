#include "whale.h"


Whale::Whale(char* debugName) {
	this->match = new Condition("match");
	this->lock = new Lock("lock");
	this->name = debugName;

	this->male = new int;
	this->female = new int;
	this->matcher = new int;
	*male = 0;
	*female = 0;
	*matcher = 0;

	readyMale = new int;
	readyFemale = new int;
	readyMatcher = new int;
	*readyMale = 0;
	*readyFemale = 0;
	*readyMatcher = 0;
}

Whale::~Whale() {
	delete this->match;
	delete this->lock;
	delete this->male;
	delete this->female;
}

bool Whale::isMatch() {
	// Whether there is a match happened now.
	ASSERT(lock->isHeldByCurrentThread());
	if ((*male) > 0 && (*female) > 0 && (*matcher) > 0) {
		return true;
	} else {
		return false;
	}
}

void Whale::Male() {
	this->lock->Acquire();

	(*male)++;
	if (isMatch()) {
		(*male)--;
        (*female)--;
        (*matcher)--;
        (*readyMale)++;
        (*readyFemale)++;
        (*readyMatcher)++;
        this->match->Broadcast(lock);
	}

	while (!((*readyMale) > 0)) {
		this->match->Wait(lock);
	}

	(*readyMale)--;

	this->lock->Release();
}

void Whale::Female() {
	this->lock->Acquire();

	(*female)++;
	if (isMatch()) {
		(*male)--;
        (*female)--;
        (*matcher)--;
        (*readyMale)++;
        (*readyFemale)++;
        (*readyMatcher)++;
        this->match->Broadcast(lock);
	}

	while (!((*readyFemale) > 0)) {
		this->match->Wait(lock);
	}

	(*readyFemale)--;

	this->lock->Release();
}

void Whale::Matchmaker() {
	this->lock->Acquire();

	(*matcher)++;
	if (isMatch()) {
		(*male)--;
        (*female)--;
        (*matcher)--;
        (*readyMale)++;
        (*readyFemale)++;
        (*readyMatcher)++;
        this->match->Broadcast(lock);
	}

	while (!((*readyMatcher) > 0)) {
		this->match->Wait(lock);
	}

	(*readyMatcher)--;

	this->lock->Release();
}
