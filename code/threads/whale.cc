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
	if (*male > 0 && *female > 0 && *matcher > 0) {
		return true;
	} else {
		(*male)--;
		(*female)--;
		(*matcher)--;
		return false;
	}
}

void Whale::Male() {
	this->lock->Acquire();

	(*male)++;
	this->match->Signal(lock);
	while (!isMatch()) {
		this->match->Wait(lock);
	}

	this->lock->Release();
}

void Whale::Female() {
	this->lock->Acquire();

	(*female)++;
	this->match->Signal(lock);
	while (!isMatch()) {
		this->match->Wait(lock);
	}
	this->lock->Release();
}

void Whale::Matchmaker() {
	this->lock->Acquire();

	(*matcher)++;
	this->match->Signal(lock);
	while (!isMatch()) {
		this->match->Wait(lock);
	}
	this->lock->Release();
}
