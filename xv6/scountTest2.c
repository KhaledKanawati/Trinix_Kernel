#include "types.h"
#include "stat.h"
#include "user.h"
#include "syscall.h"

int main(void) {
    int before[SYSCALLSMETHODS], after[SYSCALLSMETHODS];
    int n;

    // Get syscall counts before
    n = getsystemcallscount(before);
    if (n < 0) {
        printf(2, "scount: error fetching counts before\n");
        exit();
    }
    sleep(1);
    int fd = open("README", 0);
    if (fd >= 0) {
        char buf[16];
        int r = read(fd, buf, sizeof(buf));
        if (r > 0) {
            write(1, buf, r);
        }
        close(fd);
    }

    n = getsystemcallscount(after);
    if (n < 0) {
        printf(2, "scount: error fetching counts after\n");
        exit();
    }

    char *names[] = {
      "", "fork", "exit", "wait", "pipe", "read", "kill", "exec",
      "fstat", "chdir", "dup", "getpid", "sbrk", "sleep", "uptime",
      "open", "write", "mknod", "unlink", "link", "mkdir", "close",
      "getsystemcallscount"
    };

    printf(1, "\nSyscall counts before and after:\n");
    for (int i = 1; i < SYSCALLSMETHODS; i++) {
        printf(1, "%s:diff = %d\n",
            names[i], after[i] - before[i]);
    }

    exit();
}
