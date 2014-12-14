#include "synch.h"


class Whale {
public:
    Whale(char* debugName);		// initialize condition to
    // "no one waiting"
    ~Whale();
    char* getName() {
        return (name);
    }

    void Male(); 	// these are the 3 operations on
    // condition variables; releasing the
    // lock and going to sleep are
    // *atomic* in Wait()
    void Female();   // conditionLock must be held by
    void Matchmaker();
    // these operations

private:
    bool isMatch();
    char* name;
    // plus some other stuff you'll need to define

    Condition* match;

    int* male;
    int* female;
    int* matcher;

    int* readyMale;
    int* readyFemale;
    int* readyMatcher;

    Lock* lock;

};
