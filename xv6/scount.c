#include "types.h"
#include "stat.h"
#include "user.h"
#include "syscall.h"

int
main(void)
{
    int cnts[SYSCALLSMETHODS];
    int n = getsystemcallscount(cnts);
    if(n < 0){
        printf(2, "scount: error fetching counts\n");
        exit();
    }

    char *names[] = {
      "", "fork","exit","wait","pipe","read","kill","exec",
      "fstat","chdir","dup","getpid","sbrk","sleep","uptime",
      "open","write","mknod","unlink","link","mkdir","close",
      "getsystemcallscount"
    };

    for(int i = 1; i < SYSCALLSMETHODS; i++){
        printf(1, "%s: %d\n", names[i], cnts[i]);
    }
    exit();
}
