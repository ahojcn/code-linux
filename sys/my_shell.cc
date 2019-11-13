// 
// 一个简易的 shell 程序
//


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <vector>
#include <string>


/*
 * 切分命令和参数
 * input: 带切分命令
 * output: 切分结果，输出型参数
 * return: 多少个有效字符串
 */ 
int split(char input[], char* output[]);


/*
 * 创建子进程并且进行程序替换
 * argv: 命令 + 参数
 */ 
void createProcess(char* argv[], int n);


int main() {
  while (1) {
    // 1. 打印提示符
    printf("myshell-> ");
    fflush(stdout);
    // 2. 用户输入一个指令
    char command[1024] = { 0 };
    gets(command);
    // 3. 解析命令，把要执行的命令和参数分割
    char* argv[1024];
    int n = split(command, argv);
    // 4. 创建子进程并且进行程序替换
    createProcess(argv, n);
  }

  return 0;
}  // end of main()


int split(char input[], char* output[]) {
  // 使用 strtok 实现
  // 注意 strtok 多线程环境下会导致线程不安全
  // 在 C++ 中有一个 #include<sstream> 或者 boost boost::split(更科学) 中有更优雅的切割方法
  char* p = strtok(input, " ");
  int i = 0;
  while (p != NULL) {
    output[i] = p;
    ++i;
    p = strtok(NULL, " ");
  }
  output[i] = NULL;
  return i;

}  // end of split()


void createProcess(char* argv[], int n) {
  (void) n;
  // 1. 创建子进程
  pid_t ret = fork();
  if (ret > 0) {
    // 父进程
    // 正常来说是需要关注子进程的退出状态的
    // 暂时先不关注
    wait(NULL);
  } else if (ret == 0) {
    // 子进程
    ret = execvp(argv[0], argv);
    // 如果 exec 成功了，肯定不会执行到这个代码的，所以就可以省略 if 了
    // if (ret < 0) {
    //   // 创建执行进程失败
    //   perror("exec");
    //   exit(0);
    // }
    perror("exec");
    exit(0);
  } else {
    perror("fork");
  }
  // 2. 父进程进行等待，子进程进行程序替换
}
