//
// 程序替换
// exec 函数族 - execle
// e 表示环境变量 environment
//


#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int main() {
  pid_t ret = fork();
  // 子进程
  if (ret == 0) {
    printf("execle begin\n");
    const char* env[] = { "test_for_execle_env=test for execle", NULL };  // 这个数组必须以 NULL 结尾
    execle("./test_for_execle", "./test_for_execle", NULL, env);
  } else if (ret > 0) {  // 父进程
    wait(NULL);
    printf("execle end\n");
  } else {
    perror("fork");
  }

  return 0;
}
