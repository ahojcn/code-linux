// 
// 进程等待之 wait
//

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

int main() {
  pid_t ret = fork();

  if (ret > 0) {
    // father
    int status = 0;
    // 阻塞，等待子进程结束，返回值是子进程的 pid
    pid_t result = wait(&status);
    printf("child exit code %d, result %d\n", status, result);

    if (status & 0xff) {
      // 异常终止
      printf("异常终止：信号为 %d\n", status & 0x7f);
    } else {
      // 正常终止
      printf("正常终止：退出码：%d\n", (status >> 8) & 0xff);
    }

  } else if (ret == 0) {
    // child
    int cnt = 3;
    while (cnt > 0) {
      printf("child %d\n", getpid());
      sleep(1);
      --cnt;
    }
    exit(3);

  } else {
    perror("fork");
  }

  return 0;
}
