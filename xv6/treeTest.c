#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

void touch(char *filename) {
  int fd = open(filename, O_CREATE | O_WRONLY);
  if (fd >= 0)
    close(fd);
  else
    printf(1, "Failed to create file: %s\n", filename);
}

int main() {
  mkdir("a");
  mkdir("a/b");
  mkdir("a/b/c");

  touch("a/file1");
  touch("a/b/file2");
  touch("a/b/c/file3");

  exit();
}
