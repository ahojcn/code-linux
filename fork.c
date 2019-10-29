#include <stdio.h>
#include <unistd.h>

int main() {
  pid_t ret = fork();

  if (ret == 0) {
    printf("child process, ret == 0, pid = %d, ppid = %d\n", getpid(), getppid());
  } else if (ret > 0) {
    printf("father process, ret > 0, pid = %d, ppid = %d\n", getpid(), getppid());
  } else {
    perror("fork");
  }

  sleep(1);
  return 0;
}
