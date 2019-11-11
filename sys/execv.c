//
// 程序替换
// exec 函数族 - execv
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
    printf("execv begin\n");
    char* const argv[] = { "/usr/bin/ls", "-l", "/", NULL };
    execv("/usr/bin/ls", argv);  // 执行成功返回值没有意义
    // 执行失败返回值 -1 才有意义
    // 通常有几种情况失败
    // 1. 没有权限
    // 2. 路径不存在
    // 3. 不是一个可执行程序
    // 等等……
  } else if (ret > 0) {
    // 父进程
    wait(NULL);
    printf("execv end %d\n", ret);
  } else {
    perror("fork");
  }

  return 0;
}
