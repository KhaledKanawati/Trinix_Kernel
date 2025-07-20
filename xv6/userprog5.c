// many.c
#include "types.h"
#include "stat.h"
#include "user.h"

int main() {
  for (int i = 0; i < 20; i++) {
    if (fork() == 0) {
      for (int j = 0; j < 100000000; j++) ;
      exit();
    }
  }

  for (int i = 0; i < 20; i++)
    wait();

  exit();
}
