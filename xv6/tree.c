#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

#include "fs.h"

void tree(char *path, int depth, char *prefix);


char *strcat(char *dest, const char *src) {
  char *ptr = dest;
  while (*ptr)
    ptr++;
  while (*src)
    *ptr++ = *src++;
  *ptr = '\0';
  return dest;
}

int main(int argc, char *argv[]) {
  if (argc < 2)
    tree(".", 0, ""); //No path given: default to current directory
  else
    tree(argv[1], 0, ""); //Start from given path
  exit();
}


//Print two spaces per level of depth. i.e. root prints not spaces, its children print 2 spaces, etc...
void print_indent(int depth) {
  for (int i = 0; i < depth; i++) {
    printf(1, "  ");
  }
}

void print_branch(char *prefix, int is_last, char *name) {
  printf(1, "%s", prefix);
  if (is_last)
    printf(1, "+-- ");
  else
    printf(1, "|-- ");
  printf(1, "%s\n", name);
}

void tree(char *path, int depth, char *prefix) {
  char buf[512], *p;
  int fd, n = 0, count = 0;
  struct dirent de;
  struct stat st;

  if ((fd = open(path, 0)) < 0) {
    printf(2, "tree: cannot open %s\n", path);
    return;
  }

  if (fstat(fd, &st) < 0) {
    printf(2, "tree: cannot stat %s\n", path);
    close(fd);
    return;
  }

  //Print root directory if depth == 0
  if (depth == 0) {
    printf(1, "%s\n", path);
  }

  //Step 1: Count entries
  while (read(fd, &de, sizeof(de)) == sizeof(de)) {
    if (de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
      continue;
    count++;
  }

  //Step 2: Reset file descriptor for actual reading
  close(fd);
  fd = open(path, 0);
  if (fd < 0) {
    printf(2, "tree: cannot reopen %s\n", path);
    return;
  }

  //Step 3: Traverse
  while (read(fd, &de, sizeof(de)) == sizeof(de)) {
    if (de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
      continue;

    //Build full path: path + "/" + name
    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
      printf(1, "tree: path too long\n");
      break;
    }

    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';
    memmove(p, de.name, DIRSIZ);
    p[DIRSIZ] = 0;

    if (stat(buf, &st) < 0) {
      printf(1, "tree: cannot stat %s\n", buf);
      continue;
    }

    //Step 4: Print branch
    int is_last = (++n == count);
    print_branch(prefix, is_last, de.name);

    //Step 5: Prepare new prefix for child recursion
    char new_prefix[256];
    strcpy(new_prefix, prefix);
    if (is_last)
      strcat(new_prefix, "    ");
    else
      strcat(new_prefix, "|   ");

    if (st.type == T_DIR) {
      tree(buf, depth + 1, new_prefix);
    }
  }

  close(fd);
}