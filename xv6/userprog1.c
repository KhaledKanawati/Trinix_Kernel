// sleepy.c
#include "types.h"
#include "stat.h"
#include "user.h"

int main() {
  sleep(200);  // sleep to trigger aging
  printf(1, "woke up!\n");
  exit();
}
