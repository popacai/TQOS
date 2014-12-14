#include "syscall.h"
// go through all vitual address, only stack page is dirty
int arr[10000];

int main() {
    char * k = 0;
    char c = 0;
    int i;
    for(i = 0; i < 41600; i++) {
        c = *k++;
        //k = 41599;
        //c = *k;
    }
    return 0;
}
