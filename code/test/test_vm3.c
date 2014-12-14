#include "syscall.h"
// only access a few elements, page in number should be small.
// demonstrate that only accessed part of program is loaded into physical memory.
int arr[10000];

int main() {
    int k = 0;
    int i;
    for(i = 1; i < 5; i++) {
        // do nothing here
    }
    return 0;
}
