//
// 程序替换
// exec 函数族 - execlp
// p 表示 PATH，自动从 PATH 的目录中查找可执行程序
//


#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int main() {
  pid_t ret = fork();
  // 子进程
  if (ret == 0) {
    printf("execlp begin\n");
    execlp("ls", "ls", "/", NULL);
  } else if (ret > 0) {  // 父进程
    wait(NULL);
    printf("execlp end\n");
  } else {
    perror("fork");
  }

  return 0;
}
