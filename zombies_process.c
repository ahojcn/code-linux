#include <stdio.h>
#include <unistd.h>

int main() {
  pid_t ret = fork();

  if (ret == 0) {
    int cnt = 3;
    while (cnt > 0) {
      printf("cnt %d\n", cnt);
      sleep(1);
      --cnt;
    }
  } else if (ret > 0) {
    printf("father process...\n");
    while (1) {
      sleep(1);
    }
  } else {
    perror("fork");
  }

  sleep(1);
  return 0;
}
