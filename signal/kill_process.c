// 
// kill 系统调用
//


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>


int main(int argc, char* argv[]) {
  // 只处理一个进程
  if (argc != 3) {
    printf("参数有误");
  }

  pid_t pid = atoi(argv[2]);
  int signo = atoi(argv[1] + 1);
  printf("%d\n", pid);
  printf("%d\n", signo);

  kill(pid, signo);

  return 0;
}
