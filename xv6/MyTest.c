#include "types.h"
#include "stat.h"
#include "user.h"
#include "syscall.h"
//Random test to test if scount is keeping track properly, by seeing the difference
int
main(void)
{
    int before[SYSCALLSMETHODS], after[SYSCALLSMETHODS];

    //Get counts before any test
    if (getsystemcallscount(before) < 0) {
        printf(2, "scount: error getting initial counts\n");
        exit();
    }

    //Perform some system calls
    getpid();
    write(1, "hello\n", 6);
    sleep(1);
    uptime();
    if (getsystemcallscount(after) < 0) {
        printf(2, "scount: error getting final counts\n");
        exit();
    }
    char *names[] = {
      "", "fork","exit","wait","pipe","read","kill","exec",
      "fstat","chdir","dup","getpid","sbrk","sleep","uptime",
      "open","write","mknod","unlink","link","mkdir","close",
      "getsystemcallscount"
    };
    //Check to show difference(guarantee that system calls are beign tracked of properly)
    for (int i = 1; i < SYSCALLSMETHODS; i++) {
        int delta = after[i] - before[i];
        printf(1, "%s: %d\n", names[i], delta);
    }

    exit();
}
