//
// 程序替换
// exec 函数族 - execl
//

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {

  pid_t ret = fork();
  if (ret == 0) {
    // 子进程
    printf("execl begin\n");
    // 变长参数列表最后一个参数必须是 NULL
    execl("/usr/bin/ls", "/usr/bin/ls", "/", NULL);
  } else if (ret > 0) {
    // 父进程
    wait(NULL);
    printf("execl end %d\n", ret);
  } else {
    perror("fork");
  }

  return 0;
}
