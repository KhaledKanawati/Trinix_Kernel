#include "types.h"
#include "stat.h"
#include "user.h"

int main() {
  for (int i = 0; i < 50; i++) {
    printf(1, "io bound %d\n", i);
    sleep(10);
  }
  exit();
}
