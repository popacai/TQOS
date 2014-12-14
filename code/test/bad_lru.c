#include "syscall.h"

char ch[128] = {'\1'};

#define code s = ch[100];
#define code_5 code code code code code
#define code_10 code_5 code_5
#define code_50 code_10 code_10 code_10 code_10 code_10
#define code_200 code_50 code_50 code_50 code_50

int main() {
    int s;
    code_200
    return 0;
}
