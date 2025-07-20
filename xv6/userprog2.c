// filename: agingtest.c

#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[]) {
    int i, j;

    // Loop forever, consuming CPU to trigger demotion by timeslice expiration
    for(i = 0; i < 30; i++) {
        // Busy wait loop (CPU-bound)
        for(j = 0; j < 10000000; j++) {
            // just burn time
            asm("nop");
        }

        // Occasionally sleep to let other processes run and trigger aging
        if(i % 5 == 0) {
            sleep(1);
        }

        printf(1, "iteration %d done\n", i);
    }

    exit();
}
