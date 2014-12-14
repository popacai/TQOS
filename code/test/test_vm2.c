#include "syscall.h"
// modify data part, most page replacement should be dirty.
int arr[10000];

int main() {
    int k = 0;
    int i;
    for(i = 1; i < 10000; i++) {
        k = arr[i-1];
        arr[i] = k;
    }
    return 0;
}
