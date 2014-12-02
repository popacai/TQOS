#include "syscall.h"

int main() {
     Write("exec1exec\n", 11, ConsoleOutput);
     Exec("../test/thread_yield",0,0,0);
     Write("exec2exec\n", 11, ConsoleOutput);
     Exec("../test/thread_yield",0,0,0);
     Write("exec3exec\n", 11, ConsoleOutput);
     Exec("../test/user_exception",0,0,0);
     Write("exec3exec\n", 11, ConsoleOutput);
     Exec("../test/thread_yield",0,0,0);
}
