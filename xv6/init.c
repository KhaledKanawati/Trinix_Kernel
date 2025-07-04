// init: The initial user-level program
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

char *argv[] = {"sh", 0};
#define MAX_INPUT 32
#define USERS_FILE "users.txt"//Used for storing passwords for future login after signup

//Creation of users.txt adn initialization of values inside
void init_users_file() {
  int fd = open(USERS_FILE, O_RDONLY);
  if (fd < 0) {
    fd = open(USERS_FILE, O_WRONLY | O_CREATE);
    if (fd >= 0) {
      write(fd, "admin password\n", 15);
      close(fd);
    }
  } else {
    close(fd);
  }
}


//Allows for signing up by entering a new username and password(USERNAME AND PASSWORD ARE BOTH CASE SENSITIVE)
int signUp(void)
{
  char user[MAX_INPUT];
  char pass[MAX_INPUT];

  while (1) {
    printf(1, "New Username: ");
    if (gets(user, MAX_INPUT) == 0 || user[0] == '\n' || user[0] == ' ') {
      printf(1, "\nError reading username.\n");
      continue;
    }
    remove_newline(user);

    printf(1, "New Password: ");
    if (gets(pass, MAX_INPUT) == 0 || pass[0] == '\n' || pass[0] == ' ') {
      printf(1, "\nError reading password.\n");
      continue;
    }
    remove_newline(pass);

    if (check_credentials(user, pass)) {
      printf(1, "User already exists. Choose another.\n");
    } else {
      //Add user to the file in case unique/new username
      char temp[1];
      // Reopen in write-only, append mode
      int fd = open(USERS_FILE, O_RDWR  | O_CREATE);
      if (fd < 0) {
        printf(1, "Could not open users file.\n");
        return 0;
      }
      while (read(fd, temp, 1) == 1){;}
      //Add the new user and password
      write(fd, user, strlen(user));
      write(fd, " ", 1);
      write(fd, pass, strlen(pass));
      write(fd, "\n", 1);
      close(fd);
      printf(1, "Signup successful!\n");
      break;
    }
  }
  return 1;
}


//Opens file and checks if the username password combination exists or not, format of file is {user password} on each line
int check_credentials(char *user, char *pass) {
  int fd = open(USERS_FILE, O_RDONLY);
  if (fd < 0)
    return 0; //Signal error

  char buf[256];
  int n;
  char line[70];
  int line_idx = 0;

  while ((n = read(fd, buf, sizeof(buf))) > 0) {
    for (int i = 0; i < n; i++) {
      if (buf[i] == '\n' || line_idx >= sizeof(line) - 1) {
        line[line_idx] = '\0';

        //Extract username and password manually(char by char)
        char stored_user[MAX_INPUT];
        char stored_pass[MAX_INPUT];
        int j = 0, k = 0;
        while (line[j] != ' ') {//Skip till space is found
          stored_user[k++] = line[j++];
        }
        stored_user[k] = '\0';
        if (line[j] == ' ')//Skip spaces
          j++;

        k = 0;
        while (line[j] != '\0') {
          stored_pass[k++] = line[j++];
        }
        stored_pass[k] = '\0';//Extracted password

        if (strcmp(stored_user, user) == 0 &&
            strcmp(stored_pass, pass) == 0) {
          close(fd);
          return 1;
        }
        else if(strcmp(stored_user, user) == 0){//Signal that only username is compatible(useful for signup)
          close(fd);
          return 2;//Special value to distinguish between the 2 cases(pass and user vs only user)
        }
        line_idx = 0;
      } else {
        line[line_idx++] = buf[i];
      }
    }
  }

  close(fd);
  return 0;
}



// gets method appends \n, need to make sure it doesnt exist for easier time in comparing strings
// This method simply removes newline that is read
void remove_newline(char *s)
{
  int len = strlen(s);
  if (len > 0 && s[len - 1] == '\n')
    s[len - 1] = '\0';
}

int login(void)
{
  char user[MAX_INPUT];
  char pass[MAX_INPUT];

  while (1)
  {
    printf(1, "Username: ");
    if (gets(user, MAX_INPUT) == 0)
    {
      printf(1, "\nError reading input.\n");
      continue;
    }


    
    remove_newline(user);

    printf(1, "Password: ");
    if (gets(pass, MAX_INPUT) == 0)
    {
      printf(1, "\nError reading input.\n");
      continue;
    }
    remove_newline(pass);

    if (check_credentials(user, pass)==1)
    {
      printf(1, "Login successful!\n");
      return 1;
    }
    else
    {
      printf(1, "Login failed. Try again.\n");
      return 0;
    }
  }
}

int main(void)
{
  int pid, wpid;

  if (open("console", O_RDWR) < 0)
  {
    mknod("console", 1, 1);
    open("console", O_RDWR);
  }
  dup(0); // stdout
  dup(0); // stderr
  init_users_file();//Initializes users.txt
  int sign_Up = 0;
  char ans[MAX_INPUT];
  printf(1, "Welcome!\n1. Login\n2. Signup\n");
  while (1)
  {
    printf(1, "Choice: ");
    if (gets(ans, MAX_INPUT) == 0)
    {
      printf(1, "\nError reading input.\n");
      continue;
    }
    remove_newline(ans);
    if(!strcmp(ans, "Signup")){
      sign_Up = 1;
      if(signUp()){break;}
      continue;
    }
    else if(!strcmp(ans, "Login")){
      if(login()){break;}
      continue;
    }
    printf(1, "Input valid choice!\n");
  }
    for (;;)
    {
      printf(1, "init: starting sh\n");
      pid = fork();
      if (pid < 0)
      {
        printf(1, "init: fork failed\n");
        exit();
      }
      if (pid == 0)
      {
        exec("sh", argv);
        printf(1, "init: exec sh failed\n");
        exit();
      }
      while ((wpid = wait()) >= 0 && wpid != pid)
        printf(1, "zombie!\n");
    }
  exit();
}
