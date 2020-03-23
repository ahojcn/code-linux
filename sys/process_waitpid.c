// 
// 进程等待 - waitpid 
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
  pid_t ret1 = fork();
  if (ret1 == 0) {
    printf("child1 %d\n", getpid());
    sleep(3);
    exit(0);
  }

  pid_t ret2 = fork();
  if (ret2 == 0) {
    printf("child2 %d\n", getpid());
    sleep(1);
    exit(0);
  }

  printf("father %d\n", getpid());

  int ret = 0;
  // 轮询
  while (1) {
    // WNOHANG 表示非阻塞等待
    // 非阻塞轮询 waitpid 更灵活的控制代码，充分利用等待时间做其他事情
    // 但是代码更加复杂，两全其美的办法也还是有的
    // 如果子进程结束了，给父进程发个信号
    ret = waitpid(ret1, NULL, WNOHANG);
    printf("wait1 %d\n", ret);
    if (ret > 0) {
      break;
    }
  }
  ret = waitpid(ret2, NULL, 0);
  printf("wait2 %d\n", ret);

  while (1) {
    sleep(1);
  }

  return 0;
}
